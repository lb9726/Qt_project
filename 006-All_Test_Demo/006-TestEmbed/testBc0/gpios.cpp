#include "gpios.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <QDebug>

static const char *Output[] = {IO_PATH(32), IO_PATH(33), IO_PATH(34), IO_PATH(35), IO_PATH(36), IO_PATH(37),
                         IO_PATH(38), IO_PATH(39), IO_PATH(40), IO_PATH(41), IO_PATH(42), IO_PATH(43),
                         IO_PATH(113), IO_PATH(114), IO_PATH(115), IO_PATH(116), IO_PATH(18), IO_PATH(19),
                         IO_PATH(20), IO_PATH(21), IO_PATH(22), IO_PATH(23), IO_PATH(24), IO_PATH(25) };

static const char *Intput[] = {IO_PATH(128), IO_PATH(129), IO_PATH(130), IO_PATH(131), IO_PATH(132), IO_PATH(133),
                         IO_PATH(134), IO_PATH(135), IO_PATH(136), IO_PATH(137), IO_PATH(48), IO_PATH(49),
                         IO_PATH(50), IO_PATH(51), IO_PATH(52), IO_PATH(53), IO_PATH(18), IO_PATH(19),
                         IO_PATH(20), IO_PATH(21),IO_PATH(22), IO_PATH(23), IO_PATH(24), IO_PATH(25) };

Gpios::Gpios(QThread *parent) : QThread(parent)
  , runThreadFlag(true)
  , mDipValue(0)
{
    qDebug()<<__FUNCTION__<<"is constructor!"<<"add const";
    qDebug()<<__FUNCTION__<<"is constructor!"<<"runThreadFlag = "<<runThreadFlag;
//    connect(this, &Gpios::sigBtnPressed, this, &Gpios::onSigBtnPressed, Qt::QueuedConnection);
//    connect(this, &Gpios::sigDailChanged, this, &Gpios::onSigDailChanged, Qt::QueuedConnection);
    int i;/*
    int CtrlLightNum[BTNNUM];
    int ButtonPressFlag[BTNNUM];
    int ButtonLightOnPressFlag[BTNNUM];
    int count[4];
    int sendPressSignalFlag[BTNNUM];
    int sendCancelSignalFlag[BTNNUM];*/
    for(i = 0; i < DAILSWITCH; i++)  // 先所有按钮设置为低电平
    {
        if (i < BTNNUM)
        {
//            setButtonStatus(i, LED_OFF);
            CtrlLightNum[i] = 2;
            ButtonPressFlag[i] = 0;
            ButtonLightOnPressFlag[i] = 0;
            sendPressSignalFlag[i] = 0;
            sendCancelSignalFlag[i] = 0;
            ledLightFlag[i] = 2;
        }
        else if (i >= BTNNUM && i < DAILSWITCH)
        {
            count[i - BTNNUM] = 0;
        }
    }
}

Gpios::~Gpios()
{
    setRunThreadFlag(false);
}

void Gpios::setButtonStatus(unsigned char index, unsigned char sta)
{
    int fd;
    fd = open(Output[index], O_RDWR);
    if(fd < 0){
        perror("open error:");
        return;
    }
    write(fd, &sta, 1);
    close(fd);
}

//亮灯情况下读取按钮状态值方法
// index : 0-11 对应BC0 - BC11
unsigned char Gpios::lightOnCheckInputStatus(unsigned char index)
{
    int fd;
    char sta;
    setButtonStatus(index, LED_OFF);
    fd = open(Intput[index], O_RDONLY|O_NONBLOCK);
    if(fd < 0)
    {
        perror("open error:");
        setButtonStatus(index, LED_ON);
        return 100;
    }
    read(fd, &sta, 1);
    close(fd);
    setButtonStatus(index, LED_ON);
    //    printf("Read sta:BC%d is %c\n", ButtonBuf[index], sta);
    return sta;
}

//正常情况下读取按钮状态值方法
// index : 0-11 对应BC0 - BC11
unsigned char Gpios::normalInputStatus(unsigned char index)
{
    int fd;
    char sta;
    fd = open(Intput[index], O_RDONLY|O_NONBLOCK);
    if(fd < 0){
        perror("open error:");
        setButtonStatus(index, LED_ON);
        return 100;
    }
    read(fd, &sta, 1);
    close(fd);
    return sta;
}

//void Gpios::startCheckBtnStatus()
//{
//    int i;
//    int CtrlLightNum[BTNNUM];
//    int ButtonPressFlag[BTNNUM];
//    int count[4];
//    int sendPressSignalFlag[BTNNUM];
//    int sendCancelSignalFlag[BTNNUM];
//    for(i = 0; i < DAILSWITCH; i++)  // 先所有按钮设置为低电平
//    {
//        if (i < BTNNUM)
//        {
//            setButtonStatus(i, LED_OFF);
//            CtrlLightNum[i] = 0;
//            ButtonPressFlag[i] = 0;
//            sendPressSignalFlag[i] = 0;
//            sendCancelSignalFlag[i] = 0;
//            ledLightFlag[i] = 2;
//        }
//        else if (i >= BTNNUM && i < DAILSWITCH)
//        {
//            count[i - BTNNUM] = 0;
//        }
//    }
//    while(runThreadFlag)
//    {
//        usleep(20333);
//        for(i = 0; i < DAILSWITCH + 1; ++i)
//        {
//            if (i >= 0 && i < BTNNUM)       // 12个板载楼层按钮
//            {
//                if(CtrlLightNum[i] == '0' ) //按下 , 亮灯情况下
//                {
//                    if(lightOnCheckInputStatus(i) == '0') //按下灯亮
//                    {
//                        ButtonPressFlag[i] = 1;   //记录按下动作
//                        sendCancelSignalFlag[i] = 0;
//                    }
//                    else    //松开
//                    {
//                        if(ButtonPressFlag[i])
//                        {
//                            if (0 == sendCancelSignalFlag[i])  // 发送给控制柜,一次请求只发送一次
//                            {
//                                emit sigBtnCanceld(i+1);  // 发送给控制柜，按下信息
//                                sendCancelSignalFlag[i] = 1;    //  发送过信号
//                                printf("will send cancel signal\n");
//                            }
//                            ButtonPressFlag[i] = 0;      //清除按下操作
//                        }
//                    }
//                    if (0 == ledLightFlag[i]) //if(控制柜灭灯)
//                    {
//                        printf("Led off signal is arrive!");
//                        setButtonStatus(i, LED_OFF); //灭灯
//                        CtrlLightNum[i] = '1';
//                        ledLightFlag[i] = 2;
//                    }
//                }
//                else //非亮灯情况下
//                {
//                    if(ButtonPressFlag[i])
//                    {
//                        if(lightOnCheckInputStatus(i) == '1') //松开
//                        {
//                            ButtonPressFlag[i] = 0;//清除按下动作
//                            if (ledLightFlag[i] == 1)        // 控制柜点灯信号
//                            {
//                                printf("will light on BC%d\n", i);
//                                CtrlLightNum[i] = '0';
//                                setButtonStatus(i, LED_ON);  //点灯
//                                ledLightFlag[i] = 2;         // 清成非0和1的数值
//                            }
//                            else
//                            {
//                                CtrlLightNum[i] = '1';
//                            }
//                        }
//                    }
//                    else
//                    {
//                        if(normalInputStatus(i) == '0') //按下动作
//                        {
//                            ButtonPressFlag[i] = 1;     //记录按下动作
//                            if (0 == sendPressSignalFlag[i])
//                            {
//                                emit sigBtnPressed(i+1);
//                                printf("will send pressed signal\n");
//                                sendPressSignalFlag[i] = 1;
//                            }
//                        }
//                        else
//                        {
//                            sendPressSignalFlag[i] = 0;
//                        }
//                    }
//                }
//            }
//            else if (i >= BTNNUM && i < BTNNUM + 4)    // 4个特殊按钮
//            {
//                if ('0' == normalInputStatus(i))  // 按下时为低电平
//                {
//                    if (0 == count[i-BTNNUM])
//                    {
//                        emit sigSpecialBtnPressed(i+1);
//                        setButtonStatus(i, LED_ON);
//                    }
//                    count[i-BTNNUM] = 1;
//                }
//                else if ('1' == normalInputStatus(i))
//                {
//                    count[i-BTNNUM] = 0;
//                }
//            }
//            else if (i >= DAILSWITCH)  //  8个拨码开关
//            {
//                getDialSwitchValue();
//            }
//        }
//    }
//}

void Gpios::startCheckBtnStatus()
{
//    printf("timeout is calling!\n");
    int i;
//    int CtrlLightNum[BTNNUM];
//    int ButtonPressFlag[BTNNUM];
//    int ButtonLightOnPressFlag[BTNNUM];
//    int count[4];
//    int sendPressSignalFlag[BTNNUM];
//    int sendCancelSignalFlag[BTNNUM];
//    for(i = 0; i < DAILSWITCH; i++)  // 先所有按钮设置为低电平
//    {
//        if (i < BTNNUM)
//        {
//            setButtonStatus(i, LED_OFF);
//            CtrlLightNum[i] = 2;
//            ButtonPressFlag[i] = 0;
//            ButtonLightOnPressFlag[i] = 0;
//            sendPressSignalFlag[i] = 0;
//            sendCancelSignalFlag[i] = 0;
//            ledLightFlag[i] = 2;
//        }
//        else if (i >= BTNNUM && i < DAILSWITCH)
//        {
//            count[i - BTNNUM] = 0;
//        }
//    }
//    while(runThreadFlag)
//    {
//        usleep(20333);
    for(i = 0; i < DAILSWITCH + 1; ++i)
    {
        if (i >= 0 && i < BTNNUM)   // 12个板载楼层按钮
        {
            if(CtrlLightNum[i] == '0' )//按下 , 亮灯情况下
            {
                if(lightOnCheckInputStatus(i) == '0') //按下灯亮
                {
                    ButtonPressFlag[i] = 1;//记录按下动作
                }
                else //松开
                {
                    if(ButtonPressFlag[i])
                    {
                        ButtonPressFlag[i] = 0;      //清除按下操作
                        setButtonStatus(i, LED_OFF); //灭灯
                        //发送给控制柜，按下信息
                        emit sigBtnPressed(i+1);
                        //if(控制柜灭灯)
                        //{
                        printf("BC%d is cancled\n", i);
                        CtrlLightNum[i] = '1';
                        //}
                    }
                }
            }
            else //非亮灯情况下
            {
                if(ButtonPressFlag[i])
                {
                    if(lightOnCheckInputStatus(i) == '1') //松开
                    {
                        //SetButtonStatus(i, LED_ON);//点灯
                        ButtonPressFlag[i] = 0;//清除按下动作
                        //测试需要
                        CtrlLightNum[i] = '0';
                        //发送给控制柜
                        //if(控制柜点灯){
                        //	CtrlLightNum[i] = '0';
                        //}else{
                        //  CtrlLightNum[i] = '1';
                        //}
                    }
                }
                else
                {
                    if(normalInputStatus(i) == '0') //按下动作
                    {
                        ButtonPressFlag[i] = 1;     //记录按下动作
                        emit sigBtnPressed(i+1);
                        printf("BC%d is pressed\n", i);
                        setButtonStatus(i, LED_ON);//点灯
                    }
                }
            }

        }
        else if (i >= BTNNUM && i < BTNNUM + 4)    // 4个特殊按钮
        {
            if ('0' == normalInputStatus(i))  // 按下时为低电平
            {
                if (0 == count[i-BTNNUM])
                {
                    emit sigSpecialBtnPressed(i+1);
                    setButtonStatus(i, LED_ON);
                }
                count[i-BTNNUM] = 1;
            }
            else if ('1' == normalInputStatus(i))
            {
                count[i-BTNNUM] = 0;
            }
        }
        else if (i >= DAILSWITCH)  //  8个拨码开关
        {
            getDialSwitchValue();
        }
    }
//    }
}

void Gpios::run()
{
    startCheckBtnStatus();
}

void Gpios::setLedLightFlag(int index, int flag)
{
    qDebug("%s index = %d, flag = %d", __FUNCTION__, index, flag);
    ledLightFlag[index] = flag;
}

void Gpios::setRunThreadFlag(bool runflag)
{
    runThreadFlag = runflag;
}

quint8 Gpios::getDialSwitchValue()
{
    int fd = -1;
    char ch = '2';
    quint8 tmpValue = 0;
    for (int i = DAILSWITCH; i < DAILSWITCH + 8; ++i)
    {
        fd = open(Intput[i], O_RDONLY|O_NONBLOCK);
        if (fd < 0)
        {
            printf("%s fd < 0 open error!", __FUNCTION__);
        }
        read(fd, &ch, 1);
        if ('0' == ch)
        {
            tmpValue |= (1<<(i - DAILSWITCH));
        }
        else if ('1' == ch)
        {
            tmpValue &= ~(1<<(i - DAILSWITCH));
        }
        close(fd);
    }
    if (mDipValue != tmpValue)
    {
        mDipValue = tmpValue;
        emit sigDailChanged(mDipValue);
    }
    return mDipValue;
}

void Gpios::onSigBtnPressed(quint8 btnindex)
{
    qDebug()<<__FUNCTION__<<"btnindex = "<<btnindex;
}

void Gpios::onSigDailChanged(quint8 pNum)
{
    qDebug()<<__FUNCTION__<<"onSigDailChangedpNum = "<<pNum;
}

