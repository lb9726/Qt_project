#include "mainwindow.h"
#include <QApplication>
#include <QPalette>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow m_pWidget;
    m_pWidget.resize(300, 300);
    QPalette pal(m_pWidget.palette());

    //设置背景黑色
    pal.setColor(QPalette::Background, Qt::red);
    m_pWidget.setAutoFillBackground(true);
    m_pWidget.setPalette(pal);

    m_pWidget.show();
    return a.exec();
}
