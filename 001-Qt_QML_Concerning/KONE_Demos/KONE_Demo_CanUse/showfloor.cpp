#include "showfloor.h"
#include <QtQml>
#include <QObject>
ShowFloor::ShowFloor(QObject *parent) : QObject(parent)
{
    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(SetFloorNumber()));
    t->start(1000);
}

void ShowFloor::SetFloorNumber()
{
    qDebug("%s", __PRETTY_FUNCTION__);
    static int i = 1;
    static int j = 9;
    static bool Upflag = true;
    if (Upflag)
    {
        if (i < 9)
        {
//            sendShowPicture(true);
            sendSignalFloor(i);
            i ++;
        }
        else
        {
            Upflag = false;
            i = 1;
        }
        qDebug("i = %d", i);
    }
    else
    {
        if (j > 1)
        {
//            sendShowPicture(false);
            sendSignalFloor(j);
            j--;
        }
        else
        {
            Upflag = true;
            j = 9;
        }
        qDebug("j = %d", j);
    }
}
