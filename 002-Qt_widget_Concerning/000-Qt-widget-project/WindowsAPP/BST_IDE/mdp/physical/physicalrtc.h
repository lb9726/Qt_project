#ifndef PHYSICALRTC_H
#define PHYSICALRTC_H

#include "physicalbase.h"

/*
#!/bin/sh

date -s "2015-02-27 10:01"
hwclock -w
*/

class PhysicalRTC : public PhysicalDevice
{
    Q_OBJECT
public:
    PhysicalRTC(QObject *parent = 0);
    ~PhysicalRTC();

    quint32 InitPara();

    QDateTime GetTime();
    bool SyncTime(QDateTime pDateTime);
    bool SetValue(QString pValue);

    void UpdateClockUI();
    quint32 Monitor(quint32 pCount);

signals:
    void sUpdateTime(QDateTime pTime);

public slots:
    void UpdatePara(QObject* pObject);
    void slot_Initialize();

public:
    QDateTime       mCurDateTime;
    bool            mSecondEnable;
};

#endif // PHYSICALRTC_H
