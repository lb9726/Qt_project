#ifndef GLOBAL_H
#define GLOBAL_H

#include "globalfun.h"
#ifdef SURRPORT_LOG
#include "logger.h"
#include "propertyconfigurator.h"
#include "fileappender.h"
#endif

class bstproManager;
class uiManager;
class mdpManager;
class stcManager;
class rcManager;
class cfgManager;
class logTipManager;
class mapManager;
class sevManager;

class IMAGEITEMINFO;
class Global;

#define D_MENU_NEW          tr("&New")
#define D_MENU_OPEN         tr("&Open")
#define D_MENU_CLOSE        tr("&Close")
#define D_MENU_ADD          tr("&Add")
#define D_MENU_DELETE       tr("&Delete")
#define D_MENU_CLEAR        tr("&Clear")
#define D_MENU_UPDATE       tr("&Update")
#define D_MENU_PERFERENCE   tr("&Preference")
#define D_MENU_OPTIONS      tr("&Options")
#define D_MENU_VIEW         tr("&View")

#define D_MENU_REFRESH      tr("&Refresh")
#define D_MENU_EXPLORER     tr("&Explorer")
#define D_MENU_SYNC         tr("&Sync")
#define D_MENU_ASYNC        tr("&ASync")
#define D_MENU_REGULAR      tr("&Regular")
#define D_MENU_DOWNLOAD     tr("&Download")
#define D_MENU_UPLOAD       tr("&Upload")
#define D_MENU_RUNNING      tr("&Set as Running")

extern Global GlobalPara;

void Logger(MSG_LEVEL pLevel, QString pMsg);
void Logger(QString Key = QString(), QStringList tmplist = QStringList());

void GetApplicationDirectory();
QString GetHostPCInfo();
bool GetScreenInfo();
bool InitLoggerInfo();
bool GetBootConfigure();
bool InitializeLinguist();

#ifdef DISPLAY
    class DeviceManager;
#endif
class BootParaGen;
class basemedia;
extern void SystemAudio(QString pAudioPath, int pLoop=1, bool pPrior=false);

class Global : public QObject
{
    Q_OBJECT
public:
    explicit Global(QObject *parent=0);
    ~Global();

    QRect m_DeskRect;
    QRect m_ScreenRect;
    int   m_ScreenNum;

    QString m_SoftwareDir;          //>@应用程序目录

    //>@日志相关
#ifdef SURRPORT_LOG
    Log4Qt::Logger* m_Logger;
#endif
    QHash<QString, LOGTIP_INFO*> m_LogTipGroup;

    //>@音频器
    basemedia *m_SystemAudio;

    //>@QML相关
#ifdef SURRPORT_QML
    QDeclarativeEngine *m_QMLEngine;
#endif

    //>@BOOT.XML的全局变量
    QHash<QString, QDomElement>         NewCategoryGroup;               //>@新建form中支持的类型集合
    QHash<QString, SolutionCategory>    SolutionCategoryGroup;          //>@支持的项目类型
    QHash<QString, FileCategory>        FileCategoryGroup;              //>@支持的文件类型

    QHash<QString, ComCategory*>        m_ComCategoryGroup;             //>@可用的界面组件

    QHash<QString, QDomElement>         m_ConfigInfoGroup;
    QHash<MW_Type, QDomElement>         m_MWonfigInfoGroup;             //>@中间件
    QHash<PH_Type, QDomElement>         m_PHonfigInfoGroup;
    QHash<LG_Type, QDomElement>         m_LGonfigInfoGroup;

    QHash<QString, QDomElement>         m_TestCaseGroup;                //>@

    IMAGEITEMINFO *m_ActiveImageItemInfo;

#ifdef DISPLAY
    DeviceManager *m_DeviceManager;
    BootParaGen   *m_BootParse;
#endif
#ifdef IDE
    QMainWindow   *m_MainWindow;

    QToolBar      *m_ProjectBar;
    QTabWidget    *m_TabPageManager;
    QToolBar      *m_SqlToolBar;
    QToolBar      *m_DockViewToolBar;

    //状态栏中标签
    QStatusBar    *statusBar;
    QLabel        *docEditStatus,
                  *keyboardStatus,
                  *searchStatus;
    QProgressBar  *progressStatus;
    QTextEdit     *m_OutputMsgList;
#endif

public slots:
    void showMessage(QString pShowMsg);
    void outputMessage(QString pShowMsg);
    void updateProgress(int pProgress);
    void updateProgress(QString pShowMsg, int pProgress);
};

class cmdManager : public xmlParse
{
public:
    cmdManager();                   //>@解析XML格式的内容
    cmdManager(QString fileName);   //>@解析XML文件

    ~cmdManager(){}

public:
    //>@解析XML文件
    static const QString CmdSuffix;
    static const QString CmdFileType;

    //>@操作ui文件
    bool NewCMD();
    bool OpenCMD();
    bool CloseCMD();
    bool DelCMD();
    bool SaveCMD();
    bool RenameCMD(QString FileName);

    //>@操作文件中的元素
    bool GetCMDGroup();

    QHash<QString, QDomElement> m_CfgItemGroup;
};

#define D_FONTFAMILY  QString("family")
#define D_FONTSIZE    QString("size")
#define D_FONTCOLOR   QString("color")

class FONT_INFO
{
public:
    FONT_INFO()
    {
        mFont = QFont("Arial", 11, QFont::DemiBold, false/*italic*/);
        mColor = Qt::black;
        mValid = true;
    }
    FONT_INFO(QFont pFont, QColor pColor)
    {
        mFont = pFont;
        mColor = pColor;
        mValid = true;
    }
    FONT_INFO(QString pPara)
    {
        *this = pPara;
    }
    FONT_INFO & operator=(const FONT_INFO &s)
    {
        mFont = s.mFont;
        mColor = s.mColor;
        mValid = s.mValid;
        return *this;
    }
    bool isValid()
    {
        return mValid;
    }
    FONT_INFO & operator=(QString pPara)  /*family:Arial;size:11;color:0,0,0,0*/
    {
        if(pPara.isEmpty())
            mValid = false;
        mValid = true;
        QStringList tmpParaList = pPara.split(';');
        for(int i=0;i<tmpParaList.count();i++)
        {
            QString tmpString = tmpParaList.at(i);
            QStringList tmpChildParaList = tmpString.split(':');
            if(tmpChildParaList.count() != 2)
            {
                IDE_TRACE();
                mValid = false;
                continue;
            }
            QString tmpChildString = tmpChildParaList.at(0);
            if(tmpChildString.compare(D_FONTFAMILY, Qt::CaseInsensitive) == 0)
            {
                mFont.setFamily(tmpChildParaList.at(1));
            }
            else if(tmpChildString.compare(D_FONTSIZE, Qt::CaseInsensitive) == 0)
            {
                mFont.setPointSize(tmpChildParaList.at(1).toInt());
            }
            else if(tmpChildString.compare(D_FONTCOLOR, Qt::CaseInsensitive) == 0)
            {
                QString tmpColorString = tmpChildParaList.at(1);
                QStringList tmpColorItems = tmpColorString.split(',');
                if(tmpColorItems.count() == 3)
                    mColor.setRgb(tmpColorItems.at(0).toInt(),
                                  tmpColorItems.at(1).toInt(),
                                  tmpColorItems.at(2).toInt());
                else if(tmpColorItems.count() == 4)
                    mColor.setRgb(tmpColorItems.at(0).toInt(),
                                  tmpColorItems.at(1).toInt(),
                                  tmpColorItems.at(2).toInt(),
                                  tmpColorItems.at(3).toInt());
                else
                {
                    IDE_TRACE();
                    mValid = false;
                }
            }
        }
        return *this;
    }
    QString toString()
    {
        if(mValid == false)
            return QString();
        return QString("%1:%2;%3:%4;%5:%6,%7,%8,%9")
                      .arg(D_FONTFAMILY).arg(mFont.family())
                      .arg(D_FONTSIZE).arg(mFont.pointSize())
                      .arg(D_FONTCOLOR).arg(mColor.red()).arg(mColor.green()).arg(mColor.blue()).arg(mColor.alpha());
    }
    bool operator==(const FONT_INFO &s)
    {
        if(mFont != s.mFont)
            return false;
        if(mColor != s.mColor)
            return false;
        if(mValid != s.mValid)
            return false;
        return true;
    }
    QFont  mFont;
    QColor mColor;
    bool   mValid;
};

class fontManager : public xmlParse
{
public:
    fontManager(QString fileName);
    ~fontManager(){}

    static const QString FontSuffix;
    static const QString FontFileType;

    //>@操作ui文件
    bool NewFont();
    bool OpenFont();
    bool CloseFont();
    bool DelFont();
    bool SaveFont();
    bool RenameFont(QString FileName);

    bool GetParagraphGroup();

    bool AddParagraph(QString pDesc, QString pString, FONT_INFO pInfo);
    bool ModifyParagraph(QString pDesc, QString pString, FONT_INFO pInfo);
    bool DelParagraph(QString pDesc);

    bool GetParagraph(QString pDesc, QString &pString, FONT_INFO &pInfo);

    QHash<QString, QDomElement> m_ParagraphGroup;
};

bool ExecCmd(QString pFile);
bool isDevExist(QString pPath);
QString GetMountAddr(QString pDriver, QString pStorageBufPath);
QString MountStorage(QString pDriver, QString pStorageBufPath);

bool Reboot(bool pSync = true);

#endif // GLOBAL_H

