#ifndef USBHELPER_H
#define USBHELPER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QStringList>
#include <QStringList>
#include "Common/qdevicewatcher/qdevicewatcher.h"
#include "Common/qdevicewatcher/qdevicewatcher_p.h"

typedef struct USB_DISK_INFO
{
    QString DirPath;
    float TotalMemory;
    float FreeMemory;
}USB_DISK_INFO;

class UsbHelper : public QObject
{
    Q_OBJECT
public:
    explicit UsbHelper(QObject *parent = 0);

    void addDisk(QString &DiskName);
    bool searchDisk(QString &DiskName, int &iter);
    void getMemorySize(USB_DISK_INFO &disk);
    void currentDiskChanged(QString e);
    USB_DISK_INFO getCurrentDisk();
    QString getCurrentDiskDir();

signals:
    void sigDeviceAdd(QString dir);
    void sigDeviceRemove(QString dir);

public slots:
    void scanHD();
    QString formatUdisk(QString diskDir);
    void deleteDisk(QString &DiskName);

private:
    QVector<USB_DISK_INFO> usb_disk_info;
    USB_DISK_INFO CurrentUseDisk;
    QDeviceWatcher mDeviceWatcher;
    QStringList mUsbPathList;
    QString mMntPath;

public:
    void detectUsbIsInsert(const QString &path);
    void usbDetected();
    void slot_DeviceAdded(QString pName);
    void slot_DeviceRemoved(QString pName);
    void setMountPath(QString pPath);
    void onDeviceAdded();
    void onDeviceRemoved();
};


#endif // USBHELPER_H
