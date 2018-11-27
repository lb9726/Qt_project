#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>
#include "xmlparser.h"
#include "global/define.h"
#include "global/global.h"
class uiManager : public XmlParser
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

#endif // UIMANAGER_H
