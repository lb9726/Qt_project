#include "mainwindow.h"
#include <QApplication>
//#include "bc0test1.h"
//#include "bcbutton.h"
#include "buttons.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Bc0Test1 bt;
    MainWindow w;
//    w.show();
//    BCButtons bbn;
//    ButtonS bts;
//    bts.start();
    int ret = a.exec();
    return ret;
}
