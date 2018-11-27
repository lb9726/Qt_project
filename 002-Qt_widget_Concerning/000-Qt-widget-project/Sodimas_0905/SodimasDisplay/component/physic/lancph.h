#ifndef LANCPH_H
#define LANCPH_H

#include <QObject>
#include "physicbase.h"

class LancPh : public PhysicBase
{
    Q_OBJECT
public:
    LancPh(GlobalSetting *globalSetting);

    bool initDevice(PH_Type pPHType, QDomElement pElement);
signals:

public slots:

public:
    bool m_Wifion;
    QString m_WifiIp;
    bool m_Dhcp;
    int m_UdpPort;
    int m_TcpPort;
};

#endif // LANCPH_H
