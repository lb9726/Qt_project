#ifndef WIDGETTIME_H
#define WIDGETTIME_H

#include "widgetbase.h"

class WidgetTime : public WidgetBase
{
    Q_OBJECT
public:
    WidgetTime(QWidget *parent=0);
    ~WidgetTime();

    void InitTime(CLOCKE_ELEMENT pIndex, WidgetBase *pClockWidget);
    void InitRC(QString pRcPath);

public slots:
    void slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara);

public:
    //>@Digital : 图元(指显示数字的一个个区域)个数为19个：RectYear0~RectSecond1 ---- 0~9 SUN~WES : -
    //>@          图片资源个数为10+7+1+1 = 19
    //>@          0~9 Sun~Sat : -
    //>@Analog : 图元个数为11+3个：RectYear0~RectWeek  时指针 分指针 秒指针
    //>@         图片资源个数为10+7+1+4+1 = 23(1个表针最上面的点)个
    //>@         0~9 Sun~Sat - 时指针 分指针 秒指针 时钟底图
    QHash<CLOCKE_ELEMENT, WidgetBase*>    m_ClockWidget;
    CLOCK_INFO                            m_SavedClock;
};

#endif // WIDGETTIME_H
