#ifndef WIDGETSCROLL_H
#define WIDGETSCROLL_H

#include "flickcharm.h"
#include "widgetbase.h"

class WidgetScroll : public QScrollArea
{
    Q_OBJECT
public:
    explicit WidgetScroll(QWidget *parent = 0);

    void Init();

public slots:
    void slot_ScrollFinished();
    
public:
    FlickCharm           mFlickCharm;
};

#endif // WIDGETSCROLL_H
