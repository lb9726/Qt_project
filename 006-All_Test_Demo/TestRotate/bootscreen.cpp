#include "bootscreen.h"

#include <QThread>
#include <QPixmap>

BootScreen::BootScreen(QObject *parent) : QObject(parent)
{
    qlabel = new QLabel();
    qlabel->setGeometry(0, 0, 193, 45);
    QPixmap qp("/home/libo/Desktop/logo.png");
    qlabel->setPixmap(qp);
    qlabel->show();
    connect(&qtime, SIGNAL(timeout()),this, SLOT());
    qtime.start(8000);
}

void BootScreen::hideScreen()
{
    qlabel->hide();
}
