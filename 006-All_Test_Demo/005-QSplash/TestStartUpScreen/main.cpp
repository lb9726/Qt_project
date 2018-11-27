#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QDebug>
#include <QElapsedTimer>
#include <QDateTime>
#include "cmysplashscreen.h"
#include "mywidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//#if 1
//    int delayTime = 5;
//    QElapsedTimer timer;
//    timer.start();
//    while(timer.elapsed() < (delayTime * 1000))
//    {
//         app.processEvents();
//    }
//#endif

    CMySplashScreen splash(4500);
//    splash->setDisabled(true); //禁用用户的输入事件响应
    splash.show();
    QtSleep(5500);

    MyWidget widget;
    widget.show();
    splash.finish(&widget);

    return app.exec();
}
