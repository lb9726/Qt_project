#include "backlightctrl.h"
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include <fcntl.h>
#include <QtCore/qmath.h>
#include "define.h"

const quint8 BACKLIGHT_LEVEL = 10;

BacklightCtrl::BacklightCtrl(QObject *parent) :
    QObject(parent)
{

}

int BacklightCtrl::getLightGrade()
{
    int fd = open(BRIGHTNESS_CTR, O_RDONLY);
    if (-1 == fd)
    {
        IDE_TRACE_STR(QString("open brightness file is failed"));
        return 70;
    }
    char ch;
    read(fd, &ch, 1);
//    printf("ch = %c\n", ch);
    int light = ch - '0';
    IDE_TRACE_INT(light);
    close(fd);
    return light*10;
}

//echo 4.6 > /sys/class/backlight/backlight/brightness
bool BacklightCtrl::setBacklightValue(int brightness)
{
    if(brightness > 100)
    {
        brightness = 100.0;
    }
    else if (brightness < 0)
    {
        brightness = 0.0;
    }
    IDE_TRACE_FLOAT(brightness);
    qreal level = (brightness/100.0);
    IDE_TRACE_FLOAT(level);
    QByteArray ba;
    // 最大为7级，最小为0级
    int temp = (qFloor(level * BACKLIGHT_LEVEL) > BESTGRADE) ? BESTGRADE : (qFloor(level * BACKLIGHT_LEVEL));
    ba.append(QString("%1").arg(temp));
    qDebug()<<" brightness "<< ba;
    int fd = open(BRIGHTNESS_CTR, O_WRONLY);
    if(-1 == fd)
    {
        IDE_TRACE_STR("Open backlight node failed.");
        return false;
    }
    else
    {
        int ret = write(fd, ba.data(), ba.count());
        if(ret == ba.count())
        {
            close(fd);
            return true;
        }
        else
        {
            IDE_TRACE();
            close(fd);
            return false;
        }
    }
}
