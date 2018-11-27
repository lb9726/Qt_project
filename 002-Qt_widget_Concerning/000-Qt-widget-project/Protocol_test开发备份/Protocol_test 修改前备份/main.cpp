#include "widget.h"
#include <QApplication>
#include <QDebug>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
    Widget w;
    w.show();
//    w.showFullScreen();
    return a.exec();
}
