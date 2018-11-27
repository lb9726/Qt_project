#ifndef PHYSICALMEM_H
#define PHYSICALMEM_H

#include "physicalbase.h"

class PhysicalMEM : public PhysicalDevice
{
    Q_OBJECT
public:
    PhysicalMEM(QObject *parent = 0);
    ~PhysicalMEM();

    quint32 InitPara();
    quint32 Monitor(quint32 pCount);

    QString GetInformation() { return QString("%1>%2  %3%").arg(mDevName).arg(mMemModel).arg(mUsageRate); }

public slots:
    void UpdatePara(QObject* pObject);

public:
    QString         mMemModel;
    bool            mAutoRelease;
    NUM_INFO        mThreshold;
    int             mUsageRate;
};

#endif // NODEMEM_H
