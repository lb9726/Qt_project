#include "threadson.h"
#include <QDebug>

ThreadSon::ThreadSon(ThreadBase *parent): ThreadBase(parent)
{

}

void ThreadSon::run()
{
    qDebug()<<__PRETTY_FUNCTION__<<"son is call";
}
