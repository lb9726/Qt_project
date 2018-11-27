#include "server.h"
#include <QDebug>

Server::Server(QObject *parent, int port)
    :QTcpServer(parent)
{
    listen(QHostAddress::Any, port);
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
}

/*
 * 文档中说incomingConnection(qintptr socketDescriptor)是个虚函数，当有新的连接来时会自动调用这个函数
，这个函数应该是为套接字跨线程设计的，像MFC中的CSocket一样QTcpSocket也是不能夸线程，
 在fmc中想让Csocket跨线程就得先分离套接字句柄再在线程中再绑定句柄以生成新的CSocket，
 Qt示例中也是这样搞的 把 socketDescriptor传入QThread 再在QThread中生成一个新的QTcpSocket。
 newConnection（）是个信号 当有连接来时 会自动发射该信号。
 */

void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
    TcpClientSocket *tcpClientSocket = new TcpClientSocket(this);
    connect(tcpClientSocket, SIGNAL(updateClients(QString, int)), this, SLOT(updateClients(QString, int)));
    connect(tcpClientSocket, SIGNAL(disconnected(int)), this, SLOT(slotDisconnected(int)));

    tcpClientSocket->setSocketDescriptor(socketDescriptor);

    tcpClientSocketList.append(tcpClientSocket);
}

// 当数据到达时，通知服务器write数据给客户端
void Server::updateClients(QString msg, int length)
{
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
    qDebug()<<__PRETTY_FUNCTION__<<"this pointer = "<< this;
    emit updateServer(msg, length);
    for(int i = 0; i < tcpClientSocketList.count(); ++i)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->write(msg.toLatin1(), length) != length)
        {
            qDebug()<<__PRETTY_FUNCTION__<<"length = "<<length;
            continue;
        }
    }
}

void Server::slotDisconnected(int descriptor)
{
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
    for(int i = 0; i < tcpClientSocketList.count(); ++i)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            qDebug()<<__PRETTY_FUNCTION__<<"descriptor = "<<descriptor;
            tcpClientSocketList.removeAt(i);
            item->deleteLater();
            return;
        }
    }
    return;
}
