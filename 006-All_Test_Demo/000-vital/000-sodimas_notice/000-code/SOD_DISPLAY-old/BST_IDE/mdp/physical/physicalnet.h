#ifndef PHYSICALNETWORK_H
#define PHYSICALNETWORK_H

#include "physicalprotocol.h"
#include "tooltcp.h"
#include "tooludp.h"
#include <QHostAddress>

class PhysicalLANClient : public PhysicalHost
{
    Q_OBJECT
public:
    PhysicalLANClient(ToolTcpClient *pTcpClient, QObject *pParent = 0);
    PhysicalLANClient(QString pAddr=QString(), QString pBufferPath=QString(), QObject *pParent = 0);
    ~PhysicalLANClient();

    void SetDestoryFlag(bool pFlag) {mDestoryFlag = pFlag;}

    bool CanAutoConnect();

    void SetTcpClient(ToolTcpClient *pTcpClient, bool pSync = false, quint32 pSyncDelay = 100);  //pSync表示当前的SetTcpClient操作是否需要同步一下其它类或者界面，一般只有PhysicalLANClient更换TcpClient时才会使用true

    bool Sync(quint32 pTimeout = 0);
    void Async();
    void Async(bool pSync);
    bool isConnected();
    bool isSync(quint32 pTimeout = 0);
    int  GetSyncTime();

    qint64 write(const QByteArray &data);
    qint64 reply(QByteArray data);
    QByteArray readAll();

    void UpdateDevAddr();    //>@支持10.200.19.1:8080或者10.200.19.1两种格式

    //>@先将boot以及map文件拷贝至当前主题中，再下载
    bool Download(QString pThemeName, QString pThemePath=QString());
    bool Upload(QString pThemeName);

    bool DownloadPreference(QString pMdpName);
    bool UploadPreference(QString pMdpName);

signals:
    void sAutoConnect(bool pFlag);

public slots:
    void slot_ConnState(bool pFlag);
    void slot_ReadData();
    void slot_UpdateConnState();
    void slot_HBTimeout(quint32 pID);

public:
    QHostAddress                 mIP;
    quint16                      mPort;

    QString                      mCode;
    QString                      mPasswd;
    QString                      mLocation;

    bool                         mDestoryFlag;

    ToolTcpClient               *mTcpClient;
};

class PhysicalLANServer : public PhysicalProtocol
{
    Q_OBJECT
public:
    PhysicalLANServer(QObject *pParent = 0);
    ~PhysicalLANServer();

    virtual quint32 InitPara();
    virtual QHBoxLayout* InitSubPara(QString pName, QDomElement pElement);
    virtual void UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement);

    quint32 Monitor(quint32 pCount);

    bool isConnected();  //>@指客户机中有没有连接着的

    //>@主动连接，不存储在mClients，仅仅返回一个PhysicalLANClient
    PhysicalLANClient* Connect(QHostAddress pAddr, quint16 pPort, quint32 pTimeout = 0);

    bool AddClient(PhysicalLANClient *pLanClient);
    PhysicalLANClient* AddClient(QHostAddress pAddr, quint16 pPort, bool &pNewConn);
    bool DelClient(QHostAddress pAddr, quint16 pPort);
    void DelClient(PhysicalLANClient *pLanClient);

signals:
    void sNetState(bool pFlag);
    void sAbortAll();

public slots:
    virtual void slot_Initialize();
    virtual void slot_InitNetwork();
    void slot_ReleaseNetwork();

    virtual void slot_NewSocket(QTcpSocket *pSocket);
    virtual void slot_ConnState(bool pFlag);

    void slot_ReadData();

public:
    bool                         mShowMessage;
    int                          mMonitorCount;

    QString                      mNetName;
    QHostAddress                 mIP;   //>@创建的服务器的IP
    bool                         mDHCP;
    NUM_INFO                     mTCPSPort;
    NUM_INFO                     mUDPPort;

    ToolUdp                     *mUdpDevice;
    ToolTcpServer               *mTcpServer;    //>@用于建立服务器，允许注册应用
    QList<PhysicalLANClient*>    mClients;      //>@外部发起的连接同网段客户机群
};

class WifiOperate : public QThread
{
    Q_OBJECT
public:
    WifiOperate(QObject *parent = 0);
    ~WifiOperate();

    bool Open(QHostAddress pIpAddr, bool pForce=false);
    bool Close(bool pForce=false);

    void ForceQuit();
    void Wait(int pTimeout);
    void run();

public:
    int                        m_OperateType;
    QHostAddress               m_IpAddr;
};

/*
  专指电梯上的显示器设备
    增加了WIFI AP的功能
    自动连接服务器
*/
class PhysicalLANC : public PhysicalLANServer
{
    Q_OBJECT
public:
    PhysicalLANC(QObject *pParent = 0);
    ~PhysicalLANC();
    bool PrepareRelease();

    quint32 InitPara();
    QHBoxLayout* InitSubPara(QString pName, QDomElement pElement);
    void UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement);

    bool SetValue(QString pValue);

    void OpenWIFI(bool pForce=false);
    void CloseWIFI(bool pForce=false);

    quint32 Monitor(quint32 pCount);

    void Connected();
    void Disconnected();

    //>@读写函数均是PhysicalUdp作为从机时的操作
    qint64 write(const QByteArray &data);
    qint64 reply(QByteArray data);
    QByteArray readAll();

public slots:
    void slot_InitNetwork();

    void slot_Connect();
    void slot_RouterConnState(bool pFlag);

    void slot_ConnState(bool pFlag);

public:
    //>@WIFI应用
#ifdef SUPPORT_MUTEX
    QMutex                     *m_WifiMutex;
#else
    bool                        m_WifiMutex;
#endif
    WifiOperate                 m_WifiOperate;
    bool                        m_WifiOpened;
    bool                        m_WifiBootON;   //>@是否开机启动
    QHostAddress                m_WifiIpAddr;

    PhysicalLANClient*          m_RouterClient;      //>@用于连接Router服务器的
    WaitInfo                    m_RouterWait;
    QHostAddress                m_RouterIpAddr;
    NUM_INFO                    m_RouterPort;
    NUM_INFO                    m_RouterTimeout; //>@自动重连的时间间隔
};

/*
  专指小区服务器中网络设备（物联网）
    相对于云端，其为客户机
    增加了外网服务器：等待云端的客户机连接
         内网服务器：等待内网的显示器NAT跨网段连接
         内网客户机群：主动连接同网段的显示器（指一些非标的显示器或者其它网络设备）
         UDP，同网段扫描，以添加动态增加的设备
  注意事项：
    1、如果同一个设备同时有LANC/LANR，那么只能有一个地方有IP地址。也即
            <LANC>
                <UDP Para="num" List="60000;65000">60001</UDP>
                <TCPS Para="num" List="60000;65000">60002</TCPS>
                <IP Para="string">192.168.1.1</IP>
            </LANC>
            <LANR>
                <UDP Para="num" List="60000;65000">60011</UDP>
                <TCPS Para="num" List="60000;65000">60012</TCPS>
            </LANR>
    2、
*/
class PhysicalLANR : public PhysicalLANServer
{
    Q_OBJECT
public:
    PhysicalLANR(QObject *parent = 0);
    ~PhysicalLANR();

    //>@使用UDP主动扫描同网段设备
    QStringList Scan(QString pContent, quint32 pTimeout);
    QList<PhysicalLANClient*> ScanDevices(quint32 pTimeout);  //>@返回此次增加的连接

    quint32 Monitor(quint32 pCount);

    QHBoxLayout* InitSubPara(QString pName, QDomElement pElement);
    void UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement);

public slots:
    void slot_InitNetwork();

    void slot_Connect();
    void slot_ServerConnState(bool pFlag);

    void slot_Init(QVariant);

public:
    //>@外网连接
    PhysicalLANClient          *m_ServerClient;       //>@外网客户机
    WaitInfo                    m_ServerWait;
    QHostAddress                m_ServerIpAddr;
    NUM_INFO                    m_ServerPort;
    NUM_INFO                    m_ServerTimeout; //>@自动重连的时间间隔
};

#endif // PHYSICALNETWORK_H

