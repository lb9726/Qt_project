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
    return light * 37;
}

int BacklightCtrl::countLightLevel(qreal brightness)
{
    int level = 0;
    if (brightness == 0.0)
    {
        level = 0;
    }
    else if (brightness > 0.0 && brightness < 36.4)
    {
        level = 1;
    }
    else if (brightness > 36.4 && brightness < 36.4*2) // 255.0/7~~36.4
    {
        level = 2;
    }
    else if (brightness > 36.4*2 && brightness < 36.4*3) // 255.0/7~~36.4
    {
        level = 3;
    }
    else if (brightness > 36.4*3 && brightness < 36.4*4) // 255.0/7~~36.4
    {
        level = 4;
    }
    else if (brightness > 36.4*4 && brightness < 36.4*5) // 255.0/7~~36.4
    {
        level = 5;
    }
    else if (brightness > 36.4*5 && brightness < 36.4*6) // 255.0/7~~36.4
    {
        level = 6;
    }
    else
    {
        level = 7;
    }
    IDE_TRACE_INT(level);
    return level;
}

//echo 4.6 > /sys/class/backlight/backlight/brightness
bool BacklightCtrl::setBacklightValue(int brightness)
{

    if (brightness <= 0)
    {
        brightness = 0.0;
    }
    else if (brightness > 0 && brightness < 255)
    {
        brightness = brightness * 1.0;
    }
    else if(brightness > 255)
    {
        brightness = 255.0;
    }
    IDE_TRACE_FLOAT(brightness);
    QByteArray ba;
    ba.clear();
    // 最大为7级，最小为0级
    int temp = countLightLevel(brightness);
    IDE_TRACE_INT(temp);
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
