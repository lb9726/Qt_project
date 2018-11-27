#ifndef THREADSUN_H
#define THREADSUN_H

#include <QObject>
#include "threadbase.h"

class ThreadSon : public ThreadBase
{
    Q_OBJECT
public:
    explicit ThreadSon(ThreadBase *parent = 0);
    virtual void run();
signals:

public slots:
};

#endif // THREADSUN_H
