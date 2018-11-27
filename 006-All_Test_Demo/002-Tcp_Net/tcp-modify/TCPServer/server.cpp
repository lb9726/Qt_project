#include "server.h"

Server::Server(QObject *parent, int port)
    :QTcpServer(parent)
{
    listen(QHostAddress::Any, port);
}
//将收到的信息广播出去，就像群聊一样。
void Server::updataClients(QString msg, int length)
{
    emit updataServer(msg, length);

    for(int i=0; i<tcpClientSocketList.count(); ++i)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        codec = QTextCodec::codecForName("system");
        if(item->write(codec->fromUnicode( msg ), length) != length)
        {
            continue;
        }
    }
}
//如果有client断开连接，服务器要在tcpClientSocketList表里，将该ClientSocket，remove掉
void Server::slotDisconnected(int descriptor)
{
    for(int i = 0; i < tcpClientSocketList.count(); ++i)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            tcpClientSocketList.removeAt(i);
            continue;
        }
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    TCPClientSocket *tcpClientSocket = new TCPClientSocket(this);
    connect(tcpClientSocket, SIGNAL(updataClients(QString, int)),
            this, SLOT(updataClients(QString, int))); //连接上执行的槽函数
    connect(tcpClientSocket, SIGNAL(disconnected(int)),
            this, SLOT(slotDisconnected(int))); //断开连接的时候执行的函数

    tcpClientSocket->setSocketDescriptor(socketDescriptor); //将添加的套接字描述符指定参数socketDescriptor
    tcpClientSocketList.append(tcpClientSocket); //将tcpClientSocket加入套接字列表中，以便调用。
}
