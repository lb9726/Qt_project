#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFile>
#include "ui/cmysplashscreen.h"
#include "common/define.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsScene tmpScene;
    QGraphicsView tmpView;
    Uiloader tmploader;
    int angle = 180;
    CMySplashScreen *splash = new CMySplashScreen(3500);
    tmploader.mView = &tmpView;

    QString tmpAppStr = qApp->applicationDirPath();
    tmpAppStr.replace("\\", "/");
    if(!tmpAppStr.endsWith("/"))
    {
        tmpAppStr.append("/");
    }
    tmpAppStr.replace("//", "/");

    tmpView.setScene(&tmpScene);
    tmpView.showFullScreen();

    splash->setDirection(1);
    splash->init();

    tmpScene.addWidget(splash);
    tmploader.mView->setGeometry(0, 0, 480, 272);
    tmpView.rotate(angle);

    tmploader.mView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tmploader.mView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tmploader.mView->setGeometry(-5, -5, 480 + 10, 272 + 10);

    splash->show();
    QtSleep(3500);

    QWidget* tmpWidget = tmploader.Init(tmpAppStr + "Theme/sodimas/");
    tmpScene.addWidget(tmpWidget);
    tmploader.createMessagebar();
    tmploader.createSaveScreen();

    tmpView.rotate(0 - angle);

    splash->finish(tmpWidget);
    splash->deleteLater();
    splash = 0;

    return a.exec();
}


