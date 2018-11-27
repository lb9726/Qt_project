#ifndef LOGICSSH_H
#define LOGICSSH_H

#include "logicbase.h"

class LogicSSH : public LogicDevice
{
    Q_OBJECT
public:
    LogicSSH(QObject *parent = 0);
    ~LogicSSH();

public:
    quint32 InitPara();

public slots:
    void UpdatePara(QObject* pObject);
    void ParseDataStream(){}
    void DeviceStateChanged() {}

public:
    QString             mPassphrase;
};

#endif // LOGICSSH_H
