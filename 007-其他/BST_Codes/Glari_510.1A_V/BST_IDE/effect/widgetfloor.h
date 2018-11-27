#ifndef WIDGETFLOOR_H
#define WIDGETFLOOR_H

#include "widgetbase.h"

class WidgetFloor : public WidgetBase
{
    Q_OBJECT
public:
    WidgetFloor(QWidget *parent=0);
    ~WidgetFloor();

    void InitFloor(WidgetBase *pHundred, WidgetBase *pTen, WidgetBase *pSingle);
    void InitRC(QString pRcPath, bool pInititial = true);

public slots:
    void slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara);

public:
    FLOOR_INFO      m_OldFloor;
    WidgetBase     *m_HundredWidget;
    WidgetBase     *m_TenWidget;
    WidgetBase     *m_SingleWidget;
};

#endif // WIDGETFLOOR_H
