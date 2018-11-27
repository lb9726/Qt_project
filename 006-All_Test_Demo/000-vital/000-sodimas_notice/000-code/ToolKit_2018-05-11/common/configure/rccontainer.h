#ifndef RCCONTAINER_H
#define RCCONTAINER_H
#include <QHash>
#include <QString>
#include <QRectF>
#include <QDomElement>
#include <QVariant>
#include <QPixmap>
#include <QMovie>
#include <QByteArray>

#include "globalfun.h"
#include "themeparser.h"
//#include "xmlparse.h"


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
    QString                 mTipText;   //>@提示文本
    QHash<int, QVariant>    mParameterHash;  //>@对于某个资源的参数定义
};


class RcContainer
{
public:
    RcContainer(ThemeParser *m_UiManager,QRectF pRect=QRectF());
    ~RcContainer();

    void setThemeManager(ThemeParser *pthemeManager);
    virtual bool modifyGeomotery(QStringList pGeoList) {return false;}
    virtual void setComGeometory(QRectF pRect) {}
    virtual bool modifyMap(QStringList pParaList) {return false;}
    virtual QString execCommand(QString pCommand) {return QString();}
    QString getAttribute(QString pEffect, QString pName);
    bool SetAttribute(QString pEffect, QString pName, QString pValue);

    bool SetPara(QDomElement &pElement, XmlParse *pParse);
    bool GetPara(QDomElement &pElement, XmlParse *pParse);

    void RemoveRc(int pKey);

    RC_INFO *LoadRc(int pKey, QDomElement pRcElement);
    RC_INFO *LoadPath(int pKey, QString pPixmapName);


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

    void releaseRC();


    bool initState(QDomElement pElement);

    //>@直接从文件夹中提取指定前缀的文件作为资源文件
    virtual void InitRC(QString pRcPath, QString pPrefix = D_DEFAULT_PREFIX, bool pInititial = true);

    bool InitSubPara(QString pStateName, QString pLabel, QString pContent);

    bool InitRcPara(AREA_STYLE* pAreaStyle, QString pCommonPara);
    virtual bool InitSubRcPara(AREA_STYLE* pAreaStyle, QString pLabel, QString pContent){return false;}

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
    QDomElement                         m_ComElement;
    COM_TYPE                            m_ComType;
    QString                             m_ComPath;
    QRectF                              m_ComRect[3];

    QString                             m_RcPrefix;
    QString                             m_CurResource;  //>@当前正在使用的图片资源
    QHash<int, RC_INFO*>                m_ComponentRC;
    ThemeParser *themeManager;
    bool InitComponent(int layout, DIRECTION dirction, QDomElement &pElement, bool pOffset);
    virtual bool Start() {return true;}  //>@执行完InitEffect需要执行Start来启动特效
};

#endif // RCCONTAINER_H
