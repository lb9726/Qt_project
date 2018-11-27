#ifndef UIPARSER_H
#define UIPARSER_H

#include "xmlparser.h"

//>@---------------UI依赖函数-------------------

UI_TYPE GetUiType(QString pUiName);
QString GetUiName(UI_TYPE pUiType);
QString GetUiPath(UI_TYPE pType);
UI_TYPE GetParentUiType(QString pUiPath);
UI_TYPE GetParentUiType(UI_TYPE pType);
QString GetCurUiName(QString pUiPath);
QString GetParentUiPath(QString pUiPath);
QString GetUiPath(QString pUiName);
UI_TYPE GetCurUiType(QString pUiPath);

/*
    横竖显自动布局算法： 除桌面外，对于具有图像元素的组件必须保证宽高相同，修改坐标时需要实时判断是否宽高超出横竖版桌面的宽高
    1、竖屏自动布局原则：
        如果所有有区域的左上角坐标都为0，则表示从未自动布局过，则进行一次自动布局；否则不对区域进行自动布局
            所有区域进行列排放，优先级取决于UI_TYPE的大小
        区域内部的组件进行多行排放，也即排完第一行再排第二行的原则，优先级取决于UI_TYPE的大小
            通过二维数组记录
            一行最多有三个组件，每一行都是平均分配空隙（除因子为行组件个数+1），
            在列方面，每个组件在自己所在的行中要居中
        最终由区域内部已排列的组件计算区域的最小大小，如果区域的大小大于此最小大小，则不变，否则区域最终的坐标中高度使用最小高度，宽度使用桌面宽度

    2、横屏自动布局原则
        从不对区域进行自动布局，完全由用户进行拖放设置，因此一般设计界面时先设计横版，再设计竖版
        区域内部的组件进行多行排放，也即排完第一行再排第二行的原则，优先级取决于UI_TYPE的大小
        最终由区域内部已排列的组件计算区域的最小大小，如果区域的大小大于此最小大小，则不变，否则区域最终的坐标中高度使用最小高度，宽带使用最小宽度

    3、拖动组件，同步原则
        对于区域，不需要进行任何检测
        对于组件，需要实时检测其宽度或者高度，查看其所在方位是否已经超出当前横竖版的桌面，如果是，则提示注意，如果执意修改，则锁定当前主题的横竖切换功能使之仅能适应一种界面

    4、界面初始化调整区域原则
        首先针查看自己所有的子控件是否在区域外，如果是，则自动调整到子控件的最大大小，
        然后查看区域是否在桌面外，如果是，则调整到桌面内
*/

#define D_INVALID       QString("Invalid")
#define D_TYPE          QString("Type")
#define D_PARA          QString("Para")

#define PARA_CURVESHAPE  QString("AnimateType")  //>@特效动画类型
#define PARA_FRAMERANGE  QString("FrameRange")   //>@特效帧频率
#define PARA_LOOPCOUNT   QString("LoopCount")    //>@循环次数
#define PARA_INTERVAL    QString("Interval")     //>@特效时长
#define PARA_DIRECTION   QString("Direction")    //>@特效方向
#define PARA_ORIENTATION QString("Orientation")  //>@特效正逆序
#define PARA_AMOUNT      QString("Amount")       //>@数量
#define PARA_DUTYRATIO   QString("DutyRatio")    //>@占空比
#define PARA_AUTOPLAY    QString("AutoPlay")
#define PARA_RCTYPE      QString("RcType")       //>@资源类型：Image/Text

#define PARA_LAYOUT      QString("Layout")       //>@布局
#define PARA_ROW         QString("Row")          //>@布局中行总数
#define PARA_COLUMN      QString("Column")       //>@布局中列总数
#define PARA_RSPACE      QString("RowSpace")     //>@布局中元素横向间隔
#define PARA_CSPACE      QString("ColumnSpace")  //>@布局中元素纵向间隔

#define PARA_STYLE       QString("Style")        //>@布局样式
#define PARA_ALIGN       QString("Align")
#define PARA_PATTERN     QString("Pattern")

//>@3D
#define PARA_ANGLEDIFF   QString("AngleDiff")       //>@一次旋转角度

typedef int         ( *_StrToEnum )  ( QString );
typedef QVariant	( *_StrToValue )  ( int, QString );

typedef QString     ( *_EnumToStr )  ( int );
typedef QString     ( *_ValueToStr )  ( int, QVariant );

typedef int         ( *_StrToInt )  ( QString );

typedef bool     	( *_ParsePara )  ( QString, QString, QHash<int, QVariant> );
typedef QString  	( *_MergePara )  ( int, QVariant);

class AREA_ANIMATE;
typedef bool     	( *_ParseAnimatePara )  ( QString, QString, AREA_ANIMATE* );

#define D_INVALID_NUM       (0x7f)

#define D_PREFIX              QString("Prefix")
#define D_INITVALUE           QString("Init")
#define D_DEFAULT_PREFIX      QString("RC")

#define D_MAP                 QString("Map")

/****************************************************************************
    界面字符串
****************************************************************************/

QRect GetRectFromString(QString pRectString);
QString GetStringFromRect(QRect pRect);

QRectF GetRotateRect(QRectF pRect, ROTATE_ANGLE pAngle);
CONSTRAINT GetRotateConstraint(CONSTRAINT pConstraint, ROTATE_ANGLE pAngle);

/****************************************************************************
  结构体
****************************************************************************/

inline QTimeLine::CurveShape GetTimelineType(QString pContent)
{
    if(pContent.compare(QString("EaseIn"), Qt::CaseInsensitive) == 0)
        return QTimeLine::EaseInCurve;
    else if(pContent.compare(QString("EaseOut"), Qt::CaseInsensitive) == 0)
        return QTimeLine::EaseOutCurve;
    else if(pContent.compare(QString("EaseInOut"), Qt::CaseInsensitive) == 0)
        return QTimeLine::EaseInOutCurve;
    else if(pContent.compare(QString("Linear"), Qt::CaseInsensitive) == 0)
        return QTimeLine::LinearCurve;
    else if(pContent.compare(QString("Sine"), Qt::CaseInsensitive) == 0)
        return QTimeLine::SineCurve;
    else if(pContent.compare(QString("Cosine"), Qt::CaseInsensitive) == 0)
        return QTimeLine::CosineCurve;
    return QTimeLine::EaseInOutCurve;
}

#define D_ENUMSPLIT     QString(",")
#define D_RANGESPLIT    QString("~")
QStringList PreprocPara(QString &pContent);

class UiComParser;

/*
对于XX:1@xx,xx格式的属性
QVariant
    0：int类型，序号
    1：QList<QVariant>类型，可选值
*/
class UiParaParser
{
public:
    UiParaParser(UiComParser *pComParser);
    UiParaParser(QString pString, UiComParser *pComParser);
    ~UiParaParser();

    void SetString(QString pString);

    //>@ XX:100;XX:1@xx,xx;
    QVariant GetPara(int pName);
    void SetPara(int pName, QVariant pValue);

    QString GetString(_MergePara pMergeParaFun);

public:
    QHash<int, QVariant>        mParaHash;  //>@对于某个资源的参数定义
    UiComParser                *mComParser;
};

//<Layout0 H="258,10,198,198" V="37,222,198,198"/>
class UiRectParser : public UiParaParser
{
public:
    UiRectParser(QDomElement pElement, UiComParser *pComParser);

    QRect GetRect(DIRECTION pDirection);
    void SetRect(DIRECTION pDirection, QRect pRect);

public:
    QDomElement                 mElement;
    bool                        mHVSame;        //>@横竖版资源是否相同，如果为true，则只需要取m_Rect【0】即可
    QRect                       mRect[2];      //>@0:Vertical, 1:Horitical
};

/*
    <Rect>
        <Layout0 H="258,10,198,198" V="37,222,198,198"/>
        <Layout1 H="258,10,198,198" V="37,222,198,198"/>
        <Layout2 H="258,10,198,198" V="37,222,198,198"/>
        <Layout3 H="258,10,198,198" V="37,222,198,198"/>
    </Rect>
*/
class UiRectListParser : public UiParaParser
{
public:
    UiRectListParser(QDomElement pElement, UiComParser *pComParser);
    ~UiRectListParser();

    UiRectParser* GetRect(int pLayout);
    QRect GetRect(int pLayout, DIRECTION pDirection);
    bool SetRect(int pLayout, DIRECTION pDirection, QRect pRect);

    UiRectParser* AddRect(int pLayout, XmlParser* pParser);
    void DelRect(int pLayout);

public:
    QDomElement                 mElement;
    QHash<int, UiRectParser*>   mRects;
};

typedef union{
    void          *mVAR;
    QImage        *mIMG;
    QMovie        *mGIF;
#ifdef  SUPPORT_SVG
    QSvgRenderer  *mSVG;
#endif
#ifdef  SUPPORT_QUI
    iWidget       *mQUI;
#endif
} UD_RCBUFFER;       //>@图形指针

typedef struct{
    int            mKey;        //>@映射UI对象的资源号
    AREA_OPERATE   mOperate;    //>@映射UI对象的最终操作
} UD_MAPITEM;

//<RC0 Para="Cache:true;Absolute:false" H="Desktop/Resource/RC0.png" Map="Arrow:1@Run;"/>
class UiRcParser: public UiParaParser
{
public:
    UiRcParser(QDomElement pElement, UiComParser *pComParser);
    ~UiRcParser();

    void UpdateHV(bool pSame);
    void Release();

    RC_TYPE GetType();
    bool GetRc(DIRECTION pDirection, QSize pSize=QSize());
    void SetRc(QString pDir, QString pFile, DIRECTION pDirection);  //>@pFile为绝对路径下的地址

    QImage* GetImage(DIRECTION pDirection, QSize pSize=QSize());
    bool IsImage(DIRECTION pDirection);

public:
    QDomElement                     mElement;

    bool                            mHVSame;        //>@横竖版资源是否相同
    RC_TYPE                         mType[2];          //>@资源类型，默认为PNG图片
    QString                         mFile[2];          //>@资源文件

    DIRECTION                       mCurIndex;
    UD_RCBUFFER                     mBuffer;        //>@图形指针

    //>@映射，用于同步其它控件的显示
    QHash<UI_TYPE, UD_MAPITEM*>     mMaps;          //>@映射列表
};

class UiRcListParser : public UiParaParser
{
public:
    UiRcListParser(QDomElement pElement, UiComParser *pComParser);
    ~UiRcListParser();

    UiRcParser* NextRc();
    UiRcParser* PrevRc();

    bool Contains(int pKey);
    UiRcParser* GetRc(int pKey);

    void AddRc(int pKey, UiRcParser* pRcInfo);
    void AddRc(int pKey, XmlParser* pParser);
    void DelRc(UiRcParser* pRcInfo);
    void DelRc(int pKey);

public:
    QDomElement                     mElement;

    QHash<int, UiRcParser*>         mRCs;
    QString                         mPrefix;

    UiRcParser                     *mCurValue;
    int                             mCurKey;     //>@当前正在使用的资源序号
};

//<Run Type="Roll" Para="AnimateType:EaseInOut;Interval:2000;FrameRange:20;Direction:V;Orientation:P;"/>
class UiComParser;
class UiOperateParser : public QThread
{
    Q_OBJECT
public:
    UiOperateParser(EffectType pType, QDomElement pElement, UiComParser *pComParser);
    ~UiOperateParser();

    enum ParaType{
        E_TYPE,         //>@特效类型，默认为无特效
        E_DIRECTION,    //>@图片特效方向
        E_ORIENTATION,  //>@特效正逆序
        E_LOOP,         //>@为0表示永远循环
        E_FRASTART,
        E_FRAEND,
        E_TIME,
        E_DUTYRATIO,
        E_CURVETYPE
    };

    void Render(UiComParser *pRenderCom);
    void UnRender(UiComParser *pRenderCom);

    void Start();
    void Stop();
    void SetPause(bool pFlag);

    void SetPara(ParaType pType, int pPara);
    int  GetPara(ParaType pType);

    void run();

    QTimeLine::State state();

public slots:
    void slot_FrameChanged(int pFrame);
    void slot_Finished();

signals:
    void sStop();
    void sSetPaused(bool paused);
    void sFinished();

public:
    QDomElement                         mElement;
    AREA_ANIMATE                       *mAnimatePara;

    QList<UiComParser*>                 mRenderComs;

    UiComParser                        *mComParser;

    QTimeLine::State                    mRunMode;
};

class UiOperateListParser : public UiParaParser
{
public:
    UiOperateListParser(QDomElement pElement, UiComParser *pComParser);
    ~UiOperateListParser();

    UiOperateParser* GetOperate(AREA_OPERATE pKey);
    AREA_ANIMATE* GetOperatePara(AREA_OPERATE pKey);

    void AddOperate(AREA_OPERATE pKey, UiOperateParser* pInfo);
    UiOperateParser *AddOperate(AREA_OPERATE pKey, XmlParser* pParser);
    void DelOperate(AREA_OPERATE pKey);

public:
    QDomElement                            mElement;
    QHash<AREA_OPERATE, UiOperateParser*>  mOperations;  //>@特效描述
};

class UiParser;

class UiComParser : public UiParaParser
{
public:
    UiComParser();
    ~UiComParser();

    bool Init(QDomElement pElement, UiParser *pParser = 0);
    virtual bool InitSubUI(QDomElement pElement, UiParser *pParser = 0);

    void Clear(bool pSync = true);

    //>@----------------------------------------------------------------------
    //>@----    参数解析实现
    virtual bool ParsePara(QString pLabel, QString pContent, QHash<int, QVariant> pHash) {return false;}

    //>@Rect
    virtual bool ParseRectPara(QString pLabel, QString pContent, QHash<int, QVariant> pHash) {return false;}
    virtual bool ParseSubRectPara(QString pLabel, QString pContent, QHash<int, QVariant> pHash) {return false;}

    //>@Resource
    virtual bool ParseRcPara(QString pLabel, QString pContent, QHash<int, QVariant> pHash) {return false;}
    virtual bool ParseSubRcPara(QString pLabel, QString pContent, QHash<int, QVariant> pHash) {return false;}

    //>@Map
    virtual bool ParseMapPara(QString pLabel, QString pContent, QHash<int, QVariant> pHash) {return false;}
    virtual bool ParseSubMapPara(QString pLabel, QString pContent, QHash<int, QVariant> pHash) {return false;}

    //>@Map
    virtual bool ParseOperationPara(QString pLabel, QString pContent, QHash<int, QVariant> pHash) {return false;}
    virtual bool ParseSubOperationPara(QString pLabel, QString pContent, AREA_ANIMATE* pAreaEffect);

    virtual bool PaintBuffer(UiOperateParser* pAnimate, int pFrame) = 0;

public:
    UiParser                               *mUiParser;
    QDomElement                             mElement;
    bool                                    mExternPara;  //>@指示资源是否为外部带入的，如果是，则在注销时不能删除，只清空

    int                                     mLayout;      //
    int                                     mStyle;

    UiRectListParser                       *mComRect;
    UiRcListParser                         *mComResource;      //>@只需要记录路径的资源
    UiOperateListParser                    *mComOperation;

    QList<QImage*>                          mCurRcs;
    QImage                                  mImgBuffer;  //一级缓冲
    E_WORK_STATE                            mBufState;
};

#define D_TYPE_USER_DEFINE      100

/*
<Desktop Para="" Layout="1" Style="2">
    <Rect>
        <Layout0 H="258,10,198,198" V="37,222,198,198"/>
        <Layout1 H="258,10,198,198" V="37,222,198,198"/>
        <Layout2 H="258,10,198,198" V="37,222,198,198"/>
        <Layout3 H="258,10,198,198" V="37,222,198,198"/>
    </Rect>
    <Resource Prefix="RC" Init="1">
        <RC0 Para="Cache:true;Absolute:false" H="Desktop/Resource/RC0.png"/>
        <RC1 Para="StyleSheet:rc.qss" H="Desktop/Resource/RC0.ui"/>
        <RC2 HV="Desktop/Resource/RC0.qml"/>
        <RC3 Para="Repeat:true;SoloTrack:true;Volume:80;" HV="Desktop/Resource/RC0.wav"/>
        <RC4 H="Desktop/Resource/RC0.mp4"/>
    </Resource>
    <Operation>
        <HAState Type="Invalid" Para=""/>
        <Run Type="Roll" Para="AnimateType:EaseInOut;Interval:2000;FrameRange:20;Direction:V;Orientation:P;"/>
        <Halt Type="Invalid"/>
    </Operation>
    <SubUI>
        <Desktop Para="">
            <Rect>
                <Layout0 H="258,10,198,198" V="37,222,198,198"/>
                <Layout1 H="258,10,198,198" V="37,222,198,198"/>
                <Layout2 H="258,10,198,198" V="37,222,198,198"/>
                <Layout3 H="258,10,198,198" V="37,222,198,198"/>
            </Rect>
        </Desktop>
    </SubUI>
</Desktop>
*/

class UiParser : public XmlParser
{
    Q_OBJECT
public:
    UiParser(QString fileName);

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

    int GetLayout();
    bool SetLayout(int pLayout);

    int GetStyle();
    bool SetStyle(int pStyle);

    //>@操作文件中的元素
    void GetUiGroup();

public:
    //>@xx.ui文件中的组件（subui和control）的Dom元素(xx.ui)
    QHash<UI_TYPE, QDomElement>  m_ComElementGroup;

    int                          mLayout;
    int                          mStyle;
};

class QUiParser : public XmlParser
{
    Q_OBJECT
public:
    QUiParser(QString pFileName);
    void GetUiGroup(QDomElement pElement);

    QRect GetRect(UI_TYPE pType);
    void ModifyRect(UI_TYPE pType, QRect pRect);

public:
    QHash<UI_TYPE, QDomElement>  m_ComElementGroup;
};

#endif // UIPARSER_H
