#ifndef PHYSICALPROTOCOL_H
#define PHYSICALPROTOCOL_H

#include "physicalbase.h"
#ifdef SURRPORT_ZIP
#include "zipfun.h"
#endif
#include <QHostInfo>
#include <QMutex>
#include "tooludp.h"
#include "tooltcp.h"
#include "serialport.h"

#define D_FRAMEMINLEN       5

#define D_SETFRAME_TIMEOUT  2000

#define D_PACKAGE_SIZE      10240
#define D_PACKAGE_TIMEOUT   500   //>@传输1包数据的时延(ms)

#define D_CONTENTSPLIT      QString(';')

inline qint64 GetTransTimeout(qint64 pSize) { return qMax((qint64)(pSize * D_PACKAGE_TIMEOUT / D_PACKAGE_SIZE), (qint64)(5000)); }

class PhysicalProtocol;
class PhysicalPlayer;

#ifdef SUPPORT_SCENE
class GraphicsMedia;
#elif SUPPORT_WIDGET
class WidgetMedia;
#endif

class FRAME_STRUCT : public QObject
{
    Q_OBJECT
public:
    FRAME_STRUCT()
    {
        mDevType = P_INVALID;
        mFlag = Flag_Normal;
        mType = Type_NONE;
        mCmd = 0;
        mAddr = 0;
        mLength = 0;
        mCRC = 0;
    }
    void ClearData()
    {
        mData.clear();
        mLength = 0;
    }
    void SetDev(PH_Type pType, QIODevice* pDev)
    {
        mDevType = pType;
        mDev = pDev;
    }
    bool IsFromHost() {return (mFlag==Flag_Normal);}
public:
    FRAME_FLAG                  mFlag;  //>@主从机
    FRAME_TYPE                  mType;
    quint8                      mCmd;
    quint16                     mAddr; //>@帧计数，也可以理解为（文件/寄存器)地址
    quint16                     mLength;  //>@一般用于接收数据计数
    QByteArray                  mData;  //>@包大小，通过mData计算出来
    quint8                      mCRC;
    quint32                     mError; //>@帧执行的错误类型

    PH_Type                     mDevType;
    QPointer<QIODevice>         mDev;   //>@发送此帧的设备
} ;

typedef enum {RU_NONE, RU_FLRFUNC} RU_TYPE;     //>@显示组件功能复用

inline RU_TYPE GetReuseType(QString pType)
{
    if(pType.compare("Flr_Func", Qt::CaseInsensitive) == 0)
        return RU_FLRFUNC;
    return RU_NONE;
}

inline QString GetReuseTypeString(RU_TYPE pType)
{
    if(RU_FLRFUNC == pType)
        return QString("Flr_Func");
    return QString("None");
}

typedef enum {ID_NONE, ID_FLRLOGO, ID_AUTORUN} IDLE_RUN;     //>@闲状态下自动播放动作

inline IDLE_RUN GetIdleRunType(QString pType)
{
    if(pType.compare("Flr_Logo", Qt::CaseInsensitive) == 0)
        return ID_FLRLOGO;
    else if(pType.compare("AutoRun", Qt::CaseInsensitive) == 0)
        return ID_AUTORUN;
    return ID_NONE;
}

inline QString GetIdleRunTypeString(IDLE_RUN pType)
{
    if(ID_FLRLOGO == pType)
        return QString("Flr_Logo");
    else if(ID_AUTORUN == pType)
        return QString("AutoRun");
    return QString("None");
}

typedef struct{
    IDLE_RUN            mIdleRunType;
    int                 mIdleTimeout, mMinIdleTimeout, mMaxIdleTimeout;
    int                 mIdleFreq, mMinIdleFreq, mMaxIdleFreq;
} IDLE_INFO;

class ReadFileOp :public QObject
{
    Q_OBJECT
public:
    ReadFileOp(QObject *parent = 0);
    ~ReadFileOp();

    void unbind();
    bool bind(QString pSrc);
    void setpos(qint64 pPos);
    bool read(qint64 maxlen);
    int progress();

    QByteArray getmd5() {return QByteArray();}

public:
    bool        mEnable;
    QFile       mFile;
    qint64      mSize;
    qint64      mPos;
    qint64      mSurplus; //>@剩余字节数
    QByteArray  mCurPack;  //>@当前读到的包数据
};

class WriteFileOp :public QObject
{
    Q_OBJECT
public:
    WriteFileOp(QObject *parent = 0);
    ~WriteFileOp();

    void unbind();
    bool bind(QString pSrc, qint64 pSize);
    bool write(QByteArray pByteArray, qint64 pPos=-1); //>@pPos如果为负数，表示直接追加
    bool repair();  //>@将保存的文件恢复
    int progress();

    QByteArray getmd5() {return QByteArray();}

public:
    bool        mEnable;
    QFile       mFile;
    qint64      mSize;
    qint64      mPos;
    qint64      mSurplus; //>@剩余字节数
};

typedef enum{
    Flag_GetSnapShot = 0x01,
    Flag_GetCamera = 0x02,
    Flag_SendSnapShot = 0x10,
    Flag_SendCamera = 0x20
}STREAM_TYPE;

enum {
    REG_ADDR_PH = 0,
    REG_ADDR_LG = 10000,
    REG_ADDR_COM = 20000
};

class ProtocolState{
public:
    ProtocolState()
    {
        for(int i=0;i<3;i++)
        {
            mType[i] = 0;
            mState[i] = Unstart;
        }
    }

    int         mType[3];  //>@目前支持3层，0层表示最高一层，1：表示符合指令，2表示指令以及文件帧传输层
                           //>@mType如果为0表示无任务。
    TaskState   mState[3];
};

/*++++++++========================================================================
PhysicalProtocol  :
    1、支持双向传输，接收时根据接收到的帧类型返回数据。
                  发送时根据形参确定主从类型（0x80为主，0xc0为从）
    2、协议收发完整性
          主有一个缓冲队列，是个FIFO：
            将当前发送的帧入列（如果不需要响应，则不需要入列），接收到响应时查看与帧队列中的是否一致
            主机接收到从机的响应时，不需要进行任何响应
          从，不需要缓冲，根据响应位选择是否响应即可
    3、文件传输
          通过4号指令修改文件传输状态。
          通过filetest修改待操作文件指针，根据文件操作指针状态确定当前文件传输的状态。
            比如文件指针为空，则接收的文件读写命令全部失败。
            如果上一次传输失败后，希望此次可以断点续传，那么必须先使用filetest修改待操作文件指针，然后再通过修改帧计数来达到此目的。
    4、
//++++++++========================================================================*/

class PhysicalProtocol : public PhysicalDevice
{
    Q_OBJECT
public:
    PhysicalProtocol(QObject *parent);
    ~PhysicalProtocol();

    virtual quint32 InitPara();
    virtual QHBoxLayout* InitSubPara(QString pName, QDomElement pElement){}
    virtual void UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement){}

    void UpdateBtnRange(QString pValue);

    virtual quint32 Monitor(quint32 pCount);

    virtual void Connected();
    virtual void Disconnected();
    virtual bool isConnected();
    virtual bool CanAutoConnect() {return false;}

    bool CanAbort();
    void WaitForLeisure(quint32 pTimeout);

    //>@相对于QtWait增加了断线检测的功能
    quint32 ProtocolWait(int &pFlag, int pEqlFlag, unsigned int msec);

    void SetCurDev(PH_Type pType, QIODevice* pDev);

    void SeteAutoConnect(bool pFlag) {mAutoConnFlag = pFlag;}
    void SetHostFlag(bool pFlag)    {mHostFlag = pFlag;}

    quint32 SendData(FRAME_STRUCT *pFrame);

    quint32 SendFrame(FRAME_STRUCT *pFrame);
    quint32 ReplyFrame(FRAME_STRUCT *pFrame);

    virtual quint32 ParseFrameBuffer(PH_Type pType, QIODevice *pDev, QByteArray pFrameBuffer);

    quint32 ParseFrame(FRAME_STRUCT *pFrame);
    virtual quint32 ParseHeartbeatFrame(FRAME_STRUCT *pFrame);
    virtual quint32 ParseUrgencyFrame(FRAME_STRUCT *pFrame);
    virtual quint32 ParseFloorFrame(FRAME_STRUCT *pFrame);
    virtual quint32 ParseButtonFrame(FRAME_STRUCT *pFrame);
    virtual quint32 ParseDisBtnFrame(FRAME_STRUCT *pFrame);
    virtual quint32 ParseScandevFrame(FRAME_STRUCT *pFrame);
    virtual quint32 ParseConfFrame(FRAME_STRUCT *pFrame);//>@检测到一个新的配置指令
    virtual quint32 ParseFileFrame(FRAME_STRUCT *pFrame, bool pEmitPrg=true);
    virtual quint32 ParseStreamFrame(FRAME_STRUCT *pFrame);

    //>@File
    void SetMaxBlock(qint64 pSize) { m_MaxBlockSize = pSize; }

    quint32 SendFileData(FRAME_STRUCT* pPreFrame, bool pEmitPrg=false);
    quint32 SendFileMD5(FRAME_STRUCT* pPreFrame);
    quint32 SendFileStop(FRAME_STRUCT* pPreFrame);
    quint32 ReplyFileStop(FRAME_STRUCT* pPreFrame);

    quint32 ReplyReTrans(FRAME_STRUCT* pFrame, quint16 pAddr);
    quint32 ReplyOneTrans(FRAME_STRUCT* pFrame, quint16 pAddr);

    //>@流传输
    quint32 SendStream(STREAM_TYPE pType);
    quint32 GetStream(STREAM_TYPE pType);

    //>@心跳
    quint32 SendHBFrame(quint32 pTimeout, bool pAck = true);  //>@发送心跳
    quint32 ReplyHBFrame(FRAME_STRUCT *pFrame, QByteArray pContent);

    bool HB(quint32 pTimeout, bool pWait=false);  //>@使能心跳，由（非显示器）一方发送。如果pTimeout为0，表示关闭心跳

    bool WaitHB(quint32 pTimeout);

    //>@Conf命令
    quint32 SendConfFrame(QString pInstruc);  //>@发送配置指令
    bool SendConfFrame(QString pInstruc, int pTimeout);
    quint32 ReplyConfFrame(SetCmdInfo pInfo, FRAME_STRUCT* pFrame);  //>@主要针对CMD指令帧返回

    quint32 SendConfFrame(SetCmdInfo pInfo);

    void SendFileTest(QString pCover, QString pSrc, QString pDst, quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd);
    void GetFileTest(QString pCover, QString pSrc, QString pDst, quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd);
    void StartGetFile(quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd);
    void ListPath(QString pPath, quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd);

    //>@指令缓冲：远程解压、压缩
    void Zip(QString pSrc, QString pDst, quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd);
    void Unzip(QString pSrc, QString pDst, quint64 pDietime, qreal pRation, SETCMD_TYPE pFatherType, bool pEnd);

    //>@非缓冲解压、压缩，isLocal表示是否为本地解压、压缩
    bool Zip(QString pSrc, QString pDst, qint64 pTimeout, bool isLocal);
    bool UnZip(QString pSrc, QString pDst, qint64 pTimeout, bool isLocal);

    bool Set(QString pContent, qint64 pTimeout=-1);
    bool Get(QString pContent, qint64 pTimeout=-1);
    virtual void Bootup();
    void Msg(QString pMessage)    { SendConfFrame(QString("Msg#%1").arg(pMessage)); }
    void Cmd(QString pMessage)    { SendConfFrame(QString("Cmd#%1").arg(pMessage)); }

    //>@设置对方的寄存器
    bool WriteReg(quint16 pRegAddr, QByteArray pContent, bool pNeedAck=false);
    QByteArray ReadReg(quint16 pRegAddr, quint32 pTimeout);

    //>@操作本地寄存器
    /* 0~10000地址，为物理设备参数地址，计算方法：PH_Type*100+n，也即最大支持100个物理设备
     * 10000~20000地址，为逻辑设备参数地址，计算方法：10000+LG_Type*100+n
     * 20000~30000地址，为界面组件参数地址，计算方法：20000+COM_Type*100+n
    */
    virtual QByteArray SetRegister(quint16 pAddr, QByteArray pContent);
    virtual QByteArray GetRegister(quint16 pAddr);

    quint32 ParseRegFrame(FRAME_STRUCT *pFrame);

    //>@配置指令队列
    bool StopTask(); //>@停止当前任务，包括文件传输以及符合命令

    void ClearConfQueue();
    void PushConfQueue(SetCmdInfo pInfo);
    quint32 ProcConfQueue(SetCmdInfo &pInfo, bool pSuccess);

    bool StartConfMonitor(qint64 pTime);
    void StopConfMonitor();

    bool GetDevInfo(QString pName, QString pPasswd, quint64 pLife);

    //>@复合指令
    bool SendFile(QString pCover, QString pSrc, QString pDst);
    bool GetFile(QString pCover, QString pSrc, QString pDst);
    bool SendDir(QString pSrc, QString pDir);
    bool GetDir(QString pSrc, QString pDir);

    bool GetDesktop(QString pSrc, QString pDir, qint64 pTime=30000) {}

    //>@
    quint32 SendScanFrame(QString pContent);
    virtual QStringList Scan(QString pContent, quint32 pTimeout);

    void SendFloorFrame(quint32 pFloorNum, quint8 pFunc, quint32 pResvFunc);
    void SendFloorFrame(quint8 pExFunc, quint32 pFloorNum, ARROWSTATE pArrowState, quint8 pRadioFun, quint8 pAudio);
    void ReplyFloorFrame(quint8 pTouch, quint8 pSwitch, quint8 pFun0);
    void SendButtonFrame(QVariant pBtnList, quint32 pDisablity);
    void ReplyButtonFrame(QVariant pBtnList, quint32 pDisablity);

public slots:
    void UpdatePara(QObject* pObject);
    void slot_Initialize();
    void slot_StandbyCallback(bool pFlag);

    void slot_ConfMonitorTimeout();

    virtual void slot_Init(QVariant) {}

    //>@如果是APP，指发送心跳，如果是Display，表示一段时间内没有收到有效心跳
    virtual void slot_HBTimeout(quint32 pID);
    virtual void slot_HB(quint32 pTimeout);  //>@发送心跳

    virtual void slot_Connect() {}      //>@尝试建立连接
    virtual void slot_Disconnect() {}   //>@尝试断开连接

    void slot_BtnCallEvent(QVariant pPara);

    void slot_Zip(FRAME_STRUCT* pFrame);
    void slot_Unzip(FRAME_STRUCT* pFrame);

    void slot_ZipTaskEnd();
    void slot_ZipTaskErr();
    void slot_ZipTaskProgress(int ratio);

signals:
    void sInit(QVariant);

    void sConnect();
    void sDisconnect();

    void sHBTimeout(quint32 pID);
    void sBootup();

    void sTouchOutputChange(quint8 pValue);
    void sSwitchOutputChange(quint8 pValue);

    void sFloorEvent(quint32 pFloorNum);
    void sArrowEvent(quint32 pArrowState);
    void sRadioFunEvent(quint32 pFundex);
    void sMediaEvent(quint32 pMediaNum);
    void sAudioEvent(quint32 pAudioNum);
    void sButtonEvent(QVariant pBtnList, BUTTONSTATE pState);
    void sDisablityEvent(bool pDisablity);

    void sZip(FRAME_STRUCT* pFrame);
    void sUnzip(FRAME_STRUCT* pFrame);

    void sZip(QString pSrc, QString pDir, bool pCover, bool pIngore);
    void sUnzip(QString pSrc, QString pDir, bool pCover, bool pIngore);

    void sSet(QString pContent);
    void sGet(QString pContent);

    void sMessage(QString pMessage, int pDelay=5000);

    void sSendFile(bool pState);
    void sGetFile(bool pState);

    void sSendDir(bool pState);
    void sGetDir(bool pState);

    void sDownLoadThemeZip(bool pState);
    void sUpLoadThemeZip(bool pState);

    void sHB(quint32 pTimeout);

public:
#ifdef SUPPORT_MUTEX
    QMutex                 *m_ParseDataMutex;
#else
    bool                    m_ParseDataMutex;
#endif

    QPointer<QIODevice>     m_CurTransDev;    //>@当前接收的设备
    PH_Type                 m_CurDevType;

    quint32                 m_MonitorCount;

    TaskState               m_HBState;
    quint32                 m_HBOverflow[3];   //>@发送心跳的超时次数

    //>@第0个表示客户机向APP发送心跳的频率，
    //>@第1个表示客户机监测APP发送来的心跳的频率
    //>@第2个表示Host机器监测显示器响应来的心跳
    NUM_INFO                m_HBFreq[3];
    WaitInfo                m_HBWait[3];
    quint32                 m_HBCount[3];      //>@发送的心跳命令计数，当超过m_HBOverflow时表式断线

    bool                    m_Elevator;
    NUM_INFO                m_BtnBaseNum;

    QByteArray              m_DataBuffer;
    FRAME_STATE             m_FrameState;

    //>@指令传输状态集合
    FRAME_STRUCT           *m_CurRevFrame;      //>@当前接收的帧
    FRAME_STRUCT           *m_CurSendFrame;     //>@当前发送的帧

    ProtocolState           m_CurTaskStatus;

    bool                    m_Connected;

    qint64                  m_WaitTimeout;  //>@用于超时等待的全局变量

    SetCmdInfo              m_RcvScanInfo;

    bool                    mHostFlag;    //>@是否为主动连接服务器的Socket
    bool                    mAutoConnFlag, mAutoConnState;//>@自动重连
    bool                    mAutoConnLog;  //>@指示是否曾经自动重连过，当正常能接收到心跳时复位。

    //>@电梯接口参数
    bool                    m_DebugMode;        //>@是否开启调试模式
    RU_TYPE                 m_ReuseType;
    IDLE_INFO               m_IdleInfo;

    QString                 m_BtnRange;
    QList<int>              m_ValidBtns;

    NUM_INFO                m_FlrStationNum;

    //>@文件传输
    qint64                  m_MaxBlockSize;
    ReadFileOp             *m_ReadFileOp;
    WriteFileOp            *m_WriteFileOp;
    TaskState               m_FileTransState;

    TaskState               m_SetTaskState;
    TaskState               m_GetTaskState;

    TaskState               m_ReadRegState;
    QByteArray              m_ReadRegBuffer;

    bool                    m_RetransFile; //>@是否重复当前的文件传输操作。

    //>@配置指令
    SetCmdInfo              m_SendConfInfo, m_RcvConfInfo;
    QQueue<SetCmdInfo>      m_ConfQueue;  //>@主机发送配置指令缓冲
    TaskState               m_ConfState;
    QTimer                  m_ConfMonitorTimer;   //>@监控某个setcmdinfo是否超时

    TaskState               m_SendDirTaskState;
    TaskState               m_GetDirTaskState;

    TaskState               m_ZipTaskState;
    //>@复合配置指令
    quint32                 m_QueueProgress;

    //>@流式传输
    QPixmap                 m_SnapShotPix;
    TaskState               m_SnapShotState;

    //>@ScanDevice
    QStringList             m_DevAddrList;      //>@设备扫描列表， IP：PORT：CODE 或者 IP:CODE
    bool                    m_LoopFlag;         //>@用于扫描设备时等待标识符，用于解决正在扫描的过程中释放此对象的bug

    QHash<QString, QString> m_DevInfoList;
    TaskState               m_DevInfoTaskState;

    QStringList             m_LsFileList;
    TaskState               m_LsTaskState;

    bool                    m_StopTaskFlag;

    //>@断线重连功能


    QPointer<PhysicalPlayer>     mPlayer;
#ifdef SUPPORT_SCENE
    QPointer<GraphicsMedia>      mMedia;
#elif SUPPORT_WIDGET
    QPointer<WidgetMedia>        mMedia;
#endif
};


typedef enum{
    TYPE_LOCAL = 0,     //>@本地缓冲，也即离线制作主题
    TYPE_NET,           //>@网络设备
    TYPE_STORAGE,       //>@存储设备
    TYPE_COM            //>@串口设备
}DEVICETYPE;

inline DEVICETYPE GetDeviceType(QString pDevType)
{
    if(pDevType.compare("NET", Qt::CaseInsensitive) == 0)
        return TYPE_NET;
    else if(pDevType.compare("STORAGE", Qt::CaseInsensitive) == 0)
        return TYPE_STORAGE;
    else if(pDevType.compare("COM", Qt::CaseInsensitive) == 0)
        return TYPE_COM;
    return TYPE_LOCAL;
}

typedef enum{
    TYPE_TREE = 1,
    TYPE_LIST,
    TYPE_TABLE,
    TYPE_NONE
}DEVCANTYPE;

class DevContainer
{
public:
    DevContainer()
    {
        mItem = 0;
        mType = TYPE_NONE;
    }
public:
    void*            mItem;       //>@设备容器指针
    DEVCANTYPE       mType;       //>@设备容器类型
};

typedef struct{
    DEVICETYPE      mType;
    QString         mPath;
} RC_CAN;

#define D_DELFLAG       10001

class PhysicalHost : public PhysicalProtocol
{
    Q_OBJECT
public:
    PhysicalHost(QString pAddr=QString(), QString pBufferPath=QString(), QObject *parent=0);
    ~PhysicalHost();

    virtual bool isValid();

    void SetDevCan(QTreeWidgetItem *pDevItem, DEVICESTATE pState=S_OFF);
    void SetDevCan(QListWidgetItem *pDevItem, DEVICESTATE pState=S_OFF);
    void SetDevCan(QTableWidgetItem *pDevItem, DEVICESTATE pState=S_OFF);

    virtual bool Scan();        //>@扫描缓冲路径下的工程
    virtual void Clear() {}
    QList<RC_CAN> ScanDirectiory(QString pPath);

    virtual bool isSync(quint32 pTimeout = 0);
    virtual bool Sync() {return false;}
    bool Sync(quint32 pTimeout);
    virtual void Async() { Disconnected(); }
    bool Async(quint32 pTimeout);

    virtual void Connected();
    virtual void Disconnected();

    virtual void ChangeDevAddr(QString pDevAddr);  //>@不允许修改设备类型，但可以修改设备地址，比如修改IP
    virtual void ChangeBuffer(QString pBufferPath);

    virtual void OpenLocation(QString pLocation) {}  //>@打开某个路径

    virtual bool DeleteTheme(QString pThemeName) {return true;}
    virtual bool SetRunningTheme(QString pThemeName) {return true;}
    virtual QString GetRunningTheme() {return QString();}

signals:
    void sConnState(bool pState);

public:
    DevContainer     mDevCan;           //>@设备容器
    DEVICETYPE       mDevType;          //>@设备类型

    QString          mDevNickname;
    QString          mDevAddr;
    QString          mBufferPath;

    bool             mIsBusy;
};


#endif // PHYSICALPROTOCOL_H
