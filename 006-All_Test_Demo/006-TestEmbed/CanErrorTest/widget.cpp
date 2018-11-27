#include "widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
   qDebug()<<__func__<<" is enter";
}

Widget::~Widget()
{

}
