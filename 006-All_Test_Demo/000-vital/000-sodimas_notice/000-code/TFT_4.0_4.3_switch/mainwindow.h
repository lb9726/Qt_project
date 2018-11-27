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
#include <QSerialPort>
#include "mplayer.h"
#include "dipsio.h"
#include "bcbuttons.h"
#include "safthread.h"
#include "mdpparser.h"
#include "splashform.h"
//#if defined(Q_OS_LINUX)
//    #include "openclient.h"
//#endif
#include "qdevicewatcher.h"
#include <QTcpServer>
#include <QTcpSocket>

////>@__PRETTY_FUNCTION__ 用于打印函数名
//#if defined(D_MSVC)
//    #define IDE_DEBUG(x)         qDebug(QString("[%1,%2(),Line %3]-%4").arg("1").arg("2").arg("3").arg(QString(x)).toLatin1().data())
//    #define IDE_TRACE()			 qDebug(QString("[%1,%2(),Line %3]-Trace").arg("1").arg("2").arg("3").toLatin1().data())
//    #define IDE_TRACE_INT(x)	 qDebug(QString("[%1,%2(),Line %3]-%4=%5").arg("1").arg("2").arg("3").arg(#x).arg((int)x).toLatin1().data())
//    #define IDE_TRACE_FLOAT(x)	 qDebug(QString("[%1,%2(),Line %3]-%4=%5").arg(__FILE__).arg(__func__).arg(__LINE__).arg(#x).arg((float)x).toStdString().c_str())
//    #define IDE_TRACE_STR(x)	 qDebug(QString("[%1,%2(),Line %3]-%4=%5").arg("1").arg("2").arg("3").arg(#x).arg(x).toLatin1().data())
//#else
//    #define IDE_DEBUG(x)         qDebug(QString("[%1,%2(),Line %3]-%4").arg(__FILE__).arg(__func__).arg(__LINE__).arg(QString(x)).toStdString().c_str())
//    #define IDE_TRACE()			 qDebug(QString("[%1,%2(),Line %3]-Trace").arg(__FILE__).arg(__func__).arg(__LINE__).toStdString().c_str())
//    #define IDE_TRACE_INT(x)	 qDebug(QString("[%1,%2(),Line %3]-%4=%5").arg(__FILE__).arg(__func__).arg(__LINE__).arg(#x).arg((int)x).toStdString().c_str())
//    #define IDE_TRACE_STR(x)	 qDebug(QString("[%1,%2(),Line %3]-%4=%5").arg(__FILE__).arg(__func__).arg(__LINE__).arg(#x).arg(x).toStdString().c_str())
//    #define IDE_PRINTF(fmt, ...) qDebug("[%s,%s(),Line %s]-"#fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
//#endif
//void QtSleep(unsigned int msec, QEventLoop::ProcessEventsFlags flags = QEventLoop::AllEvents);

#define GBKToUnicode(x)    QTextCodec::codecForName("GBK")->toUnicode(x)
#define GBKToUTF8(x)       QTextCodec::codecForName("UTF-8")->fromUnicode(GBKToUnicode(x)).data()

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

    void setScreenSave(/*bool pFlag*/);

public slots:
    void slot_readAll();
    void slot_AutoTest();

    void slot_StandBy();
    void slot_ScreenSave();
    void slot_ScreenSaveHold();

public:
    QTimer                      mnewTest;
    QDeviceWatcher              mDeviceWatcher;
    QLabel                     *mLabelUSB;

    QTcpServer                  mTcpServer;
    QLabel                     *mLabelWIFI;
    QTcpSocket                 *mWifiSocket;
    QTimer                      mWifiBusyTimer;

    QSerialPort                *mPort;

    QTimer                      mAutoTestTimer;
    int                         mAutoTestCount;

    SafThread                  *mSaf;

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
    QTimer                       m_standbyTimer;
    QTimer                       m_screensaveTimer;
    QTimer                       m_screensaveHoldTimer;
    SplashForm                  *m_screenSaveForm;

    bool                         m_autotest;
    int                          m_testfreq;

    int                          m_layout;

    //-----------------------------------------------------------

    ThemeParser                 *m_themeParser;
    int                          m_themeLayout;
    DIRECTION                    m_themeDirection;
    int                          m_themeRotate;

    UiDesktop                  *mDesktop;
    UiArrow                    *mArrow;
    UiFloor                    *mFloor;
    UiIcon                     *mIcon;
//    UiVideo                    *mVideo;
//    UiText                     *mText;
//    UiClock                    *mClock;

     Mplayer                   *m_Player;

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

    //void slot_SafFrameNum(quint32 pSafFrameNum);
    void slot_Debug(quint8 pDipsValue);
    void slot_NodeState(int pState);

    void slot_Orient(int pOrientation); //SDO旋转方向
    void slot_Backlight(int pBacklight);  //SDO背光
    void slot_Vitscrol(int PVit_scrol);  //SDO箭头速度

    ///RPDO1
    void slot_FCT_AFF(bool pIsFCT);
    void slot_ArrowAction(bool pArrows_FLM,bool pArrows_FLD,bool pArrows_SCROL);
    void slot_MainVolume(int pDivGong,int pDivBip);
    void slot_MusicVolume(int pDivMusic);
    void slot_ButtonLightOn(int pBtLightOnIndex,bool pIsBuzz);
    void slot_ButtonLightOff(int pBtLightOffIndex);
    void slot_Buzz(int pIndex);
    //void sigBuzz();

    ///RPDO2
    void slot_FloorArrive(int pGe,int pShi,int pBai,int pSoundNumber);
    void slot_ShowFuncLogo(int pFuncLogoIndex,int pSoundNumber);
    void slot_ShowFloorOrLogo(bool PIsShowLogo);
    ///RPDO3&RPDO4
    void slot_Message(QString pM1,QString pM2,QString pM3,QString pM4);
    //void sigRealMessage(QString pM1,QString pM2);

};

#endif // MAINWINDOW_H
