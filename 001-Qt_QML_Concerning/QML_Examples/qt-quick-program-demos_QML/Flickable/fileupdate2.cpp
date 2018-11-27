#include "fileupdate2.h"

FileUpdate2::FileUpdate2(QObject *parent) : QObject(parent)
{
    QTimer *tim = new QTimer(this);
    QTimer *t = new QTimer(this);
    QTimer *t1 = new QTimer(this);
    connect(tim, SIGNAL(timeout()), this, SLOT(ChangePercent()));
    connect(t, SIGNAL(timeout()), this, SLOT(StartAnimDown()));
    connect(t1, SIGNAL(timeout()), this, SLOT(StartAnimUp()));
    tim->start(1000);
    t->start(10000);
    t1->start(5000);
}

void FileUpdate2::ChangePercent()
{
    static int i = 0;
    i++;
    sendPercent(i);
    qDebug("%s, i = %d", __PRETTY_FUNCTION__, i);
    if (i == 100)
        i = 0;
}

void FileUpdate2::StartAnimDown()
{
    qDebug("%s", __PRETTY_FUNCTION__);
    sendStartAniDown(true);
}

void FileUpdate2::StartAnimUp()
{
    qDebug("%s", __PRETTY_FUNCTION__);
    sendStartAniUp(true);
}
