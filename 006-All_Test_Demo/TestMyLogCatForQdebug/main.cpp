#include "widget.h"
#include <QApplication>
#include "logcat.h"

int main(int argc, char *argv[])
{
    MYDebug("%s just test for my logcat %d", "libo", 2018);
    QApplication a(argc, argv);
    Widget w;
    w.show();
    MYDebug("%s just test for my logcat %d", "libo", 2018);
    return a.exec();
}
