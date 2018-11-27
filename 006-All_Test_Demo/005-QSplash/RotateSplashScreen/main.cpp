#include "widget.h"
#include <QApplication>
#include "cmysplashscreen.h"
#include <QGraphicsScene>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CMySplashScreen *splash = new CMySplashScreen(4500);

    QGraphicsScene tmpScene;
    QGraphicsView tmpView;

    DIRECTION tmpAngle;
    tmpAngle = Vertical;

    tmpView.setScene(&tmpScene);
    tmpView.show();

//    tmpView.setGeometry(0, 0, 272, 480);
//    splash->setDirection(Vertical);
    if (tmpAngle == Horizontal)
    {
        tmpView.setGeometry(0, 0, 480, 272);
    }
    else
    {
        tmpView.setGeometry(0, 0, 272, 480);
    }
    splash->setDirection(tmpAngle);
    splash->init();

    tmpScene.addWidget(splash);
    tmpView.rotate(90);
//    tmpView.rotate(180);

    if (tmpAngle == Horizontal)
    {
        tmpView.setGeometry(-5, -5, 480+10, 272+10);
    }
    else
    {
        tmpView.setGeometry(-5, -5, 272+10, 480+10);
    }

    tmpView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tmpView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    splash->show();
    QtSleep(5500);

    Widget w;
    splash->finish(&w);

    splash->deleteLater();
    splash = 0;

    return a.exec();
}
