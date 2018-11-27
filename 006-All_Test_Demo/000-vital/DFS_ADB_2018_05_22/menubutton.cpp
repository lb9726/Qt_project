#include "menubutton.h"
#include <QPainter>

MenuButton::MenuButton(QWidget *parent) : QWidget(parent)
{
    mFlag = 0;
}

void MenuButton::mousePressEvent(QMouseEvent *event)
{

    mFlag = 1;

    mPixmap = QPixmap(":/SetMenu/rc/SetMenu/setitem.png");

    update();

    emit sSelected();
}

void MenuButton::mouseReleaseEvent(QMouseEvent *event)
{
    mFlag = 0;


    update();
}

void MenuButton::SetPressed(bool pFlag)
{
    if(pFlag)
    {
        mPixmap = QPixmap(":/SetMenu/rc/SetMenu/setitem.png");
    }
    else if(!pFlag)
    {
        mPixmap = QPixmap();
    }
    update();
}

void MenuButton::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.drawPixmap(rect(),mPixmap);
}



