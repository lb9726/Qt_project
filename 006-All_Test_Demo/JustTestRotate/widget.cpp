#include "widget.h"
#include "ui_widget.h"
#include <QCoreApplication>
#include <QPixmap>
#include <QDateTime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    runpath(QCoreApplication::applicationDirPath()),
    floorsNumber(10)
{
    ui->setupUi(this);
    init();
    currentDirection = Horizontal;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    qDebug()<<"runpath = "<<runpath;
    QPixmap tmp1(runpath + "/Theme/sodimas/arrow/1.png");
    ui->label_Arrow->setPixmap(tmp1);

    QPixmap tmp2(runpath + "/Theme/sodimas/floor/T50.png");
    ui->label_floor->setPixmap(tmp2);

    ui->label_time->setText(getTimeFormat(false));
    ui->label_Destination->setText("Destionation " + getFloor());

    connect(&mUpdateTimeAndFloor, &QTimer::timeout, this, &Widget::updateTimeAndFloor);
    connect(&mChangeLayout, &QTimer::timeout, this, &Widget::setMyDirection);

    mUpdateTimeAndFloor.start(1000);
    mChangeLayout.start(3000);
}

void Widget::setMyDirection()
{
    changeLayouts(mtmpviews);
}

void Widget::updateTimeAndFloor()
{
    ui->label_time->setText(getTimeFormat(false));
    ui->label_Destination->setText("Destionation " + getFloor());
}

void Widget::changeLayouts(QGraphicsView *view)
{
    static int flag = 0;
    ++flag;
    if (NULL != view)
    {
        if (flag <= 4)
        {
            qDebug()<<__FUNCTION__<<" flag = " << flag;

            if (1 == flag)
            {
                view->rotate(90);
                currentDirection = Vertical;
            }
            else if (2 == flag)
            {
                 view->rotate(90);
                 currentDirection = Horizontal;
            }
            else if (3 == flag)
            {
                view->rotate(90);
                currentDirection = Vertical;
            }
            else if (4 == flag)
            {
                view->rotate(90);
                currentDirection = Horizontal;
            }

            if (currentDirection == Horizontal)
            {
                view->resize(480, 272);
            }
            else if(currentDirection == Vertical)
            {
                view->resize(272, 480);
            }
        }
        else
        {
            flag = 0;
        }
    }
}

QString Widget::getTimeFormat(bool is12)
{
    QString timelocal = "";
    timelocal.clear();
    if (is12)
    {
        timelocal = QDateTime::currentDateTime().toString("hh:mm:ss a");
        timelocal =  timelocal.split(" ").at(0);
    }
    else
    {
        timelocal = QDateTime::currentDateTime().toString("HH:mm:ss");
    }
//    qDebug()<< "timelocal = "<< timelocal;
    return "Lvl-00  " + timelocal;
}

QString Widget::getFloor()
{
    static int floors = 0;
    ++floors;
    if (floors > floorsNumber)
    {
        floors = 0;
    }
    QString s = QString("%1").arg(floors);
//    qDebug()<<__func__<<" s = "<< s;
    return s;
}

void Widget::setFloorNumber(int num)
{
    floorsNumber = num;
//    qDebug()<<__FUNCTION__<<" floorsNumber " << floorsNumber;
}

void Widget::setViews(QGraphicsView *view)
{
    mtmpviews = view;
}
