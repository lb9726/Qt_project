#include <QApplication>
#include "tcpclient.h"

//#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));
    TCPClient w;
    w.show();

    return a.exec();
}
