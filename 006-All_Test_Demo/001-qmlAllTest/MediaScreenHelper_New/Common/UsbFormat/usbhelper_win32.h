#ifndef USBHELPER_H
#define USBHELPER_H

#include <windows.h>
#include <winbase.h>
#include <winnls.h>
#include <dbt.h>

#include <QObject>
#include <QString>
#include <QVector>
#include <QStringList>

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
//    void deleteDisk(QString &DiskName);
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
    QString formatUdisk2(QString diskDir);
public:
    QVector<USB_DISK_INFO> usb_disk_info;
    USB_DISK_INFO CurrentUseDisk;

    bool isValideDriver(QString diskDir);
};


#endif // USBHELPER_H
