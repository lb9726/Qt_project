#ifndef USBMANAGER_H
#define USBMANAGER_H

#include <QObject>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusConnection>
#include <QtCore/QMap>
#include <QDBusReply>
#include "haldevice.h"

#define DBUS_HAL_SERVICE           "org.freedesktop.Hal"
#define DBUS_HAL_PATH              "/org/freedesktop/Hal/Manager"
#define DBUS_HAL_INTERFACE         "org.freedesktop.Hal.Manager"
#define DBUS_HAL_DEVICE_PATH       "/org/freedesktop/Hal/devices"
#define DBUS_HAL_DEVICE_INTERFACE  "org.freedesktop.Hal.Device"

class usbmanager : public QObject
{
    Q_OBJECT
public:
    explicit usbmanager(QObject *parent = 0);

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

#endif // USBMANAGER_H
