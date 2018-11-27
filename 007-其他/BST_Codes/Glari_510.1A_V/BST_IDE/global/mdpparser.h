#ifndef MDPPARSER_H
#define MDPPARSER_H

#include "xmlparser.h"

/*
    <Theme>bst</Theme>
*/

class MdpParser : public XmlParser
{
    Q_OBJECT
public:
    MdpParser(QString fileName);

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

    bool ShowLog(bool pEnable);
    bool CanShowLog();

    bool SetPattern(QString pPattern);
    QString GetPattern();

    bool SetTheme(QString pPattern);
    QString GetTheme();

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

#endif // MDPPARSER_H
