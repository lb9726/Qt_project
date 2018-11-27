#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QObject>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->label_floor->setTextFormat(Qt::RichText);
    ui->label_floor->setText("1");
//    QTimer* tim = new QTimer(parent);
//    connect(tim, SIGNAL(timeout()), this, SLOT(sett()));
//    tim->start(3000);
    pro = new Protocoldeal;
    connect(pro, SIGNAL(AcceptDataFormBottom(QString)), this ,SLOT(setstring(QString)));
    pro->start();
}

Widget::~Widget()
{
    delete pro;
    delete ui;
}

void Widget::sett()
{
    ui->label_floor->setText("15");
}

void Widget::setstring(QString str)
{

    ui->label_floor->setText(str);
}

void Widget::DealData(char str[])
{

}



