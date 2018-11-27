#include "desktopui.h"
#include <QCoreApplication>
#include <QPainter>
#include <QDebug>

DesktopUi::DesktopUi(QWidget *parent) : QWidget(parent)
  , mRunPath(QCoreApplication::applicationDirPath())
{
    init();
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
//    update();
}
