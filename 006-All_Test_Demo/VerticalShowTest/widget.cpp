#include "widget.h"
#include "ui_widget.h"
#include <QCoreApplication>
#include <QPixmap>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    runPath(QCoreApplication::applicationDirPath())
{
    ui->setupUi(this);
    initScence();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initScence()
{
    qDebug()<<"runPath = "<<runPath;
    QPixmap tmp1(runPath + "/Theme/sodimas/arrow/1.png");
    QPixmap tmp2(runPath + "/Theme/sodimas/floor/T50.png");
    ui->label_arrow->setPixmap(tmp1);
    ui->label_floor->setPixmap(tmp2);
    ui->label_time->setText("20/01/18 11:21");
    ui->label_destination->setText("Destination: --");
}
