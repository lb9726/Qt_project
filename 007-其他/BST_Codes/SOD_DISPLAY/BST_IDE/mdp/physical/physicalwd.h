#ifndef PHYSICALWD_H
#define PHYSICALWD_H

#include "physicalbase.h"

class PhysicalWD : public PhysicalDevice
{
    Q_OBJECT
public:
    PhysicalWD(QObject *parent = 0);
    ~PhysicalWD();

    quint32 InitPara();
    quint32 Monitor(quint32 pCount);

public slots:
    void UpdatePara(QObject* pObject);

public:
    QFile          *m_WDFile;
    NUM_INFO        m_WDTimeout;
    int             m_WDFreq;  //>@实际的看门狗超时时间。
};

#endif // PHYSICALWD_H

