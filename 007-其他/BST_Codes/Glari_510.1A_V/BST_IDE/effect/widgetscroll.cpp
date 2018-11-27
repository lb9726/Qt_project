#include "widgetscroll.h"

WidgetScroll::WidgetScroll(QWidget *parent) : QScrollArea(parent)
{
    mFlickCharm.activateOn(this);
    connect(&mFlickCharm, SIGNAL(sFinished()), this, SLOT(slot_ScrollFinished()));
}

void WidgetScroll::Init()
{
    QWidget *tmpWidget = widget();
    QList<WidgetBase*> tmpList = tmpWidget->findChildren<WidgetBase*>();
}

void WidgetScroll::slot_ScrollFinished()
{
    int y = verticalScrollBar()->value();
    int height = widget()->width();
    int diff = y % height;
    int index = y / height;
    //verticalScrollBar()->setValue(y - diff);
}
