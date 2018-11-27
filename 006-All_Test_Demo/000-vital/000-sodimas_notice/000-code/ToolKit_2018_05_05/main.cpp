#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QResource>
#include <QFile>
#include "ui/cmysplashscreen.h"
#include <QElapsedTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsScene tmpScene;
    QGraphicsView tmpView;

    QString tmpAppStr = qApp->applicationDirPath();
    tmpAppStr.replace("\\","/");
    if(!tmpAppStr.endsWith("/"))
        tmpAppStr.append("/");
    tmpAppStr.replace("//","/");

    CMySplashScreen splash(3500);
//    splash.setDisabled(true); //禁用用户的输入事件响应
    splash.show();

#if 1
    int delayTime = 5;
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < (delayTime * 1000))
    {
         a.processEvents();
    }
#endif

    tmpView.setScene(&tmpScene);
    tmpView.showFullScreen();

    Uiloader tmploader;
    tmploader.mView = &tmpView;
    tmploader.mView->setGeometry(-5, -5, 480+10, 272+10);
    tmploader.mView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tmploader.mView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QWidget* tmpWidget = tmploader.Init(tmpAppStr + "Theme/sodimas/");

    tmpScene.addWidget(tmpWidget);
    tmploader.createSaveScreen();
    tmploader.createMessagebar();
    return a.exec();
}


