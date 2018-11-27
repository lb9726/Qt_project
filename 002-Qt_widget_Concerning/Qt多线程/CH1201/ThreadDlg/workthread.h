#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>

class WorkThread : public QThread
{
    Q_OBJECT
public:
    WorkThread();
    int id;
protected:
    void run();
};

#endif // WORKTHREAD_H
