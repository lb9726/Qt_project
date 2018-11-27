#ifndef DEVICESETBASE_H
#define DEVICESETBASE_H

#include "global.h"
#include "physicalnet.h"
#include "logicbase.h"
#include "networkmanager.h"
#include <QSysInfo>
#include <QtGlobal>
#include "messagebox.h"
#include "bstui.h"
#ifdef SURRPORT_USBWATCHE
#include "devicewathcer.h"
#endif
#include "basemedia.h"

#ifdef ANDROID
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <jni.h>
#endif

#define D_PREFER_IP             QString("192.168.1.10")

#define  D_LISTFILEPATH         1000
#define  D_IPADDR               1010
#define  D_NETSSID              1020

#define  D_DEFAULTSSID          QString("BST")

#define D_PC_DPI    96
#define D_USEWIDGET
/*
    支持通过网络、WIFI、usb线调试设置设备
*/

class DeviceBase : public QWidget, public DevManager
{
    Q_OBJECT
public:
    DeviceBase(QWidget *parent = 0);
    ~DeviceBase();

    virtual bool Connected();
    virtual bool Disconnected(bool pMsg = true);
    virtual bool isConnected();

    bool OpenBootMdp(QString pPath);
    void CloseBootMdp();
    bool SaveBootMdp();

    bool OpenThemeUI(QString pPath);
    void CloseThemeUI();
    bool SaveThemeUI();

    virtual bool isChanged();

    //showbusy一定要在继承类的setupui函数执行完后才能执行，否则显示不出来。
    void ShowBusy(bool pFlag, quint64 pTimeout = 60000);  //>@如果pTimeout为0，则表示一直等待直到退出。

    bool ProcessCommand(int pType, QString pCmd, bool pWait = true);  //>@pWait是否需要等待
    bool isOfflineLogin() {return mOffline;}  //>@当前是否处于离线模式

    void AddNetworkToList(NetworkItem *pNetItem);

signals:
    void sConnect();
    void sDisconnect();

public slots:
    virtual void slot_Initialize();  //>@

    virtual void slot_GotoStep(int id) {}
    virtual void slot_BackStep(int id) {}
    virtual void slot_BackStep() {}

    virtual void slot_InitFromLocal(QString pPath, bool pNew=true) {}
    virtual void slot_InitFromNetwork() {}

    virtual void slot_Connect();
    virtual void slot_Disconnect();

    void slot_PHConnState(bool pConn);
    void slot_CloseBusy();

public:
    QPointer<PhysicalLANClient>         m_TcpSocket;  //>@当前正在使用的LANClient
    QString                             mDevCode;

    QListWidget*                        mNetworkList;
    QListWidgetItem*                    mCurNetDev;  //>@当前选择的网络设备节点
    NetworkManager*                     mNetworkManager;

    QHash<int, QPixmap>                 m_FloorPixs;

    bool                                mManualExit;  //>@用于区分是否为自动断线还是手动关闭

    QTimer                              mBusyTimeout;
    gifplayer                          *mBusyFlagIcon;
    bool                                mNetTransFlag;
    QString                             mRetContent;

    bool                                mUploadState;
    bool                                mOperateState;

    bool                                mOffline;

    bool                                mHostAsync;  //>@是否为主动断开连接

    QString                             mBufferPath;   //>@当前设备的缓冲地址，包含CODE名
    mdpManager                         *mMdpManager;
    uiManager                          *mUiManager;

    MEDIA_PLAYER                       *m_MediaPlayer;
    bool                                m_LoginState;

    bool                                m_Connected;

    QString                             m_CmdString;
};

class DeviceSetBase : public DeviceBase
{
    Q_OBJECT
public:
    DeviceSetBase(QWidget *parent = 0, PhysicalLANR* pLanR = 0);
    ~DeviceSetBase();

    quint32 Monitor(quint32 pCount);

    QStringList ScanNetwork();   //>@扫描网络设备

    bool isSync(quint32 pTimeout = 0);
    void Sync();
    virtual void Sync(QHostAddress pIP, quint16 pPort);
    virtual void Async();

public slots:
    virtual void slot_Disconnect();

public:
    bool                               m_FromOutside;
    //>@Arp扫描
    QPointer<PhysicalLANR>             m_LanRouter;
};

class DeviceSetApp : public DeviceSetBase
{
    Q_OBJECT
public:
    DeviceSetApp(QWidget *parent = 0);
    ~DeviceSetApp();

    QString ResizeQSS(QString pQSS);

    void keyPressEvent(QKeyEvent * event);

    void resizeEvent(QResizeEvent *e);
    virtual void InitUI() {}

public slots:
#ifdef QT5
    virtual void onAppStateChanged(Qt::ApplicationState state) {}
#endif

public:
    QSize                   m_SizeLog[2];
};

inline QRect StrToRect(QString pStr)
{
    QStringList tmpList = pStr.split(' ');
    if(tmpList.count() != 4)
        return QRect();
    bool ok = false;
    int x, y, w, h;
    x = tmpList.at(0).toInt(&ok);
    if(ok == false)
        return QRect();
    ok = false;
    y = tmpList.at(1).toInt(&ok);
    if(ok == false)
        return QRect();
    w = tmpList.at(2).toInt(&ok);
    if(ok == false)
        return QRect();
    h = tmpList.at(3).toInt(&ok);
    if(ok == false)
        return QRect();
    return QRect(x,y,w,h);
}

//mNetManager->SendCommand("reboot");
inline bool CreateDeviceDir(QString pPath)
{
    if(!pPath.endsWith("/"))
        pPath.append("/");
    if(!CreatPath(pPath))
    {
        IDE_TRACE_STR(pPath);
        return false;
    }
    if(!CreatPath(pPath + D_BOOTDIR))
    {
        IDE_TRACE_STR(pPath);
        return false;
    }
    if(!CreatPath(pPath + D_THEMEDIR))
    {
        IDE_TRACE_STR(pPath);
        return false;
    }
    if(!CreatPath(pPath + D_SYSTEMDIR))
    {
        IDE_TRACE_STR(pPath);
        return false;
    }
    if(!CreatPath(pPath + D_EXEDIR))
    {
        IDE_TRACE_STR(pPath);
        return false;
    }
    return true;
}

inline bool CheckDeviceDir(QString pPath)
{
    if(pPath.count() < 3)
        return false;
    if(!pPath.endsWith("/"))
        pPath.append("/");
    QDir tmDir;
    if(!tmDir.exists(pPath))
    {
        IDE_TRACE_STR(pPath);
        return false;
    }
    if(!tmDir.exists(pPath + D_BOOTDIR))
    {
        IDE_TRACE_STR(pPath);
        return false;
    }
    if(!tmDir.exists(pPath + D_THEMEDIR))
    {
        IDE_TRACE_STR(pPath);
        return false;
    }
    if(!tmDir.exists(pPath + D_SYSTEMDIR))
    {
        IDE_TRACE_STR(pPath);
        return false;
    }
    if(!tmDir.exists(pPath + D_EXEDIR))
    {
        IDE_TRACE_STR(pPath);
        return false;
    }
    return true;
}

//>@获取某一文件夹中所有子文件夹中的第一个图片
inline QStringList GetFirstFileInSubFolder(QString pFolder)
{
    if(!pFolder.endsWith("/"))
        pFolder.append("/");
    QDir dir = QDir(pFolder);
    QStringList getAllList,tmpList;
    QString tmpString;
    if(!dir.exists())
        return getAllList;
    tmpList = dir.entryList(QDir::Dirs);
    for(int i = 0;i<tmpList.count();i++)
    {
        tmpString = QString("%1%2").arg(pFolder).arg(tmpList.at(i));
        tmpString.replace(QString("//"),QString("/"));
        QDir tmpSubDir(tmpString);
        QStringList tmpFileList = tmpSubDir.entryList(QDir::Files);
        if(tmpFileList.isEmpty())
            continue;
        QString tmpSubFile = QString("%1/%2").arg(tmpString).arg(tmpFileList.first());
        tmpSubFile.replace(QString("//"),QString("/"));
        getAllList.append(tmpSubFile);
    }
    return getAllList;
}

inline bool CopyComFolder(QString pSrcDir, QString pDstDir, QString pPrefix)
{
    QFileInfo tmpFileInfo(pSrcDir);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return false;
    }
    if(!pSrcDir.endsWith('/'))
        pSrcDir.append('/');
    if(!pDstDir.endsWith('/'))
        pDstDir.append('/');
    //>@如果路径相同，则不进行拷贝
    if(pSrcDir.compare(pDstDir, Qt::CaseInsensitive) == 0)
        return true;
    if(!CreatPath(pDstDir))
    {
        IDE_TRACE_STR(pDstDir);
        return false;
    }
    QDir tmpSrcDir(pSrcDir);
    QStringList folders = tmpSrcDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < folders.size(); ++i)
    {
        QString tmpSrcPath = QString("%1%2").arg(pSrcDir).arg(folders[i]);
        QString tmpDstPath = QString("%1%2").arg(pDstDir).arg(folders[i]);
        CopyComFolder(tmpSrcPath, tmpDstPath, pPrefix);
    }
    QStringList files = findFiles(tmpSrcDir, QString(), QString(), Qt::CaseInsensitive);
    for(int i = 0; i < files.size(); ++i)
    {
        QString tmpDstFilePath = QString("%1%2").arg(pDstDir).arg(getFileName(files[i]));
        QtSleep(10);
        CopyFile(files[i], tmpDstFilePath, pPrefix);
    }
    return true;
}

class SimpleCustomEvent : public QEvent
{
public:
    SimpleCustomEvent(int arg1 = 0, const QString &arg2 = QString());
    ~SimpleCustomEvent();

    static Type eventType();

    int m_arg1;
    QString m_arg2;

private:
    static Type m_evType;
};

#endif // DEVICESETBASE_H

