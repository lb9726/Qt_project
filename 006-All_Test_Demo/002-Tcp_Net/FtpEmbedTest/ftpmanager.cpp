#include "ftpmanager.h"
#include <QFileInfo>
#include <QHostInfo>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDebug>
#include <QList>
#include <QSslConfiguration>

FtpManager::FtpManager(QObject *parent)
    : QObject(parent)
{
    m_pUrl.setScheme("ftp"); // 设置协议
}

// 获取本机IP
QString FtpManager::getIp()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
    {
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }
    qDebug()<<"ipString = "<<ipAddress;
    return ipAddress;
}

// 设置地址和端口
void FtpManager::setHostPort(const QString &host, int port)
{
    qDebug()<<"host = "<<host<<"port = "<<port;
    m_pUrl.setHost(host);
    m_pUrl.setPort(port);
}

// 设置登录 FTP 服务器的用户名和密码
void FtpManager::setUserInfo(const QString &userName, const QString &password)
{
    qDebug()<<"userName = "<<userName<<"password = "<<password;
    m_pUrl.setUserName(userName);
    m_pUrl.setPassword(password);
}

// 上传文件
void FtpManager::put(const QString &fileName, const QString &serverPath)
{
    QFile file(fileName);
    bool putOpenFlag = file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
//    qDebug()<<__PRETTY_FUNCTION__<<"data = "<<data;
    m_pUrl.setPath("pub/" + serverPath.split("/").last());
    qDebug()<<__PRETTY_FUNCTION__<<"putOpenFlag = "<<putOpenFlag;

//    QSslConfiguration config;

//    config.setPeerVerifyMode(QSslSocket::VerifyNone);
//    config.setProtocol(QSsl::TlsV1_1);
//    QNetworkRequest pRequset = QNetworkRequest(m_pUrl);
//    pRequset.setSslConfiguration(config);

//    QNetworkReply *pReply = m_manager.put(pRequset, data);

//    m_pUrl.setPath(path);
    QNetworkRequest pRequset = QNetworkRequest(m_pUrl);
//    pRequset.setMaximumRedirectsAllowed(100);
    QNetworkReply *pReply = m_manager.put(pRequset, data);

    connect(pReply, SIGNAL(uploadProgress(qint64, qint64)), this, SIGNAL(uploadProgress(qint64, qint64)));
    connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));
}

// 下载文件
void FtpManager::get(const QString &serverPath, const QString &fileName)
{
    QFileInfo info;
    info.setFile(fileName);

    m_file.setFileName(fileName);
    bool getOpenFlag = m_file.open(QIODevice::WriteOnly | QIODevice::Append);
    m_pUrl.setPath("pub/" + serverPath.split("/").last());
    qDebug()<<__PRETTY_FUNCTION__<<"getOpenFlag = "<<getOpenFlag;
    QNetworkRequest pRequset = QNetworkRequest(m_pUrl);
//    pRequset.setMaximumRedirectsAllowed(1000);
    QNetworkReply *pReply = m_manager.get(pRequset);

    connect(pReply, SIGNAL(finished()), this, SLOT(finished()));
    connect(pReply, SIGNAL(downloadProgress(qint64, qint64)), this, SIGNAL(downloadProgress(qint64, qint64)));
    connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));
}

// 下载过程中写文件
void FtpManager::finished()
{
    QNetworkReply *pReply = qobject_cast<QNetworkReply *>(sender());
    switch (pReply->error())
    {
    case QNetworkReply::NoError :
    {
        m_file.write(pReply->readAll());
        m_file.flush();
    }
        break;
    default:
        break;
    }

    m_file.close();
    pReply->deleteLater();
}
