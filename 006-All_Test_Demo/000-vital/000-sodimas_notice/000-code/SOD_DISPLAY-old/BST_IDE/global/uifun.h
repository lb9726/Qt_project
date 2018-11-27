#ifndef UIFUN_H
#define UIFUN_H

#include "bggen.h"
#include "bstui.h"

#include <QTime>
#include <QVector>
#include <QDir>
#include <QKeyEvent>


class GraphicsScene;

#define D_SETCTRL_ZVALUE  10000 //>@设置控件Z序
#define D_MSGCTRL_ZVALUE  10001 //>@消息控件Z序
#define D_PRGCTRL_ZVALUE  10002 //>@进度条控件Z序
#define D_KEYBOARD_ZVALUE  10003//>@键盘控件Z序
#define D_TEXT_ZVALUE     9000//>@文本控件Z序

typedef enum{
    SM_NORMAL = 0,
    SM_PERMANENT,
    SM_TEMPORARY,
    SM_HOLD
}E_SHOWMODE;

inline bool operator==(QPixmap b1, QPixmap b2) { return b1.data_ptr() == b2.data_ptr(); }

bool PreprocContent(QString &pContent);

class PIX_INFO
{
public:
    PIX_INFO()
    {
        mGIF = 0;
    #ifdef  SURRPORT_SVG
        mSVG = 0;
    #endif
    }
    void clear(RC_TYPE pType)
    {
        if(pType == RC_PNG)
        {
            mPNG = QPixmap();
        }
        else if(pType == RC_GIF)
        {
            if(mGIF)
            {
                mGIF->deleteLater();
                mGIF = 0;
            }
        }
#ifdef  SURRPORT_SVG
        else if(pType == RC_SVG)
        {
            if(mSVG)
            {
                mSVG->deleteLater();
                mSVG = 0;
            }
        }
#endif
#ifdef  SURRPORT_3D
        else if(pType == RC_3DS)
            m3DS = -1;
#endif
    }
public:
    QPixmap         mPNG;
    QMovie         *mGIF;
#ifdef  SURRPORT_SVG
    QSvgRenderer   *mSVG;
#endif
#ifdef  SURRPORT_3D
    GLuint          m3DS;
#endif
};

class RC_INFO
{
public:
    RC_INFO() {mRcType = RC_INVALID; clear();}
    ~RC_INFO() {clear();}
    void clear();
public:
    QDomElement             mElement;
    QString                 mRcFile;    //>@资源文件路径
    RC_TYPE                 mRcType;    //>@资源类型
    PIX_INFO                mRcPix;     //>@资源图片
    QString                 mTipText;   //>@提示文本
    QHash<int, QVariant>    mParaHash;  //>@对于某个资源的参数定义
};

typedef struct _MAP_INFO{
    void clear() { mMap = -1; mParaHash.clear(); }
    int                     mMap;       //>@目标映射
    QHash<int, QVariant> mParaHash;  //>@对于某个资源的参数定义
} MAP_INFO;

typedef struct _FORM_INFO{
    void clear() { mParaHash.clear(); }
    QString              mFile;       //>@
    QString              mStyleSheet;
    QHash<int, QVariant> mParaHash;  //>@对于某个资源的参数定义
} FORM_INFO;

typedef struct _RECT_INFO{
    QRectF m_Rect[2];  //>@0:Vertical
} RECT_INFO ;

//>@pRect表示的是当不通过Element进行初始化坐标或者当前ui文件中没有此控件时，通过此rect进行初始化ui中的节点以及设置组件位置。
class RcContainer
{
public:
    RcContainer(
#ifdef SUPPORT_SCENE
                GraphicsScene *pContainer=0,
#elif SUPPORT_WIDGET
                UiWidget *pContainer=0,
#elif SUPPORT_COMEDIT
                GraphicsScene *pContainer=0,
#endif
                QRectF pRect=QRectF());
    ~RcContainer();

    void SetUiContainer(
    #ifdef SUPPORT_SCENE
                            GraphicsScene *pContainer
    #elif SUPPORT_WIDGET
                            UiWidget *pContainer
    #elif SUPPORT_COMEDIT
                            GraphicsScene *pContainer
    #endif
            );
    virtual bool ModifyGeomotery(QStringList pGeoList) {return false;}
    virtual void SetComGeometory(QRectF pRect) {}

    virtual bool ModifyMap(QStringList pParaList) {return false;}

    virtual QString ExecCommand(QString pCommand) {return QString();}

    QString GetAttribute(QString pEffect, QString pName);
    bool SetAttribute(QString pEffect, QString pName, QString pValue);

    bool SetPara(QDomElement &pElement, xmlParse *pParse); //>@修改pElement为结果Element
    bool GetPara(QDomElement &pElement, xmlParse *pParse);

    virtual bool WriteReg(quint16 pRegAddr, QByteArray pContent) {return true;}
    virtual QByteArray ReadReg(quint16 pRegAddr, quint32 pTimeout) {return QByteArray();}

    MAP_INFO* AddMap(int pSrc, int pMap);
    bool DelMap(int pSrc);
    bool DelAllMap();

    void RemoveRc(int pKey);
    FORM_INFO *LoadForm(int pKey, QDomElement pRcElement);
    RC_INFO *LoadRc(int pKey, QDomElement pRcElement);
    RC_INFO *LoadPath(int pKey, QString pPixmapName);
    virtual MAP_INFO* LoadMap(QDomElement pElement);

    RC_INFO *LoadPixmap(int pKey, QPixmap pPixmap);
    QPixmap LoadPixmap(QString pPixmapName, QSizeF pSize=QSizeF());
    QPixmap ScalePixmap(QPixmap pPixmap, QSizeF pSize=QSizeF());

    RC_INFO *CreateRcInfo(QPixmap pPixmap);

    QString GetPixmapPath(int pKey);
    QMovie* GetGif(RC_INFO *pRcInfo, QSizeF pSize=QSizeF());
    QPixmap GetPixmap(RC_INFO *pRcInfo, QSizeF pSize=QSizeF());
    QPixmap GetPixmap(int pKey, QSizeF pSize=QSizeF());  //>@从m_EffectXXX中加载图片

    bool hasResource();

    virtual void Update() {}

    int getNextRc(int pCurRcIndex);
    int getPrevRc(int pCurRcIndex);
    QPixmap getNextImg();
    QPixmap getPrevImg();

    void ReleaseEffectGroup();

    void ReleaseRC();
    bool RefreshRC();

    void InitScene(
#ifdef SUPPORT_SCENE
            GraphicsScene* pScene
#elif SUPPORT_WIDGET
            UiWidget* pScene
#elif SUPPORT_COMEDIT
            GraphicsScene* pScene
#endif
            );

    void DeleteComponent();  //>@从UI文件中删除此节点的数据
    bool InitComponent(QHash<int, RC_INFO*> pRcList, QHash<AREA_OPERATE, AREA_ANIMATE*>  pEffectGroup, QRectF pRect=QRectF());
    bool InitComponent(QDomElement &pElement, bool pOffset = false);
    bool InitState(QDomElement pElement);
    virtual bool InitEffectState(EffectType pStateType, AREA_OPERATE pOperate, QDomElement pElement);

    //>@直接从文件夹中提取指定前缀的文件作为资源文件
    virtual void InitRC(QString pRcPath, QString pPrefix = D_DEFAULT_PREFIX, bool pInititial = true);

    bool InitSubPara(QString pStateName, QString pLabel, QString pContent);

    bool InitRcPara(AREA_STYLE* pAreaStyle, QString pCommonPara);
    virtual bool InitSubRcPara(AREA_STYLE* pAreaStyle, QString pLabel, QString pContent){return false;}

    bool InitMapPara(MAP_STYLE* pStyle, QString pPara);
    virtual bool InitSubMapPara(MAP_STYLE* pStyle, QString pLabel, QString pContent){return false;}

    bool InitEffectPara(AREA_ANIMATE* pAreaEffect, QString pEffectPara);
    virtual bool InitSubEffectPara(AREA_ANIMATE* pAreaEffect, QString pLabel, QString pContent);

    //>@转换字符串为特殊数字，用于ParserParameter函数中。
    virtual int StrToEnum(QString pString) {return -1;}
    //>@根据类型转换字符串为QVariant类型
    virtual QVariant StrToValue(int pEnum, QString pString) {return QVariant();}

    virtual void ParserParameter(FORM_INFO *pInfo, QString pParameter);
    virtual void ParserParameter(MAP_INFO *pInfo, QString pParameter);
    virtual void ParserParameter(RC_INFO *pInfo, QString pParameter);

    virtual void Restart() {}
    virtual bool Start() {return true;}  //>@执行完InitEffect需要执行Start来启动特效

    QList<EFFECT*> GetDefaultEffects(QString pStateName);
    EFFECT* GetDefaultEffect(QList<EFFECT*> pEffects, QString pEffectName);
    EFFECT* GetDefaultEffect(QString pStateName, QString pEffectName);
    QDomElement GetComStateElement(QString pStateName, QString &pEffectName, QString &pEffectPara);
    QString GetDefaultEffectPara(QString pStateName, QString pEffectName);

    QString GetComStateEffect(QString pStateName);
    QString GetComStateEffectPara(QString pStateName, QString pParaName);
    QString GetComStateRcFile(QString pStateName, QString pRcName);

    //>@获得某个界面组件相对路径下的资源文件夹
    QString GetRelativeRcLocation();

    virtual QRectF  GetCurRect();

public:
#ifdef IDE
    ComCategory                        *m_ComCategory;
#endif
#ifdef SUPPORT_SCENE
    QPointer<GraphicsScene>             m_UiContainer;
#elif SUPPORT_WIDGET
    QPointer<UiWidget>                  m_UiContainer;
#elif SUPPORT_COMEDIT
    QPointer<GraphicsScene>             m_UiContainer;
#endif
    QDomElement                         m_ComElement;
    COM_TYPE                            m_ComType;
    QString                             m_ComPath;

    QRectF                              m_ComRect[3];  //>@当前使用的大小坐标

    QString                             m_RcPrefix;

    //>---------------------------------------------------------------------------------
    bool                                m_ExternRC;  //>@指示资源是否为外部带入的，如果是，则在注销时不能删除
    //>@Form
    QHash<int, FORM_INFO*>              m_FormList;     //>@
    //>@RC
    QList<int>                          m_RcList;       //>@用于查找上一个或下一个资源
    QHash<int, RC_INFO*>                m_EffectRC;     //>@只需要记录路径的资源
    AREA_STYLE*                         m_RcStyle;     //>@资源描述
    int                                 m_RcIndex;     //>@当前正在使用的资源序号
    PIXMAP_POINTER                      m_PixmapPointer;  //>@当前正在使用的图片资源
    //>@MAP
    QHash<int, MAP_INFO*>               m_MapList;     //>@
    MAP_STYLE*                          m_MapStyle;  //>@映射描述
    //>@EFFECT
    QHash<AREA_OPERATE, AREA_ANIMATE*>  m_EffectGroup;  //>@特效描述
    bool                                m_EffectValid;  //>@特效使能
};

#endif // LOGFUN_H

