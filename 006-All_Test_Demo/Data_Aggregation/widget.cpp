#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    for (int i = 2; i < 100; ++i)
    {
        int tmp = mDataHandle.isPrime(i);
        if (0 == tmp)
        {
            qDebug("i = %d is not Prime", i);
        }
        else if (1 == tmp)
        {
            qDebug("i = %d is  Prime", i);
        }
    }
    int a[64];
    int flag = sizeof(a) / sizeof(a[0]);
    qDebug("flag = %d ", flag);
    mDataHandle.generateRandom(a, flag, 40);
    mDataHandle.powValue(4, 6);
}

Widget::~Widget()
{
    delete ui;
}
