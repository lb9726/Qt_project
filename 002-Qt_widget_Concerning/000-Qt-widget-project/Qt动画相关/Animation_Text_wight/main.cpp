#include "widget.h"
#include <QApplication>
#include<QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));//解决乱码显示问题
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));//解决乱码显示问题
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));//解决乱码显示问题
    Widget w;
    w.setWindowTitle(QObject::tr("Ticker"));//设置标题
    w.setText(QObject::tr("设置滚动的文字"));//设置滚动的文字

    w.show();

    return a.exec();
}
