#ifndef LOGICMONITOR_H
#define LOGICMONITOR_H

#include "logicbase.h"

class LogicMonitor : public LogicDevice
{
    Q_OBJECT
public:
    LogicMonitor(QObject *parent = 0);
    ~LogicMonitor();

public:
    quint32 InitPara();
    bool SetValue(QString pValue);

public slots:
    void UpdatePara(QObject* pObject);
    void ParseDataStream(){}
    void DeviceStateChanged() {}

public:
    RESOLUTION          mSize;
    int                 mTDM, mMinTDM, mMaxTDM;
    bool                mAudio;
    bool                mMotionAlarm;
};

#endif // LOGICMONITOR_H
