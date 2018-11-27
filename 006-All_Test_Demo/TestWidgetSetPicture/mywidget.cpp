#include "mywidget.h"
#include "ui_mywidget.h"
#include <QPainter>
#include <QRect>
#include <QCoreApplication>
#include <QDebug>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget),
    mRunPath(QCoreApplication::applicationDirPath())
{
    ui->setupUi(this);
    qDebug()<<__PRETTY_FUNCTION__;
    mdesktop = ui->widget;
    mdesktop->getPointer();
    qDebug()<<__PRETTY_FUNCTION__<<"mdesktop = "<<mdesktop;
    init();
//    ui->widget->update();
//    update();
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
    mdesktop->update();
}

void MyWidget::paintEvent(QPaintEvent *event)
{
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
    Q_UNUSED(event);
    QPainter p(this);

    p.drawPixmap(rect(), mHorizontalPixmap);
}
