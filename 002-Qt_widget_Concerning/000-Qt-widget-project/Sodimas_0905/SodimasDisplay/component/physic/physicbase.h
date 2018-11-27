#ifndef DEVICEBASE_H
#define DEVICEBASE_H

#include <QObject>
#include <QDomElement>
#include <QPointer>
//#include "themeparser.h"
//#include "../logic/themelg.h"
#include "../globalsetting.h"

class LcdPh;
class GlobalSetting;
class PhysicBase : public QObject
{
    Q_OBJECT
public:
    PhysicBase();
    PhysicBase(GlobalSetting *globalSetting);
    void setGlobalSetting(GlobalSetting *globalSetting);
    ~PhysicBase();
    virtual bool initDevice(PH_Type pPHType, QDomElement pElement);///初始化第一步
    virtual bool SetEnable(bool pFlag, DEV_MODE pDevMode, bool pForce=false);    ///初始化第三步

    bool setElementText(QDomElement pElement, QString pValue);
    bool setPara(QString pParaName, QString pParaValue, bool pSyncUI=true);
    bool setPara(QDomElement pElement, QString pParaValue, bool pSyncUI=true);
    bool setPara(QDomElement &pElement, XmlParse *pParse);
    QString getPara(QString pParaName);
    QString getPara(QDomElement pElement);
    bool getPara(QDomElement &pElement, XmlParse *pParse);

    //获取本节点中的重要信息
    virtual QString GetInformation() { return QString(); }
    virtual bool SetValue(QString pValue) {return false;}
    virtual QString GetValue(QString pValue) { return QString(); }

    bool saveMdp();

public slots:
    ///初始化第二步
    virtual void slot_Initialize(); //当物理、逻辑设备、主题全部加载好后执行此函数 初始化完毕函数
signals:

public:
    bool modifyBool(QDomElement &pElement, bool &pDstValue, bool pNewValue);
    bool modifyEnum(QDomElement &pElement, QString &pDstValue, QString pNewValue);
    bool modifyNum(QDomElement &pElement, int &pDstValue, int pNewValue, int pMinValue=-1, int pMaxValue=-1);
    bool modifyText(QDomElement &pElement, QString &pDstValue, QString pNewValue);

public:
    bool m_initialized;
    bool m_enabled;

    GlobalSetting      *m_globalSetting;
    ThemeLg            *mThemeManager;
    LcdPh              *m_LCD;
    QDomElement         m_NodeElement;

    QString             mDevName;
    DEV_MODE            mDevMode;
};
#endif // DEVICEBASE_H
