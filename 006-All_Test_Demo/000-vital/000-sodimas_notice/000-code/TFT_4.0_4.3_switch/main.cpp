#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QResource>
#include <QFile>

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

    tmpView.setScene(&tmpScene);
    tmpView.showMaximized();

    Uiloader tmploader;
    tmploader.mView = &tmpView;
    QWidget* tmpWidget = tmploader.Init(tmpAppStr + "Theme/sodimas/");

    tmpScene.addWidget(tmpWidget);

    return a.exec();
}
