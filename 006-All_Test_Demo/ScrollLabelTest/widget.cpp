#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    mScrollLabel.setParent(this);
    mScrollLabel.setScrollText("hello world!");
    mScrollLabel.setGeometry(20, 20, 200, 40);
    mScrollLabel.setInterval(1000);
}

Widget::~Widget()
{
    delete ui;
}
