#include "tcpmodule.h"

MyTcpServer::MyTcpServer(QObject *parent, int port)
    : QTcpServer(parent)
    , mHasConnectFlag(false)
{
    listen(QHostAddress::Any, port);
    qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__<< "will listening";
}

MyTcpServer::~MyTcpServer()
{
    if (mMyTcpSocket)
    {
        qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__;
        mMyTcpSocket->deleteLater();
    }
}

void MyTcpServer::writeMsgToClient(QByteArray msg, int length)
{
    if (mMyTcpSocket && mHasConnectFlag)
    {
        int tmplen = mMyTcpSocket->write(msg.data(), length);
        qDebug()<<__PRETTY_FUNCTION__<<"length = "<<length<< "tmplen = "<<tmplen;
    }
}

/*
 * 当MyTcpSocket数据到达时，MyTcpSocket通知MyTcpServer服务器对数据进行处理，例如检查json格式是否正确等
 * 组合生成需要回复的QJson字符串，调用MyTcpSocket write数据给Client
*/
void MyTcpServer::slotMyTcpServerDealWithMsg(QByteArray msg, int length)
{
    qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__;
    // 处理QJson数据
    int ret = mJsonParse.parserJsonFormat(msg);
//    writeMsgToClient(json, json.length());
    emit myTcpServerSendMsgToScreen(msg, length);  // TcpServer 发送信号到主界面用于通知显示
}

void MyTcpServer::slotMyTcpServerDisconnected(int)
{
    qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__;
    mHasConnectFlag = false;
    mMyTcpSocket->close();
    mMyTcpSocket->deleteLater();
    mMyTcpSocket = 0;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__<< "mHasConnectFlag = "<< mHasConnectFlag;
    if (!mHasConnectFlag)
    {
        mHasConnectFlag = true;
        mMyTcpSocket = new MyTcpSocket();
        if (mMyTcpSocket)
        {
            connect(mMyTcpSocket, SIGNAL(myTcpSocketRecvMsg(QByteArray, int)),
                    this, SLOT(slotMyTcpServerDealWithMsg(QByteArray,int)));
            connect(mMyTcpSocket, SIGNAL(myTcpSocketDisconnected(int)),
                    this, SLOT(slotMyTcpServerDisconnected(int)));
        }
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__<<" already connect do nothing";
    }
}

// TcpSocket 模块
MyTcpSocket::MyTcpSocket(QObject *parent)
{
    qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__;
    connect(this, SIGNAL(readyRead()), this, SLOT(slotMyTcpSocketDataReceived()));
    connect(this, SIGNAL(disconnected()), this, SLOT(slotMyTcpSocketDisconnected()));
}

void MyTcpSocket::slotMyTcpSocketDataReceived()
{
    qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__;
    int length = bytesAvailable();
    while (length > 0)
    {
        QByteArray recvMsg = read(length);
        qDebug()<<__PRETTY_FUNCTION__<<recvMsg;
        emit myTcpSocketRecvMsg(recvMsg, length);
    }
    qDebug()<<__PRETTY_FUNCTION__;
}

void MyTcpSocket::slotMyTcpSocketDisconnected()
{
    int tmpSocketDesp = this->socketDescriptor();
    qDebug()<<__PRETTY_FUNCTION__<<"tmpSocketDesp = "<< tmpSocketDesp;
    emit myTcpSocketDisconnected(tmpSocketDesp);
}


