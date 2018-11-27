#include "shapewidget.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QBitmap>

ShapeWidget::ShapeWidget(QWidget *parent)//构造函数的时候显示第一幅图片，并打开定时器
    : QWidget(parent)
{
    pix=new QPixmap();
    pix->load(":/images/p1.png",0,Qt::AvoidDither|Qt::ThresholdDither|Qt::ThresholdAlphaDither);
    resize(pix->size());
    setMask(QBitmap(pix->mask()));

    count=1;
    timer=new QTimer(this);
    timer->start(500);
    connect(timer,SIGNAL(timeout()),this,SLOT(changePicture()));//定时器溢出关联changePicture()函数

}

ShapeWidget::~ShapeWidget()
{

}

void ShapeWidget::changePicture()//使用switch语句根据情况选择不同的图片
{
    QString picName;
    count=count+1;
    if(count>5) count=1;
    switch(count)
    {
    case 1:picName=":/images/p1.png";break;
    case 2:picName=":/images/p2.png";break;
    case 3:picName=":/images/p3.png";break;
    case 4:picName=":/images/p4.png";break;
    case 5:picName=":/images/p5.png";break;
    default:break;
    }
    pix->load(picName,0,Qt::AvoidDither|Qt::ThresholdDither|Qt::ThresholdAlphaDither);
    resize(pix->size());
    setMask(QBitmap(pix->mask()));//setMask()函数，图片之外透明部分都不显示
    update();
}

void ShapeWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        dragPosition =event->globalPos()-frameGeometry().topLeft();
        event->accept();
    }
    if(event->button()==Qt::RightButton)
    {
       close();
    }
}

void ShapeWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()&Qt::LeftButton)
    {
        move(event->globalPos()-dragPosition);
        event->accept();
    }
}

void ShapeWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,*pix);
}

