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
#include "common/define.h"
#include <QCoreApplication>

//sodimas I/O输入总共有24个(0-23) 0无按下 1有按下
//0-11:BC0-BC11
//12-15:Elib1-Elib4
//16-23:8位拨码开关
static unsigned char PreFlag[GPIONUM];
static unsigned char LightSta[16];  //BC0-BC11 Elib1-Elib4按钮输入灯状态处理
static unsigned char GpiosValue[GPIONUM];
static int fd;
static int ledLightOffFlag[12] = {0};
static unsigned char protection[24] = {0};

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
//    unsigned char array[24];
//    for (int i = 0; i < 24; ++i)
//    {
//        array[i] = 0;
//    }
    lseek(fd, 0, SEEK_SET);
    write(fd, protection, 24);
}

void Gpios::setElibLed(bool flag)
{
    IDE_TRACE_STR(QString("will write led"));
    unsigned char sta;
    int index;
    if (flag)
    {
        sta = 1;
        index = 16;
    }
    else
    {
        sta = 0;
        index = 17;
    }
//    printf("flag = %d index = %d fd = %d\n", flag, index, fd);
    if (fd > 0)
    {
        lseek(fd, index, SEEK_SET);
        write(fd, &sta, 1);
    }
}

quint8 Gpios::oneGetDialSwitchValue()
{
    quint8 tmpValue = 0;
    if (fd < 0)
    {
        IDE_TRACE_STR(QString("%1 is < 0 ").arg(fd));
        return tmpValue;
    }

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
    IDE_DEBUG(QString("%1%2").arg(" mDipValue is = ").arg(mDipValue));
    return tmpValue;
}

void Gpios::mySignalFunc(int signum)
{
    if (fd > 0)
    {
        read(fd, GpiosValue, 24);
    }
    else
    {
       IDE_DEBUG(QString("%1%2").arg("fd is smaller than 0 is ").arg(fd));
    }
    for (int i = 0; i < 12; ++i)
    {
        if (0 == GpiosValue[i])
        {
            if (1 == ledLightOffFlag[i])
            {
                ledLightOffFlag[i] = 0;
                lseek(fd, i, SEEK_SET);
                write(fd, &GpiosValue[i], 1);
                IDE_TRACE_STR(QString("release will off light"));
            }
        }
    }
}

void Gpios::initGpiosFd()
{
    int flag;
    int sysFlag;
    memset(PreFlag, 0, 24);
    memset(LightSta, 0, 16);
    signal(SIGIO, mySignalFunc);     // 在应用程序中捕捉SIGIO信号(这个信号是驱动程序发送出来的)

    runPath = "insmod " + runPath +"/gpio-sodimas-canopen.ko";
    IDE_DEBUG(QString("%1%2").arg("runPath = ").arg(runPath));
    sysFlag = sysExecuteKoFile(runPath.toLatin1().data());    // 挂载驱动

    fd = open("/dev/canopen", O_RDWR);
    if(fd < 0)
    {
        IDE_DEBUG(QString("%1%2").arg("Can't Open file id is:").arg(fd));
        return;
    }
    IDE_DEBUG(QString("%1%2").arg("Open file id is:").arg(fd));
//    printf(" %s Open file id is:%d\n", __FUNCTION__, fd);

    fcntl(fd, F_SETOWN, getpid());      // 将当前进程PID设置为fd文件所对应驱动程序将要发送SIGIO信号进程PID
    flag = fcntl(fd, F_GETFL);          // 获取fd的打开方式
    fcntl(fd, F_SETFL, flag|FASYNC);    // 将fd的打开方式设置为FASYNC, 即支持异步通知
    QThread::msleep(50);
    read(fd, GpiosValue, 24);
    IDE_TRACE_STR(QString("end void Gpios::initGpiosFd()"));
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
                    if (0 == PreFlag[Cnt])
                    {
                        /* 可在此处将按钮信息发送给控制柜或是作为事件触发 */
                        if(Cnt < 16) // BC0-BC11 Elib1-Elib4 按钮输入灯状态处理
                        {
                            LightSta[Cnt] ^= 1;
                            if(LightSta[Cnt])
                            {
                                if (Cnt < 12)
                                {
                                    printf("btn%d is pressed\n", Cnt);
                                    emit sigBtnPressed(Cnt+1);
                                }
                                else if (Cnt < 16)
                                {
                                    printf("btn%d is pressed\n", Cnt+25);
                                    emit sigBtnPressed(Cnt+25);
                                }
                            }
                            else
                            {
                                if (Cnt < 12)
                                {
                                    printf("btn%d is cancled\n", Cnt);
                                    emit sigBtnCanceld(Cnt+1);
                                }
                                else if (Cnt < 16)
                                {
                                    emit sigBtnCanceld(Cnt+25);
                                    printf("btn%d is cancled\n", Cnt+25);
                                }
                            }
                        }
                    }
                    PreFlag[Cnt] = 1;  // 记录按下标志                    
                }
                else
                {
                    if(PreFlag[Cnt])   // 松开按键
                    {
                        if (Cnt < 12)
                        {
                            emit sigBtnReleased(Cnt+1);
                            PreFlag[Cnt] = 0; // 清除按下标志
                        }
                        else if (Cnt < 16)
                        {
                            emit sigBtnReleased(Cnt+25);
                            PreFlag[Cnt] = 0; // 清除按下标志
                        }
                    } /*end of if(PreFlag[Cnt])*/
                }
            }
            else if (16 == Cnt) //16-23: 8位拨码开关
            {
                getDialSwitchValue();
            }
        }
    }
}

void Gpios::setLedLightOffFlag(int index, int flag)
{
    printf("%s() index = %d flag = %d\n", __PRETTY_FUNCTION__, index, flag);
    ledLightOffFlag[index - 1] = flag;  // 数组索引需要减掉1
    printf("%s() ledLightOffFlag[%d] = flag = %d\n", __PRETTY_FUNCTION__, index - 1, flag);
}

void Gpios::setRunThreadFlag(bool runflag)
{
    IDE_TRACE();
    runThreadFlag = runflag;
}

void Gpios::getDialSwitchValue()
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

