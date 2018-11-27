#include "tcpclient.h"
#include <QMessageBox>
#include <QHostInfo>


TCPClient::TCPClient(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("TCP Client");

    textBrowser = new QTextBrowser;
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
    mainLayout->addWidget(textBrowser, 0, 0, 1, 2);
    mainLayout->addWidget(sendLineEdit, 1, 0);
    mainLayout->addWidget(sendBtn, 1, 1);
    mainLayout->addWidget(userNameLabel, 2, 0);
    mainLayout->addWidget(userNameLineEdit, 2, 1);
    mainLayout->addWidget(serverIPLabel, 3, 0);
    mainLayout->addWidget(serverIPLineEdit, 3, 1);
    mainLayout->addWidget(portLabel, 4, 0);
    mainLayout->addWidget(portLineEdit, 4, 1);
    mainLayout->addWidget(enterBtn, 5, 0, 1, 2);

    status = false; //用于记录当前的状态 true 表示已经进入聊天室
    port = 8080;
    portLineEdit->setText(QString::number(port));
    userNameLineEdit->setText(tr("林向链"));
    serverIPLineEdit->setText(tr("192.168.200.14"));

    serverIP = new QHostAddress(); //建立一个0.0.0.0

    connect(textBrowser, SIGNAL(cursorPositionChanged()), this, SLOT(autoScroll()));

    connect(enterBtn, SIGNAL(clicked()), this, SLOT(slotEnter()));
    connect(sendBtn, SIGNAL(clicked()), this, SLOT(slotSend()));

    sendBtn->setEnabled(false);

}

TCPClient::~TCPClient()
{
    delete textBrowser;
    delete sendLineEdit;
    delete sendBtn;
    delete userNameLabel;
    delete userNameLineEdit;
    delete serverIPLabel;
    delete serverIPLineEdit;
    delete portLabel;
    delete portLineEdit;
    delete enterBtn;
    delete mainLayout;
}

void TCPClient::slotEnter()
{
    if(!status)
    {

        QString ip = serverIPLineEdit->text();
        if(!serverIP->setAddress(ip))// 输入的地址是否合法
        {
            QMessageBox::information(this, "error", "server ip address error!");
            return;
        }

        if(userNameLineEdit->text() == "") //用户名是否为空
        {
            QMessageBox::information(this, "error", "user name error!");
            return;
        }
        //以上的代码，主要是判断用户输入是否合法，下面就是建立连接。

        userName = userNameLineEdit->text();

        tcpSocket = new QTcpSocket(this);//创建一个QTcpSocket对象，再建立一些信号槽，断开、连接、接收数据
        connect(tcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
        connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));

        tcpSocket->connectToHost(*serverIP, port); //建立连接，会发出connected信号
        status = true;
    }
    else
    {
        int length = 0;
        QString msg = userName + ":" + " Leave Char Room!";
        if((length = tcpSocket->write(msg.toLocal8Bit(),
                                      msg.toLocal8Bit().length())) != msg.toLocal8Bit().length())
        {
            return;
        }
        tcpSocket->disconnectFromHost(); //断开连接，会发出disconnected 信号

        status = false;
    }
}


void TCPClient::slotConnected()
{
     enterBtn->setText(tr("退出聊天室"));
     sendBtn->setEnabled(true);

     int length = 0;
     codec = QTextCodec::codecForName("system");
     QString msg = userName + ":" + " enter Char Room!";
     if((length = tcpSocket->write(msg.toLocal8Bit(), msg.toLocal8Bit().length())) != msg.toLocal8Bit().length())
     {
         return;
     }

}

void TCPClient::slotDisconnected()
{
    enterBtn->setText(tr("进入聊天室"));
    sendBtn->setEnabled(false);

}

void TCPClient::dataReceived()
{
    while(tcpSocket->bytesAvailable() > 0)
    {
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());

        tcpSocket->read(datagram.data(), datagram.size());

        QString msg = datagram.data();
        textBrowser->insertPlainText(msg.left(datagram.size()) + "\n");
    }
}

void TCPClient::slotSend()
{
    if(sendLineEdit->text() == "")
    {
        return;
    }

    QString msg = userName+":"+sendLineEdit->text();
 //   userNameLabel->setText(QString::number(msg.toLocal8Bit().length()));
    tcpSocket->write(msg.toLocal8Bit(), msg.toLocal8Bit().length());
    sendLineEdit->clear();
}

void TCPClient::autoScroll()
{
    QTextCursor cursor = textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    textBrowser->setTextCursor(cursor);
}


