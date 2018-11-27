#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

//const QString putClientPath = QString("/usr/bst/usrfs/canfestival_Test.zip");
//const QString putServerPath = QString("/home/ftp/canfestival_Test.zip");
//const QString getServerSrcPath = QString("/home/ftp/canfestival_Test.zip");
//const QString getClientDestPath = QString("/usr/bst/tmpFileDir/canfestival_Test.zip");

const QString putClientPath = QString("/usr/bst/usrfs/Ftp.zip");
const QString putServerPath = QString("/home/ftp/pub/Ftp.zip");
const QString getServerSrcPath = QString("/home/ftp/pub/Ftp.zip");
const QString getClientDestPath = QString("/usr/bst/Ftp2.zip");

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
//    connect(ui->btn_Download, &QPushButton::clicked, this, &Widget::download);
//    connect(ui->btn_Upload, &QPushButton::clicked, this, &Widget::upload);
    removeFile(putServerPath);
    connect(&mTimer, &QTimer::timeout, this, &Widget::upload);
    connect(&mTimerDownLoad, &QTimer::timeout, this, &Widget::download);
    mTimer.setSingleShot(true);
    mTimer.start(10000);
    mTimerDownLoad.setSingleShot(true);
    mTimerDownLoad.start(20000);
    removeFile(getClientDestPath);
    mFtpManager.setHostPort(mFtpManager.getIp(), 21);
    mFtpManager.setUserInfo("ftp", "ftp");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::upload()
{
    mFtpManager.put(putClientPath, putServerPath);
    qDebug()<<__PRETTY_FUNCTION__<<"clientPath = "<<putClientPath<<"serverPath = "<< putServerPath;
    connect(&mFtpManager, &FtpManager::error, this, &Widget::error);
    connect(&mFtpManager, &FtpManager::uploadProgress, this, &Widget::uploadProgress);
}

void Widget::download()
{
    mFtpManager.get(getServerSrcPath, getClientDestPath);
    connect(&mFtpManager, &FtpManager::error, this, &Widget::error);
    connect(&mFtpManager, &FtpManager::downloadProgress, this, &Widget::downloadProgress);
}

void Widget::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qDebug()<<"bytesSent = "<<bytesSent<<"bytesTotal = "<<bytesTotal;
    ui->progress_upload->setMaximum(bytesTotal);
    ui->progress_upload->setValue(bytesSent);
    if (bytesSent / bytesTotal == 1)
    {
        int flag = system("chmod -R 777 /home/ftp/pub");
        qDebug()<<__PRETTY_FUNCTION__<<"flag = "<<flag;
    }
}

void Widget::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug()<<"bytesReceived = "<<bytesReceived<<"bytesTotal = "<<bytesTotal;
    ui->progress_download->setMaximum(bytesTotal);
    ui->progress_download->setValue(bytesReceived);
}

void Widget::error(QNetworkReply::NetworkError error)
{
    switch (error) {
    case QNetworkReply::HostNotFoundError :
        qDebug() << QString::fromLocal8Bit("HostNotFoundError");
        break;
        // 其他错误处理
    case QNetworkReply::ConnectionRefusedError:
        qDebug() << QString::fromLocal8Bit("ConnectionRefusedError");
        break;
    case QNetworkReply::RemoteHostClosedError:
        qDebug() << QString::fromLocal8Bit("RemoteHostClosedError");
        break;
    case QNetworkReply::TimeoutError:
        qDebug() << QString::fromLocal8Bit("TimeoutError");
        break;
    case QNetworkReply::OperationCanceledError:
        qDebug() << QString::fromLocal8Bit("OperationCanceledError");
        break;
    case QNetworkReply::SslHandshakeFailedError:
        qDebug() << QString::fromLocal8Bit("SslHandshakeFailedError");
        break;
    case QNetworkReply::TemporaryNetworkFailureError:
        qDebug() << QString::fromLocal8Bit("TemporaryNetworkFailureError");
        break;
    case QNetworkReply::NetworkSessionFailedError:
        qDebug() << QString::fromLocal8Bit("NetworkSessionFailedError");
        break;
    case QNetworkReply::BackgroundRequestNotAllowedError:
        qDebug() << QString::fromLocal8Bit("BackgroundRequestNotAllowedError");
        break;
    case QNetworkReply::TooManyRedirectsError:
        qDebug() << QString::fromLocal8Bit("TooManyRedirectsError");
        break;
    case QNetworkReply::InsecureRedirectError:
        qDebug() << QString::fromLocal8Bit("InsecureRedirectError");
        break;
    case QNetworkReply::ProxyConnectionRefusedError:
        qDebug() << QString::fromLocal8Bit("TooManyRedirectsError");
        break;
    case QNetworkReply::ProxyConnectionClosedError:
        qDebug() << QString::fromLocal8Bit("InsecureRedirectError");
        break;
    case QNetworkReply::ProxyNotFoundError:
        qDebug() << QString::fromLocal8Bit("TooManyRedirectsError");
        break;
    case QNetworkReply::ProxyTimeoutError:
        qDebug() << QString::fromLocal8Bit("InsecureRedirectError");
        break;
    case QNetworkReply::ContentAccessDenied:
        qDebug() << QString::fromLocal8Bit("TooManyRedirectsError");
        break;
    case QNetworkReply::ContentOperationNotPermittedError:
        qDebug() << QString::fromLocal8Bit("InsecureRedirectError");
        break;
    case QNetworkReply::ContentNotFoundError:
        qDebug() << QString::fromLocal8Bit("TooManyRedirectsError");
        break;
    case QNetworkReply::AuthenticationRequiredError:
        qDebug() << QString::fromLocal8Bit("InsecureRedirectError");
        break;
    case QNetworkReply::ContentReSendError:
        qDebug() << QString::fromLocal8Bit("ContentReSendError");
        break;
    case QNetworkReply::ContentConflictError:
        qDebug() << QString::fromLocal8Bit("ContentConflictError");
        break;
    case QNetworkReply::ContentGoneError:
        qDebug() << QString::fromLocal8Bit("ContentGoneError");
        break;
    case QNetworkReply::InternalServerError:
        qDebug() << QString::fromLocal8Bit("InternalServerError");
        break;
    case QNetworkReply::OperationNotImplementedError:
        qDebug() << QString::fromLocal8Bit("OperationNotImplementedError");
        break;
    case QNetworkReply::ServiceUnavailableError:
        qDebug() << QString::fromLocal8Bit("ServiceUnavailableError");
        break;
    case QNetworkReply::ProtocolUnknownError:
        qDebug() << QString::fromLocal8Bit("OperationNotImplementedError");
        break;
    case QNetworkReply::ProtocolInvalidOperationError:
        qDebug() << QString::fromLocal8Bit("ServiceUnavailableError");
        break;
    case QNetworkReply::UnknownNetworkError:
        qDebug() << QString::fromLocal8Bit("UnknownNetworkError");
        break;
    case QNetworkReply::UnknownProxyError:
        qDebug() << QString::fromLocal8Bit("UnknownProxyError");
        break;
    case QNetworkReply::UnknownContentError:
        qDebug() << QString::fromLocal8Bit("UnknownContentError");
        break;
    case QNetworkReply::ProtocolFailure:
        qDebug() << QString::fromLocal8Bit("ProtocolFailure");
        break;
    case QNetworkReply::UnknownServerError:
        qDebug() << QString::fromLocal8Bit("UnknownServerError");
        break;
    default:
        qDebug() << QString::fromLocal8Bit("others Unknow error");
        break;
    }
}

void Widget::removeFile(QString path)
{
    QFile tmpFile(path);
    bool flag = tmpFile.exists();
    qDebug()<<__PRETTY_FUNCTION__<<"flag = "<<flag;
    if (flag)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"is exists";
        tmpFile.remove();
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"is not exists";
    }
}
