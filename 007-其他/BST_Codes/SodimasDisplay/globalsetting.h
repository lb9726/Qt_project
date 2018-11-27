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

class MessageBarUi;
class ProgressBarUi;
class DesktopUi;
class ThemeLg;
class PhysicBase;
class LcdPh;
class LogicBase;

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

    Q_INVOKABLE int qmlSlotTest();
signals:
    void sigInfo(QString info);
    void sigInfoProgress(QString sInformation, int sPercent);
    void sigProgress(int percent);
    void sigError(QString errorInfo);

    void sProgress(QString pMsg, quint32 pPrg);
    void sProgress(QString pMsg, quint32 pPrg, quint32 pExtPrg);
    void sBeep(QString pBeepName);

    void sIcon(QString pIconName);
    void sShowIcon(QString);  //>@发送显示图标信号
    void sHideIcon(QString);  //>@发送隐藏图标信号

    void sSetMenu();

public slots:
    void slot_InitDevices(){}
    void slot_Initialized(){}
    void slot_UiShowed(){}
    void slot_StartMonitor(){}

    void SetDevicesEnable(bool pFlag, DEV_MODE pDevMode){}

    void slot_Info(QString pMsg){}
    void slot_Progress(quint32 pValue){}
    void slot_Progress(QString pMsg, quint32 pValue){}

public:
    MessageBarUi    *m_MessageBar;
    ProgressBarUi   *m_ProgressBar;
    DesktopUi       *m_Desktop;    
    LcdPh           *m_Lcd;   
    ThemeLg         *m_themeLg;
    //QML Item
    QGraphicsObject *m_RootObj;
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
    QHash<PH_Type, PhysicBase*>         m_PHDevList;
    QHash<LG_Type, LogicBase*>          m_LGDevList;

    QtQuick1ApplicationViewer    *m_declarativeView;
    QDeclarativeEngine  *m_declarativeEngine;
    QDeclarativeContext  *m_rootContent;

    void initUI();
};

///extern GlobalSetting appGlobalSetting;

#endif // GLOBALSETTING_H
