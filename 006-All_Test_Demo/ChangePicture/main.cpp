#include "widget.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGraphicsScene tmpScene;
    QGraphicsView tmpView;

    int angle = -90;
    Widget w;

    tmpView.setScene(&tmpScene);
    tmpScene.addWidget(&w);

    tmpView.show();

    tmpView.rotate(angle);
    tmpView.setGeometry(-5, -5, 1290, 810);
    return a.exec();
}
