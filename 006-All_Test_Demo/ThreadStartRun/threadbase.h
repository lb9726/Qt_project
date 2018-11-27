#ifndef THREADBASE_H
#define THREADBASE_H

#include <QObject>

class ThreadBase : public QObject
{
    Q_OBJECT
public:
    explicit ThreadBase(QObject *parent = 0);
    void start();

signals:

public slots:

protected:
    virtual void run();
};

#endif // THREADBASE_H
