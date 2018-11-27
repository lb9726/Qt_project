#ifndef USBDETECT_H
#define USBDETECT_H

#include <QWidget>
#include <QDebug>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusConnection>
#include <QString>

#define DBUS_HAL_SERVICE           "org.freedesktop.Hal"
#define DBUS_HAL_PATH              "/org/freedesktop/Hal/Manager"
#define DBUS_HAL_INTERFACE         "org.freedesktop.Hal.Manager"
#define DBUS_HAL_DEVICE_PATH       "/org/freedesktop/Hal/devices"
#define DBUS_HAL_DEVICE_INTERFACE  "org.freedesktop.Hal.Device"

class UsbDetect : public QWidget
{
    Q_OBJECT
public:
    explicit UsbDetect(QWidget *parent = 0);

signals:

public slots:
    void slotDeviceAdded(QString udi);
    void slotDeviceRemoved(QString udi);
public:
    QString currentUDI;
};

#endif // USBUPDATE_H
