#ifndef USBEVENTFILTER_H
#define USBEVENTFILTER_H
//#if defined(Q_OS_WIN)
#include <QObject>
#include <windows.h>
#include <winbase.h>
#include <winnls.h>
#include <dbt.h>
#include <QAbstractNativeEventFilter>
#include "Common/UsbFormat/usbhelper_win32.h"
class UsbEventFilter : public QAbstractNativeEventFilter
{
public:
    explicit UsbEventFilter(QAbstractNativeEventFilter *parent = 0);
    ~UsbEventFilter();

    void initUsbDetector();
    void AddDisk(QString &DiskName);
    void DeleteDisk(QString &DiskName);

    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *) Q_DECL_OVERRIDE;
private:
    char FirstDriveFromMask(ULONG unitmask);
public:
    UsbHelper *usbHelper;
};

#endif // USBEVENTFILTER_H
//#endif //Q_OS_WIN32
