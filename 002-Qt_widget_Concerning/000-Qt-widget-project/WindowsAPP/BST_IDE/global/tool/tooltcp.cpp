#include "tooltcp.h"

#if 1
ToolTcpServer::ToolTcpServer(quint16 pPort, QObject *parent)
    :QTcpServer(parent)
{
    if(pPort <= 0)
        pPort = D_NMPORT;
    Open(QHostAddress::Any, pPort);
}

ToolTcpServer::~ToolTcpServer()
{
    Close();
}

bool ToolTcpServer::Open(QHostAddress pIP, quint16 pPort)
{
    Close();
    //>@如果上次运行的程序启动外部程序，比如Mplayer，并且最后关闭了Display但没有关闭Mpalayer，
    //>@那么表明上一个应用程序依然没有完全退出，其所占用的端口号没有被释放，造成此次监听端口失败。
    if(listen(pIP, pPort))
       return true;
    IDE_DEBUG(GBKToUTF8(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
              ">>>>>>请检测是否上次运行的程序有残留？>>>>>>\n"
              ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"));
    return false;
}

void ToolTcpServer::Close()
{
    if(isListening())
        close();
}

void ToolTcpServer::incomingConnection(int handle)
{
    ToolTcpClient* tmpSocket = new ToolTcpClient;
    if(!tmpSocket->setSocketDescriptor(handle))
    {
        tmpSocket->deleteLater();
        return;
    }
    tmpSocket->mConnectTime = QDateTime::currentDateTime();
    emit sNewSocket(tmpSocket);
}
#else
ToolTcpServer::ToolTcpServer(quint16 pPort, QObject *parent)
    :QTcpServer(parent)
{
    if(pPort <= 0)
        pPort = D_NMPORT;
    Open(QHostAddress::Any, pPort);
}

ToolTcpServer::~ToolTcpServer()
{
    Close();
}

bool ToolTcpServer::Open(QHostAddress pIP, quint16 pPort)
{
    Close();
    //>@如果上次运行的程序启动外部程序，比如Mplayer，并且最后关闭了Display但没有关闭Mpalayer，
    //>@那么表明上一个应用程序依然没有完全退出，其所占用的端口号没有被释放，造成此次监听端口失败。
    if(listen(pIP, pPort))
       return true;
    IDE_DEBUG(GBKToUTF8(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
              ">>>>>>请检测是否上次运行的程序有残留？>>>>>>\n"
              ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"));
    return false;
}

void ToolTcpServer::Close()
{
    if(isListening())
        close();
}

bool ToolTcpServer::Abort(TcpSocketPtr pSocket)
{
    if(!pSocket)
        return true;
    return Remove(pSocket, true);
}

void ToolTcpServer::slot_AbortAll()
{
    AbortAll();
}

bool ToolTcpServer::AbortAll()
{
    while(!m_UndefClients.isEmpty())
    {
        QTcpSocket* tmpSocket = m_UndefClients.takeFirst();
        if(tmpSocket)
        {
            emit sDisconnected(tmpSocket->peerAddress(), tmpSocket->peerPort());
            tmpSocket->deleteLater();
        }
    }
    QList<TcpSocketPtr> tmpClientList = m_Clients.values();
    while(!tmpClientList.isEmpty())
    {
        QTcpSocket* tmpSocket = tmpClientList.takeFirst();
        if(tmpSocket)
        {
            emit sDisconnected(tmpSocket->peerAddress(), tmpSocket->peerPort());
            tmpSocket->deleteLater();
        }
    }
    m_Clients.clear();
}

qint64 ToolTcpServer::Write(uint pType, QByteArray data)
{
    if(pType <= 0)
        return -1;
    QList<TcpSocketPtr> tmpPtrs = m_Clients.values(pType);
    TcpSocketPtr tmpPtr;
    qint64 ret = 0;
    foreach(tmpPtr, tmpPtrs)
    {
        QTcpSocket *tmpSocket = (QTcpSocket *)tmpPtr;
        if(!tmpSocket)
            continue;
        ret += tmpSocket->write(data);
    }
    return ret;
}

uint ToolTcpServer::GetType(QTcpSocket *pSocket)
{
    TcpSocketPtr tmpPtr = (TcpSocketPtr)pSocket;
    return m_Clients.key(tmpPtr);
}

bool ToolTcpServer::isNull()
{
    if(!m_UndefClients.isEmpty())
        return false;
    if(!m_Clients.isEmpty())
        return false;
    return true;
}

bool ToolTcpServer::isConnected(QTcpSocket* pSocket)
{
    if(!pSocket)
        return false;
    if(pSocket->state() == QAbstractSocket::ConnectedState)
        return true;
    return false;
}

void ToolTcpServer::incomingConnection(int handle)
{
    QTcpSocket* tmpSocket = new ToolTcpClient;
    if(!tmpSocket->setSocketDescriptor(handle))
    {
        tmpSocket->deleteLater();
        return;
    }
    connect(tmpSocket, SIGNAL(disconnected()), this, SLOT(slot_Disconnected()));
    TcpSocketPtr tmpPtr = (TcpSocketPtr)tmpSocket;
    m_UndefClients.append(tmpPtr);
    IDE_DEBUG(QString("New Connection: IP[%1]!").arg(tmpSocket->peerAddress().toString()));

    emit sNewSocket(tmpSocket);
}

bool ToolTcpServer::SetType(QTcpSocket *pSocket, uint pType)
{
    if(!pSocket)
        return false;
    if(m_UndefClients.contains(pSocket))
    {
        m_UndefClients.removeOne(pSocket);
        m_Clients.insert(pType, pSocket);
        return true;
    }
    return false;
}

bool ToolTcpServer::Remove(QTcpSocket *pSocket, bool pDel)
{
    if(!pSocket)
        return false;
    QTcpSocket* tmpSocket = 0;
    int index = m_UndefClients.indexOf(pSocket);
    if(index>=0)
    {
        tmpSocket = m_UndefClients.takeAt(index);
    }
    else
    {
        uint tmpType = GetType(pSocket);
        m_Clients.remove(tmpType, pSocket);
        tmpSocket = pSocket;
    }
    if(!tmpSocket)
        return false;
    if(pDel)
    {
        emit sDisconnected(tmpSocket->peerAddress(), tmpSocket->peerPort());
        tmpSocket->deleteLater();
    }
    return true;
}

void ToolTcpServer::slot_Disconnected()
{
    QTcpSocket* tmpSocket = VAR_CAST<QTcpSocket*>(sender());
    if(!tmpSocket)
        return;
    Remove((TcpSocketPtr)tmpSocket, true);
}
#endif

/******************************************************
//>@
******************************************************/

ToolTcpClient::ToolTcpClient(uint pType, QObject *parent)
    :QTcpSocket(parent)
{
    mType = pType;
}

//>@执行deletelater后不阻塞，但是会隔一段时间后才会真正执行~ToolTcpClient()里面的内容。因此不要对ToolTcpClient使用智能指针QPointer
ToolTcpClient::~ToolTcpClient()
{
    Close();
}

bool ToolTcpClient::isConnected()
{
    if(state() == QAbstractSocket::ConnectedState)
        return true;
    return false;
}

void ToolTcpClient::Close()
{
    disconnect(this, SIGNAL(connected()), this, SLOT(slot_Connected()));
    disconnect(this, SIGNAL(disconnected()), this, SLOT(slot_Disconnected()));

    if(state() != QAbstractSocket::UnconnectedState)
        disconnectFromHost();

    if(!(state() == QAbstractSocket::UnconnectedState || waitForDisconnected(1000)))
        IDE_DEBUG(QString("Socket[%1] can't be Closed!").arg(state()));

    if(isOpen())
        QIODevice::close();
}

bool ToolTcpClient::Open(QHostAddress pIP, quint16 pPort, quint32 pTimeout)
{
    if(state() == ConnectedState)
        return true;
    connect(this, SIGNAL(connected()), this, SLOT(slot_Connected()));
    connect(this, SIGNAL(disconnected()), this, SLOT(slot_Disconnected()));
    quint32 delayTime = pTimeout / 2;
    QtSleep(delayTime);
    connectToHost(pIP, pPort);
    quint32 i = 0;
    while(((i+=100) < delayTime) && (state() != ConnectedState))
        QtSleep(100);
    if(state() == ConnectedState)
        return true;
    return false;
}

void ToolTcpClient::Open(QHostAddress pIP, quint16 pPort)
{
    connect(this, SIGNAL(connected()), this, SLOT(slot_Connected()));
    connect(this, SIGNAL(disconnected()), this, SLOT(slot_Disconnected()));
    if(state() != ConnectedState)
        connectToHost(pIP, pPort);
}

int ToolTcpClient::GetConnectedTime()
{
    if(mConnectTime.isValid())
        return qAbs(QDateTime::currentDateTime().secsTo(mConnectTime));
    return -1;
}

void ToolTcpClient::slot_Connected()
{
    mConnectTime = QDateTime::currentDateTime();
    emit sConnState(true);
}

void ToolTcpClient::slot_Disconnected()
{
    emit sConnState(false);
}

