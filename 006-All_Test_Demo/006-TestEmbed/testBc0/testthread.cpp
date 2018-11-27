#include "testthread.h"

TestThread::TestThread(QThread *parent) : QThread(parent)
{
    counts = 0;
}

void TestThread::init()
{
    while(1)
    {
        counts++;
        if (counts >= 30000)
        {
//            qDebug()<<"counts = "<<counts;
            counts = 0;
        }
    }
}

void TestThread::run()
{
    init();
}

TestThread2::TestThread2(QThread *parent) : QThread(parent)
{
    counts = 0;
}

void TestThread2::init()
{
    while(1)
    {
        counts++;
        if (counts >= 40000)
        {
//            qDebug()<<"counts = "<<counts;
            counts = 0;
        }
    }
}

void TestThread2::run()
{
    init();
}

TestThread3::TestThread3(QThread *parent) : QThread(parent)
{
    counts = 0;
}

void TestThread3::init()
{
    while(1)
    {
        counts++;
        if (counts >= 50000)
        {
//            qDebug()<<"counts = "<<counts;
            counts = 0;
        }
    }
}

void TestThread3::run()
{
    init();
}



TestThread4::TestThread4(QThread *parent) : QThread(parent)
{
    counts = 0;
}

void TestThread4::init()
{
    while(1)
    {
        counts++;
        if (counts >= 60000)
        {
//            qDebug()<<"counts = "<<counts;
            counts = 0;
        }
    }
}

void TestThread4::run()
{
    init();
}


TestThread5::TestThread5(QThread *parent) : QThread(parent)
{
    counts = 0;
}

void TestThread5::init()
{
    while(1)
    {
        counts++;
        if (counts >= 70000)
        {
//            qDebug()<<"counts = "<<counts;
            counts = 0;
        }
    }
}

void TestThread5::run()
{
    init();
}
