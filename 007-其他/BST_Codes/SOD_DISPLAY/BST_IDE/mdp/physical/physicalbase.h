#ifndef PHYSICALBASE_H
#define PHYSICALBASE_H

#include "devfun.h"
#ifdef SURRPORT_USBWATCHE
#include "qdevicewatcher/qdevicewatcher.h"
#include "qdevicewatcher/qdevicechangeevent.h"
#endif

class LogicDevice;

typedef enum{
    CONTENT_NONE=0,
    CONTENT_CMD,
    CONTENT_DISK,///>@
    CONTENT_BOOT,
    CONTENT_THEME,
    CONTENT_SYSTEM,
    CONTENT_EXE,
    CONTENT_COM,//>@
    CONTENT_LOGO,
    CONTENT_VIDEO,
    CONTENT_IMAGE,
    CONTENT_TEXT
} CONTENT_TYPE;   //>@升级内容

class PhysicalDevice : public IODeviceBase
{
    Q_OBJECT
public:
    PhysicalDevice(QObject *parent = 0);
    ~PhysicalDevice();

    virtual void SetDevState(DEVICESTATE pState, QString pDevPath);

    bool InitDevice(PH_Type pPHType, QDomElement pElement);
    void AppendLGDev(LG_Type pType, LogicDevice *pDevice);
    QString GetSystemPath()  {return D_FSRCPATH+D_SYSTEMDIR+D_PHYSICALDIR+mDevName+QString("/");}

signals:
    void sDeviceStateChanged();

public:
    QHash<LG_Type, LogicDevice*> mLGDevices;       //>@协议类型节点表

    DEVICESTATE  mDevState;     //>@设备状态(插入/拔下)
    QString      mDevPath;      //>@设备路径:设备驱动或者可访问地址
    PH_Type      mDevType;
};

class PhysicalUpgrade : public PhysicalDevice
{
    Q_OBJECT
public:
    PhysicalUpgrade(QObject *parent = 0) : PhysicalDevice(parent) {}
    ~PhysicalUpgrade() {}

    virtual CONTENT_TYPE CanUpgrade() {return CONTENT_NONE;}
    virtual bool UpgradeTo(QString pDstPasth) {return false;}

    bool CheckCmdFile(QString pFile);
    bool ExecCmdList();

public:
    QMultiHash<CONTENT_TYPE, QString>    mContentList;
};

class PhysicalMovable : public PhysicalUpgrade
{
    Q_OBJECT
public:
    PhysicalMovable(QObject *parent = 0);
    ~PhysicalMovable();

    bool isMovable() { return mIsMovable; }
    bool OpenDeviceWatcher();
    void CloseDeviceWatcher();

    quint32 InitPara();
    virtual bool InitSubPHPara(QString pName, QDomElement pElement, QHBoxLayout*& pHBoxLayout) { return false; }
    virtual void UpdateSubPHPara(QString pName, QObject *pObject, QDomElement pElement) {}
    virtual void PreInitProcess() {}
    virtual void PreUpdateProcess() {}
    virtual void PostInitProcess() {}
    virtual void PostUpdateProcess() {}

    virtual bool SetPlugged() {return false;}
    virtual bool SetUnplugged() {return false;}

public slots:
    void UpdatePara(QObject* pObject);
    void slot_DevStateChanged(DEVICESTATE pState, QString pDevPath);
    virtual void slot_DeviceAdded(QString dev) { IDE_TRACE_STR(dev); }
    virtual void slot_DeviceRemoved(QString dev) { IDE_TRACE_STR(dev); }

public:
    bool                   mIsMovable;
#ifdef SURRPORT_USBWATCHE
    static QThread        *mWatcherThread;
    static QDeviceWatcher *mDeviceWatcher;
    static int             mOccupyCount;
#endif
};

#endif // PHYSICALBASE_H

