
#ifndef HALDEVICE_H
#define HALDEVICE_H

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <QObject>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusObjectPath>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusVariant>
#include <QTimer>
#include <QtDBus/QDBusMetaType>
#include <QtDBus/QDBusArgument>
#include <QFile>
//#include "filebrowser.h"

#ifdef DEBUG_LOG
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG( format, ... )  "\n"
#endif

#define DBUS_HAL_SERVICE					"org.freedesktop.Hal"
#define DBUS_HAL_PATH						"/org/freedesktop/Hal/Manager"
#define DBUS_HAL_INTERFACE					"org.freedesktop.Hal.Manager"

#define DBUS_HAL_DEVICE_INTERFACE			"org.freedesktop.Hal.Device"


struct ChangeStruct
{
    QString propertyName;
    bool added;
    bool removed;
};

class haldevice : public QObject
{
    Q_OBJECT
signals:
    void mountedStatusChanged(bool);
    void readyBrowserUsb(QString path);
public:
   // usbdevice(QObject *parent = 0);
    haldevice(QDBusInterface *interface,const QString &deviceUdi);
   ~haldevice();

public slots:
    void propertyModified(int numChanges,const QList<ChangeStruct> &changes);
    void proMountedStatusChanged(bool);
    void proReadyBrowserUsb(QString path);
public:
    const QString& udi() const;
    const QString& parentUdi() const;
    const QString& blockName() const;
    const QString& category() const;
    const QString& bus() const;
    int majorNumber() const;
    bool isMounted() const;
    bool isMountedByDBus() const;
    const QString& currentMountPointByDBus();

    void setParentUdi(QString);
    void setBlockName(QString);
    void setCurrentMountPoint(QString);
    void setCategory(QString);
    void setBus(QString);
    void setIsMounted(bool);
    void setMajorNumber(int);
private:
    QString deviceUdi;
    QString deviceParentUdi;
    QString deviceBlockName;
    QString deviceCurrentMountPoint;
    QString deviceCategory;
    QString deviceBus;
    bool deviceIsMounted;
    int major;
    QDBusInterface *informationCenter;
};

#endif
