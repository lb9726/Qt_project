#include "widget.h"
#include "ui_widget.h"
#include <QPalette>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    tim = new QTimer(this);
    connect(tim, SIGNAL(timeout()), this, SLOT(SetBackground()));
    tim->start(1200);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::SetBackground()
{
    static int i = 0;
    QPalette pal;
    //设置背景色
    if (i % 3 == 0)
    {
        pal.setColor(QPalette::Background, Qt::red);
    }
    else if (1 == i % 3)
    {
        pal.setColor(QPalette::Background, Qt::yellow);
    }
    else if (2 == i % 3)
    {
        pal.setColor(QPalette::Background, Qt::blue);
    }
    i++;
    if ( i == 3)
        i = 0;
    this->setAutoFillBackground(true);
    this->setPalette(pal);
//    this->show();
}
