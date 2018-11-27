#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>

class DeviceManager : public XmlParse
{
    Q_OBJECT
public:
    DeviceManager(QString fileName);

    //>@操作文件
    bool createMdp();
    bool openMdp();
    bool closeMdp();
    bool delMdp();
    bool saveMdp();
    bool renameMdp(QString FileName);

    //>@操作文件中的元素
    bool GetCfgparaGroup();

    bool setMonitor(quint32 pFreq);
    quint32 getMonitor();

    bool setPassword(QString pPasswd);
    QString getPassword();

    bool setLocation(QString pLocation);
    QString getLocation();

    bool showLog(bool pEnable);
    bool canShowLog();

    bool setPattern(QString pPattern);
    QString getPattern();

    QString getVersion();

    QDomElement addDeviceDom(QString pPath, QDomElement pInitElement);
    void delDeviceDom(QString pPath);
    void delDeviceDom(MW_Type pType, QString pPath);
    void delDeviceDom(PH_Type pType, QString pPath);
    void delDeviceDom(LG_Type pType, QString pPath);
    QDomElement createParaDom(QDomElement pInitElement);
    bool clrMdp();
    bool clrDomCldNodes(QDomElement pElement);
    bool delDom(QDomElement pElement);

public:
    static const QString MdpSuffix;
    static const QString MdpFileType;

    QHash<QString, QDomElement> m_CfgparaElementGroup;
    QHash<MW_Type, QDomElement> m_MWElementGroup;
    QHash<PH_Type, QDomElement> m_PHElementGroup;
    QHash<LG_Type, QDomElement> m_LGElementGroup;
};

#endif // DEVICEMANAGER_H
