#ifndef LOGICTHEMEMANAGER_H
#define LOGICTHEMEMANAGER_H

#include <QGraphicsView>
#include "logicbase.h"

#ifdef DISPLAY
#include "eventmap.h"
#ifdef SUPPORT_SCENE
#include "uiscene.h"
class UiView;
class UiScene;
#elif SUPPORT_WIDGET
#include "uiwidget.h"
#elif SUPPORT_PLUGIN
#include "uiplugin.h"
#endif
#endif
#include "physicalplayer.h"

class LogicThemeManager : public LogicDevice
{
    Q_OBJECT
public:
    LogicThemeManager(QObject *parent = 0);
    ~LogicThemeManager();

    quint32 InitPara(); //>@返回处理的参数个数
#ifdef DISPLAY
#ifdef SUPPORT_SCENE
    void InitTheme(UiView* pView, uiManager *pManager);
#endif
#endif
    QStringList SearchThemes(QString pDirPath);
    void UpdateThemeList();

    bool hasTheme();
    bool SetValue(QString pValue);

    //>@返回主题文件夹路径、活动主题以及所有主题名
    QString GetInformation();

    bool SetAutoTest(bool pFlag);
    bool SetEnable(bool pFlag, E_DEVMODE pDevMode, bool pForce=false);
    void ReleaseImageBuffer() {} //>@释放主题中的图片缓存

public slots:
    void slot_ReportOperate(AREA_OPERATE pOperate, QVariant pPara);
    void UpdatePara(QObject* pObject);
    void DeviceStateChanged();

    void slot_Initialize();

    void slot_OperateTheme(AREA_OPERATE pOperate, QVariant pPara);

    void InitTheme();
    void ReleaseTheme();
    void ClearTheme();  //>@清空主题的各个组件的显示
    void ShowTheme();
    void HideTheme();
    void RotateTheme(int pNewRotate);

    bool ModifyCurTheme(QString pThemeName) {return true;}

    void slot_TestTimerTimeout();

public:
#ifdef DISPLAY
#ifdef SUPPORT_SCENE
    UiScene            *m_UiContainer;
    QPointer<GraphicsMedia>  m_Media;
#elif SUPPORT_WIDGET
    UiWidget           *m_UiContainer;
    QPointer<WidgetMedia>    m_Media;
#elif SUPPORT_PLUGIN
    UiPlugin           *m_UiContainer;
#endif
    EventMap           *m_EventMap;
#endif

    bool                mAutoTestEnable;
    int                 mThemeLayout[2];

    //>@bk,对于此类备份参数，在initpara时需要初始化，在网络建立连接时需要保存当前值，在断开连接时恢复此值
    int                 mThemeLayoutBk;

    QPointer<PhysicalPlayer>       mPlayer;

    RANKS               mLocation;
    QString             mRender;
    QString             mThemesPath;        //>@主题包所在的路径
    QStringList         mThemeNameList;     //>@主题包所在的路径下的所有主题文件夹名
    QString             mActiveTheme;       //>@当前活动主题文件夹名

#ifdef N329
    QPixmap             m_TransparentPix;
#endif
};

#endif // PARSERTHEMEOP_H
