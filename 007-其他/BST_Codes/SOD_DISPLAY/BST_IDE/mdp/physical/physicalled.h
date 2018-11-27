#ifndef PHYSICALLED_H
#define PHYSICALLED_H

#include "physicalbase.h"

class PhysicalLED : public PhysicalDevice
{
    Q_OBJECT
public:
    PhysicalLED(QObject *parent = 0);
    ~PhysicalLED();

    void SetBlink(QString pSpeed);
    quint32 InitPara();
    quint32 Monitor(quint32 pCount);

public slots:
    void UpdatePara(QObject* pObject);

public:
    QFile      *m_File;
    int         m_BlinkFreq;
    char        m_LEDState;

    QString     m_NormalSpeed;
    QString     m_BusySpeed;
    QString     m_ErrorSpeed;
};

#endif // PHYSICALLED_H

