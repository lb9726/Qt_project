#include "widget.h"
#include "ui_widget.h"
#include <QPixmap>
#include <QPainter>
#include <QLinearGradient>
#include <QPen>
#include <QGradient>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->pictureLabel->setVisible(true);
    ui->pictureLabel->setPixmap(QPixmap(":/new/prefix1/images/bg1.png"));
    connect(&mTimer, &QTimer::timeout, this, &Widget::changePicture);
    mTimer.setSingleShot(true);
    mTimer.start(2000);
    update();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::changePicture()
{
    static int count = 0;
    ++count;
    if (1 == count)
    {
        ui->pictureLabel->setVisible(false);
        update();
        mTimer.start(4000);
    }
    else if (2 == count)
    {
        ui->pictureLabel->setVisible(true);
        ui->pictureLabel->setPixmap(QPixmap(":/new/prefix1/images/bg2.png"));
        mTimer.start(4000);
        count = 0;
    }
}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    QPainter painter(this);

    // 反走样
    painter.setRenderHint(QPainter::Antialiasing, false);

    // 设置渐变色
    QLinearGradient linear(QPointF(0, 0), QPointF(800, 1280));
    linear.setColorAt(0, Qt::black);
    linear.setColorAt(1, Qt::white);

    // 设置显示模式
    linear.setSpread(QGradient::ReflectSpread);

////     设置画笔颜色、宽度
////  painter.setPen(QPen(QColor(0, 160, 230), 0));

    // 设置画刷填充
    painter.setBrush(linear);
    painter.drawRect(0, 0, 800, 1280);
//    for (int i = 0; i < 64; ++i)
//    {
//        painter.drawRect(0, i*20, 800, 20*i);
//    }
}
