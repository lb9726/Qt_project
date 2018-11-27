#include "tooludp.h"

ToolUdp::ToolUdp(QObject *parent) :
    QIODevice(parent)
{
    mUdpSocket = 0;
    mPort = 0;
}

ToolUdp::~ToolUdp()
{
    mPort = 0;
    Close();
}

bool ToolUdp::OpenUdp(quint16 pPort)
{
    Close();
    mUdpSocket = new QUdpSocket(this);
    mUdpSocket->setReadBufferSize(64*1024);
    if(!mUdpSocket->bind(QHostAddress::Any, pPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint))
    {
        Close();
        return false;
    }
    mPort = pPort;
    connect(mUdpSocket, SIGNAL(readyRead()), this, SIGNAL(readyRead()));
    return true;
}

void ToolUdp::Close()
{
    if(mUdpSocket != 0)
    {
        disconnect(mUdpSocket, SIGNAL(readyRead()), 0, 0);
        mUdpSocket->abort();
        mUdpSocket->close();
        mUdpSocket->deleteLater();
        mUdpSocket = 0;
    }
}

qint64 ToolUdp::write(const QByteArray &data)
{
    if(mUdpSocket == 0)
        return 0;
    quint64 byteWrite = 0;
    UpdateBroadcastAddresses();
    foreach (QHostAddress address, mBroadcastIpAddrs)
        byteWrite += mUdpSocket->writeDatagram(data, data.length(), address, mPort);
    return byteWrite;
}

qint64 ToolUdp::write(QByteArray data, QHostAddress host)
{
    if(mUdpSocket == 0)
        return 0;
    return (mUdpSocket->writeDatagram(data, data.length(), host, mPort));
}

qint64 ToolUdp::echo()
{
    if(mUdpSocket == 0)
        return 0;
    return (mUdpSocket->writeDatagram(m_RevByteArray, m_RevByteArray.length(), m_SenderHost, mPort));
}

//>@回复发送者信息
qint64 ToolUdp::reply(const QByteArray &data)
{
    if(mUdpSocket == 0)
        return 0;

#if (defined(SUPPORT_LOCALTEST) && defined(WINDOWS))
    return (mUdpSocket->writeDatagram(data, data.length(), QHostAddress::Broadcast, mPort));
#else
    return (mUdpSocket->writeDatagram(data, data.length(), m_SenderHost, mPort));
#endif
}

bool ToolUdp::isOpen()
{
    if(!mUdpSocket)
        return false;
    if(mUdpSocket->state() < QAbstractSocket::ConnectedState)
        return false;
    return true;
}

bool ToolUdp::IsLocalIP(QHostAddress pIP)
{
    QStringList localIPList = GetLocalIPList();
    for(int i=0;i<localIPList.count();i++)
    {
        QHostAddress localIP = QHostAddress(localIPList.at(i));
        if(localIP == pIP)
            return true;
    }
    return false;
}

QByteArray ToolUdp::readAll()
{
    if(mUdpSocket == 0)
        return QByteArray();
    QHostAddress senderIP;
    quint16 sendPort;
    m_RevByteArray.clear();
    m_RevByteArray.resize(mUdpSocket->pendingDatagramSize());
    mUdpSocket->readDatagram(m_RevByteArray.data(), m_RevByteArray.size(), &senderIP, &sendPort);
#ifdef UDPLOCALFLITER
    if(IsLocalIP(senderIP, sendPort))
        return QByteArray();
#endif
    m_SenderHost = senderIP;
    m_SenderPort = sendPort;
    return m_RevByteArray;
}

void ToolUdp::UpdateBroadcastAddresses()
{
    mBroadcastIpAddrs.clear();
    foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        foreach (QNetworkAddressEntry entry, interface.addressEntries())
        {
            QHostAddress broadcastAddress = entry.broadcast();
            if (broadcastAddress != QHostAddress::Null
#ifdef UDPLOCALFLITER
                    && entry.ip() != QHostAddress::LocalHost
#endif
                    )
            {
                mBroadcastIpAddrs << broadcastAddress;
            }
        }
    }
}


