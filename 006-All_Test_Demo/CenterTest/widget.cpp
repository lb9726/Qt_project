#include "widget.h"
#include "ui_widget.h"
#include <QPixmap>
#include <QCoreApplication>
#include <QDebug>

static int num = 0;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    runPath(QCoreApplication::applicationDirPath())
{
    qDebug()<<"runPath = "<<runPath;
    ui->setupUi(this);
    ui->label_bai->setScaledContents(true);
    ui->label_ten->setScaledContents(true);
    ui->label_ge->setScaledContents(true);
    connect(&mtimer, &QTimer::timeout, this, &Widget::initScreen);
    mtimer.start(1000);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initScreen()
{
    QPixmap tmp1(runPath + "/Theme/sodimas/floor/T48.png");
    QPixmap tmp2(runPath + "/Theme/sodimas/floor/T50.png");
    QPixmap tmp3(runPath + "/Theme/sodimas/floor/T51.png");
    num ++;
    qDebug()<<"num = "<< num;
    if (1 == num)
    {
        ui->label_bai->setPixmap(tmp1);
        ui->label_ten->setPixmap(QPixmap());
        ui->label_ge->setPixmap(QPixmap());
    }
    else if (2 == num)
    {
        ui->label_bai->setPixmap(QPixmap());
        ui->label_ge->setPixmap(tmp1);
        ui->label_ten->setPixmap(tmp2);

    }
    else if (3 == num)
    {
        num = 0;
        ui->label_bai->setPixmap(tmp1);
        ui->label_ten->setPixmap(tmp2);
        ui->label_ge->setPixmap(tmp3);
    }
}
