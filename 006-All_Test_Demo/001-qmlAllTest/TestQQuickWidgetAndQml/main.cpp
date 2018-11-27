#include "widget.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStackedWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGraphicsScene tmpScene;
    QGraphicsView tmpView;

    tmpView.setScene(&tmpScene);
    tmpView.resize(700, 600);
    tmpView.show();
//    tmpView.showFullScreen();
    QStackedWidget qsw;
    qsw.setGeometry(0, 0, 700, 600);
    Widget w;
    qsw.addWidget(w.setQmlIntoWidget());
    tmpScene.addWidget(&qsw);

    return a.exec();
}
