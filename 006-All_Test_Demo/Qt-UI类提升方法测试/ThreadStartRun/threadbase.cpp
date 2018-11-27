#include "threadbase.h"
#include <QDebug>

ThreadBase::ThreadBase(QObject *parent) : QObject(parent)
{

}

void ThreadBase::start()
{
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
    run();
}

void ThreadBase::run()
{
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
}

