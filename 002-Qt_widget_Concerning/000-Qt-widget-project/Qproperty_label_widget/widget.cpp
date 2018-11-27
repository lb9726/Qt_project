#include "widget.h"
#include "ui_widget.h"
#include <QPropertyAnimation>
#include <QLabel>
#include <QString>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize(400, 400);

    GeometryAnimation();
    PosAnimation();
    OpacityAnimation();
}

void Widget::GeometryAnimation()
{
    QLabel *mylabel = new QLabel(this);
    mylabel->setText("this is example geometry!");
    QPropertyAnimation *panimation = new QPropertyAnimation(mylabel, "geometry");
    panimation->setDuration(8000);
    panimation->setStartValue(QRect(0, 0, 160, 30));
    panimation->setEndValue(QRect(250, 250, 160, 30));
    panimation->setLoopCount(-1);
    panimation->start();
}

void Widget::PosAnimation()
{
    QLabel *mylabel = new QLabel(this);
    QString s = "this is example Position!";
    mylabel->setText(s);
    QPropertyAnimation *panimation = new QPropertyAnimation(mylabel, "pos");
    panimation->setDuration(20000);
    panimation->setStartValue(QPoint(this->width() - this->fontMetrics().width(s), 0));
    panimation->setEndValue(QPoint(0, 0));
    panimation->setLoopCount(-1);
    panimation->start();
}

void Widget::OpacityAnimation()
{
    QLabel *mylabel = new QLabel(this);
    mylabel->setText("this is example Opacity!");
    QPropertyAnimation *panimation = new QPropertyAnimation(mylabel, "windowOpacity");
    panimation->setDuration(4000);
    panimation->setKeyValueAt(0, 1);
    panimation->setKeyValueAt(0.5, 0);
    panimation->setKeyValueAt(1, 1);
    panimation->setEasingCurve(QEasingCurve::InCirc);
    panimation->setLoopCount(-1);
    panimation->start();
}

Widget::~Widget()
{
    delete ui;
}
