#include "widget.h"
#include "ui_widget.h"
#include <QPoint>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
//    const char a[5] = "libo";
//    const char b[6] = "hello";
    QPoint *qp = new QPoint(20, 20);
    TTipWidget::Instance().SetMesseage("hello", qp);
}

Widget::~Widget()
{
    delete ui;
}
