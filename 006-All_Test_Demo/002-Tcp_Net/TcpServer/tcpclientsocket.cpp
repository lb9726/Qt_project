#include "tcpclientsocket.h"
#include <QDebug>

TcpClientSocket::TcpClientSocket(QObject *parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    connect(this, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
}

void TcpClientSocket::dataReceived()
{
    while(bytesAvailable() > 0)
    {
        int length = bytesAvailable();
        char buf[1024];
        read(buf, length);

        QString msg(buf);
        emit updateClients(msg, length);
        qDebug()<<__PRETTY_FUNCTION__<<msg;
    }
}

void TcpClientSocket::slotDisconnected()
{
    emit disconnected(this->socketDescriptor());
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
}
