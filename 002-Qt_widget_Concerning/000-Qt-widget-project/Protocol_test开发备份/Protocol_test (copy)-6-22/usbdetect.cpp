#include "usbdetect.h"

UsbDetect::UsbDetect(QWidget *parent) : QWidget(parent)
{
    QDBusInterface *dBusInterface = new QDBusInterface(DBUS_HAL_SERVICE,
                                                       DBUS_HAL_PATH,
                                                       DBUS_HAL_INTERFACE,
                                                       QDBusConnection::systemBus(),
                                                       this);
    if(!dBusInterface->isValid())
        qDebug()<< "初始化错误，找不到QDBusInterface！";
    QDBusConnection::systemBus().connect(DBUS_HAL_SERVICE, DBUS_HAL_PATH, DBUS_HAL_INTERFACE, "DeviceAdded", this, SLOT(slotDeviceAdded(QString)));
    QDBusConnection::systemBus().connect(DBUS_HAL_SERVICE, DBUS_HAL_PATH, DBUS_HAL_INTERFACE, "DeviceRemoved", this, SLOT(slotDeviceRemoved(QString)));
}

void UsbDetect::slotDeviceAdded(QString udi)
{
    static int num = 0;
    QDBusInterface *device = new QDBusInterface(DBUS_HAL_SERVICE, udi, DBUS_HAL_DEVICE_INTERFACE, QDBusConnection::systemBus(), this);

    if(!device->isValid())
        qDebug()<< "创建device失败！";
    else
        num++;
    QString devicePath = device->path();
    if(devicePath.contains("volume"))
    {
        currentUDI = udi;
    }
    else
        qDebug()<<"识别结果：不是USB存储设备！";
}

void UsbDetect::slotDeviceRemoved(QString udi)
{

    if(udi == currentUDI || udi.contains("volume"))
    {
        qDebug()<<"您的U盘已弹出！";
        currentUDI = "";
    }
    else
        qDebug()<< "已弹出---";
}
