#include "usbhelper_linux.h"
#include "define.h"

#include <QFileInfo>
#include <QDir>

UsbHelper::UsbHelper(QObject *parent) : QObject(parent)
//  , mFileList(QStringList())
  , mUsbPathList(QStringList())
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
}

void UsbHelper::scanHD()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    // ubuntu下判断是否插入了U盘
    if (QFile::exists("/proc/scsi/usb-storage"))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"U disk is insert";
        detectUsbIsInsert("/media/");
//        if (!mUsbPathList.isEmpty())
//        {
//            foreach (QString DiskName, mUsbPathList) // U盘在程序启动之前就插入的需要补发通知
//            {
//                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"DiskName = "<<DiskName;
//                emit sigDeviceAdd(DiskName);
//            }
//        }
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"U disk is not insert";
    }
}

void UsbHelper::addDisk(QString &DiskName)
{
    IDE_TRACE_STR(DiskName);
    int iter = 0;
    if(searchDisk(DiskName, iter))
    {
        return ;
    }
    USB_DISK_INFO usbtemp;
    usbtemp.DirPath = DiskName;
    getMemorySize(usbtemp);//获取设备内存信息
    this->usb_disk_info.push_back(usbtemp);
    emit sigDeviceAdd(DiskName);
}

void UsbHelper::deleteDisk(QString &DiskName)
{
    int iter = 0;
    if (searchDisk(DiskName, iter))
    {
        this->usb_disk_info.remove(iter);
        emit sigDeviceRemove(DiskName);
    }
}

bool UsbHelper::searchDisk(QString &DiskName, int &iter)
{
    Q_UNUSED(DiskName);
    Q_UNUSED(iter);
    return false;
}

void UsbHelper::getMemorySize(USB_DISK_INFO &disk)
{
    Q_UNUSED(disk);
}

void UsbHelper::currentDiskChanged(QString e)
{
    Q_UNUSED(e);
}

USB_DISK_INFO UsbHelper::getCurrentDisk()
{
    return this->CurrentUseDisk;
}

QString UsbHelper::getCurrentDiskDir()
{
    return this->CurrentUseDisk.DirPath;
}

QString UsbHelper::formatUdisk(QString diskDir)
{
    Q_UNUSED(diskDir)
    return QString();
}

void UsbHelper::detectUsbIsInsert(const QString &path)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"path = "<<path;
    QDir dir(path);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList tmpFileList;
    foreach (QFileInfo fileInfo, folder_list)
    {
        tmpFileList.append(fileInfo.fileName());
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"tmpFileList = "<<tmpFileList;
    }
    foreach(QString tmp, tmpFileList)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"tmp = "<< tmp;
        if (!tmp.contains("floppy",  Qt::CaseInsensitive)) // 去掉/media路径下floppy的文件夹
        {
            QDir dir2(path + tmp + "/");
            mMntPath = path + tmp + "/";  // /meida/libo/
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"mMntPath = "<<mMntPath;
            QFileInfoList flist = dir2.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
            foreach (QFileInfo fileInfo, flist)
            {
                QString tmpPath = path + tmp + "/"+ fileInfo.fileName();
                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<" tmppath = "<<tmpPath;

                if (!mUsbPathList.contains(tmpPath))
                {
                    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<" is diff string";
                    mUsbPathList.append(tmpPath);
                    emit sigDeviceAdd(tmpPath);
                }
                else
                {
                    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is same string";
                }
            }
        }
    }
    static int printNum = 1;
    foreach(QString tmp, mUsbPathList)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<tmp<<" "<<printNum++<<" counts";
    }
    printNum = 1;
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<mUsbPathList;
}

void UsbHelper::usbDetected()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is call";
    connect(&mDeviceWatcher, &QDeviceWatcher::deviceAdded, this, &UsbHelper::slot_DeviceAdded, Qt::UniqueConnection);
    connect(&mDeviceWatcher, &QDeviceWatcher::deviceRemoved, this, &UsbHelper::slot_DeviceRemoved, Qt::UniqueConnection);
    mDeviceWatcher.start();
}

void UsbHelper::slot_DeviceAdded(QString pName)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"pName = "<<pName;
    onDeviceAdded();
}

void UsbHelper::slot_DeviceRemoved(QString pName)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"pName = "<< pName;
    onDeviceRemoved();
}

void UsbHelper::setMountPath(QString pPath)
{
    Q_UNUSED(pPath);
}

void UsbHelper::onDeviceAdded()
{
    QtSleep(2000);
    detectUsbIsInsert("/media/");
//    if (!mMntPath.isEmpty())
//    {
//        QDir dir(mMntPath);
//        QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

//        foreach (QFileInfo fileInfo, folder_list)
//        {
//            if (!mFileList.contains(mMntPath + fileInfo.fileName(), Qt::CaseInsensitive))
//            {
//                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<mMntPath + fileInfo.fileName();
//                emit sigDeviceAdd(mMntPath + fileInfo.fileName());
//                mFileList.append(mMntPath + fileInfo.fileName());
//            }
//        }
//    }
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"mFileList = "<< mFileList;
    QtSleep(100);
}

void UsbHelper::onDeviceRemoved()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"mMntPath = "<<mMntPath;
    QtSleep(2000);
    if (!mMntPath.isEmpty())
    {
        QDir dir(mMntPath);
        QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        QStringList tmpFileList;
        tmpFileList.clear();
        foreach (QFileInfo fileInfo, folder_list)
        {
            tmpFileList.append(mMntPath + fileInfo.fileName());
        }
        if (tmpFileList.isEmpty())
        {
            if (mUsbPathList.isEmpty())
            {
                return;
            }
            else
            {
                foreach (QString tmp, mUsbPathList)
                {
                    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<" tmp = "<<tmp;
                    emit sigDeviceRemove(tmp);
                    mUsbPathList.removeOne(tmp);
                }
            }
        }
        else
        {
            foreach (QString tmp, mUsbPathList)
            {
                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<" mUsbPath = "<<mUsbPathList;
                if (!tmpFileList.contains(tmp, Qt::CaseInsensitive))
                {
                    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"tmp = "<< tmp;
                    emit sigDeviceRemove(tmp);
                    mUsbPathList.removeOne(tmp);
                }
            }
        }
    }
    QtSleep(100);
}
