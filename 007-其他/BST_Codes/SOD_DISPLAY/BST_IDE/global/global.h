#ifndef GLOBAL_H
#define GLOBAL_H

#include "globalfun.h"
#include "logfun.h"

class bstproManager;
class uiManager;
class mdpManager;
class mapManager;
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

bool GetScreenInfo();
bool InitLoggerInfo();
bool InitializeLinguist();


class DeviceManager;

class BootParaGen;

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

    QHash<QString, LOGTIP_INFO*> m_LogTipGroup;

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

    DeviceManager *m_DeviceManager;
    BootParaGen   *m_BootParse;

public slots:
    void showMessage(QString pShowMsg);
    void outputMessage(QString pShowMsg);
    void updateProgress(int pProgress);
    void updateProgress(QString pShowMsg, int pProgress);
};

inline QRectF ToSceneRect(QRectF pRect)
{
    return pRect;

}

inline QRectF FromSceneRect(QRectF pRect)
{
    return pRect;
}

#define D_RCINVALID      QString("Invalid")
#define D_EFFECTTYPE     QString("EffectType")
#define D_EFFECTPARA     QString("EffectPara")

class RC_MAP
{
public:
    RC_MAP(){}
    bool operator==(const RC_MAP &s)
    {
        if(mSrc.compare(s.mSrc))
            return false;
        if(mMap.compare(s.mMap))
            return false;
        return true;
    }
    bool operator!=(const RC_MAP &s)  { return !operator==(s); }
    RC_MAP & operator=(const RC_MAP &s)
    {
        mSrc = s.mSrc;
        mMap = s.mMap;
        return *this;
    }
public:
    QString mSrc;
    QString mMap;
};

class QUiManager : public xmlParse
{
    Q_OBJECT
public:
    QUiManager(QString pFileName);
    void GetComponentGroup(QDomElement pElement);

    QRect GetRect(COM_TYPE pType);
    void ModifyRect(COM_TYPE pType, QRect pRect);

public:
    QHash<COM_TYPE, QDomElement>  m_ComElementGroup;
};

class uiManager : public xmlParse
{
    Q_OBJECT
public:
    uiManager(QString fileName);

    static const QString UiSuffix;
    static const QString UiFileType;

    //>@操作ui文件
    bool NewUI();
    bool OpenUI();
    bool CloseUI();
    bool DelUI();
    bool SaveUI();
    bool RenameUI(QString FileName);

    bool isValid();

    QString GetSolution();
    bool SetSolution(QString pSolution);
    DIRECTION GetDirectionPermisson();
    bool SetDirectionPermisson(DIRECTION pDirection);
    bool canSwitch() { return (m_DirectionPermisson == HVBoth); }

    //>@操作文件中的元素
    void GetComponentGroup();

    //>@查找文件的全路径
    FILEPATH_TYPE GetRcFileType(QString pFileName);
    FILEPATH_TYPE RegulatRcFile(QString& pFileName);
    //>@查看是否为相对路径
    bool IsRelativePath(QString pFileName);
    //>@规整资源到目标路径
    bool RegularFile(COM_TYPE pComType, QString pRcName, QString &pRcFile, QString pDstPath, QSize pSize);

    bool GetComRect(int pLayout, QDomElement pComElement, QRect &pHRect, QRect &pVRect);
    QRect GetComRect(int pLayout, QDomElement pComElement, DIRECTION pDirection);
    bool ModifyComRect(int pLayout, QDomElement pComElement, QRect pRect, DIRECTION pDirection, bool pForceNew = false);

    QDomNodeList GetComStateList(QDomElement pComElement);
    QDomElement GetComState(QDomElement pComElement, QString pStateName, bool pCreateFlag = false); //>@pCreateFlag如果不存在则自动创建
    bool ModifyEffectType(QDomElement pStateElement, QString pEffectType);
    QDomElement ModifyComState(QDomElement pComElement, QString pStateName, QString pEffectType, QString pEffectPara);

    bool ModifyEffectPara(QDomElement pStateElement, QString pEffectPara);

    //>@获取某个组件的父QDomElement，同时查看是否为规整的资源路径，否则进行创建此路径
    bool GetParentElement(QDomElement &pParentElement, QString pComPath);
    //>@获取组件的父组件
    QDomElement GetParComDom(QDomElement pComElement);
    QDomElement GetParComDom(QString pComPath);

    bool AddMap(QDomElement pMapStateElement, RC_MAP pMap, QString pPrefix=D_DEFAULT_PREFIX);
    bool DelMap(QDomElement pMapStateElement, RC_MAP pMap, QString pPrefix=D_DEFAULT_PREFIX);
    QList<RC_MAP> GetMapList(QDomElement pComElement, QString &pMapType, QString pPrefix=D_DEFAULT_PREFIX);
    QHash<QString, QString> GetMapHash(QDomElement pComElement, QString &pMapType, QString pPrefix=D_DEFAULT_PREFIX);

    //>@添加一个控件的Dom资源
    QDomElement AddComDom(COM_TYPE pComType, QRect pRect, DIRECTION pDirection);
    //>@在Dom中删除一个控件的Dom资源
    bool DeleteComDom(COM_TYPE pComType);

    //>@将资源全部保存为相对路径，并存储在UI对应的文件夹下
    bool SaveRelativeComDom(QDomElement pComElement);

public:
    //>@xx.ui文件中的组件（subui和control）的Dom元素(xx.ui)
    QHash<COM_TYPE, QDomElement>  m_ComElementGroup;
    DIRECTION                     m_DirectionPermisson;
};

class mdpManager : public xmlParse
{
    Q_OBJECT
public:
    mdpManager(QString fileName);

    //>@操作文件
    bool NewMDP();
    bool OpenMDP();
    bool CloseMDP();
    bool DelMDP();
    bool SaveMDP();
    bool RenameMDP(QString FileName);

    //>@操作文件中的元素
    bool GetCfgparaGroup();

    bool SetMonitor(quint32 pFreq);
    quint32 GetMonitor();

    bool SetPassword(QString pPasswd);
    QString GetPassword();

    bool SetLocation(QString pLocation);
    QString GetLocation();

    bool ShowLog(bool pEnable);
    bool CanShowLog();

    bool SetPattern(QString pPattern);
    QString GetPattern();

    QString GetVersion();

    QDomElement AddDeviceDom(QString pPath, QDomElement pInitElement);
    void DelDeviceDom(QString pPath);
    void DelDeviceDom(MW_Type pType, QString pPath);
    void DelDeviceDom(PH_Type pType, QString pPath);
    void DelDeviceDom(LG_Type pType, QString pPath);
    QDomElement CreateParaDom(QDomElement pInitElement);
    bool ClrMDP();
    bool ClrDomCldNodes(QDomElement pElement);
    bool DelDom(QDomElement pElement);

public:
    static const QString MdpSuffix;
    static const QString MdpFileType;

    QHash<QString, QDomElement> m_CfgparaElementGroup;
    QHash<MW_Type, QDomElement> m_MWElementGroup;
    QHash<PH_Type, QDomElement> m_PHElementGroup;
    QHash<LG_Type, QDomElement> m_LGElementGroup;
};

class devManager : public xmlParse
{
    Q_OBJECT
public:
    devManager(QString fileName);

    ~devManager(){}

    static const QString DevSuffix;
    static const QString DevFileType;

    //>@操作ui文件
    bool NewDEV();
    bool OpenDEV();
    bool CloseDEV();
    bool DelDEV();
    bool SaveDEV();
    bool RenameDEV(QString FileName);

    //>@操作文件中的元素
    bool GetDevGroup() {return true;}

    bool SetCode(QString pCode);
    QString GetCode();

public:
    QHash<PH_Type, QDomElement>   m_DevInfoGroup;
};



class cmdManager : public xmlParse
{
    Q_OBJECT
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

class MEDIA_INFO
{
public:
    MEDIA_INFO()
    {
    }
    ~MEDIA_INFO()
    {
    }
    QString mFuncMap;
    QString mImage;
    QString mAudio;
    QString mVideo;
    QString mLogo;
    QString mText;
    MEDIA_INFO & operator=(const MEDIA_INFO &s)
    {
        mFuncMap = s.mFuncMap;
        mImage = s.mImage;
        mAudio = s.mAudio;
        mVideo = s.mVideo;
        mLogo = s.mLogo;
        mText = s.mText;
        return *this;
    }
    bool operator==(const MEDIA_INFO &s)
    {
        if(mFuncMap != s.mFuncMap)
            return false;
        if(mImage != s.mImage)
            return false;
        if(mAudio != s.mAudio)
            return false;
        if(mVideo != s.mVideo)
            return false;
        if(mLogo != s.mLogo)
            return false;
        if(mText != s.mText)
            return false;
        return true;
    }
};

typedef struct{
    QString    mEventValue;
    MEDIA_INFO mMediaInfo;
} MAPITEM_INFO;

class mapManager : public xmlParse
{
    Q_OBJECT
public:
    mapManager(QString fileName);

    ~mapManager(){}

    static const QString MapSuffix;
    static const QString MapFileType;

    QHash<QString, QDomElement> m_CfgparaElementGroup;
    QMultiHash<QString, MAPITEM_INFO*> m_MapItemInfoGroup;

    //>@操作map文件
    bool NewMAP();
    bool OpenMAP();
    bool CloseMAP();
    bool DelMAP();
    bool SaveMAP();
    bool RenameMAP(QString FileName);

    bool SetUIPath(QString UiPath);
    QString GetUIPath();
    bool GetCfgparaGroup();

    QDomElement GetMapElement(MAP_EVENT pTypeFilter);
    bool SetMapElement(MAP_EVENT pType, QString pEvent, QString pActions);

    bool ClrMap();
    bool ClrDomCldNodes(QDomElement pElement);
    bool DelDom(QDomElement pElement);
    bool AddDomNewNode(DomInfo pDomInfo);

    bool MapItemToElement(MAPITEM_INFO *pMapItemInfo, QDomElement &pParentElement);
    MAPITEM_INFO * ElementToMapItem(QDomElement pElement);

    bool GetMapItemGroup();
    bool SaveMapItemGroup();

    bool AddMapItem(QString pEventName, QString pEventValue, MEDIA_INFO mMediaInfo);
    bool DelMapItem(QString pEventName, QString pEventValue);
    bool ModifyMapItem(QString pEventName, QString pEventValue, MEDIA_INFO mMediaInfo);
    MAPITEM_INFO *GetMapItem(QString pEventName, QString pEventValue);
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
    Q_OBJECT
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

#endif // GLOBAL_H


