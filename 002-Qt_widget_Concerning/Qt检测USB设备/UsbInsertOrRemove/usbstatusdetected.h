#ifndef USBSTATUSDETECTED_H
#define USBSTATUSDETECTED_H

#include <QObject>
#include "include/qdevicewatcher.h"
#include "include/qdevicewatcher_p.h"
#include <QString>
class UsbStatusDetected : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(QString usbAction READ getUsbAction WRITE setUsbAction)
public:
    explicit UsbStatusDetected(QObject *parent = 0);
    ~UsbStatusDetected();

signals:
    void uActionChanged(QString uact);
public slots:
    void addUsb(QString strAdd);
    void rmUsb(QString strRm);
    QString getUsbAction();
    void setUsbAction(const QString &uact);
public:
    QDeviceWatcher *qdw;
    QString usbAction;
};

#endif // USBSTATUSDETECTED_H
