#ifndef GLOBALSETTING_H
#define GLOBALSETTING_H

#include <QObject>
#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDeclarativeItem>
#include <QGraphicsObject>
#include "qtquick1applicationviewer.h"

#include "define.h"
#include "globalfun.h"
#include "themeparser.h"
#include "mdpparser.h"

#include "component/ui/uibase.h"
#include "component/bootsplash.h"

#include "protocolengine.h"

#ifdef CANOPEN
#include "canopen/openclient.h"
#endif
#include "component/customlogo.h"

class MessageBarUi;
class ProgressBarUi;
class DesktopUi;
class ThemeLg;
class PhysicBase;
class LcdPh;
class LogicBase;
class PlayerPh;
class ButtonController;
class MediaUi;
class AppClient;

class GlobalSetting : public QObject        
{
    Q_OBJECT
public:
    explicit GlobalSetting(QObject *parent = 0);
    ~GlobalSetting();
    static GlobalSetting* getGlobalSetting();

    void clearData();

    bool initApp();

    bool initEarly();

    bool initScreen();
    bool initMdp();
    bool initTheme();

    bool initLate();

    void startingApp();

    void setQuickView(QtQuick1ApplicationViewer *declarativeView);

    void initUI();


    void getAppRunningDir();
    QString getAppVersion(QString pFile);
    int getRotate(QString mdpPath);
    void initQmlItemGroup();

    bool addComponent(COM_TYPE pComType, QDomElement &pComElement);
    bool addPHDevice(PH_Type pPHType, QDomElement pPHTypeElement);
    bool delPHDevice(PH_Type pPHType);
    bool addLGDevice(LG_Type pLGType, QDomElement pLGTypeElement);
    bool delLGDevice(LG_Type pLGType);
    PhysicBase *getPHDev(PH_Type pPHType);
    LogicBase *getLGDev(LG_Type pLGType);
    void rotateView();

public:
    Q_INVOKABLE int qmlSlotTest();
    Q_INVOKABLE void rotateDesktop(int angle);
signals:
    void sigInfo(QString info);
    void sigProgress(int percent);
    void sigError(QString errorInfo);

    void sProgress(QString pMsg, quint32 pPrg);
    void sProgress(QString pMsg, quint32 pPrg, quint32 pExtPrg);
    void sBeep(QString pBeepName);

    void sIcon(QString pIconName);
    void sShowIcon(QString);  //发送显示图标信号
    void sHideIcon(QString);  //发送隐藏图标信号

    void sSetMenu();
    //void sigPlayMusic();
public slots:
    void slot_InitDevices(){}
    void slot_Initialized(){}
    void slot_UiShowed(){}
    void slot_StartMonitor(){}

    void SetDevicesEnable(bool pFlag, DEV_MODE pDevMode){}

    void slot_Info(QString pMsg){}
    void slot_Progress(quint32 pValue){}
    void slot_Progress(QString pMsg, quint32 pValue){}

public slots:
    void playMusic(int index);
    void sltBuzz(int index);
    void sltBuzz();
    void updateInfoProgess(QString msg,int progress);
    void clearInfoBuffer();
public:///For button control
    void initButtonController();
public slots: ///For Canopen
    void initCanopen();
    void startDevice();
    void nodeStateChanged(int state);
    void fct_affChanged(bool isFCT);
    void arrowAction(bool arrows_FLM,bool arrows_FLD,bool arrows_SCROL);
    void mainVolumeChange(int divGong,int divBip);
    void musicVolumeChange(int div);
//    void buttonLightOn(int btIndex,bool isBuzz);
//    void buttonLightOff(int btIndex);
//    void sigButtonLightOff(int btIndex);
    void floorArrive(int ge,int shi,int bai,int soundNumber);
    void showFuncLogo(int funcLogoIndex,int soundNumber);
    bool showFloorOrLogo(bool isShowLogo);
    void textMessage(QString m1,QString m2,QString m3,QString m4);
    void realtimeMessage(QString m1,QString m2);
    void sltOrientation(int  value);
    void sltContraste(int  value);
    void sltBacklight(int  value);
    void sltVitscrol(int  value);
    void reinitUI();
    void sDO1(int orientation,int contraste,int backlight,int vit_scrol);
    void sDO2(int logo,int periode,int declenchement);
public slots:///App Slots
    void initWifi();
    void sltAppConnectedSucceed();
    void sltAppDisconnected();
    void sltAppLogin(QString ssid,QString passwd);
    void sltParameterSetting(QString,QVariant);
    void sltSyncParametere();
    void sltUpdateFinished();
public:
    QGraphicsObject *m_RootObj;
    QtQuick1ApplicationViewer    *m_declarativeView;
    QDeclarativeEngine  *m_declarativeEngine;
    QDeclarativeContext  *m_rootContent;
    QHash<QString, QDeclarativeItem*>  m_QmlItemGroup;



    QString          m_appAbsPath;
    BootSplash       *m_bootSplash;
    QString          m_bundlePath;
    QString          m_curThemeName;
    DIRECTION        m_curDirection;
    int              m_curLayout;
    ROTATE_TYPE      m_curRotate;

    ThemeParser     *m_themeParser;
    MdpParser       *m_mdpParser;

    QHash<COM_TYPE, UiBase*>            m_ComComponentGroup;
    MessageBarUi    *m_MessageBar;
    ProgressBarUi   *m_ProgressBar;
    DesktopUi       *m_Desktop;
    MediaUi         *m_MediaUi;

    QHash<PH_Type, PhysicBase*>         m_PHDevList;
    LcdPh           *m_Lcd;
    PlayerPh        *m_Player;
    QHash<LG_Type, LogicBase*>          m_LGDevList;
    ThemeLg         *m_themeLg;

    ///Message Box buffer
    QStringList m_infoList;

#ifdef CANOPEN
    QThread *m_canopenThread;
    OpenClient *m_canopenClient;
    ButtonController *m_buttoncontroller;
#endif

    CustomLogo* m_customLogo;
    int m_logo;
    int m_periode;
    int m_declenchement;

    QString m_RealtimeTextLine1;
    QString m_RealtimeTextLine2;
    QString m_FullScreenTextLine1;
    QString m_FullScreenTextLine2;
    QString m_FullScreenTextLine3;
    QString m_FullScreenTextLine4;

    bool m_showFloorOrLogo;
    int m_beepIndex;
    bool m_beeped;

public:
    AppClient* m_appClient;
};
#endif // GLOBALSETTING_H
