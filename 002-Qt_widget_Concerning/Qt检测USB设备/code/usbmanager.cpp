#include "usbmanager.h"


usbmanager::usbmanager(QObject *parent)
    : QObject parent)
{
     mDeviceMap.clear();
     interfaceHal = new QDBusInterface(DBUS_HAL_SERVICE,DBUS_HAL_PATH,DBUS_HAL_INTERFACE,QDBusConnection::systemBus());
     QDBusConnection::systemBus().connect(DBUS_HAL_SERVICE, DBUS_HAL_PATH, DBUS_HAL_INTERFACE, "DeviceAdded", this, SLOT(slotDeviceAdded(QString))); 
     QDBusConnection::systemBus().connect(DBUS_HAL_SERVICE, DBUS_HAL_PATH, DBUS_HAL_INTERFACE, "DeviceRemoved", this, SLOT(slotDeviceRemoved(QString))); 
    QTimer::singleShot(0, this, SLOT(scan())); 
}

void usbmanager::scan() 
{ 
    if (!interfaceHal ->isValid()) 
    { 
        printf("%s,%s,%d error message:%s\n",__FILE__,__FUNCTION__,__LINE__,QDBusConnection::systemBus().lastError().message().toLocal8Bit().data());
        return; 
    } 
 
    QDBusReply<QStringList> deviceList = interfaceHal->call(QDBus::Block, "GetAllDevices"); 
    if(deviceList.isValid()) 
    { 
        for(int i = 0; i < deviceList.value().length(); i++) 
        { 
            slotDeviceAdded(deviceList.value().at(i)); 
        } 
    } 
} 


void usbmanager::slotDeviceAdded(QString udi)
{ 
     QDBusInterface *remoteApp = new QDBusInterface(DBUS_HAL_SERVICE, udi, DBUS_HAL_DEVICE_INTERFACE, QDBusConnection::systemBus()); 
  
     if(!remoteApp->isValid()) 
     { 
         printf("%s,%s,%d error message:%s\n",__FILE__,__FUNCTION__,__LINE__,remoteApp->lastError().message().toLocal8Bit().data());
         return; 
     } 

     getDevice(remoteApp,udi);
 }


void usbmanager::slotDeviceRemoved(QString udi)
{
    mDeviceMap.remove(udi);
}

void usbmanager::getDevice(QDBusInterface *interface,const QString &udi)
{
    QDBusInterface *informationCenter;
    QString deviceUdi;

    informationCenter = interface;
    deviceUdi = udi;
    
    QString deviceCategory;
    QString deviceBlockName;
    QString deviceParentUdi;
    QString deviceBus;
    QString deviceCurrentMountPoint;
    int major = 0;
    bool isMounted = false;

    
    QDBusReply<QString> categoryReply = informationCenter->call("GetPropertyString","info.category");
    deviceCategory = categoryReply.value();

    QDBusReply<QString> blockReply = informationCenter->call("GetPropertyString","block.device");
    deviceBlockName = blockReply.value();

    QDBusReply<int> majorReply = informationCenter->call("GetPropertyInteger","block.major");
    major = majorReply.isValid() ? majorReply.value() : 0;

    LOG("%s,%s,%d category=%s blockName=%s major=%d\n", __FILE__,__FUNCTION__,__LINE__,deviceCategory.toLocal8Bit().data(),deviceBlockName.toLocal8Bit().data(),major);

    if (((isVolume(deviceCategory))||(isDisk(deviceCategory)))
        &&(isBlockDev(deviceBlockName))
        &&(isValidMajor(major))
        )
    {
        QDBusReply<QString> parentReply = informationCenter->call("GetPropertyString","info.parent");
        deviceParentUdi = parentReply.value();

        //if is volume
        if (isVolume(deviceCategory))
        {
            //if parent exist in map and not a usb device,return;
            if (mDeviceMap.contains(deviceParentUdi))
            {
                haldevice *devParent = mDeviceMap.value(deviceParentUdi);
                if (NULL != devParent)
                {
                    if (!isUsb(devParent->bus()))
                    {
                        return;
                    }
                }
            }
            
            QDBusReply<bool> isMountReply = informationCenter->call("GetPropertyString", "volume.is_mounted"); 
            if (isMountReply.isValid() )
            {
                isMounted = isMountReply.value();
            }

            //if already mounted, get mount point
            if (isMounted)
            {
                QDBusReply<QString> mountReplay = informationCenter->call(QDBus::AutoDetect, "GetPropertyString", "volume.mount_point"); 
                if ( (mountReplay.isValid()) && (mountReplay.value() != "")) 
                {
                    deviceCurrentMountPoint = mountReplay.value();
                }
            }
        }


        QDBusReply<QString> storageBusReply = informationCenter->call("GetPropertyString","storage.bus");
        if (storageBusReply.isValid()) deviceBus = storageBusReply.value();

        haldevice *dev = NULL;
        
        if (mDeviceMap.contains(udi))
        {
            dev = mDeviceMap.take(udi);
        }
        else
        {
            dev = new haldevice(informationCenter,udi);
        }
        dev->setParentUdi(deviceParentUdi);
        dev->setCategory(deviceCategory);
        dev->setBlockName(deviceBlockName);
        dev->setMajorNumber(major);
        dev->setBus(deviceBus);
        dev->setCurrentMountPoint(deviceCurrentMountPoint);
        dev->setIsMounted(isMounted);

        if (isVolume(deviceCategory))
        {
        //only volumn device needs connect the hal dbus signal
        informationCenter->connection().connect(DBUS_HAL_SERVICE,udi,DBUS_HAL_DEVICE_INTERFACE,"PropertyModified",
                            dev,SLOT(propertyModified(int, const QList<ChangeStruct>&)));
        }

        //TODO
        //if is already mounted, open filebrowser

        mDeviceMap.insert(udi,dev);


        LOG("*****begin print device struct*****\n");
        LOG("deviceUdi:%s ",udi.toLocal8Bit().data());
        LOG("deviceCategory:%s ",deviceCategory.toLocal8Bit().data());
        LOG("deviceBlockName:%s ",deviceBlockName.toLocal8Bit().data());
        LOG("major:%d \n",major);
        LOG("deviceBus:%s ",deviceBus.toLocal8Bit().data());
        LOG("deviceCurrentMountPoint:%s ",deviceCurrentMountPoint.toLocal8Bit().data());
        LOG("isMounted:%d \n",isMounted);
        LOG("*****end   print device struct*****\n");
        
    }

}


bool usbmanager::isVolume(QString value)
{
    QString volume("volume");
    if (0 == volume.compare(value,Qt::CaseInsensitive))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool usbmanager::isDisk(QString value)
{
    QString storage("storage");
    if (0 == storage.compare(value,Qt::CaseInsensitive))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool usbmanager::isBlockDev(QString value)
{
    return value.startsWith("/dev/");   
}

bool usbmanager::isValidMajor(int value)
{
    return (0 != value);
}

bool usbmanager::isUsb(QString value)
{
    QString usb("usb");
    if (0 == usb.compare(value,Qt::CaseInsensitive))
    {
        return true;
    }
    else
    {
        return false;
    }

}

