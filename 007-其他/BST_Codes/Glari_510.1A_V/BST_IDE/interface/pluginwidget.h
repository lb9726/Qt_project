#ifndef PLUGINWIDGET_H
#define PLUGINWIDGET_H

#include "plugininterface.h"
#include "serialport.h"
#include "qdevicewatcher/qdevicewatcher.h"
#include "qdevicewatcher/qdevicechangeevent.h"
#include "widgetbutton.h"
#include "widgetmedia.h"
#include "tooltcp.h"
#include "ilistview.h"
#include "stackedmenu.h"

#define D_ID_ELE       0x00
#define D_ID_ELE1      0x01
#define D_ID_BTN       0x02
#define D_ID_REG       0x08

#ifdef LINUX
#define IOCTL_LCD_BRIGHTNESS	_IOW('v', 27, unsigned int)	//brightness control

#define VIDEO_DISPLAY_ON		_IOW('v', 24, unsigned int)
#define VIDEO_DISPLAY_OFF		_IOW('v', 25, unsigned int)
#else
#define IOCTL_LCD_BRIGHTNESS

#define VIDEO_DISPLAY_ON
#define VIDEO_DISPLAY_OFF
#endif

#define D_FB_DEV        QString("/dev/fb0")

class PluginWidget;
class MainForm;

QString GetSerialPort(QString num);

class ThreadTick : public QThread
{
public:
    ThreadTick(QObject *parent = 0);
    ~ThreadTick();

    void FeedDog();

protected:
    void run();

public:
    QFile                    *m_WDFile;
    QFile                    *m_LedFile;
    quint8                    m_LEDState;
    QPointer<PluginWidget>    m_MainForm;
};

#define D_BUF_TYPE_COM     0
#define D_BUF_TYPE_WIFI    1
#define D_BUF_TYPE_SERVER  2

class PluginWidget : public QMainWindow, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
public:
    PluginWidget(QWidget*parent=0);
    ~PluginWidget();

    virtual void InitSetMenu();
    virtual void ShowSetMenu();
    virtual void HideSetMenu();
    void WaitForSetMenu(QPoint pPos, int pTimeout);
    void StopWait();
public slots:
    void slot_DelayTimeout();
public:
    QTimer                                m_DelayTimer;
    int                                   m_DelayCount;
    QLabel                               *m_DelayLabel;

public:
    virtual bool eventFilter(QObject *o, QEvent *e);

    virtual bool InitParameter(QString pMdpFile, QString pUiFile);
    virtual void InitMdpPara(QDomElement pElement);
    virtual void InitUiPara();

    bool SetLuminance(int pValue);
    void TcpConnect(QHostAddress pIP, quint16 pPort);

    void SetName(QString pPluginName) { m_PluginName = pPluginName; }
    QString Name() { return m_PluginName; }

    void SetRcPath(QString pRcPath);
    QString RcPath() { return m_RcPath; }

    virtual MainForm* Widget() {return (MainForm*)0;}

    QList<QAction*>* Actions() {return m_ActList;}

    QString ResizeQSS(QString pQSS);
    void DelBtnFocus();
    void resizeEvent(QResizeEvent *e);

    void SyncTime(QDateTime pDateTime);
    void SyncLuminance(int pLuminance);
    void SyncVolume(int pAudio, int pVideo);
    void SyncProtocol(int pType, int pBuad, int pID);

    void SetReportCallback(LogicThemeManager *pThemeManager, EmitReport pFunc);

    void UpdateProgress(QString pMsg, int pValue);

    virtual void ReportOperate(AREA_OPERATE pOperate, QVariant pPara);
    virtual void ExecOperate(AREA_OPERATE pOperate, QVariant pPara);

    void Init(bool pAutoTest = false, int pMinFlr = 1, int pMaxFlr = 10);
    bool UDiskUpgrade(QString pDev);
    void SetState(int pType, bool pState);

    virtual void ProcessFrame(int pType, QByteArray pArray);

    virtual void ParseFloor(int pType = 0);
    virtual void ParseButton(int pType = 0);
    virtual void ParseReg(int pType = 0);

    virtual QByteArray ReadReg(int pAddr);
    virtual bool WriteReg(int pAddr, QByteArray pValue);

    virtual void FloorChanged(FLOOR_INFO *pFlrInfo) {}

    void InitFlrBtn();
    void InitKeyBtn();
    void SetKeyEchoLCD(QLabel *pLabel) {mKeyEchoLCD = pLabel;}//>@

public slots:
    virtual void slot_Initialize();
    void slot_SetMenuHide();

    void slot_ReportOperate(AREA_OPERATE pOperate, QVariant pPara);  //>@调用此函数，即执行回调函数
    void slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara);
    virtual void slot_Tick();

    virtual void slot_DateTime(AREA_OPERATE pOperate, QVariant pPara);

    virtual void slot_DeviceAdded(QString dev);
    virtual void slot_DeviceRemoved(QString dev);
    void slot_UpdateUDisk();

    virtual void slot_ProcessData();
    virtual void slot_TcpReadyRead();
    virtual void slot_ServerReadyRead();

    virtual void slot_BtnPress(int id);
    virtual void slot_BtnRelease(int id);
	
	virtual void slot_KeyBtnPress(int id);
    virtual void slot_KeyBtnRelease(int id);
    virtual void slot_KeyBtnConfirm(QString pNum);

	virtual void slot_ButtonRelease(int id);
    void slot_BtnLight(int id);
    void slot_BtnCancel(int id);

    virtual void slot_RegisteFlr(int pFlrNum, int pIndex);

    virtual void slot_NewSocket(QTcpSocket* pSocket);
    virtual void slot_TcpConnState(bool pFlag);

    void slot_TcpConnect(QHostAddress pIP, quint16 pPort);

signals:
    void sArrow(AREA_OPERATE pOperate, QVariant pPara);
    void sFloor(AREA_OPERATE pOperate, QVariant pPara);
    void sFunction(AREA_OPERATE pOperate, QVariant pPara);
    void sButtonRelease(int pID);
    void sMedia(AREA_OPERATE pOperate, QVariant pPara);
	void sTcpConnect(QHostAddress pIP, quint16 pPort);

    void sDateTime(AREA_OPERATE pOperate, QVariant pPara);

public:
    MdpParser           *mMdpParser;
    UiParser            *mUiParser;

    QWidget             *mSetMenuWidget;
    StackedMenu         *mSetMenu;

    QList<QAction*>     *m_ActList;
    QString              m_PluginName;
    QString              m_RcPath;

    QString              m_MDDev;

    ThreadAudio         *mAudio;
    ThreadMedia         *mVideo;

    ThreadTick          *m_TickThread;
    E_TRANS_PORT        *m_SerialPort;

    QThread             *mWatcherThread;
    QDeviceWatcher      *mDeviceWatcher;

    bool                 mBusy;
    QByteArray           mCurFrame[3];  //0表示串口/spi通讯接口，1：表示WIFI，2：表示Server
    quint32              m_FrameFlag;  //>@0：表示0x80开头的帧
    FRAME_STATE          m_FrameState;

    QSignalMapper       mBtnPressSigMap, mKeyBtnPressSigMap;
    QSignalMapper       mBtnReleaseSigMap, mKeyBtnReleaseSigMap;


    QHash<int, WidgetButton*>  mFlrBtnList;
    QHash<int, WidgetButton*>  mKeyBtnList;
    QLabel                    *mKeyEchoLCD;

    QList<QLabel*>             mRegistedFlrs;
    int                        mRegistedFlrCount;

    bool                m_AutoTest;
    int                 m_MinFlr, m_MaxFlr;

    bool                m_UDiskBusy;

    quint64             m_Ticks;

    QLabel              *mDebugLabel;
	bool                m_FilterFrame;
	quint8              m_BtnBk[7];

    ToolTcpServer      *mTcpServer;
    ToolTcpClient      *mServerTcp;

    ToolTcpClient      *mEternetTcp;

    QHostAddress        m_WifiIP;
    QHostAddress        m_EternteIP;
    QHostAddress        m_ServerIP;
    quint16             m_ServerPort;

    QFile              *m_LcdFile;
    int                 m_Luminance;

    int                 m_AudioVol, m_VideoVol;

    int                 m_ProtocolType;
    //CAN
    int                 m_CanBuad;
    int                 m_CanID;
    //485
    int                 m_485Buad;
    int                 m_485ID;
};

#endif // PLUGINWIDGET_H
