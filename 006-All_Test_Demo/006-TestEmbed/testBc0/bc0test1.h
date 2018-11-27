#ifndef BC0TEST_H
#define BC0TEST_H

#define  DIAL_NUM           8
#define GPIO_VALUE(x)       QString("/sys/class/gpio/gpio%1/value").arg(x)      // 输入输出电平值设备
#define GPIO_EXPORT         "/sys/class/gpio/export"              // GPIO 设备导出设备
#define GPIO_DIRECT(x)      QString("/sys/class/gpio/gpio%1/direction").arg(x)  // GPIO 输入输出控制设备
#include <QString>
#include <QByteArray>
#include <QFile>
#include <QThread>
#include <QDebug>

class Bc0Test1 : public QThread
{
public:
    Bc0Test1();
    void init();
    QFile               mDipsExport[12];
    QFile               mDirection[12];
    void run();
    void bc0On(int index);
    void bc0Off(int index);
    void bc0Staus();
};

#endif // BC0TEST_H
