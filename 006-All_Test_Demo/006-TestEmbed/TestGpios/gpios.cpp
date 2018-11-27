#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "gpios.h"
#include <QDebug>
#include "define.h"
#include <QCoreApplication>

//sodimas I/O输入总共有24个(0-23) 0无按下 1有按下
//0-11:BC0-BC11
//12-15:Elib1-Elib4
//16-23:8位拨码开关
static unsigned char PreFlag[GPIONUM];
static unsigned char LightSta[16];  //BC0-BC11 Elib1-Elib4按钮输入灯状态处理
static unsigned char GpiosValue[GPIONUM];
static int fd;

Gpios::Gpios(QThread *parent) : QThread(parent)
  , runThreadFlag(true)
  , mDipValue(0)
  , runPath(QCoreApplication::applicationDirPath())
{
    initGpiosFd();
//    currentProtection();
}

Gpios::~Gpios()
{
    IDE_TRACE();
    setRunThreadFlag(false);
}

int Gpios::sysExecuteKoFile(const char *str)
{
    printf("%s %s\n", __FUNCTION__, str);
    int flag = system(str);
    printf("%s result is %d\n", __FUNCTION__, flag);
    return flag;
}

void Gpios::currentProtection()
{
    printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    unsigned char array[3];
    array[0] = 0x00;
    array[1] = 0x00;
    array[2] = 0x00;
    write(fd, array, 3);
}

void Gpios::mySignalFunc(int signum)
{
    read(fd, GpiosValue, 24);
    //    printf("%s %s lines %d read \n",__FILE__, __FUNCTION__, __LINE__);
    //    signal = 1;
}

void Gpios::initGpiosFd()
{
    int flag;
    int sysFlag;
    memset(PreFlag, 0, 24);
    memset(LightSta, 0, 16);
    signal(SIGIO, mySignalFunc);     // 在应用程序中捕捉SIGIO信号(这个信号是驱动程序发送出来的)

    runPath = "insmod " + runPath +"/gpio-sodimas-canopen.ko";
    qDebug()<<"runPath = "<<runPath;
    sysFlag = sysExecuteKoFile(runPath.toLatin1().data());    // 挂载驱动

    //    if (-1 == sysFlag)
    //    {
    //        printf("%s %s lines %d insmod drive error exit programe!\n",__FILE__, __FUNCTION__, __LINE__);
    //        exit(-1);
    //    }

    fd = open("/dev/canopen", O_RDWR);
    if(fd < 0)
    {
        printf("Can't Open file id is:%d\n", fd);
        return;
    }
    printf(" %s Open file id is:%d\n", __FUNCTION__, fd);

    fcntl(fd, F_SETOWN, getpid());      // 将当前进程PID设置为fd文件所对应驱动程序将要发送SIGIO信号进程PID
    flag = fcntl(fd, F_GETFL);          // 获取fd的打开方式
    fcntl(fd, F_SETFL, flag|FASYNC);    // 将fd的打开方式设置为FASYNC, 即支持异步通知
    while(read(fd, GpiosValue, 24) < 0);
}

void Gpios::run()
{
    IDE_TRACE();
    unsigned char Cnt;
    while (runThreadFlag)
    {
        //sodimas I/O输入总共有24个(0-23) 0无按下 1有按下
        msleep(15);
        for(Cnt = 0; Cnt < 17; ++Cnt) //0-11: BC0-BC11  //12-15: Elib1-Elib4
        {
            if (Cnt < 16)
            {
                if(GpiosValue[Cnt])   // 有按键按下
                {
                    PreFlag[Cnt] = 1;  // 记录按下标志
                }
                else
                {
                    if(PreFlag[Cnt])   // 松开按键
                    {
                        PreFlag[Cnt] = 0; // 清除按下标志
                        /* 可在此处将按钮信息发送给控制柜或是作为事件触发 */
                        /*test func: press on and again off*/
                        if(Cnt < 16) // BC0-BC11 Elib1-Elib4 按钮输入灯状态处理
                        {
                            LightSta[Cnt] ^= 1;
                            if(LightSta[Cnt])
                            {
                                printf("btn%d is pressed\n", Cnt);
                                emit sigBtnPressed(Cnt);
                            }
                            else
                            {
                                printf("btn%d is cancled\n", Cnt);
                                emit sigBtnCanceld(Cnt);
                            }
                        }
                    } /*end of test*/
                }
            }
            else if (16 == Cnt) //16-23: 8位拨码开关
            {
                getDialSwitchValue();
            }
        }
    }
}

void Gpios::setLedLightFlag(int index, int flag)
{

}

void Gpios::setRunThreadFlag(bool runflag)
{
    IDE_TRACE();
    runThreadFlag = runflag;
}

quint8 Gpios::getDialSwitchValue()
{    
    quint8 tmpValue = 0;
    for (int i = 16; i < 24; ++i)
    {
        if (1 == GpiosValue[i])
        {
            tmpValue |= (1<<(i - 16));
        }
        else
        {
            tmpValue &= ~(1<<(i - 16));
        }
    }
    if (mDipValue != tmpValue)
    {
        mDipValue = tmpValue;
        emit sigDailChanged(mDipValue);
        qDebug()<<__FUNCTION__<< " mDipValue is changed "<< mDipValue;
    }
    return mDipValue;
}

void Gpios::onSigBtnPressedLightOn(quint8 btnindex)
{
    unsigned char sta = 1;
    qDebug()<<"void Gpios::"<<__FUNCTION__<<"will lighton "<<btnindex;
    lseek(fd, btnindex, SEEK_SET);
    write(fd, &sta, 1);
}

void Gpios::onSigBtnPressedLightOff(quint8 btnindex)
{
    unsigned char sta = 0;
    qDebug()<<"void Gpios::"<<__FUNCTION__<<"will lightoff "<<btnindex;
    lseek(fd, btnindex, SEEK_SET);
    write(fd, &sta, 1);
    //    LightSta[Cnt] ^= 1;  //////////?????????
}

void Gpios::onSigDailChanged(quint8 pNum)
{

}

