#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    mylabel.setText("hello\njust have a fun");
    mylabel.setFrameStyle(QFrame::Box);
    mylabel.setMyStyleSheet();
    mylabel.resize(200, 90);
    mylabel.setWindowFlags(Qt::FramelessWindowHint);
    mylabel.setWindowOpacity(0.8);
    mylabel.show();
}

Widget::~Widget()
{

}
