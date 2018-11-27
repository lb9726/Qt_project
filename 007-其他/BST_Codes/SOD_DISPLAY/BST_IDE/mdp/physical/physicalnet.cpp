#include "physicalnet.h"
#include "logicbase.h"
#include "deviceinc.h"

PhysicalLANClient::PhysicalLANClient(ToolTcpClient *pTcpClient, QObject *pParent):
    PhysicalHost(QString(), QString(), pParent)
{
    mTcpClient = 0;
    connect(this, SIGNAL(sAutoConnect(bool)), this, SLOT(slot_ConnState(bool)), Qt::QueuedConnection);
    mDestoryFlag = true;
    mPort = D_NMPORT;
    mDevType = TYPE_NET;
    SetTcpClient(pTcpClient);
}

PhysicalLANClient::PhysicalLANClient(QString pAddr, QString pBufferPath, QObject *parent):
    PhysicalHost(pAddr, pBufferPath, parent)
{
    mTcpClient = 0;
    connect(this, SIGNAL(sAutoConnect(bool)), this, SLOT(slot_ConnState(bool)), Qt::QueuedConnection);
    mDestoryFlag = true;
    mPort = D_NMPORT;
    mDevType = TYPE_NET;
}

PhysicalLANClient::~PhysicalLANClient()
{
    if(mDestoryFlag)
        Async();
}

bool PhysicalLANClient::CanAutoConnect()
{
    return (mHostFlag && mAutoConnFlag);  //>@如果为Host，并且使能了自动重连
}

void PhysicalLANClient::SetTcpClient(ToolTcpClient *pTcpClient, bool pSyncParUI, quint32 pSyncDelay)
{
    if(mTcpClient)
    {
        if(mTcpClient == pTcpClient)
            return;
        mTcpClient->Close();
        mTcpClient->deleteLater();
        mTcpClient = 0;
    }
    mTcpClient = pTcpClient;
    if(mTcpClient)
    {
        if(mTcpClient->isConnected())
        {
            mIP = mTcpClient->peerAddress();
            mPort = mTcpClient->peerPort();
        }

        disconnect(mTcpClient, SIGNAL(readyRead()), 0, 0);
        disconnect(mTcpClient, SIGNAL(sConnState(bool)), 0, 0);

        connect(mTcpClient, SIGNAL(readyRead()), this, SLOT(slot_ReadData()));
        connect(mTcpClient, SIGNAL(sConnState(bool)), this, SLOT(slot_ConnState(bool)));

        SetCurDev(P_TCP, mTcpClient);

        if(pSyncParUI)
        {
            if(pSyncDelay == 0)
                slot_UpdateConnState();
            else
                QTimer::singleShot(pSyncDelay, this, SLOT(slot_UpdateConnState()));
        }
    }
}

void PhysicalLANClient::slot_UpdateConnState()
{
    bool tmpFlag = isConnected();
    if(tmpFlag)
        Connected();
    else
        Disconnected();
    emit sConnState(tmpFlag);
}

bool PhysicalLANClient::isConnected()
{
    if(mTcpClient)
        return mTcpClient->isConnected();//>@此处不能使用m_Connected标志位，此标志位用于protocol中的判断
    return false;
}

bool PhysicalLANClient::Sync(quint32 pTimeout)
{
    if(!mTcpClient)
    {
        mTcpClient = new ToolTcpClient;
        connect(mTcpClient, SIGNAL(readyRead()), this, SLOT(slot_ReadData()));
        connect(mTcpClient, SIGNAL(sConnState(bool)), this, SLOT(slot_ConnState(bool)));
        SetCurDev(P_TCP, mTcpClient);
    }
    if(!isConnected())
    {
        if(mPort == 0)
            mPort = D_NMPORT;
        SetHostFlag(true);
        SeteAutoConnect(true);
        if(pTimeout == 0)
            mTcpClient->Open(mIP, mPort);
        else
            return mTcpClient->Open(mIP, mPort, pTimeout);
    }
    return true;
}

void PhysicalLANClient::Async(bool pSync)
{
    if(pSync)
        PhysicalHost::Async();
    if(mTcpClient)
    {
        disconnect(mTcpClient, SIGNAL(readyRead()), 0, 0);
        disconnect(mTcpClient, SIGNAL(sConnState(bool)), 0, 0);
        mTcpClient->Close();
        mTcpClient->deleteLater();
        mTcpClient = 0;

        IDE_DEBUG(QString("%1:%2 Async!").arg(mIP.toString()).arg(mPort));
    }
}

void PhysicalLANClient::Async()
{
    PhysicalHost::Async();
    if(mTcpClient)
    {
        disconnect(mTcpClient, SIGNAL(readyRead()), 0, 0);
        disconnect(mTcpClient, SIGNAL(sConnState(bool)), 0, 0);
        mTcpClient->Close();
        mTcpClient->deleteLater();
        mTcpClient = 0;

        IDE_DEBUG(QString("%1:%2 Async!").arg(mIP.toString()).arg(mPort));
    }
}

bool PhysicalLANClient::isSync(quint32 pTimeout)
{
    int i = 0;
    while(!isConnected() && ((i+=100) < pTimeout))
    {
        QtSleep(100);
    }
    return isConnected();
}

int PhysicalLANClient::GetSyncTime()
{
    if(mTcpClient)
        return mTcpClient->GetConnectedTime();
    return -1;
}

void PhysicalLANClient::slot_ConnState(bool pFlag)
{
    if(pFlag)
    {
        if(!m_Connected)   //>@不重复发送建立连接的信号
        {
            Connected();
            emit sConnState(pFlag);
        }
    }
    else
    {
        if(!mAutoConnState)
        {
            mAutoConnState = true;
            if(mHostFlag)  //>@主动连接别人的Socket的自动重连
            {
                mAutoConnLog = true;
                m_RetransFile = true;
                bool tmpFlag = false;
                //>@再次判断是否真的已经断开了连接
                if(mTcpClient)
                    tmpFlag = mTcpClient->isConnected();

                int tmpFreqBk = m_HBFreq[2].mNum;
                if(!tmpFlag)  //>@重连过程中，不能修改m_Connected的值
                {
                    //>@尝试重连的过程中需要禁能心跳
                    m_HBFreq[2].mNum = 0;
                    //>@
                    for(int i=0;i<3 && !tmpFlag && m_Connected;i++)
                        tmpFlag = Sync(300);
                }
                if(!tmpFlag && CanAutoConnect())
                {
                    mAutoConnFlag = false;//>@如果自动重连失败，则复位自动重连标志位

                    if(m_Connected)   //>@只允许执行一次Disconnected
                    {
                        Disconnected();
                        emit sConnState(pFlag);
                    }
                }
                else
                {
                    m_HBCount[2] = 0;
                    m_HBFreq[2].mNum = tmpFreqBk;
                    m_HBWait[2].update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
                    emit sHB(m_HBFreq[2].mNum);
                }
            }
            else                //>@服务器生成的被动连接的Socket
            {
                //>@主动Socket可能支持自动重连，所以等待
                bool tmpFlag = false;
                //>@等待重连的过程中需要禁能心跳，如果建立连接，则主动Socket会发来心跳更改num的。
                m_HBFreq[1].mNum = 0;
                for(int i=0;i<3 && !tmpFlag;i++)
                    tmpFlag = isSync(300);

                if(!tmpFlag)
                {
                    if(m_Connected)   //>@只允许执行一次Disconnected
                    {
                        Disconnected();
                        emit sConnState(pFlag);
                    }
                }
            }
            mAutoConnState = false;
        }
    }
}

void PhysicalLANClient::slot_HBTimeout(quint32 pID)
{
    if(pID == 1)            //>@如果一定时间没有接收到外部APP发来的心跳信号
    {
        Async(false);  //>@此处还没有确定一定断开连接了，因此不能执行disconnected函数
        emit sAutoConnect(false);
    }
    else if(pID == 2)       //>@如果一定时间没有接收到显示器响应的心跳信号(针对有时会出现m_TcpSocket读取是连接的但实际上已经断开的问题。)
    {
        //>@如果之前有自动重连过，则直接断开连接，否则尝试自动重连一次
        if(!mAutoConnLog)
        {
            emit sAutoConnect(false);
        }
        else
        {
            Async();
            emit sConnState(false);
        }
    }
}

qint64 PhysicalLANClient::write(const QByteArray &data)
{
    if(isConnected())
    {
        return mTcpClient->write(data);
    }
    return -1;
}

qint64 PhysicalLANClient::reply(QByteArray data)
{
    if(isConnected())
    {
        return mTcpClient->write(data);
    }
    return -1;
}

QByteArray PhysicalLANClient::readAll()
{
    if(isConnected())
    {
        return mTcpClient->readAll();
    }
    return QByteArray();
}

void PhysicalLANClient::slot_ReadData()
{
    ParseFrameBuffer(P_TCP, VAR_CAST<QIODevice *>(sender()), readAll());
}

void PhysicalLANClient::UpdateDevAddr()
{
    if(isSync())
    {
        mIP = mTcpClient->peerAddress();
        mPort = mTcpClient->peerPort();
    }
    mDevAddr = QString("%1:%2:%3").arg(mIP.toString()).arg(mPort).arg(mCode);
}

bool PhysicalLANClient::Download(QString pThemeName, QString pThemePath)   //>@此处的ThemeName是主题路径
{
    if(!isSync())
        return false;
    if(pThemeName.isEmpty())
        return false;
    mIsBusy = true;
    //>@下载到当前工程此主题的OutPut目录下
    QString tmpSrcPath = pThemePath + pThemeName;
    if(GetDir(tmpSrcPath, mBufferPath))
    {
        mIsBusy = false;
        return true;
    }
    mIsBusy = false;
    return false;
}

//>@如果系统已经存在同名主题，则覆盖，否则直接上传
bool PhysicalLANClient::Upload(QString pThemeName)
{
    if(!isSync())
        return false;
    return false;
}

bool PhysicalLANClient::DownloadPreference(QString pMdpName)
{
    if(!isSync())
        return false;
    return false;
}

bool PhysicalLANClient::UploadPreference(QString pMdpName)
{
    if(pMdpName.isEmpty())
        return false;
    if(!isSync())
        return false;
    return false;
}

PhysicalLANServer::PhysicalLANServer(QObject *parent):
    PhysicalProtocol(parent)
{
    mShowMessage = true;

    mMonitorWait.SetTimeout(10);

#ifdef WINDOWS
    mNetName = GBKToUTF8("本地连接");
    mIP = QHostAddress("10.200.23.158");
#else
    mNetName = QString("eth0");
    mIP = QHostAddress("192.168.0.10");
#endif

    mTCPSPort.setnum(D_NMPORT, 60000, 65000);
    mUDPPort.setnum(D_CMDPORT, 60000, 65000);

    mUdpDevice = 0;
    mTcpServer = 0;
    mDHCP = false;

    PhysicalProtocol::Connected();
}

PhysicalLANServer::~PhysicalLANServer()
{
    slot_ReleaseNetwork();

    QString tmpCMD = QString("ifconfig eth0 down &");
    IDE_DEBUG(tmpCMD);
#ifndef WINDOWS
    system(tmpCMD.toLatin1().data());
#endif
}

quint32 PhysicalLANServer::InitPara()
{
    quint32 tmpRet = PhysicalProtocol::InitPara();
#ifdef WINDOWS
    SetNetwork(mNetName, false, mIP, QHostAddress("255.255.0.0"), QHostAddress("10.200.0.254"), QHostAddress("10.200.0.1"));
#else
    QString tmpCMD;
    if(mDHCP)
    {
        QString tmpDHCPFile = D_FSRCPATH + D_SYSTEMDIR + QString("Physical/LANC/udhcpd.conf");
        SetDHCP(tmpDHCPFile, mIP.toString());

        tmpCMD = QString("ifconfig %1 %2 up | route add default dev %1 | udhcpd -f %3 &").arg(mNetName).arg(mIP.toString()).arg(tmpDHCPFile);
    }
    else
    {
        tmpCMD = QString("ifconfig %1 %2 up &").arg(mNetName).arg(mIP.toString());
    }
    system(tmpCMD.toLatin1().data());
#endif
    return tmpRet;
}

QHBoxLayout* PhysicalLANServer::InitSubPara(QString pName, QDomElement pElement)
{
    QHBoxLayout* tmpHBoxLayout = 0;
    if(!pName.compare("TCPS", Qt::CaseInsensitive))
    {
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()==2)
        {
            mTCPSPort.setnum(pElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
        }
        tmpHBoxLayout = CreateNum(pName, mTCPSPort.mNum, mTCPSPort.mMinNum, mTCPSPort.mMaxNum);
    }
    else if(!pName.compare("UDP", Qt::CaseInsensitive))
    {
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()==2)
        {
            mUDPPort.setnum(pElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
        }
        tmpHBoxLayout = CreateNum(pName, mUDPPort.mNum, mUDPPort.mMinNum, mUDPPort.mMaxNum);
    }
    else if(!pName.compare("IP", Qt::CaseInsensitive))
    {
        QString tmpValue = pElement.text();
        mIP = QHostAddress(tmpValue);
        if(!mIP.isNull())
        {
            tmpHBoxLayout = CreateTextEdit(pName, tmpValue);
        }
    }
    else if(!pName.compare("DHCP", Qt::CaseInsensitive))
    {
        QString tmpValue = pElement.text();
        mDHCP = (tmpValue.compare("true", Qt::CaseInsensitive))?(false):(true);
        tmpHBoxLayout = CreateBool(pName, tmpValue);
    }
    else if(!pName.compare("NetName", Qt::CaseInsensitive))
    {
        mNetName = pElement.text();
        tmpHBoxLayout = CreateTextEdit(pName, mNetName);
    }
    return tmpHBoxLayout;
}

void PhysicalLANServer::UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement)
{
    if(!pName.compare("TCPS", Qt::CaseInsensitive))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, mTCPSPort.mNum, tmpComponent->value(), mTCPSPort.mMinNum, mTCPSPort.mMaxNum))
            {
            }
        }
    }
    else if(!pName.compare("UDP", Qt::CaseInsensitive))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, mUDPPort.mNum, tmpComponent->value(), mUDPPort.mMinNum, mUDPPort.mMaxNum))
            {
            }
        }
    }
    else if(!pName.compare("IP", Qt::CaseInsensitive))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            QString tmpValue = mIP.toString();
            if(!ModifyTextEdit(tmpComponent, pElement, tmpValue, tmpComponent->text()))
            {
            }
            else
            {
                mIP = QHostAddress(tmpValue);
                QString tmpCMD;
                if(mDHCP)
                {
                    QString tmpDHCPFile = D_FSRCPATH + D_SYSTEMDIR + QString("Physical/LANC/udhcpd.conf");
                    SetDHCP(tmpDHCPFile, tmpValue);

                    tmpCMD = QString("ifconfig eth0 down | ifconfig eth0 %1 up | udhcpd -f % &").arg(mIP.toString()).arg(tmpDHCPFile);
                }
                else
                {
                    tmpCMD = QString("ifconfig eth0 down | ifconfig eth0 %1 up &").arg(mIP.toString());
                }
                IDE_DEBUG(tmpCMD);
#ifndef WINDOWS
                system(tmpCMD.toLatin1().data());
#endif
            }
        }
    }
    else if(!pName.compare("DHCP", Qt::CaseInsensitive))
    {
        Switch *tmpComponent = qobject_cast<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, pElement, mDHCP, tmpComponent->isChecked()))
            {
            }
        }
    }
    else if(!pName.compare("NetName", Qt::CaseInsensitive))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyTextEdit(tmpComponent, pElement, mNetName, tmpComponent->text()))
            {
            }
            else
            {
            }
        }
    }
}

quint32 PhysicalLANServer::Monitor(quint32 pCount)
{
    PhysicalLANClient *tmpTcpClient = 0;
    foreach(tmpTcpClient, mClients)
    {
        if(tmpTcpClient)
        {
            tmpTcpClient->Monitor(pCount);
        }
    }
}

void PhysicalLANServer::slot_Initialize()
{
    PhysicalProtocol::slot_Initialize();
    QTimer::singleShot(1000, this, SLOT(slot_InitNetwork()));
}

void PhysicalLANServer::slot_ReleaseNetwork()
{
    if(mUdpDevice)
    {
        mUdpDevice->deleteLater();
        mUdpDevice = 0;
    }
    if(mTcpServer)
    {
        mTcpServer->deleteLater();
        mTcpServer = 0;
    }
}

void PhysicalLANServer::slot_InitNetwork()
{
    if(!mUdpDevice)
    {
        mUdpDevice = new ToolUdp;
        mUdpDevice->OpenUdp(mUDPPort.mNum);
        connect(mUdpDevice, SIGNAL(readyRead()), this, SLOT(slot_ReadData()), Qt::QueuedConnection);
    }
    if(!mTcpServer)
    {
        mTcpServer = new ToolTcpServer(mTCPSPort.mNum);
        connect(mTcpServer, SIGNAL(sNewSocket(QTcpSocket*)), this, SLOT(slot_NewSocket(QTcpSocket*)));
    }
    if(m_DevManager)
        mMonitorWait.update(mMonitorCount, m_DevManager->m_DevMonitorFreq);
}

bool PhysicalLANServer::isConnected()
{
    PhysicalLANClient *tmpTcpClient = 0;
    foreach(tmpTcpClient, mClients)
    {
        if(tmpTcpClient && tmpTcpClient->isConnected())
        {
            IDE_DEBUG(QString("%1:%2 is connected!").arg(tmpTcpClient->mIP.toString()).arg(tmpTcpClient->mPort));
            return true;
        }
    }
    return false;
}

void PhysicalLANServer::slot_NewSocket(QTcpSocket *pSocket)
{
    bool pNewConn = true;
    PhysicalLANClient *tmpTcpClient = 0;
    foreach(tmpTcpClient, mClients)
    {
        //>@因为是服务器端，所以只要比较IP地址即可(默认一个IP只能连接一次)
        if(tmpTcpClient && (tmpTcpClient->mIP == pSocket->peerAddress()))
        {
            pNewConn = false;
            break;
        }
    }
    if(pNewConn)  //>@如果当前缓冲区内没有某个连接，则新增
    {
        tmpTcpClient = new PhysicalLANClient;
        tmpTcpClient->SetDeviceManager(m_DevManager);
        connect(tmpTcpClient, SIGNAL(sConnState(bool)), this, SLOT(slot_ConnState(bool)));
        mClients.append(tmpTcpClient);
        tmpTcpClient->slot_Initialize();
        tmpTcpClient->SetHostFlag(false);
    }
    tmpTcpClient->SetTcpClient(VAR_CAST<ToolTcpClient*>(pSocket), true, 0);  //>@执行后立即进行状态判断并发出sConnState信号
}

void PhysicalLANServer::slot_ConnState(bool pFlag)
{
    PhysicalLANClient *tmpTcpClient = VAR_CAST<PhysicalLANClient*>(sender());
    if(tmpTcpClient)
    {
        if(pFlag)
        {
            Connected();
            if(mShowMessage) //>@在Msg中显示一维码
            {
                emit sInfo(QString("%1:%2 Connected!").arg(tmpTcpClient->mIP.toString()).arg(tmpTcpClient->mPort), 2000);
            }
        }
        else
        {
            Disconnected();
            if(mShowMessage)  //>@在Msg中显示一维码
            {
                emit sInfo(QString("%1:%2 Disconnected!").arg(tmpTcpClient->mIP.toString()).arg(tmpTcpClient->mPort), 2000);
            }
        }
        emit sNetState(pFlag);
    }
}

bool PhysicalLANServer::AddClient(PhysicalLANClient *pLanClient)
{
    if(!mClients.contains(pLanClient))
        mClients.append(pLanClient);
    return true;
}

PhysicalLANClient* PhysicalLANServer::AddClient(QHostAddress pIP, quint16 pPort, bool &pNewConn)
{
    pNewConn = true;
    PhysicalLANClient *tmpTcpClient = 0;
    foreach(tmpTcpClient, mClients)
    {
        if(tmpTcpClient && (tmpTcpClient->mIP == pIP) && (tmpTcpClient->mPort == pPort))
        {
            pNewConn = false;
            break;
        }
    }
    if(pNewConn)  //>@如果当前缓冲区内没有某个连接，则新增
    {
        tmpTcpClient = new PhysicalLANClient;
        tmpTcpClient->SetDeviceManager(m_DevManager);
        tmpTcpClient->mIP = pIP;
        tmpTcpClient->mPort = pPort;
        tmpTcpClient->slot_Initialize();
        mClients.append(tmpTcpClient);
    }
    return tmpTcpClient;
}

void PhysicalLANServer::DelClient(PhysicalLANClient *pLanClient)
{
    if(pLanClient)
    {
        if(mClients.contains(pLanClient))
        {
            mClients.removeOne(pLanClient);
        }
        pLanClient->deleteLater();
    }
}

PhysicalLANClient* PhysicalLANServer::Connect(QHostAddress pIP, quint16 pPort, quint32 pTimeout)
{
    PhysicalLANClient *tmpTcpClient = new PhysicalLANClient;
    tmpTcpClient->SetDeviceManager(m_DevManager);
    tmpTcpClient->mIP = pIP;
    tmpTcpClient->mPort = pPort;
    tmpTcpClient->slot_Initialize();
    tmpTcpClient->Sync(pTimeout);
    return tmpTcpClient;
}

bool PhysicalLANServer::DelClient(QHostAddress pIP, quint16 pPort)
{
    bool find = false;
    PhysicalLANClient *tmpTcpClient = 0;
    foreach(tmpTcpClient, mClients)
    {
        if(tmpTcpClient)
        {
            if(tmpTcpClient->mIP == pIP && tmpTcpClient->mPort == pPort)
            {
                find = true;
                break;
            }
        }
    }
    if(find)
        DelClient(tmpTcpClient);
    return find;
}

void PhysicalLANServer::slot_ReadData()
{
    ParseFrameBuffer(P_UDP, VAR_CAST<QIODevice*>(sender()), readAll());
}

WifiOperate::WifiOperate(QObject *parent) :
    QThread(parent)
{
    m_OperateType = -1;
}

WifiOperate::~WifiOperate()
{
    ForceQuit();
    wait(1000);
}

void WifiOperate::ForceQuit()
{
    quit();
}

void WifiOperate::Wait(int pTimeout)
{
    quint32 i=0;
    while((i+=10) < pTimeout)
        msleep(10);
}

bool WifiOperate::Open(QHostAddress pIpAddr, bool pForce)
{
    static bool busy = false;
    if(busy)
        return false;
    busy = true;
    if(isRunning())
    {
        if(pForce)
        {
            ForceQuit();
            QtSleep(1000);
        }
        else
        {
            IDE_TRACE();
            busy = false;
            return false;
        }
    }
    m_IpAddr = pIpAddr;
    m_OperateType = 1;
    start(LowestPriority);
    busy = false;
}

bool WifiOperate::Close(bool pForce)
{
    static bool busy = false;
    if(busy)
        return false;
    busy = true;
    if(isRunning())
    {
        if(pForce)
        {
            ForceQuit();
            QtSleep(1000);
        }
        else
        {
            IDE_TRACE();
            busy = false;
            return false;
        }
    }
    m_OperateType = 2;
    start(LowestPriority);
    busy = false;
}

void WifiOperate::run()
{
    switch(m_OperateType)
    {
        case 1:  //>@open
        {
            QString tmpCMD = QString("ifconfig ra0 %1 up | route add default dev ra0 | udhcpd -f /usr/bst/SYSTEM/Middleware/WIFI/udhcpd.conf &").arg(m_IpAddr.toString());
            IDE_DEBUG(tmpCMD);
#ifndef WINDOWS
            system(tmpCMD.toLatin1().data());
#endif
            break;
        }
        case 2:   //>@close
        {
            QString tmpCMD = QString("ifconfig ra0 down | killall udhcpd &");
            IDE_DEBUG(tmpCMD);
#ifndef WINDOWS
            system(tmpCMD.toLatin1().data());
#endif
            break;
        }
        default:
        {
            break;
        }
    }
}

PhysicalLANC::PhysicalLANC(QObject *parent):
    PhysicalLANServer(parent)
{
    mDevType = P_LANC;

    m_RouterClient = 0;
    mMonitorWait.SetTimeout(0);  //>@默认不自动进行连接
    m_RouterTimeout.setnum(0, 0, 60000);
    m_RouterPort.setnum(D_NMPORT, 60000, 65000);

    m_WifiIpAddr = QHostAddress("192.168.1.10");
    m_WifiBootON = false;
    mShowMessage = !m_WifiBootON;

    m_WifiOpened = false;
#ifdef SUPPORT_MUTEX
    m_WifiMutex = new QMutex(QMutex::Recursive);
#else
    m_WifiMutex = false;
#endif

#ifdef DISPLAY
    SYSTEM_CMD::SetWIFISSID(m_DevManager->mDevCode);
#endif
}

PhysicalLANC::~PhysicalLANC()
{
    CloseWIFI();

#ifdef SUPPORT_MUTEX
    if(m_WifiMutex)
        delete m_WifiMutex;
#else
    m_WifiMutex = false;
#endif
}

quint32 PhysicalLANC::InitPara()
{
    quint32 tmpRet = PhysicalLANServer::InitPara();
    if(m_WifiBootON)
        OpenWIFI();
    return tmpRet;
}

QHBoxLayout* PhysicalLANC::InitSubPara(QString pName, QDomElement pElement)
{
    QHBoxLayout* tmpHBoxLayout = PhysicalLANServer::InitSubPara(pName, pElement);
    if(tmpHBoxLayout)
        return tmpHBoxLayout;
    if(!pName.compare("WIFION", Qt::CaseInsensitive))
    {
        QString tmpValue = pElement.text();
        m_WifiBootON = (tmpValue.compare("true", Qt::CaseInsensitive))?(false):(true);
        mShowMessage = !m_WifiBootON;
        tmpHBoxLayout = CreateBool(pName, tmpValue);
    }
    else if(!pName.compare("WIFIIP", Qt::CaseInsensitive))
    {
        QString tmpValue = pElement.text();
        m_WifiIpAddr = QHostAddress(tmpValue);
        tmpHBoxLayout = CreateTextEdit(pName, tmpValue);

        //QString tmpDHCPFile = D_FSRCPATH + D_SYSTEMDIR + QString("Middleware/WIFI/udhcpd.conf");
        //SetDHCP(tmpDHCPFile, tmpValue);
    }
    else if(!pName.compare("RouterIP", Qt::CaseInsensitive))
    {
        QString tmpValue = pElement.text();
        m_RouterIpAddr = QHostAddress(tmpValue);
        tmpHBoxLayout = CreateTextEdit(pName, tmpValue);
    }
    else if(!pName.compare("RouterPort", Qt::CaseInsensitive))
    {
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()==2)
        {
            m_RouterPort.setnum(pElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
        }
        tmpHBoxLayout = CreateNum(pName, m_RouterPort.mNum, m_RouterPort.mMinNum, m_RouterPort.mMaxNum);
    }
    else if(!pName.compare("RouterTimeout"))
    {
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()==2)
        {
            m_RouterTimeout.setnum(pElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
        }
        tmpHBoxLayout = CreateNum(pName, m_RouterTimeout.mNum, m_RouterTimeout.mMinNum, m_RouterTimeout.mMaxNum);
        if(m_RouterTimeout.mNum)
        {
            m_RouterWait.SetTimeout(m_RouterTimeout.mNum);
            m_RouterWait.update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
        }
    }
    return tmpHBoxLayout;
}

void PhysicalLANC::UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement)
{
    PhysicalLANServer::UpdateSubPara(pName, pObject, pElement);
    if(!pName.compare("WIFION", Qt::CaseInsensitive))
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, pElement, m_WifiBootON, tmpComponent->isChecked()))
            {
            }
            else
            {
                mShowMessage = !m_WifiBootON;
            }
        }
    }
    else if(!pName.compare("WIFIIP", Qt::CaseInsensitive))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            QString tmpValue = m_WifiIpAddr.toString();
            if(!ModifyTextEdit(tmpComponent, pElement, tmpValue, tmpComponent->text()))
            {
            }
            else
            {
                m_WifiIpAddr = QHostAddress(tmpValue);

                QString tmpDHCPFile = D_FSRCPATH + D_SYSTEMDIR + QString("Middleware/WIFI/udhcpd.conf");
                SetDHCP(tmpDHCPFile, tmpValue);
            }
        }
    }
    else if(!pName.compare("RouterIP", Qt::CaseInsensitive))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            QString tmpValue = m_RouterIpAddr.toString();
            if(!ModifyTextEdit(tmpComponent, pElement, tmpValue, tmpComponent->text()))
            {
            }
            else
            {
                m_RouterIpAddr = QHostAddress(tmpValue);
            }
        }
    }
    else if(!pName.compare("RouterPort", Qt::CaseInsensitive))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, m_RouterPort.mNum, tmpComponent->value(), m_RouterPort.mMinNum, m_RouterPort.mMaxNum))
            {
            }
        }
    }
    else if(!pName.compare("RouterTimeout"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, m_RouterTimeout.mNum, tmpComponent->value(), m_RouterTimeout.mMinNum, m_RouterTimeout.mMaxNum))
            {
            }
            else
            {
                if(m_RouterTimeout.mNum)
                {
                    m_RouterWait.SetTimeout(m_RouterTimeout.mNum);
                    m_RouterWait.update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
                }
            }
        }
    }
}

quint32 PhysicalLANC::Monitor(quint32 pCount)
{
    PhysicalLANServer::Monitor(pCount);

    if(m_RouterClient)
        m_RouterClient->Monitor(pCount);

    if(m_RouterTimeout.mNum && !m_RouterIpAddr.isNull() && m_RouterPort.mNum && m_RouterWait.isValid(pCount, m_DevManager->m_DevMonitorFreq))
    {
        emit sConnect();
    }
}

void PhysicalLANC::slot_InitNetwork()
{
    PhysicalLANServer::slot_InitNetwork();

#if (defined(IDE) || defined(DISPLAY))
    emit sInfo(QString("WIFI : %1").arg(m_DevManager->mDevCode), 1000, DM_NET);
#endif

    //>@第一次建立与服务器的连接
    QTimer::singleShot(1000, this, SLOT(slot_Connect())); //>@必须延时启动连接，否则可能在界面没有显示时建立连接，此时会没有WIFI图标显示。
}

void PhysicalLANC::slot_Connect()
{
    if(!m_RouterClient)
    {
        m_RouterClient = new PhysicalLANClient(new ToolTcpClient, m_DevManager);
        connect(m_RouterClient, SIGNAL(sConnState(bool)), this, SLOT(slot_RouterConnState(bool)));
        m_RouterClient->mIP = m_RouterIpAddr;
        m_RouterClient->mPort = m_RouterPort.mNum;
        m_RouterClient->SeteAutoConnect(true);
        m_RouterClient->slot_Initialize();
    }
    if(!m_RouterClient->mTcpClient)
        m_RouterClient->SetTcpClient(new ToolTcpClient);
    if(!m_RouterClient->isConnected())
    {
        slot_RouterConnState(false);
        m_RouterClient->Sync();
    }
    if(m_RouterTimeout.mNum)
        m_RouterWait.update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
}

void PhysicalLANC::slot_RouterConnState(bool pFlag)
{
    if(pFlag)
    {
#ifdef DISPLAY
        emit sShowIcon("router");
#endif
    }
    else
    {
#ifdef DISPLAY
        emit sHideIcon("router");
#endif
    }
}

void PhysicalLANC::Connected()
{
    //PhysicalLANServer::Connected();  //>@已经在创建对象时强制设置为已连接了。
#ifndef SERVER
#ifdef DISPLAY   //>@建立连接后，显示图标
    if(m_LCD)
        m_LCD->slot_SrnSaveMode(false);
    emit sShowIcon("net");
#ifndef SUPPORT_WIDGET
    emit sSetDevices(false, DM_NET);
#endif
#endif
#endif
}

void PhysicalLANC::Disconnected()
{
#ifndef SERVER
#ifdef DISPLAY  //>@断开连接后，播放声音，显示图标
    //>@查看当前是否还有有效连接
    if(!isConnected())
    {
        MEDIA_PLAYER *tmpPlayer = mPlayer.data()->GetMediaPlayer(AO_LIFTARV);
        if(tmpPlayer)
        {
            tmpPlayer->Abort();
        }

        emit sHideIcon("net");
#ifndef SUPPORT_WIDGET
        emit sSetDevices(true, DM_NET);
#endif
        //>@如果是CANOPEN设备，则需要查询当前在什么模式
        PhysicalCOM *tmpCOM = VAR_CAST<PhysicalCOM*>(m_DevManager->m_PHDevList.value(P_COM));
        if(tmpCOM)
        {
            IDE_TRACE();
            tmpCOM->SetCanopenMode(tmpCOM->mCanopenMode);
        }
    }
#endif
#endif
}

void PhysicalLANC::slot_ConnState(bool pFlag)
{
    PhysicalLANServer::slot_ConnState(pFlag);
    if(!pFlag && mShowMessage)
    {
        emit sInfo(QString("WIFI : %1").arg(m_DevManager->mDevCode), MSG_PERMANENT, DM_NET);
    }
}

bool PhysicalLANC::SetValue(QString pValue)
{
#ifndef PART_TEST
    if(pValue.isEmpty())
        return true;
    QStringList tmpList = pValue.split(';');
    for(int i=0;i<tmpList.count();i++)
    {
        QStringList tmpCmdList = tmpList.at(i).split(':');
        if(tmpCmdList.count()!=2)
            continue;
        QString tmpName = tmpCmdList.at(0);
        QString tmpContent = tmpCmdList.at(1);
        if(!tmpName.compare("wifi", Qt::CaseInsensitive))  //>@表明开启
        {
            if(!tmpContent.compare("open", Qt::CaseInsensitive))
            {
#ifdef DISPLAY
                //>@查询当前是否有COM设备，如果有，则由COM设备管理WIFI
                PhysicalCOM *tmpCOM = 0;
            #ifdef IDE
                tmpCOM = VAR_CAST<PhysicalCOM*>(m_DevManager->m_PHDevList.value(P_COM));
            #elif DISPLAY
                tmpCOM = VAR_CAST<PhysicalCOM*>(m_DevManager->m_PHDevList.value(P_COM));
            #endif
                if(tmpCOM)
                    tmpCOM->OpenWIFI();
                else
                    OpenWIFI();
#endif
            }
            else if(!tmpContent.compare("close", Qt::CaseInsensitive))
            {
#ifdef DISPLAY
#ifndef SODIMAS
                //>@查询当前是否有COM设备，如果有，则由COM设备管理WIFI
                PhysicalCOM *tmpCOM = 0;
            #ifdef IDE
                tmpCOM = VAR_CAST<PhysicalCOM*>(m_DevManager->m_PHDevList.value(P_COM));
            #elif DISPLAY
                tmpCOM = VAR_CAST<PhysicalCOM*>(m_DevManager->m_PHDevList.value(P_COM));
            #endif
                if(tmpCOM)
                    tmpCOM->CloseWIFI();
                else
                    CloseWIFI();
#endif
#endif
            }
        }
    }
#endif
    return true;
}

bool PhysicalLANC::PrepareRelease()
{
    CloseWIFI(true);

    QString tmpCMD = QString("ifconfig ra0 down | killall udhcpd &");
    IDE_DEBUG(tmpCMD);
#ifndef WINDOWS
    system(tmpCMD.toLatin1().data());
#endif
}

void PhysicalLANC::OpenWIFI(bool pForce)
{
#ifdef SUPPORT_MUTEX
    if(!m_WifiMutex->tryLock())
#else
    if(m_WifiMutex)
#endif
    {
        IDE_DEBUG(QString("Can't Open WIFI for mutex locked!"));
        return;
    }
#ifndef SUPPORT_MUTEX
    m_WifiMutex = true;
#endif

    if(!pForce && m_WifiOpened)
    {
#ifdef SUPPORT_MUTEX
        m_WifiMutex->unlock();
#else
        m_WifiMutex = false;
#endif
        return;
    }

    //>@如果已经wifi打开，则不再显示CODE
    //>@在Msg中显示一维码
#if (defined(IDE) || defined(DISPLAY))
    if(!m_WifiBootON)
        emit sInfo(QString("WIFI : %1").arg(m_DevManager->mDevCode), MSG_PERMANENT, DM_NET);
#endif

    m_WifiOpened = true;
#if 0
    QString tmpCMD = QString("ifconfig ra0 %1 up | route add default dev ra0 | udhcpd -f /usr/bst/SYSTEM/Middleware/WIFI/udhcpd.conf &").arg(m_IpAddr.toString());
    IDE_DEBUG(tmpCMD);
#ifndef WINDOWS
    system(tmpCMD.toLatin1().data());
#endif
#else
    m_WifiOperate.Open(m_WifiIpAddr, true);
#endif

#ifdef SUPPORT_MUTEX
    m_WifiMutex->unlock();
#else
    m_WifiMutex = false;
#endif
}

void PhysicalLANC::CloseWIFI(bool pForce)
{
    if(!pForce && m_WifiBootON)
    {
        return;
    }
#ifdef SUPPORT_MUTEX
    if(!m_WifiMutex->tryLock())
#else
    if(m_WifiMutex)
#endif
    {
        IDE_DEBUG(QString("Can't Close WIFI for mutex locked!"));
        return;
    }
#ifndef SUPPORT_MUTEX
    m_WifiMutex = true;
#endif

    if(!m_WifiOpened)
    {
#ifdef SUPPORT_MUTEX
        m_WifiMutex->unlock();
#else
        m_WifiMutex = false;
#endif
        return;
    }

#ifndef SERVER
#ifdef DISPLAY  //>@确保断开连接后使图标消失。
    emit sHideIcon("net");
    m_Connected = false;
#endif
#endif

    //>@在Msg中显示一维码
    emit sInfo(QString(), MSG_TEMPORARY, DM_NET);
    emit sInfo(QString("WIFI closed!"), 1000);

#if 0
    QString tmpCMD = QString("ifconfig ra0 down | killall udhcpd &");
    IDE_DEBUG(tmpCMD);
#ifndef WINDOWS
    system(tmpCMD.toLatin1().data());
#endif
#else
    m_WifiOperate.Close(true);
#endif
    m_WifiOpened = false;

#ifdef SUPPORT_MUTEX
    m_WifiMutex->unlock();
#else
    m_WifiMutex = false;
#endif
}

qint64 PhysicalLANC::write(const QByteArray &data)
{
    if(mUdpDevice)
    {
        return mUdpDevice->write(data);
    }
    return -1;
}

qint64 PhysicalLANC::reply(QByteArray data)
{
    if(mUdpDevice)
    {
        return mUdpDevice->reply(data);
    }
    return -1;
}

QByteArray PhysicalLANC::readAll()
{
    if(mUdpDevice)
    {
        return mUdpDevice->readAll();
    }
    return QByteArray();
}

PhysicalLANR::PhysicalLANR(QObject *parent):
    PhysicalLANServer(parent)
{
    mDevType = P_LANR;
    mMonitorWait.SetTimeout(0);  //>@默认不自动进行连接

    m_ServerClient = 0;
    m_ServerTimeout.setnum(0, 0, 60000);
    m_ServerPort.setnum(D_NMPORT, 60000, 65000);
}

PhysicalLANR::~PhysicalLANR()
{
}

QHBoxLayout* PhysicalLANR::InitSubPara(QString pName, QDomElement pElement)
{
    QHBoxLayout* tmpHBoxLayout = PhysicalLANServer::InitSubPara(pName, pElement);
    if(tmpHBoxLayout)
        return tmpHBoxLayout;
    if(!pName.compare("ServerIP", Qt::CaseInsensitive))
    {
        QString tmpValue = pElement.text();
        m_ServerIpAddr = QHostAddress(tmpValue);
        tmpHBoxLayout = CreateTextEdit(pName, tmpValue);
    }
    else if(!pName.compare("ServerPort", Qt::CaseInsensitive))
    {
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()==2)
        {
            m_ServerPort.setnum(pElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
        }
        tmpHBoxLayout = CreateNum(pName, m_ServerPort.mNum, m_ServerPort.mMinNum, m_ServerPort.mMaxNum);
    }
    else if(!pName.compare("ServerTimeout"))
    {
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()==2)
        {
            m_ServerTimeout.setnum(pElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
        }
        tmpHBoxLayout = CreateNum(pName, m_ServerTimeout.mNum, m_ServerTimeout.mMinNum, m_ServerTimeout.mMaxNum);
        if(m_ServerTimeout.mNum)
        {
            m_ServerWait.SetTimeout(m_ServerTimeout.mNum);
            m_ServerWait.update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
        }
    }
    return tmpHBoxLayout;
}

void PhysicalLANR::UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement)
{
    PhysicalLANServer::UpdateSubPara(pName, pObject, pElement);
    if(!pName.compare("ServerIP", Qt::CaseInsensitive))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            QString tmpValue = m_ServerIpAddr.toString();
            if(!ModifyTextEdit(tmpComponent, pElement, tmpValue, tmpComponent->text()))
            {
            }
            else
            {
                m_ServerIpAddr = QHostAddress(tmpValue);
            }
        }
    }
    else if(!pName.compare("ServerPort", Qt::CaseInsensitive))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, m_ServerPort.mNum, tmpComponent->value(), m_ServerPort.mMinNum, m_ServerPort.mMaxNum))
            {
            }
        }
    }
    else if(!pName.compare("ServerTimeout"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, m_ServerTimeout.mNum, tmpComponent->value(), m_ServerTimeout.mMinNum, m_ServerTimeout.mMaxNum))
            {
            }
            else
            {
                if(m_ServerTimeout.mNum)
                {
                    m_ServerWait.SetTimeout(m_ServerTimeout.mNum);
                    m_ServerWait.update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
                }
            }
        }
    }
}

void PhysicalLANR::slot_InitNetwork()
{
    PhysicalLANServer::slot_InitNetwork();

    //>@第一次建立与服务器的连接
    QTimer::singleShot(1000, this, SLOT(slot_Connect())); //>@必须延时启动连接，否则可能在界面没有显示时建立连接，此时会没有WIFI图标显示。
}

void PhysicalLANR::slot_Connect()
{
    if(!m_ServerClient)
    {
        m_ServerClient = new PhysicalLANClient(new ToolTcpClient, m_DevManager);
        connect(m_ServerClient, SIGNAL(sConnState(bool)), this, SLOT(slot_ServerConnState(bool)));
        m_ServerClient->mIP = m_ServerIpAddr;
        m_ServerClient->mPort = m_ServerPort.mNum;
        m_ServerClient->SeteAutoConnect(true);
        m_ServerClient->slot_Initialize();
    }
    if(!m_ServerClient->mTcpClient)
        m_ServerClient->SetTcpClient(new ToolTcpClient);
    if(!m_ServerClient->isConnected())
    {
        slot_ServerConnState(false);
        m_ServerClient->Sync();
    }
    if(m_ServerTimeout.mNum)
        m_ServerWait.update(m_MonitorCount, m_DevManager->m_DevMonitorFreq);
}

void PhysicalLANR::slot_ServerConnState(bool pFlag)
{
    if(pFlag)
    {
#ifdef DISPLAY
        emit sShowIcon("server");
#endif
    }
    else
    {
#ifdef DISPLAY
        emit sHideIcon("server");
#endif
    }
}

quint32 PhysicalLANR::Monitor(quint32 pCount)
{
    PhysicalLANServer::Monitor(pCount);

    if(m_ServerClient)
        m_ServerClient->Monitor(pCount);

    if(m_ServerTimeout.mNum && !m_ServerIpAddr.isNull() && m_ServerPort.mNum && m_ServerWait.isValid(pCount, m_DevManager->m_DevMonitorFreq))
    {
        emit sConnect();
    }
}

QStringList PhysicalLANR::Scan(QString pContent, quint32 pTimeout)
{
    PH_Type tmpType = m_CurDevType;
    QIODevice* tmpDev = m_CurTransDev.data();
    SetCurDev(P_UDP, mUdpDevice);
    QStringList tmpList = PhysicalProtocol::Scan(pContent, pTimeout);
    SetCurDev(tmpType, tmpDev);
    return tmpList;
}

//>@返回新增加的设备节点
QList<PhysicalLANClient*> PhysicalLANR::ScanDevices(quint32 pTimeout)
{
    QList<PhysicalLANClient*> tmpDevList;
    QStringList tmpList = Scan(QString(""), pTimeout);
    QString tmpDevStr;
    foreach(tmpDevStr, tmpList)
    {
        QHostAddress tmpAddr;
        QString tmpCode;
        quint16 tmpPort;
        if(!ParseIP(tmpDevStr, tmpAddr, tmpPort, tmpCode))
            continue;
        if(tmpCode.isEmpty())
            continue;
        //>@新增一个节点
        bool tmpNewConn = false;
        PhysicalLANClient* tmpTcpClient = AddClient(tmpAddr, D_NMPORT, tmpNewConn);
        if(!tmpTcpClient)
            continue;
        tmpTcpClient->mCode = tmpCode;
        //>@如果此节点之前为未发现状态，则更改此节点状态为发现状态
        if(tmpTcpClient->mDevState == S_OFF)
            tmpTcpClient->mDevState == S_ON;
        //>@
        if(tmpNewConn)
            tmpDevList.append(tmpTcpClient);
    }
    return tmpDevList;
}

void PhysicalLANR::slot_Init(QVariant pPara)
{
    PhysicalLANClient *tmpClient = VAR_CAST<PhysicalLANClient*>(sender());
    if(tmpClient)
    {
        QStringList tmpParaList = pPara.toStringList();
        if(tmpParaList.count() >= 3)
        {
            tmpClient->mCode = tmpParaList.at(0);
            tmpClient->mPasswd = tmpParaList.at(1);
            tmpClient->mLocation = tmpParaList.at(2);
        }
    }
}


