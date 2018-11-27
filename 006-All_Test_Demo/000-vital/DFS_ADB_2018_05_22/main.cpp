#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGraphicsView tmpView;
    QGraphicsScene tmpScene;
    MainWindow w;
    tmpScene.addWidget(&w);
    tmpView.setScene(&tmpScene);
    tmpView.rotate(-90);

#ifdef WINDOWS
    tmpView.showMaximized();
#else
    tmpView.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    tmpView.setDragMode(QGraphicsView::NoDrag);
    tmpView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tmpView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);    
    tmpView.setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    tmpView.setBackgroundBrush(QBrush(Qt::black));
    tmpView.setGeometry(-5, -5, 1290, 810);
    tmpView.show();
#endif
    return a.exec();
}
