#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUiLoader>
#include "uiarrow.h"
#include "uifloor.h"
#include "uidesktop.h"
#include "uiicon.h"
#include "uitext.h"
#include "uivideo.h"
#include "uiclock.h"
#include "mplayer.h"
#include "dipsio.h"
#include "bcbuttons.h"
#include "protocol/saf/safthread.h"
#include "themeparser.h"
#if defined(Q_OS_LINUX)
    #include "protocol/canopen/canopenclientfunction.h"
#endif
#include "qdevicewatcher.h"
#include <QTcpServer>
#include <QTcpSocket>
#include "driver/screensaver.h"
#include "common/mdpparser.h"
#include "driver/backlightctrl.h"
#include "splashform.h"
#include <QLabel>

const QString MediaPath = QString("/usr/bst/usrfs/Theme/sodimas/media/");
const QString backGroundMusicPath = QString("/usr/bst/usrfs/Theme/sodimas/music/");

bool CopyFileContent(QString srcFile, QString dstFile);

class Uiloader;
extern Uiloader *gUiLoader;

class Uiloader : public QUiLoader
{
    Q_OBJECT

public:
    Uiloader(QObject *parent=0);
    QWidget* createWidget(const QString &className, QWidget *parent, const QString &name);

    void InitComponent(COM_TYPE pType, QDomElement pElement);
    void SetLayout(int pLayout);
    void SetRotate(int pRotate);

    QWidget* Init(QString pUiPath);
    void createSaveScreen();
    void createMessagebar();
    QString wifiNameInfo();

private:
    void initCanOpenProtocol();
    void initSafProtocol();
    void callOpenMdp();

signals:
    void sigEnterScreenSave();
    void sigNoArrow();
    void sigUsbInsert();
    void sigUsbRemove();
//    void sigEncharChange(bool flag);

public slots:
    void slot_AutoTest();
    void setArrowScrollSpeed(int arrowSpeed);	 //TODO:配置文件设置箭头速度
    void onSigNoArrow();
    void onSigEnterScreenSave();
	void judgeArrowExist(bool up, bool down);
    void autoCallOn();
    void setScreenSave(bool pFlag);
    void updateMessageBar();

public:
#ifdef LINUX
    CanOpenClientFunction      *m_canopen;
#endif
    QDeviceWatcher             mDeviceWatcher;
    QLabel                     *mLabelUSB;

    QTcpServer                 mTcpServer;
    QLabel                     *mLabelWIFI;
    QTcpSocket                 *mWifiSocket;
    QTimer                     mWifiBusyTimer;

//    QSerialPort                *mPort;

    QTimer                      mAutoTestTimer;
    QTimer                      mScreenSaverTimers;
    int                         mAutoTestCount;

    SafThread                  *m_saf;

    QGraphicsView               *mView;

    //mdp-------------------------------------------------------
    MdpParser                   *m_mdpParser;

    int                          m_admusicvol;
    int                          m_beepvol;
    int                          m_liftarvvol;
    int                          m_liftflrvol;

    QString                      m_lednormalFreq;
    QString                      m_ledbusyFreq;
    QString                      m_lederrorFreq;

    int                          m_rotation;
    int                          m_luminance;
    int                          m_standby;
    int                          m_standbyluminance;
    int                          m_screensave;
    int                          m_screensavehold;

    bool                         m_autotest;
    int                          m_testfreq;
    bool                         m_safOpenningMessage;

    int                          m_layout;
    SplashForm                  *m_screenSaveForm;
    MessageBar                  *m_messageBar;
    QLabel                      *m_textScreen;
    //-----------------------------------------------------------

    ThemeParser                 *m_themeParser;
    int                         m_themeLayout;
    DIRECTION                   m_themeDirection;
    int                         m_themeRotate;

    UiDesktop                  *mDesktop;
    UiArrow                    *mArrow;
    UiFloor                    *mFloor;
    UiIcon                     *mIcon;
//    UiVideo                    *mVideo;
//    UiText                     *mText;
//    UiClock                    *mClock;

private:
    Mplayer                   *m_Player;
    ScreenSaver                mScreenSaver;
    bool                       mArrows_FLM;
    bool                       mArrows_FLD;
    bool                       mSaverFlag;
    bool                       mHasEnterFlag;
    UsbAction                  mUsbAct;
    BacklightCtrl              mBackLightCtrl;
    bool                       mUsbIsInsertFlag;
    int                        mMusicNumber;
    bool                       mWifiInsertFlag;
    int                        mFunctionMusicNum;
    bool                       mBigTextFlag;
    int                        mTempSound;
    QString                    mWifiNameInfo;
    int                        mLastBackLight;

public:
    int             mScrollSpeed;
    bool            mIsShowLogo;

public slots:
    void slot_NewWifiSocketAdded();
    void slot_WifiSocketRemoved();
    void slot_WifiSocketRead();
    void slot_WifiBusyStop();  //结束忙状态标志

    void slot_DeviceAdded(QString pName);
    void slot_DeviceRemoved(QString pName);

    void slot_NodeState(int pState);

    void slot_Orient(int pOrientation); //SDO旋转方向
    void slot_Backlight(int pBacklight);  //SDO背光
    void slot_Vitscrol(int PVit_scrol);  //SDO箭头速度

    ///RPDO1
    void slot_AFF_EN(bool pIsAFF);
    void slot_FCT_AFF(bool pIsFCT);
    void slot_ArrowAction(bool pArrows_FLM,bool pArrows_FLD,bool pArrows_SCROL);
    void slot_MainVolume(int pDivGong,int pDivBip);
    void slot_CanopenMainVolume(int pDivGong, int pDivBip);
    void slot_MusicVolume(int pDivMusic);
    void slot_CanopenMusicVolume(int pDivMusic);
//    void slot_ButtonLightOn(int pBtLightOnIndex,bool pIsBuzz);
//    void slot_ButtonLightOff(int pBtLightOffIndex);
//    void slot_Buzz(int pIndex);
    void slot_CanopenBuzz(int pBuzzIndex);
    void slot_CanopenshortBuzz();

    ///RPDO2
    void slot_FloorArrive(int pGe,int pShi,int pBai,int pSoundNumber);
    void slot_CanopenFloorArrive(int pGe, int pShi, int pBai, int pSoundNumber);
    void slot_ShowFuncLogo(int pFuncLogoIndex,int pSoundNumber);
    void slot_ShowFloorOrLogo(bool PIsShowLogo);
    ///RPDO3&RPDO4
    void slot_Message(QString pM1,QString pM2,QString pM3,QString pM4);
    void slot_RealMessage(QString pM1, QString pM2);

    //saf slots
    void slot_switchValue(int s1,int s2,int s3,int s4);
};

#endif // MAINWINDOW_H
