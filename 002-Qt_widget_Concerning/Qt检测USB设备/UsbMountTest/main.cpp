#include "widget.h"
#include <QApplication>
#include "usbmanager.h"
#include "haldevice.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    haldevice *hd = new haldevice;

    usbmanager *um = new usbmanager;
    Widget w;
    w.show();

    return a.exec();
}
