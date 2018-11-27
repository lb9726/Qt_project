#include "widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
     quint8 tmp = gpio.getDialSwitchValue();
     qDebug()<<"tmp = "<<tmp;
     gpio.start();
}

Widget::~Widget()
{

}
