#include "widget.h"
#include "ui_widget.h"

const QString clientPath = QString("/home/libo/Desktop/canfestival_Test.zip");
const QString serverPath = QString("/home/lb1234/canfestival_Test.zip");
const QString serverSrcPath = QString("/home/lb1234/canfestival_Test.zip");
const QString clientDestPath = QString("/home/libo/Desktop/tmpFileDir/canfestival_Test.zip");

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->btn_Download, &QPushButton::clicked, this, &Widget::download);
    connect(ui->btn_Upload, &QPushButton::clicked, this, &Widget::upload);
    mFtpManager.setHostPort(mFtpManager.getIp(), 21);
    mFtpManager.setUserInfo("lb1234", "lb1234");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::upload()
{
    mFtpManager.put(clientPath, serverPath);
    qDebug()<<"clientPath = "<<clientPath<<"serverPath = "<< serverPath;
//    connect(&mFtpManager, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
//    connect(&mFtpManager, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(uploadProgress(qint64, qint64)));
    connect(&mFtpManager, &FtpManager::error, this, &Widget::error);
    connect(&mFtpManager, &FtpManager::uploadProgress, this, &Widget::uploadProgress);
}

void Widget::download()
{
    mFtpManager.get(serverSrcPath, clientDestPath);
//    connect(&mFtpManager, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
//    connect(&mFtpManager, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)));
    connect(&mFtpManager, &FtpManager::error, this, &Widget::error);
    connect(&mFtpManager, &FtpManager::downloadProgress, this, &Widget::downloadProgress);
}

void Widget::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qDebug()<<"bytesSent = "<<bytesSent<<"bytesTotal = "<<bytesTotal;
    ui->progress_upload->setMaximum(bytesTotal);
    ui->progress_upload->setValue(bytesSent);
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
        qDebug() << QString::fromLocal8Bit("主机没有找到");
        break;
        // 其他错误处理
    default:
        break;
    }
}
