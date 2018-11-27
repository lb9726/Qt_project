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

//echo 4.6 > /sys/class/backlight/backlight/brightness
bool BacklightCtrl::setBacklightValue(quint8 brightness)
{
    if(brightness > 100.0)
    {
        brightness = 100.0;
    }
    IDE_TRACE_FLOAT(brightness);
    qreal level = (brightness/100.0);
    IDE_TRACE_FLOAT(level);
    QByteArray ba;
    int temp = (qFloor(level * BACKLIGHT_LEVEL) > 7) ? 7 : (qFloor(level * BACKLIGHT_LEVEL));
    ba.append(QString("%1").arg(temp));
    qDebug()<<" brightness "<<ba;
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
            ///IDE_TRACE();
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
