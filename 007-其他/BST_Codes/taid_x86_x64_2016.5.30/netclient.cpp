#include "netclient.h"

NetClient::NetClient(QObject *parent) : QObject(parent)
{
    m_isConnected = false;
}

void NetClient::init()
{
    m_tcpSocket = new QTcpSocket(this);
    m_HostAddress.setAddress(QObject::tr("127.0.0.1"));
    m_tcpSocket->connectToHost(m_HostAddress, 8080);
}

void NetClient::slt_connected(){
    connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(slt_connected()));
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(slt_disconnected()));
    connect(m_tcpSocket, SIGNAL(readyRead()),this, SLOT(dataReceived()));
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    m_isConnected = true;
}

void NetClient::slt_disconnected(){
    disconnect(m_tcpSocket, SIGNAL(connected()), this, SLOT(slt_connected()));
    disconnect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(slt_disconnected()));
    disconnect(m_tcpSocket, SIGNAL(readyRead()),this, SLOT(dataReceived()));
    disconnect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    m_isConnected = false;
}

void NetClient::dataReceived(){
    QByteArray qba;
    qba = m_tcpSocket->readAll();
}

bool NetClient::writeData(QString msg){
    if(msg.length() != m_tcpSocket->write(msg.toLatin1(),msg.length()))
    {
        return false;
    }
    return true;
}

bool NetClient::fileTransmit()
{
    return true;
}

void NetClient::slt_deviceList()
{

}

bool NetClient::connected() const
{
    return m_isConnected;
}

void NetClient::setConnected(bool status)
{
    if(m_isConnected != status){
        m_isConnected = status;
        emit connectedChanged(m_isConnected);
    }
}





