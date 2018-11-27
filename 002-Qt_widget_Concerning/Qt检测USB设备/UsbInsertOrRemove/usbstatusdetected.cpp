#include "usbstatusdetected.h"
#include <QDebug>
UsbStatusDetected::UsbStatusDetected(QObject *parent) : QObject(parent)
{
    qDebug()<<__FUNCTION__;
    usbAction = QString();
    qdw = new QDeviceWatcher;
    qdw->appendEventReceiver(this);
    connect(qdw, SIGNAL(deviceAdded(QString)), this, SLOT(addUsb(QString)));
    connect(qdw, SIGNAL(deviceRemoved(QString)), this, SLOT(rmUsb(QString)));
    qdw->start();
    qDebug()<< "enter gouzao function";
}

UsbStatusDetected::~UsbStatusDetected()
{
    qDebug()<<__FUNCTION__;
    delete qdw;
    qDebug()<< "enter xigou function";
}

void UsbStatusDetected::addUsb(QString strAdd)
{
    qDebug()<<__FUNCTION__;
    setUsbAction(strAdd);
    if (!strAdd.compare(usbAction))
    {
        emit uActionChanged(usbAction);
        qDebug()<< "emit addusb function";
    }
}

void UsbStatusDetected::rmUsb(QString strRm)
{
    setUsbAction(strRm);
    if (!strRm.compare(usbAction))
    {
        emit uActionChanged(usbAction);
        qDebug()<< "emit rmusb function";
    }
}

QString UsbStatusDetected::getUsbAction()
{
    qDebug()<<__FUNCTION__;
    return usbAction;
}

void UsbStatusDetected::setUsbAction(const QString &uact)
{
    qDebug()<<__FUNCTION__;
    usbAction = uact;
}
