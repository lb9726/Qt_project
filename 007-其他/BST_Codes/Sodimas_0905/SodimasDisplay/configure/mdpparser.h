#ifndef DEVICEMDP_H
#define DEVICEMDP_H

#include <QObject>
#include <QRect>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QStringList>
#include <QDomDocument>

#include "xmlparse.h"
#include "global/globalfun.h"

class MdpParser : public XmlParse
{
    Q_OBJECT
public:
    explicit MdpParser(QString fileName);

public:
    static const QString UiSuffix;
    static const QString UiFileType;

    bool newMdp();
    bool openMdp();
    bool closeMdp();
    bool delMdp();
    bool saveMdp();
    bool renameMdp(QString FileName);

    bool getCfgparaGroup();
    bool SetMonitor(quint32 pFreq);
    quint32 GetMonitor();

    bool setPassword(QString pPasswd);
    QString getPassword();

    bool SetLocation(QString pLocation);
    QString GetLocation();

    bool ShowLog(bool pEnable);
    bool CanShowLog();

    bool SetPattern(QString pPattern);
    QString GetPattern();

    QString GetVersion();

    QDomElement addDeviceDom(QString pPath, QDomElement pInitElement);
    QDomElement createParaDom(QDomElement pInitElement);
    bool clrMdp();
    bool clrDomCldNodes(QDomElement pElement);
    bool delDom(QDomElement pElement);

public:
    static const QString MdpSuffix;
    static const QString MdpFileType;

    QHash<QString, QDomElement> m_CfgparaElementGroup;
    QHash<PH_Type, QDomElement> m_PHElementGroup;
    QHash<LG_Type, QDomElement> m_LGElementGroup;

    //QHash<PH_Type, QDomElement> m_ComElementGroup;
    DIRECTION                   m_DirectionPermisson;

    QString                   m_password;
};

#endif // DEVICEMDP_H
