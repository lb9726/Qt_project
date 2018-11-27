#include "widget.h"
#include "ui_widget.h"
#include "logcat.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    MYDebug("%s just test for my logcat %d", "libo", 2018);
}

Widget::~Widget()
{
    delete ui;
}
