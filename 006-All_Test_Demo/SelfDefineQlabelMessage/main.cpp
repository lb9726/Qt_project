#include "widget.h"
#include <QApplication>
#include <qlabelselfdefine.h>
#include <QPalette>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;
//    w.show();
    QlabelSelfDefine mylabel;
    mylabel.setText("hello\njust have a fun");
    mylabel.setFrameStyle(QFrame::Box);
    mylabel.setMyStyleSheet();
    mylabel.resize(200, 90);
    mylabel.setWindowFlags(Qt::FramelessWindowHint);
    mylabel.setBackgroundRole(QPalette::Midlight);
    mylabel.setWindowOpacity(0.8);
    mylabel.show();
    return a.exec();
}
