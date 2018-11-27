#include "devicewathcer.h"

DeviceWatcher::DeviceWatcher(QObject *parent) :
    QThread(parent)
{
    start();

    moveToThread(this); //Let bool event(QEvent *e) be in another thread
    watcher = new QDeviceWatcher;
    watcher->moveToThread(this);
    watcher->appendEventReceiver(this);
    connect(watcher, SIGNAL(deviceAdded(QString)), this, SIGNAL(sDeviceAdded(QString)), Qt::AutoConnection);
    connect(watcher, SIGNAL(deviceChanged(QString)), this, SIGNAL(sDeviceChanged(QString)), Qt::AutoConnection);
    connect(watcher, SIGNAL(deviceRemoved(QString)), this, SIGNAL(sDeviceRemoved(QString)), Qt::AutoConnection);
    watcher->start();
}
