#ifndef LOGICFTP_H
#define LOGICFTP_H

#include "logicbase.h"

class LogicFTP : public LogicDevice
{
    Q_OBJECT
public:
    LogicFTP(QObject *parent = 0);
    ~LogicFTP();

public:
    quint32 InitPara();

public slots:
    void UpdatePara(QObject* pObject);
    void ParseDataStream(){}
    void DeviceStateChanged() {}

public:
    QString                 mCatalog;
    QString                 mPasswd;
};

#endif // LOGICFTP_H
