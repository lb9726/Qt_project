#include "showscreen.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
    qDebug()<<"main主线程ID为："<<QThread::currentThreadId();
    ShowScreen s;
    return a.exec();
}
