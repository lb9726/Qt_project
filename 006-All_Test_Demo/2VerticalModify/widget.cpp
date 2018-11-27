#include "widget.h"
#include "ui_widget.h"
#include <QCoreApplication>
#include <QPixmap>
#include <QDebug>
#include <QPainter>
#include <QTransform>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    runPath(QCoreApplication::applicationDirPath())
{
    ui->setupUi(this);
//    initScence();
      scene.addText("rotated clockwise");
      scene.setSceneRect(0, 0, 300, 300);
      view.setScene(&scene);
      view.translate(-100, -100);
      view.rotate(90); // the text is rendered with a 90 degree clockwise rotation
      view.show();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initScence()
{
    qDebug()<<"runPath = "<<runPath<< "test test";
    leftmatrix.rotate(-90);
    QPixmap tmp1(runPath + "/Theme/sodimas/arrow/1.png");
    ui->label_arrow->setPixmap(tmp1.transformed(leftmatrix, Qt::SmoothTransformation));

    QPixmap tmp2(runPath + "/Theme/sodimas/floor/T50.png");
//    ui->label_arrow->setPixmap(tmp1);
//    ui->label_floor->setPixmap(tmp2);
    ui->label_floor->setPixmap(tmp2.transformed(leftmatrix, Qt::SmoothTransformation));

    ui->label_time->setWordWrap(true);
    ui->label_time->setAlignment(Qt::AlignTop);
    ui->label_time->setText("20/01/18 11:21");

    ui->label_destination->setWordWrap(true);
    ui->label_destination->setAlignment(Qt::AlignTop);
    ui->label_destination->setText("Destination: --");

}

void Widget::paintEvent(QPaintEvent *e)
{
//    QPainter painter(this);
//    QFont font("Courier", 14);
//    painter.setFont(font);
//    QTransform transform;
//    transform.rotate(90.0);//旋转90度
//    painter.setWorldTransform(transform);
//    painter.drawText(0, 0, "Hello world!");

    QPainter painter(this);
    painter.save(); //保存坐标系状态
    painter.translate(0, 272);
    painter.drawLine(0, 0, 50, -50);
    painter.drawText(0, 0, "Hello world!");
    painter.restore(); //恢复以前的坐标系状态
    painter.drawLine(0, 0, 50, 50);

}
