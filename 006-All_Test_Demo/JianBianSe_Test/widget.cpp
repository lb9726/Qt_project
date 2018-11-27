#include "widget.h"
#include "ui_widget.h"
#include <QPainter>

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

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QLinearGradient linearGradient(100, 150, 300, 150);
    //从点（100，150）开始到点（300，150）结束，确定一条直线
    linearGradient.setColorAt(0, Qt::red);
    linearGradient.setColorAt(0.2, Qt::black);
    linearGradient.setColorAt(0.4, Qt::yellow);
    linearGradient.setColorAt(0.6, Qt::white);
    linearGradient.setColorAt(0.8, Qt::green);
    linearGradient.setColorAt(1, Qt::blue);
    //将直线开始点设为0，终点设为1，然后分段设置颜色
    painter.setBrush(linearGradient);
    painter.drawRect(100, 100, 200, 100);
    //绘制矩形，线性渐变线正好在矩形的水平中心线上
}
