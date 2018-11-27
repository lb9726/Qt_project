#ifndef GLOBAL_H
#define GLOBAL_H

#include "globalfunction.h"
//#include "logfun.h"

class bstproManager;
class uiManager;
class mdpManager;
class stcManager;
class rcManager;
class cfgManager;
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

bool GetScreenInfo();
bool InitLoggerInfo();
bool GetBootConfigure();
bool InitializeLinguist();

#ifdef DISPLAY
    class DeviceManager;
#endif
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

#ifdef DISPLAY
    DeviceManager *m_DeviceManager;
    BootParaGen   *m_BootParse;
#endif
#if (defined(IDE) || defined(PRODUCT))
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

/*
    XX.bstpro
        xx.ui
        xx.map
        xx.stc
        xx.sto
*/

class bstproManager : public xmlParse
{
    Q_OBJECT
public:
    bstproManager(QString fileName);

    ~bstproManager(){}

    static const QString BstProSuffix;
    static const QString BstProFileType;

    bool NewPro();
    bool OpenPro();
    bool ClosePro();
    bool DelPro();
    bool SavePro();
    bool RenamePro(QString FileName);

    bool AddFile(QString pFileName);
    bool AddFile(QString pKey, QString pFileName);
    bool DelFile(QString pKey);
    QString GetFile(QString pKey);
};

/*
    <Desktop>
        <Rect>
            <Horizontal>0,0,800,600</Horizontal>
            <Vertical>0,0,600,800</Vertical>
        </Rect>
        <StateMachine>
            <Common EffectType="Image" EffectPara="">
                <RC></RC>
            </Common>
            <Change EffectType="Roll" EffectPara="">
                <RC></RC>
            </Change>
        </StateMachine>
    </Desktop>

    横竖显自动布局算法： 除桌面外，对于具有图像元素的组件必须保证宽高相同，修改坐标时需要实时判断是否宽高超出横竖版桌面的宽高
    1、竖屏自动布局原则：
        如果所有有区域的左上角坐标都为0，则表示从未自动布局过，则进行一次自动布局；否则不对区域进行自动布局
            所有区域进行列排放，优先级取决于COM_TYPE的大小
        区域内部的组件进行多行排放，也即排完第一行再排第二行的原则，优先级取决于COM_TYPE的大小
            通过二维数组记录
            一行最多有三个组件，每一行都是平均分配空隙（除因子为行组件个数+1），
            在列方面，每个组件在自己所在的行中要居中
        最终由区域内部已排列的组件计算区域的最小大小，如果区域的大小大于此最小大小，则不变，否则区域最终的坐标中高度使用最小高度，宽度使用桌面宽度

    2、横屏自动布局原则
        从不对区域进行自动布局，完全由用户进行拖放设置，因此一般设计界面时先设计横版，再设计竖版
        区域内部的组件进行多行排放，也即排完第一行再排第二行的原则，优先级取决于COM_TYPE的大小
        最终由区域内部已排列的组件计算区域的最小大小，如果区域的大小大于此最小大小，则不变，否则区域最终的坐标中高度使用最小高度，宽带使用最小宽度

    3、拖动组件，同步原则
        对于区域，不需要进行任何检测
        对于组件，需要实时检测其宽度或者高度，查看其所在方位是否已经超出当前横竖版的桌面，如果是，则提示注意，如果执意修改，则锁定当前主题的横竖切换功能使之仅能适应一种界面

    4、界面初始化调整区域原则
        首先针查看自己所有的子控件是否在区域外，如果是，则自动调整到子控件的最大大小，
        然后查看区域是否在桌面外，如果是，则调整到桌面内

*/
inline QRectF ToSceneRect(QRectF pRect)
{
#ifdef IDE
    return QRectF(pRect.left() + StartPosX, pRect.top() + StartPosY, pRect.width(), pRect.height());
#elif DISPLAY
    return pRect;
#endif
}

inline QRectF FromSceneRect(QRectF pRect)
{
#ifdef IDE
    return QRectF(pRect.left() - StartPosX, pRect.top() - StartPosY, pRect.width(), pRect.height());
#elif DISPLAY
    return pRect;
#endif
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

/*
    <Device>  //>@设备外观及接口定义，以设备平放为参考。
        <Top img="">
            <Chip>
                <Core nickname="" desc="" rect="10,10,300,300"/>   //>@主芯片
                <Power nickname="" desc=""/> //>@电源芯片
                <Interface nickname="" desc=""/>  //>@接口芯片
            </Chip>
            <Input>
                <Key num="" />  //>@num如果不写，默认为1个，数据类型根据num确定，如果为8位，则为char类型
                <Dial num="" />
                <COM0 num="" />
                <USB0 num="2" subtype="host" />
                <USB1 num="1" subtype="device" />
            </Input>
            <Output>
                <LCD rect="10,10,300,300" />
                <LED num="1"/>
            </Output>
        </Top>
        <Bottom/>
        <Front/>
        <Behind/>
        <Left/>
        <Right/>
    </Device>
    <AutoTest>  :自动测试
        <Path loop="0" >
            <Step delay="" loop="" waitusr=""(是否等待用户操作:0-不等待，1-弹出输入框等待输入，2-弹出按钮等待OK按钮（用于操作同步）)>
                <Input>
                    <Key>1,0,0,0</Key>
                    <COM0>0x80,0x01,...</COM0>
                </Input>
                <Output> //>@指应该有什么样的输出
                </Output>
            </Step>
        </Path>
    </AutoTest>
    <ModuleTest>:模块测试，主要用于测试用户关心的相关功能点，通过拖拽右边栏中测试模块增删测试模块，ScriptEngine将所有参数变量开放给js文件。
        <BalancingLoad Desc="平衡系统负载，使之流畅运行">
            <CPU Para="num" List="50;70" Desc="当CPU占用率小于此设置的值时认为系统可流畅运行"></CPU>
            <MEM Para="num" List="50;70"></MEM>
            <TestCase Para="file">js/cpu.js</TestCase> 或者 <TestCase Para="string">alert("hello")</TestCase>
        </BalancingLoad>
    </ModuleTest>
    <ProtocolTest>  :协议测试
        <Generator>
            <SAF>
                <SF Para="bool" Desc="设置为高低电平">false</SF>
                <TALK Para="bool">false</TALK>
                <SGO Para="bool">false</SGO>
                <FLD Para="bool">false</FLD>
                <FLM Para="bool">false</FLM>
                <SAF Para="num" List="1;70" Desc="设置帧计数为xx"></SAF>
                <QuickLaunch Para="button" List="a;b;c;d"></QuickLaunch>
            </SAF>
        </Generator>
        <Analyzer>
            <BST3GP>
                <Hundread Para="num" List="32;126">32</Hundread>
                <Ten Para="num" List="32;126">32</Ten>
                <Single Para="num" List="32;126">32</Single>
                <CheckFun Para="check" List="ArrowUP;ArrowDown;Run"></CheckFun>
                <RadioFun Para="num" List="1;127">false</RadioFun>
                <Audio Para="num" List="1;127">false</Audio>
                <ExtendFun Para="check" List="Floor;Arrow;Function;Audio"></ExtendFun>
                <QuickLaunch Para="button" List="a;b;c;d"></QuickLaunch>
            </BST3GP>
        </Analyzer>
    </ProtocolTest>
    <Integrity>xxx.js</Integrity>
*/

class stcManager : public xmlParse
{
    Q_OBJECT
public:
    stcManager(QString fileName);
    ~stcManager(){}

    static const QString StcSuffix;
    static const QString StcFileType;

    //>@操作ui文件
    bool NewSTC();
    bool OpenSTC();
    bool CloseSTC();
    bool DelSTC();
    bool SaveSTC();
    bool RenameSTC(QString FileName);

    //>@操作文件中的元素
    bool GetStcGroup();

public:
    QHash<QString, QDomElement> m_ModuleCaseGroup;
    QDomElement                 m_GeneratorElement;
    QList<QDomElement>          m_AnalyzerGroup;
};

class rcManager : public xmlParse
{
    Q_OBJECT
public:
    rcManager(QString fileName);

    ~rcManager(){}

    static const QString RcSuffix;
    static const QString RcFileType;

    //>@操作ui文件
    bool NewRC();
    bool OpenRC();
    bool CloseRC();
    bool DelRC();
    bool SaveRC();
    bool RenameRC(QString FileName);

    //>@操作文件中的元素
    bool GetRcGroup();
};

class cfgManager : public xmlParse
{
    Q_OBJECT
public:
    cfgManager(QString fileName);

    ~cfgManager(){}

    static const QString BootFileName;
    static const QString CfgFileType;

    bool getNewCategory();
    bool getSolutionCategory();
    bool getFileCategory();

    void getEffectAndRcTypes(QDomElement pElement, ComCategory *pComCategory);
    bool getComponetCategory();

    //>@ /Device/RMII
    bool getCfgparaGroup();

    bool getTestcaseGroup();
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

/*>@
    电梯设备名前缀：
        第一个字符：
            C-表示cop
            L-表示lop
            H-表示横显
            S-表示摄像机
            E-表示电梯控制器
            D-表示门机控制器
        第二个字符：
            5-触控机
            6-单机版图片机
            7-网络版图片机
            8-单机版视频机
            9-网络版视频机
        第三个字符：
            CODE（一维码）
*/
class ElevatorNameParse
{
    ElevatorNameParse(QString pName)
    {
        *this = pName;
    }
    bool operator==(const ElevatorNameParse &s)
    {
        for(int i=0;i<2;i++)
        {
            if(mType[i] != s.mType[i])
                return false;
        }
        if(mCode != s.mCode)
            return false;
        return true;
    }
    bool operator!=(const ElevatorNameParse &s)  { return !operator==(s); }

    ElevatorNameParse & operator=(const ElevatorNameParse &s)
    {
        for(int i=0;i<2;i++)
            mType[i] = s.mType[i];
        mCode = s.mCode;
        return *this;
    }
    ElevatorNameParse & operator=(QString s)
    {
        if(s.count() > 2)
        {
            mType[0] = s.at(0).toLatin1();
            mType[1] = s.at(1).toLatin1();
            mCode = s.mid(2).toLatin1();
        }
        return *this;
    }
public:
    quint8  mType[2];
    QByteArray  mCode;
};

class sevManager : public xmlParse
{
    Q_OBJECT
public:
    sevManager(QString fileName);
    ~sevManager(){}

    static const QString SevSuffix;
    static const QString SevFileType;

    //>@操作sev文件
    bool NewSev();
    bool OpenSev();
    bool CloseSev();
    bool DelSev();
    bool SaveSev();
    bool RenameSev(QString FileName);

    bool GetElevatorGroup();

public:
    QHash<QString, QDomElement> m_ElevatorElementGroup;
};

class devInfoManager : public xmlParse
{
    Q_OBJECT
public:
    devInfoManager(QString fileName);
    ~devInfoManager(){}

    static const QString DevInfoSuffix;
    static const QString DevInfoFileType;

    //>@操作ui文件
    bool NewDevInfo();
    bool OpenDevInfo();
    bool CloseDevInfo();
    bool DelDevInfo();
    bool SaveDevInfo();
    bool RenameDevInfo(QString FileName);
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

class updateManager : public xmlParse
{
    Q_OBJECT
public:
    updateManager(QString fileName);
    ~updateManager(){}

    static const QString UpdateSuffix;
    static const QString UpdateFileType;

    //>@操作ui文件
    bool OpenUpdate();
    bool CloseUpdate();
    bool SaveUpdate();

    QString GetVersion(QString pName);
};

/*Burn文件定义：
    <?xml version='1.0' encoding='UTF-8'?>
    <root>
        <FileType>BST BURN File</FileType>
        <Code>BST</Code>
        <HardwareCheck>
            <LostMaterial>
                <集成块>U1;U2</集成块>
            </LostMaterial>
            <ShortCircuit>
                <集成块 type="虚焊">U1;U2</集成块>
            </ShortCircuit>
            <PowerSuface></PowerSuface>
        </HardwareCheck>
        <UsbBurn>
            <Program1>true</Program1>
            <Program2>false</Program2>
        </UsbBurn>
    </root>
*/

class burnManager : public xmlParse
{
    Q_OBJECT
public:
    burnManager(QString fileName);
    ~burnManager(){}

    static const QString BurnSuffix;
    static const QString BurnFileType;

    //>@操作Burn文件
    bool NewBurn();
    bool OpenBurn();
    bool CloseBurn();
    bool DelBurn();
    bool SaveBurn();
    bool RenameBurn(QString FileName);
};

#endif // GLOBAL_H



