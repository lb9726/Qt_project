#ifndef PHYSICALTOUCH_H
#define PHYSICALTOUCH_H

#include "physicalbase.h"

class PhysicalTouch : public PhysicalMovable
{
    Q_OBJECT
public:
    PhysicalTouch(QObject *parent = 0);
    ~PhysicalTouch();

    quint32 InitPara();

public slots:
    void slot_Initialize();
    void slot_DeviceAdded(QString dev);
    void slot_DeviceRemoved(QString dev);
    void UpdatePara(QObject* pObject);
};

#endif // PHYSICALTOUCH_H

