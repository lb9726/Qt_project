#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QResource>
#include <QFile>
#include "ui/cmysplashscreen.h"
#include <QElapsedTimer>
#include "common/define.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsScene tmpScene;
    QGraphicsView tmpView;

    QString tmpAppStr = qApp->applicationDirPath();
    tmpAppStr.replace("\\", "/");
    if(!tmpAppStr.endsWith("/"))
    {
        tmpAppStr.append("/");
    }
    tmpAppStr.replace("//", "/");

    CMySplashScreen splash(3500);

    tmpView.setScene(&tmpScene);
    tmpView.showFullScreen();

    splash.setDirection(1);
    splash.init();

    tmpScene.addWidget(&splash);
    tmpView.rotate(180);

    Uiloader tmploader;
    tmploader.mView = &tmpView;

    tmploader.mView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tmploader.mView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tmploader.mView->setGeometry(-5, -5, 480+10, 272+10);

    splash.show();
    QtSleep(3500);

    QWidget* tmpWidget = tmploader.Init(tmpAppStr + "Theme/sodimas/");

    tmpScene.addWidget(tmpWidget);
    tmploader.createSaveScreen();
    tmploader.createMessagebar();

    tmpView.rotate(-180);

    splash.finish(tmpWidget);

    return a.exec();
}


