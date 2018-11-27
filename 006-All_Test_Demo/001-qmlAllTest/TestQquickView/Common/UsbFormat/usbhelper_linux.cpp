#include "usbhelper_linux.h"
#include "define.h"

#include <QFileInfo>
#include <QDir>

UsbHelper::UsbHelper(QObject *parent) : QObject(parent)
  , mFileList(QStringList())
  , mUsbPath(QStringList())
{

}

void UsbHelper::scanHD()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    // ubuntu下判断是否插入了U盘
    if (QFile::exists("/proc/scsi/usb-storage"))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"U disk is insert";
        detectUsbIsInsert("/media/");
        if (!mUsbPath.isEmpty())
        {
            foreach (QString DiskName, mUsbPath) // U盘在程序启动之前就插入的需要补发通知
            {
                qDebug()<<__PRETTY_FUNCTION__<<"DiskName = "<<DiskName;
                emit sigDeviceAdd(DiskName);
            }
        }
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"U disk is not insert";
    }
}

void UsbHelper::addDisk(QString &DiskName)
{
    IDE_TRACE_STR(DiskName);
    int iter = 0;
    if(searchDisk(DiskName, iter))
    {
        IDE_TRACE();
        return ;
    }
    USB_DISK_INFO usbtemp;
    usbtemp.DirPath = DiskName;
    getMemorySize(usbtemp);//获取设备内存信息
    this->usb_disk_info.push_back(usbtemp);
    emit sigDeviceAdd(DiskName);
    //IDE_TRACE();
}

void UsbHelper::deleteDisk(QString &DiskName)
{
    int iter = 0;
    if(searchDisk(DiskName, iter))
    {
        //IDE_TRACE();
        this->usb_disk_info.remove(iter);
        //IDE_TRACE();
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
    qDebug()<<__PRETTY_FUNCTION__<<"path = "<<path;
    QDir dir(path);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (QFileInfo fileInfo, folder_list)
    {
        mFileList.append(fileInfo.fileName());
    }
    foreach(QString tmp, mFileList)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"tmp = "<< tmp;

        if (!tmp.contains("floppy",  Qt::CaseInsensitive)) // 去掉/media路径下floppy的文件夹
        {
            QDir dir2(path + tmp + "/");
            mMntPath = path + tmp + "/";  // /meida/libo/
            qDebug()<<__PRETTY_FUNCTION__<<"mMntPath = "<<mMntPath;
            QFileInfoList flist = dir2.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
            foreach (QFileInfo fileInfo, flist)
            {
                QString tmpPath = path + tmp + "/"+ fileInfo.fileName();
                qDebug()<<__PRETTY_FUNCTION__<<"tmppath = "<<tmpPath;
                mUsbPath.append(tmpPath);
            }
        }
    }
    foreach(QString tmp, mUsbPath)
    {
        qDebug()<<__PRETTY_FUNCTION__<<tmp;
    }
    qDebug()<<__PRETTY_FUNCTION__<<mUsbPath;
    usbDetected();
}

void UsbHelper::usbDetected()
{
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
    connect(&mDeviceWatcher, &QDeviceWatcher::deviceAdded, this, &UsbHelper::slot_DeviceAdded, Qt::UniqueConnection);
    connect(&mDeviceWatcher, &QDeviceWatcher::deviceRemoved, this, &UsbHelper::slot_DeviceRemoved, Qt::UniqueConnection);
    mDeviceWatcher.start();
}

void UsbHelper::slot_DeviceAdded(QString pName)
{
    qDebug()<<__PRETTY_FUNCTION__<<"pName = "<<pName;
    onDeviceAdded();
}

void UsbHelper::slot_DeviceRemoved(QString pName)
{
    qDebug()<<__PRETTY_FUNCTION__<<"pName = "<< pName;
    onDeviceRemoved();
}

void UsbHelper::setMountPath(QString pPath)
{
    Q_UNUSED(pPath);
}

void UsbHelper::onDeviceAdded()
{
    QtSleep(3000);
    if (!mMntPath.isEmpty())
    {
        QDir dir(mMntPath);
        QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
//        QStringList tmpFileList;
//        tmpFileList.clear();
        foreach (QFileInfo fileInfo, folder_list)
        {
//            tmpFileList.append(mMntPath + fileInfo.fileName());
            if (!mFileList.contains(mMntPath + fileInfo.fileName(), Qt::CaseInsensitive))
            {
                qDebug()<<mMntPath + fileInfo.fileName();
                emit sigDeviceAdd(mMntPath + fileInfo.fileName());
                mFileList.append(mMntPath + fileInfo.fileName());
            }
        }
    }
}

void UsbHelper::onDeviceRemoved()
{
    QtSleep(3000);
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
        qDebug()<<__PRETTY_FUNCTION__<<"mMntPath = "<<mMntPath;
        foreach (QString tmp, mFileList)
        {
            if (!tmpFileList.contains(tmp, Qt::CaseInsensitive))
            {
                qDebug()<<__PRETTY_FUNCTION__<<"tmp = "<< tmp;
                emit sigDeviceRemove(tmp);
                mFileList.removeOne(tmp);
            }
        }
    }
}
