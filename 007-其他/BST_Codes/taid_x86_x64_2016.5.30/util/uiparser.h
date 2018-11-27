#ifndef UIPARSER_H
#define UIPARSER_H

#include "xmlparser.h"
#include "filefunc.h"
//#include "gui.h"

typedef enum{
    E_GUI_NONE = 0,
    E_GUI_COMBOX,
    E_GUI_CHECKBOX,
    E_GUI_BUTTON
}E_GUI_TYPE;

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

inline int GetKey(QString pString, QString pPrefix = D_DEFAULT_PREFIX)
{
    if(pString.isEmpty())
        return -1;
    int pos = pString.lastIndexOf('.');
    QString tmpName;
    if(pos < 0)
        tmpName = pString;
    else
        tmpName = pString.mid(0, pos);
    //IDE_TRACE_STR(tmpName);
    if(pPrefix.isEmpty())
        return tmpName.toInt();
    if(tmpName.startsWith(pPrefix,Qt::CaseInsensitive) == false)
        return tmpName.toInt();
    tmpName.remove(0, pPrefix.count());
    bool ok = false;
    int index = tmpName.toInt(&ok, 10);
    if(ok == false)
        return -1;
    return index;
}

inline quint32 GetRcNum(QString pRc)
{
    bool ok=false;
    quint32 tmpNum = INVALIDRC;
    tmpNum = pRc.toUInt(&ok, 10);
    if(!ok)
    {
        if(pRc.compare("Next", Qt::CaseInsensitive) == 0)
            tmpNum = NEXTRC;
        else if(pRc.contains("Prev", Qt::CaseInsensitive))
            tmpNum =  PREVRC;
    }
    return tmpNum;
}

typedef enum{
    Vertical=0,
    Horizontal=1,
    HVBoth=2,
    HVNone
} DIRECTION;        //>@横向 纵向

inline DIRECTION GetDirection(int pRotate)
{
    if(pRotate == 90 || pRotate == 270)
        return Vertical;
    return Horizontal;
}

inline DIRECTION GetDirection(QString pDirection)
{
    if(pDirection.isEmpty())
        return HVNone;
    if(pDirection.compare("V") == 0)
        return Vertical;
    else if(pDirection.compare("H") == 0)
        return Horizontal;
    else if(pDirection.compare("HV") == 0)
        return HVBoth;
    return HVNone;
}

inline QString GetDirectionString(DIRECTION pDirection)
{
    QString tmpDirString;
    if(pDirection == Vertical)
        tmpDirString = "V";
    else if(pDirection == Horizontal)
        tmpDirString = "H";
    else if(pDirection == HVBoth)
        tmpDirString = "HV";
    return tmpDirString;
}

typedef enum{
    Positive=0,
    Negative=1
} ORIENTATION;      //>@正序 逆序

typedef enum{
    ROTATE_0 = 0,
    ROTATE_90 = 90,
    ROTATE_180 = 180,
    ROTATE_270 = 270
} ROTATE_ANGLE;

typedef struct{
    DIRECTION mDirection;
    ORIENTATION mOrientation;
} CONSTRAINT;

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

typedef enum{
    EffectTypeNone=0,
    EffectTypeTileRoll,
    EffectTypeRoll,
    EffectTypeFlip,
    EffectTypeBlink,    //>@闪烁
    EffectTypeRotate,
    EffectTypeTwirl,
    EffectTypeZoom,
    EffectTypeTransparent,
    EffectTypeSpread,
    EffectTypeWheel,
    EffectTypeSlipCycle,
    EffectTypeEnd,
    //>@还未实现的特效
    EffectTypeMove,
    EffectTypeGlide,
    EffectTypeFireworks,//>@烟花
    EffectTypeViewer,
    EffectTypeCube,
    EffectTypeCylinder,
    EffectTypeShake,    //>@震动
    EffectTypeBulge,    //>@凸出
    EffectTypeIndent,   //>@凹入
    EffectTypeSpectrum,
    EffectTypeShine,
    EffectTypeSwipe,
    EffectTypeBtnZoom
}EffectType;

Q_DECLARE_METATYPE(EffectType)   //>@自定义信号参数

inline EffectType GetEffectType(QString pType)
{
    if(pType.compare("Roll", Qt::CaseInsensitive) == 0)
        return EffectTypeRoll;
    else if(pType.compare("TileRoll", Qt::CaseInsensitive) == 0)
        return EffectTypeTileRoll;
    else if(pType.compare("Zoom", Qt::CaseInsensitive) == 0)//>@缩放
        return EffectTypeZoom;
    else if(pType.compare("Transparent", Qt::CaseInsensitive) == 0)//>@渐变透明
        return EffectTypeTransparent;
    else if(pType.compare("Spectrum", Qt::CaseInsensitive) == 0)//>@频谱
        return EffectTypeSpectrum;
    else if(pType.compare("Spread", Qt::CaseInsensitive) == 0)//>@扩散
        return EffectTypeSpread;
    else if(pType.compare("Wheel", Qt::CaseInsensitive) == 0)//>@光圈
        return EffectTypeWheel;
    else if(pType.compare("Rotate", Qt::CaseInsensitive) == 0)//>@旋转
        return EffectTypeRotate;
    else if(pType.compare("Twirl", Qt::CaseInsensitive) == 0)//>@转动
        return EffectTypeTwirl;
    else if(pType.compare("SlipCycle", Qt::CaseInsensitive) == 0)//>@回滚
        return EffectTypeSlipCycle;
    else if(pType.compare("Blink", Qt::CaseInsensitive) == 0)//>@闪烁
        return EffectTypeBlink;
    else if(pType.compare("Flip", Qt::CaseInsensitive) == 0)
        return EffectTypeFlip;
    else if(pType.compare("Move", Qt::CaseInsensitive) == 0)
        return EffectTypeMove;
    else if(pType.compare("Glide", Qt::CaseInsensitive) == 0)//>@滑动
        return EffectTypeGlide;
    else if(pType.compare("Fireworks", Qt::CaseInsensitive) == 0)//>@烟花
        return EffectTypeFireworks;
    else if(pType.compare("Viewer", Qt::CaseInsensitive) == 0)
        return EffectTypeViewer;
    else if(pType.compare("Cube", Qt::CaseInsensitive) == 0)
        return EffectTypeCube;
    else if(pType.compare("Cylinder", Qt::CaseInsensitive) == 0)
        return EffectTypeCylinder;
    else if(pType.compare("Shake", Qt::CaseInsensitive) == 0)//>@震动
        return EffectTypeShake;
    else if(pType.compare("Bulge", Qt::CaseInsensitive) == 0)//>@凸出
        return EffectTypeBulge;
    else if(pType.compare("Indent", Qt::CaseInsensitive) == 0)//>@凹入
        return EffectTypeIndent;
    else if(pType.compare("Shine", Qt::CaseInsensitive) == 0)//>@发光
        return EffectTypeShine;
    else if(pType.compare("Swipe", Qt::CaseInsensitive) == 0)//>@拨动
        return EffectTypeSwipe;
    else if(pType.compare("BtnZoom", Qt::CaseInsensitive) == 0)//>@拨动
        return EffectTypeBtnZoom;
    return EffectTypeNone;
}

inline QString GetEffectTypeString(EffectType pType)
{
    if(pType == EffectTypeRoll)
        return QString("Roll");
    else if(pType == EffectTypeTileRoll)
        return QString("TileRoll");
    else if(pType == EffectTypeZoom)//>@缩放
        return QString("Zoom");
    else if(pType == EffectTypeTransparent)//>@渐变透明
        return QString("Transparent");
    else if(pType == EffectTypeSpectrum)//>@频谱
        return QString("Spectrum");
    else if(pType == EffectTypeSpread)//>@扩散
        return QString("Spread");
    else if(pType == EffectTypeWheel)//>@光圈
        return QString("Wheel");
    else if(pType == EffectTypeRotate)//>@旋转
        return QString("Rotate");
    else if(pType == EffectTypeTwirl)//>@转动
        return QString("Twirl");
    else if(pType == EffectTypeSlipCycle)//>@回滚
        return QString("SlipCycle");
    else if(pType == EffectTypeBlink)//>@闪烁
        return QString("Blink");
    else if(pType == EffectTypeFlip)
        return QString("Flip");
    else if(pType == EffectTypeMove)
        return QString("Move");
    else if(pType == EffectTypeGlide)//>@滑动
        return QString("Glide");
    else if(pType == EffectTypeFireworks)//>@烟花
        return QString("Fireworks");
    else if(pType == EffectTypeViewer)
        return QString("Viewer");
    else if(pType == EffectTypeCube)
        return QString("Cube");
    else if(pType == EffectTypeCylinder)
        return QString("Cylinder");
    else if(pType == EffectTypeShake)//>@震动
        return QString("Shake");
    else if(pType == EffectTypeBulge)//>@凸出
        return QString("Bulge");
    else if(pType == EffectTypeIndent)//>@凹入
        return QString("Indent");
    else if(pType == EffectTypeShine)//>@发光
        return QString("Shine");
    else if(pType == EffectTypeSwipe)//>@拨动
        return QString("Swipe");
    else if(pType == EffectTypeBtnZoom)//>@拨动
        return QString("BtnZoom");
    return QString("None");
}


typedef enum{
    OPERATE_NONE = 0,
    //>@
    OPERATE_FORM,
    OPERATE_RC,
    OPERATE_MAP,
    //>@
    OPERATE_INIT,
    OPERATE_RELEASE,
    //>@
    OPERATE_PROGRESS,
    //>@动作参数
    OPERATE_CHANGE,
    OPERATE_SLIP,   //>@滑动
    OPERATE_TURN, //>@转向
    OPERATE_ADD,
    OPERATE_MODIFY,
    OPERATE_DEL,
    //>@
    OPERATE_CLEAR,
    OPERATE_SHOW,
    OPERATE_HIDE,
    //>@播放参数
    OPERATE_RUN,
    OPERATE_HALT,
    OPERATE_PLAY,
    OPERATE_PAUSE,
    OPERATE_STOP,
    OPERATE_STEADY,
    OPERATE_VOCAL,  //>@有声
    OPERATE_MUTE,   //>@静音
    OPERATE_RESUME,
    OPERATE_REPEAT,
    OPERATE_KEYCLICK,
    OPERATE_KEYPRESS,
    OPERATE_KEYLIGHT,
    OPERATE_KEYCANCEL,
    OPERATE_KEYMOVE,
    OPERATE_KEYRELEASE
} AREA_OPERATE;

Q_DECLARE_METATYPE(AREA_OPERATE)   //>@自定义信号参数

inline AREA_OPERATE GetOperateType(QString pType)
{
    if(pType.compare("RESOURCE", Qt::CaseInsensitive) == 0)
        return OPERATE_RC;
    else if(pType.compare("MAP", Qt::CaseInsensitive) == 0)
        return OPERATE_MAP;
    else if(pType.compare("FORM", Qt::CaseInsensitive) == 0)
        return OPERATE_FORM;
    else if(pType.compare("INIT", Qt::CaseInsensitive) == 0)
        return OPERATE_INIT;
    else if(pType.compare("RELEASE", Qt::CaseInsensitive) == 0)
        return OPERATE_RELEASE;
    else if(pType.compare("HASTATE", Qt::CaseInsensitive) == 0)
        return OPERATE_CHANGE;
    else if(pType.compare("Change", Qt::CaseInsensitive) == 0)
        return OPERATE_CHANGE;
    else if(pType.compare("SLIP", Qt::CaseInsensitive) == 0)
        return OPERATE_SLIP;
    else if(pType.compare("TURN", Qt::CaseInsensitive) == 0)
        return OPERATE_TURN;
    else if(pType.compare("ADD", Qt::CaseInsensitive) == 0)
        return OPERATE_ADD;
    else if(pType.compare("MODIFY", Qt::CaseInsensitive) == 0)
        return OPERATE_MODIFY;
    else if(pType.compare("DEL", Qt::CaseInsensitive) == 0)
        return OPERATE_DEL;
    else if(pType.compare("SHOW", Qt::CaseInsensitive) == 0)
        return OPERATE_SHOW;
    else if(pType.compare("HIDE", Qt::CaseInsensitive) == 0)
        return OPERATE_HIDE;
    else if(pType.compare("RUN", Qt::CaseInsensitive) == 0)
        return OPERATE_RUN;
    else if(pType.compare("HALT", Qt::CaseInsensitive) == 0)
        return OPERATE_HALT;
    else if(pType.compare("PLAY", Qt::CaseInsensitive) == 0)
        return OPERATE_PLAY;
    else if(pType.compare("PAUSE", Qt::CaseInsensitive) == 0)
        return OPERATE_PAUSE;
    else if(pType.compare("STOP", Qt::CaseInsensitive) == 0)
        return OPERATE_STOP;
    else if(pType.compare("STEADY", Qt::CaseInsensitive) == 0)
        return OPERATE_STEADY;
    else if(pType.compare("VOCAL", Qt::CaseInsensitive) == 0)
        return OPERATE_VOCAL;
    else if(pType.compare("MUTE", Qt::CaseInsensitive) == 0)
        return OPERATE_MUTE;
    else if(pType.compare("RESUME", Qt::CaseInsensitive) == 0)
        return OPERATE_RESUME;
    else if(pType.compare("REPEAT", Qt::CaseInsensitive) == 0)
        return OPERATE_REPEAT;
    else if(pType.compare("KEYPRESS", Qt::CaseInsensitive) == 0)
        return OPERATE_KEYPRESS;
    else if(pType.compare("KEYLIGHT", Qt::CaseInsensitive) == 0)
        return OPERATE_KEYLIGHT;
    else if(pType.compare("KEYCANCEL", Qt::CaseInsensitive) == 0)
        return OPERATE_KEYCANCEL;
    else if(pType.compare("KEYMOVE", Qt::CaseInsensitive) == 0)
        return OPERATE_KEYMOVE;
    else if(pType.compare("KEYRELEASE", Qt::CaseInsensitive) == 0)
        return OPERATE_KEYRELEASE;
    return OPERATE_NONE;
}

inline QString GetOperateString(AREA_OPERATE pType)
{
    switch(pType)
    {
    case OPERATE_FORM:
        return QString("FORM");
    case OPERATE_RC:
        return QString("RESOURCE");
    case OPERATE_MAP:
        return QString("MAP");
    case OPERATE_INIT:
        return QString("INIT");
    case OPERATE_RELEASE:
        return QString("RELEASE");
    case OPERATE_CHANGE:
        return QString("HASTATE");
    case OPERATE_SLIP:
        return QString("SLIP");
    case OPERATE_TURN:
        return QString("TURN");
    case OPERATE_ADD:
        return QString("ADD");
    case OPERATE_MODIFY:
        return QString("MODIFY");
    case OPERATE_DEL:
        return QString("DEL");
    case OPERATE_SHOW:
        return QString("SHOW");
    case OPERATE_HIDE:
        return QString("HIDE");
    case OPERATE_RUN:
        return QString("RUN");
    case OPERATE_HALT:
        return QString("HALT");
    case OPERATE_PLAY:
        return QString("PLAY");
    case OPERATE_PAUSE:
        return QString("PAUSE");
    case OPERATE_STOP:
        return QString("STOP");
    case OPERATE_STEADY:
        return QString("STEADY");
    case OPERATE_VOCAL:
        return QString("VOCAL");
    case OPERATE_MUTE:
        return QString("MUTE");
    case OPERATE_RESUME:
        return QString("RESUME");
    case OPERATE_REPEAT:
        return QString("REPEAT");
    case OPERATE_KEYPRESS:
        return QString("KEYPRESS");
    case OPERATE_KEYLIGHT:
        return QString("KEYLIGHT");
    case OPERATE_KEYCANCEL:
        return QString("KEYCANCEL");
    case OPERATE_KEYMOVE:
        return QString("KEYMOVE");
    case OPERATE_KEYRELEASE:
        return QString("KEYRELEASE");
    default:
        break;
    }
    return QString("UNKNOW");
}

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

class AREA_ANIMATE
{
public:
    AREA_ANIMATE()
    {
        mEffectType = EffectTypeNone;
        mDirection = Horizontal;
        mOrientation = Positive;
        mFrameStart = 0;
        mFrameEnd = 10;
        mFrameCount = mFrameEnd - mFrameStart -1;
        mAnimateTime = 200;
        mLoopCount = 1;        //>@循环次数，如果为0，则表示无限循环
        mDutyRatio = 50;        //>@占空比：0~100
        mAnimateType = QTimeLine::LinearCurve;
    }
    AREA_ANIMATE & operator=(const AREA_ANIMATE &s)
    {
        mEffectType = s.mEffectType;
        mDirection = s.mDirection;
        mOrientation = s.mOrientation;
        mLoopCount = s.mLoopCount;
        mFrameStart = s.mFrameStart;
        mFrameEnd = s.mFrameEnd;
        mFrameCount = s.mFrameCount;
        mAnimateTime = s.mAnimateTime;
        mDutyRatio = s.mDutyRatio;
        mAnimateType = s.mAnimateType;
        return *this;
    }
public:
    EffectType              mEffectType;   //>@特效类型，默认为无特效
    DIRECTION               mDirection;    //>@图片特效方向
    ORIENTATION             mOrientation;  //>@特效正逆序
    int                     mLoopCount;    //>@为0表示永远循环
    int                     mFrameStart, mFrameEnd, mFrameCount;
    int                     mAnimateTime;
    int                     mDutyRatio;
    QTimeLine::CurveShape   mAnimateType;
};
Q_DECLARE_METATYPE(AREA_ANIMATE)

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
    bool SetRc(QString pDir,QString pFile, DIRECTION pDirection);  //>@pFile为绝对路径下的地址
    QString GetAttr(QString attrName);
    void SetAttr(QString attrName,QString value);

public:
    QDomElement                     mElement;

    bool                            mHVSame;        //>@横竖版资源是否相同
    RC_TYPE                         mType[2];          //>@资源类型，默认为PNG图片
    QString                         mFile[2];          //>@资源文件

    DIRECTION                       mCurIndex;

    QHash<UI_TYPE, UD_MAPITEM*>     mMaps;          //>@映射列表
};

class UiRcListParser : public UiParaParser
{
public:
    UiRcListParser(QDomElement pElement, UiComParser *pComParser);
    ~UiRcListParser();

    bool Contains(int pKey);
    UiRcParser* GetRc(int pKey);

    void AddRc(int pKey, UiRcParser* pRcInfo);
    UiRcParser* AddRc(int pKey, XmlParser* pParser,bool forceAdd);
    void DelRc(UiRcParser* pRcInfo);
    void DelRc(int pKey);

public:
    QDomElement                     mElement;

    QHash<int, UiRcParser*>         mRCs;
    QString                         mPrefix;

    UiRcParser                     *mCurValue;
    int                             mCurKey;
};

//<Run Type="Roll" Para="AnimateType:EaseInOut;Interval:2000;FrameRange:20;Direction:V;Orientation:P;"/>
class UiComParser;
class UiOperateParser : public QObject
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



    void SetPara(ParaType pType, int pPara);
    int  GetPara(ParaType pType);


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

class UiComParser : public QObject,public UiParaParser
{
public:
    UiComParser();
    ~UiComParser();

    bool Init(QDomElement pElement, UiParser *pParser = 0);
    virtual bool InitSubUI(QDomElement pElement, UiParser *pParser = 0);
    bool InitSubElement(QString elementName);
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

    virtual bool PaintBuffer(UiOperateParser* pAnimate, int pFrame);

public:
    UiParser                               *mUiParser;
    QDomElement                             mElement;
    bool                                    mExternPara;  //>@指示资源是否为外部带入的，如果是，则在注销时不能删除，只清空

    int                                     mLayout;      //
    int                                     mStyle;

    UiRectListParser                       *mComRect;
    UiRcListParser                         *mComResource;      //>@只需要记录路径的资源
    UiOperateListParser                    *mComOperation;

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
    ~UiParser();
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

#endif // UIPARSER_H

