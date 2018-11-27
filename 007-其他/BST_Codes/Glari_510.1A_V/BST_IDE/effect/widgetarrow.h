#ifndef WIDGETARROW_H
#define WIDGETARROW_H

#include "widgetbase.h"

class WidgetArrow : public WidgetBase
{
    Q_OBJECT
public:
    WidgetArrow(QWidget *parent=0);
    ~WidgetArrow();

public slots:
    void slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara);
};

#endif // WIDGETARROW_H
