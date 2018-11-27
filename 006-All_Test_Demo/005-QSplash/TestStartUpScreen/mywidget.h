#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <unistd.h>
#include <QTime>

#include <QCoreApplication>
#include <QTime>

namespace Ui {
class MyWidget;
}

inline void QtSleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
#if defined(Q_OS_WIN)
        QEventLoop eventloop;
        QTimer::singleShot(msec, &eventloop, SLOT(quit()));
        eventloop.exec();
#else
        usleep(1000);//sleep和usleep都已经obsolete，建议使用nanosleep代替
#endif
    }
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();

private:
    Ui::MyWidget *ui;
};

#endif // MYWIDGET_H
