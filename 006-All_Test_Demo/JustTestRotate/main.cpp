#include "widget.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget tmpWidget;
    QGraphicsScene tmpScene;
    QGraphicsView tmpView;
    tmpScene.addWidget(&tmpWidget);

    tmpWidget.setViews(&tmpView);
    tmpView.setScene(&tmpScene);
    tmpView.show();

    return a.exec();
}
