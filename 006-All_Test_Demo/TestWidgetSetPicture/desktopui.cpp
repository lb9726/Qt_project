#include "desktopui.h"
#include <QCoreApplication>
#include <QPainter>
#include <QDebug>
#include <QPixmap>

DesktopUi::DesktopUi(QWidget *parent) : QWidget(parent)
  , mRunPath(QCoreApplication::applicationDirPath())
{
    qDebug()<<__PRETTY_FUNCTION__;
    init();
    getPointer();
}

void DesktopUi::paintEvent(QPaintEvent *event)
{
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
    Q_UNUSED(event);
    QPainter p(this);

    p.drawPixmap(rect(), mHorizontalPixmap);
}

void DesktopUi::init()
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

void DesktopUi::getPointer()
{
    qDebug()<<__PRETTY_FUNCTION__;
    mArrow = this->findChild<QLabel *>("label_arrow");
    QPixmap arrowPixMap(mRunPath + "2.png");
    if (mArrow)
    {
        qDebug()<<__PRETTY_FUNCTION__;
        mArrow->setScaledContents(true);
        mArrow->setPixmap(arrowPixMap);
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"mArrow is NULL";
    }

    mFloor = this->findChild<QLabel *>("label_floor");
    QPixmap floorPixMap(mRunPath + "T57.png");
    if (mFloor)
    {
        qDebug()<<__PRETTY_FUNCTION__;
        mFloor->setScaledContents(true);
        mFloor->setPixmap(floorPixMap);
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"mFloor is NULL";
    }
}
