#include "mywidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    myWidget w;
    w.setGeometry(100,100,200,120);
    w.show();

    return a.exec();
}
