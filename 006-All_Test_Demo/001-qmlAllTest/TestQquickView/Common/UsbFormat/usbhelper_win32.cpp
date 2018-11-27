#include "usbhelper.h"
#include "define.h"

#include <QFileInfo>
#include <QDir>


UsbHelper::UsbHelper(QObject *parent) : QObject(parent)
{

}

void UsbHelper::scanHD()
{
    IDE_DEBUG("Startup scan usb driver");
    char diskPath[6] = { 0 };
    DWORD allDisk = GetLogicalDrives(); //返回一个32位整数，将他转换成二进制后，表示磁盘,最低位为A盘
    if (allDisk != 0)
    {
        IDE_TRACE();
        for (int i = 0; i < MAX_DIR_NUMBER; ++i)     //假定最多有12个磁盘从A开始计数
        {
            if ((allDisk & 1) == 1)
            {
                sprintf( diskPath, "%c", 'A' + i );
                strcat( diskPath, ":" );
                LPCWSTR RootPathName =(LPCWSTR)QString(diskPath).utf16();

                if (GetDriveType(RootPathName) ==  DRIVE_REMOVABLE)
                {
                    //IDE_DEBUG("Startup scan usb driver");
                    if( GetVolumeInformation(RootPathName, 0, 0, 0, 0, 0, 0, 0) ) //判断驱动是否准备就绪
                    {
                        QString  path(diskPath);
                        this->addDisk(path);
                    }
                    IDE_TRACE();
                }
                IDE_TRACE();
            }
            allDisk = allDisk>>1;
        }
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
    //qDebug()<<"searchDisk 1"<<DiskName;
    for(int i = 0; i < this->usb_disk_info.size(); ++i)
    {
        //qDebug()<<"searchDisk 2 "<<usb_disk_info.at(i).DirPath;
        if(usb_disk_info.at(i).DirPath.compare(DiskName) == 0)
        {
            iter = i;
            //IDE_TRACE();
            return true;
        }
    }
    return false;
}

void UsbHelper::getMemorySize(USB_DISK_INFO &disk)
{
    ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
    LPCWSTR lpcwstrDriver=(LPCWSTR)disk.DirPath.utf16();
    if( !GetDiskFreeSpaceEx( lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes) )
    {
        IDE_DEBUG("ERROR: Call to GetDiskFreeSpaceEx() failed.");
        //qDebug()<<
        return ;
    }
    disk.FreeMemory  = (float)liTotalFreeBytes.QuadPart/1024/1024/1024; //剩余内存
    disk.TotalMemory = (float)liTotalBytes.QuadPart/1024/1024/1024;     //总内存
}

void UsbHelper::currentDiskChanged(QString e)
{
    int iter;
    if(this->searchDisk(e, iter))
    {
        this->CurrentUseDisk = this->usb_disk_info.at(iter);
        IDE_DEBUG(QString("Change current disk to : %1").arg(CurrentUseDisk.DirPath));
    }
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
    QString cmd = QString("cmd.exe /k FORMAT %1 /FS:FAT32 /q /y").arg(diskDir);
    QByteArray ba;
    ba.append(cmd);
    WinExec(ba.constData(), SW_HIDE);
    return "";
}

QString UsbHelper::formatUdisk2(QString diskDir)
{
    //QMessageBox::warning(this,"Formate Warning","This operate will erase all data on the disk. Please backup your files.",QMessageBox::Ok,QMessageBox::Cancel);
    QFile lgoFile("log_mediascreenhelper.log");
    if(!lgoFile.open(QIODevice::WriteOnly))
    {

    }
    IDE_TRACE_STR(diskDir);
    QString bashFile = "formatUdisk.bat";
    if(!isValideDriver(diskDir))
    {
       return "Invalid Driver Name";
    }
    QFile file(bashFile);

    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        return "Open/Generate formate bash file fail";
    }
    else
    {
        //QString cmd = QString("FORMAT %1 /FS:FAT32 /q /x /y").arg(diskDir);/X为卸载盘符
        QString cmd = QString("FORMAT %1 /FS:FAT32 /q /y").arg(diskDir);
        QByteArray ba;
        ba.append(cmd);
        file.write(ba);
        file.close();
        //QElapsedTimer timer;
        //timer.start();
        QByteArray bashPath;
        bashPath.append(bashFile);
        WinExec(bashPath, SW_HIDE);//SW_NORMAL  SW_HIDE
        return "ok";
    }
}

//the Disk Driver must bee the '[A-Z]:' style
bool UsbHelper::isValideDriver(QString diskDir)
{
    bool isDriver = false;
    if(diskDir.length() != 2)
        return false;
    if(diskDir.at(0) < 'A'||diskDir.at(0) > 'Z')
        return false;
    if(diskDir.at(1) != ':')
        return false;
    QFileInfoList driverList =  QDir::drives();
    for(int i=0; i < driverList.count(); i++)
    {
        QFileInfo file_info = driverList.at(i);
        QString dir = file_info.absolutePath();
        if(dir.startsWith(diskDir))
        {
            isDriver = true;
            break;
        }
    }
    return isDriver;
}
