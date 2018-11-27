#include "usbeventfilter_win32.h"
#include "define.h"

UsbEventFilter::UsbEventFilter(QAbstractNativeEventFilter *parent)
{
    Q_UNUSED(parent);
    initUsbDetector();
}

UsbEventFilter::~UsbEventFilter()
{
    if(usbHelper)
    {
        usbHelper->deleteLater();
    }
}

void UsbEventFilter::initUsbDetector()
{
    usbHelper = new UsbHelper();
}

bool UsbEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);
    QString DirPath;
    MSG* msg = (MSG*)message;

    int msgType = msg->message;
    if(msgType == WM_DEVICECHANGE)
    {
        //IDE_DEBUG("winEvent in MgFrame : WM_DEVICECHANGE");
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        switch(msg->wParam)
        {
        case DBT_DEVICEARRIVAL:
            if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if (lpdbv -> dbcv_flags == 0)
                {
                    //New USB Driver plugin, get Dir
                    DirPath = QString(this->FirstDriveFromMask(lpdbv ->dbcv_unitmask));
                    DirPath += ":";
                    this->AddDisk(DirPath);
                    //IDE_DEBUG(QString("USB Driver insert on Path: %1").arg(DirPath));
                }
            }
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if (lpdbv -> dbcv_flags == 0)
                {
                    //IDE_DEBUG("USB_Removed");
                    DirPath = QString(this->FirstDriveFromMask(lpdbv ->dbcv_unitmask));
                    DirPath += ":";
                    this->DeleteDisk(DirPath);
                    //IDE_DEBUG(QString("USB Driver remove on Path: %1").arg(DirPath));
                }
            }
            break;
        }
    }
    return false;
}

void UsbEventFilter::AddDisk(QString &DiskName)
{
    if(usbHelper)
    {
        usbHelper->addDisk(DiskName);
    }
    else
    {
        initUsbDetector();
    }
}

void UsbEventFilter::DeleteDisk(QString &DiskName)
{
    if(usbHelper)
    {
        usbHelper->deleteDisk(DiskName);
    }
    else
    {
        initUsbDetector();
    }
}

char UsbEventFilter::FirstDriveFromMask(ULONG unitmask)
{
    char i;
    for (i = 0; i < 26; ++i)
    {
        if (unitmask & 0x1)
        {
            break;
        }
        unitmask = unitmask >> 1;
    }
    return (i + 'A');
}
