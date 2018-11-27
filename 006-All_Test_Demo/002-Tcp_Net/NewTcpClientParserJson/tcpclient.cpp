#include "tcpclient.h"
#include <QMessageBox>
#include <QHostInfo>
#include <QDebug>
#include "jsonparser.h"
#include <QFile>
#include "define.h"
#include <QCoreApplication>

TcpClient::TcpClient(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    setWindowTitle(tr("TCP Client"));

    contentListWidget = new QListWidget;

    sendLineEdit = new QLineEdit;
    sendBtn = new QPushButton(tr("发送"));

    userNameLabel = new QLabel(tr("用户名："));
    userNameLineEdit = new QLineEdit;

    serverIPLabel = new QLabel(tr("服务器地址："));
    serverIPLineEdit = new QLineEdit;

    portLabel = new QLabel(tr("端口："));
    portLineEdit = new QLineEdit;

    enterBtn = new QPushButton(tr("进入聊天室"));

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(contentListWidget, 0, 0, 1, 2);
    mainLayout->addWidget(sendLineEdit, 1, 0);
    mainLayout->addWidget(sendBtn, 1, 1);
    mainLayout->addWidget(userNameLabel, 2, 0);
    mainLayout->addWidget(userNameLineEdit, 2, 1);
    mainLayout->addWidget(serverIPLabel, 3, 0);
    mainLayout->addWidget(serverIPLineEdit, 3, 1);
    mainLayout->addWidget(portLabel, 4, 0);
    mainLayout->addWidget(portLineEdit, 4, 1);
    mainLayout->addWidget(enterBtn, 5, 0, 1, 2);

    status = false;

    port = 60001;
    portLineEdit->setText(QString::number(port));

    serverIP = new QHostAddress();

    connect(enterBtn, SIGNAL(clicked()), this, SLOT(slotEnter()));
    connect(sendBtn, SIGNAL(clicked()), this, SLOT(slotSend()));

    sendBtn->setEnabled(false);
    mRunPath = QCoreApplication::applicationDirPath();
    mJsonParser = new JsonParser();
    if (mJsonParser)
    {
        mJsonParser->setTcpClient(this);
    }
}

TcpClient::~TcpClient()
{
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
    contentListWidget->deleteLater();
    sendLineEdit->deleteLater();
    sendBtn->deleteLater();
    userNameLabel->deleteLater();
    userNameLineEdit->deleteLater();
    serverIPLabel->deleteLater();
    serverIPLineEdit->deleteLater();
    portLabel->deleteLater();
    portLineEdit->deleteLater();
    enterBtn->deleteLater();
    mainLayout->deleteLater();
    delete serverIP;
    serverIP = 0;
    if (mJsonParser)
    {
        mJsonParser->deleteLater();
    }
}

int TcpClient::writeMsgToServer(QByteArray msg, int length)
{
    if (tcpSocket)
    {
        int tmplen = tcpSocket->write(msg.data(), length);
        qDebug()<<__PRETTY_FUNCTION__<<"length = "<<length<< "tmplen = "<<tmplen;
        return tmplen;
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"mMyTcpSocket write wrong";
        return -1;
    }
}

void TcpClient::readFileLines(QString fileName)
{
    QFile file;
    file.setFileName(fileName); // mRunPath + "/libo.txt"
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"open file failed";
        return;
    }
    while (!file.atEnd())
    {
        QByteArray byteArray = file.readLine();
        qDebug()<<__PRETTY_FUNCTION__<<"byteArray = "<<byteArray<<"\n";
        if (mJsonParser)
        {
            byteArray = mJsonParser->generateBuffer(byteArray);
            writeMsgToServer(byteArray, byteArray.length());
        }
        QtSleep(4000);
    }
}

void TcpClient::slotEnter()
{
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
    if(!status)
    {
        QString ip = serverIPLineEdit->text();
        if(!serverIP->setAddress(ip))
        {
            QMessageBox::information(this, tr("error"), tr("server ip address error!"));
            return;
        }

        if(userNameLineEdit->text() == "")
        {
            QMessageBox::information(this, tr("error"), tr("User name error!"));
            return;
        }

        userName = userNameLineEdit->text();

        tcpSocket = new QTcpSocket(this);
        if (tcpSocket)
        {
            connect(tcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
            connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
            connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
        }

        port = portLineEdit->text().toInt();
        qDebug()<<__PRETTY_FUNCTION__<<"port = "<<port;
        if (tcpSocket)
        {
            tcpSocket->connectToHost(*serverIP, port);
        }
        status = true;
    }
    else
    {
        int length = 0;
        QString msg = userName + QString(":Leave Chat Room");
        if((length = tcpSocket->write(msg.toLatin1(), msg.length())) != msg.length())
        {
            return;
        }

        tcpSocket->disconnectFromHost();
        status = false;
    }
}

void TcpClient::slotConnected()
{
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
    sendBtn->setEnabled(true);
    enterBtn->setText(tr("离开"));

    QByteArray msg = userName.toLatin1() + QByteArray(":Enter Chat Room");
    if (tcpSocket)
    {
        if (mJsonParser)
        {
            msg = mJsonParser->generateBuffer(msg);
            writeMsgToServer(msg, msg.length());
            QtSleep(3000);
            readFileLines(mRunPath + "/libo.txt");
        }
    }
}

void TcpClient::slotSend()
{
    qDebug()<<__PRETTY_FUNCTION__<< "is call";
    if(sendLineEdit->text() == "")
    {
        return ;
    }

    QString msg = sendLineEdit->text();
    tcpSocket->write(msg.toLatin1(), msg.length());
    sendLineEdit->clear();
}

void TcpClient::slotDisconnected()
{
    sendBtn->setEnabled(false);
    qDebug()<<__PRETTY_FUNCTION__<<"socket is closed and server is closed";
    enterBtn->setText(tr("进入聊天室"));
    tcpSocket->deleteLater();
    tcpSocket = 0;
    status = false;
}

void TcpClient::dataReceived()
{
    int len = -1;
    QByteArray msg;
    if (tcpSocket)
    {
        len = tcpSocket->bytesAvailable();
    }
    if (len > 0)
    {
        msg = tcpSocket->readAll();
        if (mJsonParser)
        {
            mJsonParser->parseBuffer(msg);
            contentListWidget->addItem(msg.left(len));
        }
    }
}
