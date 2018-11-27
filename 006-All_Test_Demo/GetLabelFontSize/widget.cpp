#include "widget.h"
#include "ui_widget.h"
#include <QFont>
#include <QDebug>
#include <QFontMetrics>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    int sysLength = -1;
    QFont ft;                    // 设置字号

    ui->label->setText("abcd efg");

    ft.setPointSize(12);
    QFontMetrics fm(ft);

    QRect rec = fm.boundingRect(ui->label->text());
    sysLength = rec.width();//这个就获得了字符串所占的像素宽度
    ui->label->setGeometry(20, 20, rec.width(),rec.height());
    qDebug()<<__PRETTY_FUNCTION__<<"Lines = "<<__LINE__<<"sysLength = "<<sysLength;
}

void Widget::init2()
{
    int sysLength = -1;
    QFont ft;

    ft.setPointSize(20);
    QFontMetrics fm1(ft);
    ui->label_2->setFont(ft);

    QRect rec1 = fm1.boundingRect(ui->label_2->text());
    sysLength = rec1.width();//这个就获得了字符串所占的像素宽度
    ui->label_2->setGeometry(20, 20, rec1.width(),rec1.height());

    qDebug()<<__PRETTY_FUNCTION__<<"Lines = "<<__LINE__<<"sysLength = "<<sysLength;

}
