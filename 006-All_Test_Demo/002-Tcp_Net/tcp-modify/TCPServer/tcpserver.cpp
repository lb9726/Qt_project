#include "tcpserver.h"

TCPServer::TCPServer(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("TCP Server");

    textBrowser = new QTextBrowser;
    portLabel = new QLabel(tr("端口："));
    portLineEdit = new QLineEdit;
    textLabel = new QLabel(tr("测试"));

    createBtn = new QPushButton(tr("创建聊天室"));

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(textBrowser, 0, 0, 1, 2);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(portLineEdit, 1, 1);
    mainLayout->addWidget(createBtn, 2, 0, 1, 2);
    mainLayout->addWidget(textLabel);

    port = 8080;
    portLineEdit->setText(QString::number(port));

    connect(textBrowser, SIGNAL(cursorPositionChanged()), this, SLOT(autoScroll()));
    connect(createBtn, SIGNAL(clicked()), this, SLOT(slotCreateServer()));
}

TCPServer::~TCPServer()
{
    delete textBrowser;
    delete portLabel;
    delete portLineEdit;
    delete createBtn;
}
//点击按键后，创建一个server对象，用于监听是否有client连接
void TCPServer::slotCreateServer()
{
    server = new Server(this, port);
    connect(server, SIGNAL(updataServer(QString, int)),
            this, SLOT(updataServer(QString, int)));
    createBtn->setEnabled(false);
}

void TCPServer::updataServer(QString msg, int length)
{

//    portLabel->setText(QString::number(length));
 //    textLabel->setText(QString::number(msg.toLocal8Bit().length()));

    textBrowser->insertPlainText(msg.toLocal8Bit().left(length)+"\n");
}

void TCPServer::autoScroll()
{
    QTextCursor cursor = textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    textBrowser->setTextCursor(cursor);
}
