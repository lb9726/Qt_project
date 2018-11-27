#ifndef DIPSIO_H
#define DIPSIO_H

#include "define.h"
#include <QThread>

#define GPIO_VALUE(x)       QString("/sys/class/gpio/gpio%1/value").arg(x)      // 输入输出电平值设备
#define GPIO_EXPORT         QString("/sys/class/gpio/export")            // GPIO 设备导出设备
#define GPIO_DIRECT(x)      QString("/sys/class/gpio/gpio%1/direction").arg(x)  // GPIO 输入输出控制设备

class DipsIO : public QThread
{
    Q_OBJECT
public:
    DipsIO(QObject *parent = 0);

    void run(); //当执行了QThread的start函数后，会自动在新的线程中运行此函数

signals:
    void sValueChanged(quint8 pValue);

public:
    quint8              mDipsValue;

    QFile               mDipsExport;
    QFile               mDips8[8];
};

#endif // DIPSIO_H
