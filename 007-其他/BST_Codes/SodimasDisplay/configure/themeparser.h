#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include "globalfun.h"
#include "xmlparse.h"

class ThemeParser : public XmlParse
{
    Q_OBJECT
public:
    ThemeParser(QString fileName);

    static const QString UiSuffix;
    static const QString UiFileType;

    ///操作ui文件
    bool createUi();
    bool openUi();
    bool closeUi();
    bool delUi();
    bool saveUi();
    bool renameUi(QString FileName);

    bool isValid();

    QString getSolution();
    bool setSolution(QString pSolution);
    DIRECTION getDirectionPermisson();
    bool setDirectionPermisson(DIRECTION pDirection);
    bool canSwitch() { return (m_DirectionPermisson == HVBoth); }

    ///操作文件中的元素
    void getComponentGroup();

    ///查找文件的全路径
    FILEPATH_TYPE getRcFileType(QString pFileName);
    FILEPATH_TYPE regulatRcFile(QString& pFileName);
    ///查看是否为相对路径
    bool isRelativePath(QString pFileName);
    ///规整资源到目标路径
    bool regularFile(COM_TYPE pComType, QString pRcName, QString &pRcFile, QString pDstPath, QSize pSize);

    bool getComRect(int pLayout, QDomElement pComElement, QRect &pHRect, QRect &pVRect);
    QRect getComRect(int pLayout, QDomElement pComElement, DIRECTION pDirection);
    bool modifyComRect(int pLayout, QDomElement pComElement, QRect pRect, DIRECTION pDirection, bool pForceNew = false);

    QDomElement getComRcElement(QDomElement pComElement);
    QDomNodeList getComRcList(QDomElement pComElement);
    QDomNodeList getComStateList(QDomElement pComElement);
    QDomElement getComState(QDomElement pComElement, QString pStateName, bool pCreateFlag = false); ///pCreateFlag如果不存在则自动创建
    bool modifyEffectType(QDomElement pStateElement, QString pEffectType);
    QDomElement modifyComState(QDomElement pComElement, QString pStateName, QString pEffectType, QString pEffectPara);

    bool modifyEffectPara(QDomElement pStateElement, QString pEffectPara);

    ///获取某个组件的父QDomElement，同时查看是否为规整的资源路径，否则进行创建此路径
    bool getParentElement(QDomElement &pParentElement, QString pComPath);
    ///获取组件的父组件
    QDomElement getParComDom(QDomElement pComElement);
    QDomElement getParComDom(QString pComPath);

//    bool addMap(QDomElement pMapStateElement, RC_MAP pMap, QString pPrefix=D_DEFAULT_PREFIX);
//    bool delMap(QDomElement pMapStateElement, RC_MAP pMap, QString pPrefix=D_DEFAULT_PREFIX);
//    QList<RC_MAP> getMapList(QDomElement pComElement, QString &pMapType, QString pPrefix=D_DEFAULT_PREFIX);
//    QHash<QString, QString> getMapHash(QDomElement pComElement, QString &pMapType, QString pPrefix=D_DEFAULT_PREFIX);

    ///添加一个控件的Dom资源
    QDomElement addComDom(COM_TYPE pComType, QRect pRect, DIRECTION pDirection);
    ///在Dom中删除一个控件的Dom资源
    bool deleteComDom(COM_TYPE pComType);

    ///将资源全部保存为相对路径，并存储在UI对应的文件夹下
    bool saveRelativeComDom(QDomElement pComElement);

public:
    ///xx.ui文件中的组件（subui和control）的Dom元素(xx.ui)
    QHash<COM_TYPE, QDomElement>  m_ComElementGroup;
    DIRECTION                     m_DirectionPermisson;


};

#endif // THEMEMANAGER_H
