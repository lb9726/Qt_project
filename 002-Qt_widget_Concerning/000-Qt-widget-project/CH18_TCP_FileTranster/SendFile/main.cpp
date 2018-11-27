#include "sendwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SendWidget w;
    w.show();

    return a.exec();
}
