#include "mywidget.h"
#include "ui_mywidget.h"
#include <QPainter>
#include <QRect>
#include <QCoreApplication>
#include <QDebug>
#include <QProgressBar>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget),
    mRunPath(QCoreApplication::applicationDirPath())
{
    ui->setupUi(this);
    qDebug()<<__PRETTY_FUNCTION__;
    init();

//    progressBar = new AloneProgessBar(this);
////    progressBar->setGeometry((width() - progressBar->width())/2, height()*3/4,
////                             progressBar->width(), progressBar->height());

//    update();
//    progressBar->setGeometry(20, 20, 180, 100);
//    progressBar->show();

    QProgressBar* mProg = new QProgressBar(this);
    mProg->setGeometry(20, 20, 279, 20);
//    mProg->setStyleSheet("QProgressBar {border:5px solid grey;border-radius:10px;color: rgb(255, 0, 0);text-align:center}\
//                            QProgressBar::chunk {background-color: orange;border-radius: 5px}");
    mProg->setStyleSheet("\
            QProgressBar {border:5px solid grey;border-radius:10px;\
            color: rgb(255, 0, 0);text-align:center}\
            QProgressBar::chunk {background-color: orange;border-radius: 5px}");
    mProg->setValue(88);
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::init()
{
    qDebug()<<__PRETTY_FUNCTION__<<"mRunPath = "<<mRunPath;
    if (!mRunPath.endsWith("/"))
    {
        mRunPath = mRunPath + "/";
    }
    qDebug()<<__PRETTY_FUNCTION__<<"mRunPath = "<<mRunPath;
    mHorizontalPixmap = QPixmap(mRunPath + "0.png");
    mVerticalPixmap = QPixmap(mRunPath + "100.png");
}

void MyWidget::paintEvent(QPaintEvent *event)
{
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
    Q_UNUSED(event);
    QPainter p(this);

    p.drawPixmap(rect(), mHorizontalPixmap);
}
