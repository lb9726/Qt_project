#include "fileclient.h"

FileClient::FileClient(QString IP2, quint16 port2, QObject *parent) : QIODevice(parent)
{
    this->IP = IP2;
    this->port = port2;

    socket = new QTcpSocket();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
}

void FileClient::send(QString FullPath)
{
    socket->abort();                     /* 取消已有的连接 */
    socket->connectToHost(IP, port);
}

void FileClient::get(QString FullPath)
{

}

void FileClient::readData()
{
    QByteArray data = socket->readAll();

    qDebug(data.data());
}
