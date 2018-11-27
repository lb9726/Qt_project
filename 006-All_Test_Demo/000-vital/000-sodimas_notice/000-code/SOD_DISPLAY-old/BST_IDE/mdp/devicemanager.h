#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "global.h"
#include "physicalinc.h"
#include "logicinc.h"
#include "middlewareinc.h"
#include "bootlogo.h"

#define D_RENCODE   (D_FSRCPATH + D_BOOTDIR + QString("rencode.png"))
#define D_PROGRESSDELAYTIME 10

void setShutDownSignal( int signalId );
void handleShutDown( int signalId );


class DeviceManager : public QWidget, public DevManager
{
    Q_OBJECT
public:
    DeviceManager(QWidget *parent=0);
    DeviceManager(QRect pRect, QWidget *parent=0);
    ~DeviceManager();

    void InitEnvirHash();

    bool OpenBootMdp(QString pPath);
    void CloseBootMdp();
    bool SaveBootMdp();

    void ReleaseDevices();

    QString GetVersion(QString pName);

    bool AddMWDevice(MW_Type pMWType, QDomElement pMWTypeElement, bool pCover=false);
    bool DelMWDevice(MW_Type pMWType);
    bool AddPHDevice(PH_Type pPHType, QDomElement pPHElement, bool pCover=false);
    bool DelPHDevice(PH_Type pPHType);
    bool AddLGDevice(LG_Type pLGType, QDomElement pPHElement, bool pCover=false);
    bool DelLGDevice(LG_Type pLGType);

    quint32 Monitor(quint32 pCount);

    PhysicalDevice* GetPHDev(PH_Type pPHType);
    LogicDevice*    GetLGDev(LG_Type pLGType);

    void PrepareRelease();
    bool SetCmd(QString pCmd);

    bool eventFilter(QObject *o, QEvent *e);
    void paintEvent(QPaintEvent *e);

    void PlayBeep(QString pBeepName)  {emit sBeep(pBeepName);}
    void ShowIcon(QString pIconName)  {emit sIcon(pIconName);}

signals:
    void sProgress(QString pMsg, quint32 pPrg);
    void sProgress(QString pMsg, quint32 pPrg, quint32 pExtPrg);
    void sBeep(QString pBeepName);

    void sIcon(QString pIconName);
    void sShowIcon(QString);  //>@发送显示图标信号
    void sHideIcon(QString);  //>@发送隐藏图标信号

    void sSetMenu();

public slots:
    void slot_InitDevices();
    void slot_Initialized();
    void slot_UiShowed();
    void slot_StartMonitor();

    void SetDevicesEnable(bool pFlag, E_DEVMODE pDevMode);

    void slot_Info(QString pMsg);
    void slot_Progress(quint32 pValue);
    void slot_Progress(QString pMsg, quint32 pValue);

protected:
    void _Initialize();

public:
    bool                                    m_DevFlag;
    E_DEVMODE                               m_DevMode;
    bool                                    m_DevSetBusy;
    QString                                 m_APPVersion, m_SYSVersion;
    QHash<QString, QString>                 m_EnvirHash;
    QString                                 mDevCode;
    QString                                 mPasswd;
    QString                                 mLocation;
    QPixmap                                 mRencodePix;
    mdpManager                             *mMdpManager;
    BootLogo                               *mBootLogo;
    bool                                    mPaintEnable;
};
#endif // DEVICEMANAGER_H

