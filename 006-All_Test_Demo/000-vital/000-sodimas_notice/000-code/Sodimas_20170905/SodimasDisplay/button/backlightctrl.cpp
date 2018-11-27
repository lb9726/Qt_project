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

bool BacklightCtrl::setBacklightValue(quint8 brightness){
    //echo 4.6 > /sys/class/backlight/backlight/brightness
    if(brightness >100.0){
        brightness = 100.0;
    }
    IDE_TRACE_FLOAT(brightness);
    qreal level = (brightness/100.0);
    IDE_TRACE_FLOAT(level);
    QByteArray ba;
    ba.append(QString("%1").arg(qFloor(level*BACKLIGHT_LEVEL)));
    qDebug()<<"brightness "<<ba;
    int fd = open("/sys/class/backlight/backlight/brightness",O_WRONLY);
    if(fd == -1)
    {
        IDE_TRACE_STR("Open backlight node failed.");
        return false;
    }
    else
    {
        int ret = write(fd,ba.data(),ba.count());
        if(ret == ba.count()){
            ///IDE_TRACE();
            close(fd);
            return true;
        }else{
            IDE_TRACE();
            close(fd);
            return false;
        }

    }
}
