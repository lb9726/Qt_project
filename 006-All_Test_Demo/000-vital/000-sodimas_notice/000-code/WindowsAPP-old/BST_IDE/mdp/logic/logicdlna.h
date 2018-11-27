#ifndef LOGICDLNA_H
#define LOGICDLNA_H

#include "logicbase.h"

class LogicDLNA : public LogicDevice
{
    Q_OBJECT
public:
    LogicDLNA(QObject *parent = 0);
    ~LogicDLNA();

public:
    quint32 InitPara();

public slots:
    void UpdatePara(QObject* pObject);
    void ParseDataStream(){}
    void DeviceStateChanged() {}

public:
    QString         mName;
    int             mPort, mMaxPort, mMinPort;
    QString         mMediaCatalog;
    QString         mDBCatalog;
};

#endif // LOGICDLNA_H
