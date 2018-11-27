#ifndef DEVICEMDP_H
#define DEVICEMDP_H

#include "themeparser.h"

typedef enum{
    P_INVALID = 0,  ///0
    P_WD,
    P_CPU,
    P_MEM,
    P_LED,
    P_DISK,
    P_MDISK,
    P_LCD,
    P_Touch,
    P_RTC,
    P_Player,
    P_LANC,
    P_LANR,
    P_LANH,
    P_VP,
    P_COM,
    P_SAF,
    P_DIAL,
    ///
    P_UDP,
    P_TCP,
    P_TEST
} PH_Type;

typedef enum{
    L_INVALID = 0,
    L_ContentManager,
    L_ThemeManager
} LG_Type;

PH_Type getPHType(QString pPara);
PH_Type getElementPHType(QDomElement pDevElement);
LG_Type getLGType(QString pPara);

typedef enum {
    DEV_NONE=0,
    DEV_MIDDLEWARE,
    DEV_PHYSICAL,
    DEV_LOGIC
} DEV_TYPE;

DEV_TYPE getDevType(QString pDevName);

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
    QString getProtocolType();

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
