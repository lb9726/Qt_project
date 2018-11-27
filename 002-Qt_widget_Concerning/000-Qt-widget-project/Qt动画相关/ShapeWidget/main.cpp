#include <QApplication>
#include "shapewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(res);
    ShapeWidget w;
    w.show();
    return a.exec();
}

