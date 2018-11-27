#ifndef LOGICBONJOUR_H
#define LOGICBONJOUR_H

#include "logicbase.h"

class LogicBonjour : public LogicDevice
{
    Q_OBJECT
public:
    LogicBonjour(QObject *parent = 0);
    ~LogicBonjour();

public:
    quint32 InitPara();

public slots:
    void UpdatePara(QObject* pObject);
    void ParseDataStream(){}
    void DeviceStateChanged() {}

public:
    bool            mScanEnable;
    int             mPort, mMinPort, mMaxPort;
    QString         mType;
    QString         mName;
};

#endif // LOGICBONJOUR_H
