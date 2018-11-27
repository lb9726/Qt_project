#include "haldevice.h"


Q_DECLARE_METATYPE(ChangeStruct)
Q_DECLARE_METATYPE(QList<ChangeStruct>)


const QDBusArgument &operator<<( QDBusArgument &arg, const ChangeStruct &change ) {
    arg.beginStructure();
    arg << change.propertyName << change.added << change.removed;
    arg.endStructure();
    return arg;
}

const QDBusArgument &operator>>( const QDBusArgument &arg, ChangeStruct &change ) {
    arg.beginStructure();
    arg >> change.propertyName >> change.added >> change.removed;
    arg.endStructure();
    return arg;
}

haldevice::haldevice(QDBusInterface *interface, const QString& udi)
{
    LOG("%s,%s,%d \n",__FILE__,__FUNCTION__,__LINE__);

    //init var
    informationCenter = interface;
    deviceUdi = udi;
    deviceIsMounted = false;
    major = 0;

    //register dbus meta type
    qDBusRegisterMetaType<ChangeStruct>();
    qDBusRegisterMetaType< QList<ChangeStruct> >();

    //connect signal and slot
    connect(this,SIGNAL(mountedStatusChanged(bool)),this,SLOT(proMountedStatusChanged(bool)));
    connect(this,SIGNAL(readyBrowserUsb(QString)),this,SLOT(proReadyBrowserUsb(QString)));
}

haldevice::~haldevice() {
    delete informationCenter;
    informationCenter = NULL;

    //TODO
    //if filebrowser is open,close it
}

const QString& haldevice::udi() const
{
    return deviceUdi;
}

const QString& haldevice::parentUdi() const
{
    return deviceParentUdi;
}

const QString& haldevice::blockName() const
{
    return deviceBlockName;
}

const QString& haldevice::category() const
{
    return deviceCategory;
}

const QString& haldevice::bus() const
{
    return deviceBus;
}

int haldevice::majorNumber() const
{
    return major;
}

bool haldevice::isMounted() const
{
    return deviceIsMounted;
}

bool haldevice::isMountedByDBus() const
{
    LOG("%s,%s,%d \n",__FILE__,__FUNCTION__,__LINE__);

    QDBusReply<bool> reply = informationCenter->call("GetPropertyBoolean","volume.is_mounted");
    return reply.isValid() ? reply.value() : false;
}

const QString& haldevice::currentMountPointByDBus()
{
    QDBusReply<QString> reply = informationCenter->call("GetPropertyString","volume.mount_point");
    //if (reply.isValid()) LOG("%s,%s,%d %s\n",__FILE__,__FUNCTION__,__LINE__,reply.value().toLocal8Bit().data());
    return reply.isValid() ? reply.value() : "";
}


void haldevice::propertyModified(int numChanges,const QList<ChangeStruct> &changes)
{
    LOG("%s,%s,%d  udi=%s numChanges=%d\n",__FILE__,__FUNCTION__,__LINE__,deviceUdi.toLocal8Bit().data(),numChanges);
    foreach (const ChangeStruct changeStruct, changes) 
    {
        LOG("%s,%s,%d name:%s %d %d\n",__FILE__,__FUNCTION__,__LINE__,changeStruct.propertyName.toLocal8Bit().data(),changeStruct.added,changeStruct.removed);

        if (changeStruct.propertyName == "volume.is_mounted")
        {
        
            LOG("%s,%s,%d \n",__FILE__,__FUNCTION__,__LINE__);
            emit (mountedStatusChanged(isMountedByDBus()));
        }
    }
}

void haldevice::proMountedStatusChanged(bool isMounted)
{
    LOG("%s,%s,%d isMount=%d\n",__FILE__,__FUNCTION__,__LINE__,isMounted);

    if (isMounted)
    {
        emit (readyBrowserUsb(currentMountPointByDBus()));
    }
    else
    {
        //TODO
        //close filebrowser and release lib handle
        
        LOG("%s,%s,%d close file browser\n",__FILE__,__FUNCTION__,__LINE__);
    }
}


void haldevice::proReadyBrowserUsb(QString path)
{
    //call filebrowser lib to browse usb files
    LOG("%s,%s,%d %s\n",__FILE__,__FUNCTION__,__LINE__,path.toLocal8Bit().data());

    //TODO
    //open filebrowser with path
    //filebrowser browser;
    //browser.showFileBrowser(path);
}


void haldevice::setParentUdi(QString value)
{
    deviceParentUdi = value;
}
void haldevice::setBlockName(QString value)
{
    deviceBlockName = value;

}
void haldevice::setCurrentMountPoint(QString value)
{
    deviceCurrentMountPoint = value;

}
void haldevice::setCategory(QString value)
{
    deviceCategory = value;

}
void haldevice::setBus(QString value)
{
    deviceBus = value;

}
void haldevice::setIsMounted(bool value)
{
    deviceIsMounted = value;

}
void haldevice::setMajorNumber(int value)
{
    major = value;
}

