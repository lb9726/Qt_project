#include "tcpclientsocket.h"

TCPClientSocket::TCPClientSocket(QObject *parent) :
    QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    connect(this, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
}

void TCPClientSocket::dataReceived()
{
    while(bytesAvailable() > 0)
    {
        int length = bytesAvailable();//获取数据长度
        char buf[1024];
        read(buf, length);//接收数据，放入数组buf 中

        QString msg = buf;
        emit updataClients(msg, length); //将数据以SIGNAL的方式发出
    }
}

void TCPClientSocket::slotDisconnected()
{
    emit disconnected(this->socketDescriptor());
}
