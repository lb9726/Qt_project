#include "adcprotect.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <QDebug>
#include <QCoreApplication>

#define ADC_DATA_PATH(x) QString("/sys/devices/platform/soc/2100000.aips-bus/2198000.adc/iio:device0/in_voltage%1_raw").arg(x)

//sodimas I/O输入总共有24个(0-23) 0无按下 1有按下
//0-11:BC0-BC11
//12-15:Elib1-Elib4
//16-23:8位拨码开关
static unsigned char PreFlag[24];
static unsigned char LightSta[16];  //BC0-BC11 Elib1-Elib4 按钮输入灯状态处理
static unsigned char GpiosValue[24];
static int gpiofd;

/*
# ls
dev                            in_voltage_scale
in_voltage0_raw                name                             -------------->  weim.AD0---> 12个按钮的短路保护
in_voltage1_raw                of_node                          -------------->  weim.AD1---> 4个特殊按钮的短路保护
in_voltage2_raw                power                            -------------->  weim.AD2---> ENCART背光的短路保护
in_voltage3_raw                sampling_frequency_available     -------------->  weim.AD3---> ALIM的短路保护
in_voltage4_raw                subsystem                        -------------->  weim.AD4---> 扩展板按钮的短路保护
in_voltage_sampling_frequency  uevent
# pwd
/sys/devices/platform/soc/2100000.aips-bus/2198000.adc/iio:device0
*/

AdcProtect::AdcProtect(QThread *parent) : QThread(parent)
  , runPath(QCoreApplication::applicationDirPath())
{
    currentProtection();
//    initGpiosFd();
    msleep(1000);
//    start();
    init();
}

void AdcProtect::init()
{
    int fd, ret;
    qreal vref = 3.36;
    double voltage, get_value;
    char data[20];
    for (int i = 0; i < 5; ++i)
    {
        const char* tmp = ADC_DATA_PATH(i).toLatin1().data();
        printf("tmp = %s\n", tmp);
        fd = open(tmp, O_RDONLY);
        if(fd < 0)
        {
            perror("open AI8_DATA_PATH error");
            return;
        }
        ret = read(fd, data, sizeof(data));
        if(ret < 0)
        {
            perror("read data error");
            return;
        }
        /* collect data and calculate the voltage */
        get_value = atof(data);
        int temp = (int)get_value;
        if (temp > 2872)  // 3732
        {
//            currentProtection();
            printf("over 2872, will protected\n");
        }
        voltage = vref * (get_value / 4096);
        printf("get value: %f\n", get_value);
        printf("voltage: %.3f\n", voltage);
        close(fd);
    }
}

void AdcProtect::initGpiosFd()
{
    int flag;
    int sysFlag;
    memset(PreFlag, 0, 24);
    memset(LightSta, 0, 16);
    signal(SIGIO, mySignalFunc);     // 在应用程序中捕捉SIGIO信号(这个信号是驱动程序发送出来的)

    runPath = "insmod " + runPath +"/gpio-sodimas-canopen.ko";
    qDebug()<<"runPath = "<<runPath;
    sysFlag = sysExecuteKoFile(runPath.toLatin1().data());    // 挂载驱动

    gpiofd = open("/dev/canopen", O_RDWR);
    if(gpiofd < 0)
    {
        printf("Can't Open file id is:%d\n", gpiofd);
        return;
    }
    printf(" %s Open file id is:%d\n", __FUNCTION__, gpiofd);

    fcntl(gpiofd, F_SETOWN, getpid());      // 将当前进程PID设置为fd文件所对应驱动程序将要发送SIGIO信号进程PID
    flag = fcntl(gpiofd, F_GETFL);          // 获取fd的打开方式
    fcntl(gpiofd, F_SETFL, flag|FASYNC);    // 将fd的打开方式设置为FASYNC, 即支持异步通知
}

int AdcProtect::sysExecuteKoFile(const char *str)
{
    printf("%s %s\n", __FUNCTION__, str);
    int flag = system(str);
    printf("%s result is %d\n", __FUNCTION__, flag);
    return flag;
}

void AdcProtect::mySignalFunc(int signum)
{
    printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    read(gpiofd, GpiosValue, 24);
}

void AdcProtect::currentProtection()
{
    printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    unsigned char array[3];
    array[0] = 0x00;
    array[1] = 0x00;
    array[2] = 0x00;
    write(gpiofd, array, 3);
}

void AdcProtect::run()
{
    printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    while(1)
    {
        msleep(15);
        init();
    }
}


