#ifndef LOGICNFCP2P_H
#define LOGICNFCP2P_H

#include "logicbase.h"

class LogicNFCP2P : public LogicDevice
{
    Q_OBJECT
public:
    LogicNFCP2P(QObject *parent = 0);
    ~LogicNFCP2P();

    quint32 InitPara();

public slots:
    void UpdatePara(QObject* pObject);
    void ParseDataStream(){}
    void DeviceStateChanged() {}

public:
    QStringList         mCardModes;
    QString             mKeyAlarm;
};

#endif // LOGICNFCP2P_H
