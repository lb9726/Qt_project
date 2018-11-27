#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QResource>
#include <QFile>
#include "bootscreen.h"
#include "cmysplashscreen.h"
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

    CMySplashScreen *splash = new CMySplashScreen(3500);
//    splash->setDisabled(true); //禁用用户的输入事件响应
    splash->show();

#if 1
    int delayTime = 10;
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < (delayTime * 1000))
    {
         a.processEvents();
    }
#endif
    tmpView.setScene(&tmpScene);
    tmpView.showMaximized();

    Uiloader tmploader;
    tmploader.mView = &tmpView;
    QWidget* tmpWidget = tmploader.Init(tmpAppStr + "Theme/sodimas/");
    splash->finish(tmpWidget);
    tmpScene.addWidget(tmpWidget);

    return a.exec();
}
