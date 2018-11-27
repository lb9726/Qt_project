#include "widget.h"
#include "test.h"
#include "update.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;
//    w.show();
//    test t;
//    t.show();
    update u;
    u.show();
    return a.exec();
}
