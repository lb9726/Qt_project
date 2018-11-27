#ifndef UABMANAGER_H
#define UABMANAGER_H

#include "haldevice.h"
#include <QtCore/QMap>


class usbmanager : public QObject
{
    Q_OBJECT

public:
    usbmanager(QObject *parent = 0);

public slots:
    void slotDeviceAdded(QString strdev);
    void slotDeviceRemoved(QString strdev);
    void scan();
    
private:
    QMap<QString,haldevice*> mDeviceMap;
    void getDevice(QDBusInterface *interface,const QString &udi);

    bool isVolume(QString value);
    bool isDisk(QString value);
    bool isBlockDev(QString value);
    bool isValidMajor(int value);
    bool isUsb(QString value);

    // Create interface to get information from Hal
    QDBusInterface *interfaceHal;

    
    
};

#endif
