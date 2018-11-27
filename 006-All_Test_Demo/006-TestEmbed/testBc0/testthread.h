#ifndef TESTTHREAD_H
#define TESTTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>

class TestThread : public QThread
{
    Q_OBJECT
public:
    explicit TestThread(QThread *parent = 0);
    void init();
    void run();
signals:

public slots:
private:
    int counts;
};

class TestThread2 : public QThread
{
    Q_OBJECT
public:
    explicit TestThread2(QThread *parent = 0);
    void init();
    void run();
signals:

public slots:
private:
    int counts;
};

class TestThread3 : public QThread
{
    Q_OBJECT
public:
    explicit TestThread3(QThread *parent = 0);
    void init();
    void run();
signals:

public slots:
private:
    int counts;
};

class TestThread4 : public QThread
{
    Q_OBJECT
public:
    explicit TestThread4(QThread *parent = 0);
    void init();
    void run();
signals:

public slots:
private:
    int counts;
};

class TestThread5 : public QThread
{
    Q_OBJECT
public:
    explicit TestThread5(QThread *parent = 0);
    void init();
    void run();
signals:

public slots:
private:
    int counts;
};
#endif // TESTTHREAD_H
