#include <QApplication>
#include "tcpserver.h"

//#include<QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));
    TCPServer w;
    w.show();

    return a.exec();
}
