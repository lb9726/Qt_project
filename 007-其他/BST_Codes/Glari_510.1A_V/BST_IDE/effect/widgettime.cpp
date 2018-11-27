#include "widgettime.h"
#include "effectbase.h"

WidgetTime::WidgetTime(QWidget *parent) :
    WidgetBase(parent)
{
}

WidgetTime::~WidgetTime()
{

}

void WidgetTime::InitTime(CLOCKE_ELEMENT pIndex, WidgetBase *pClockWidget)
{
    m_ClockWidget.insert(pIndex, pClockWidget);
}

void WidgetTime::InitRC(QString pRcPath)
{
    WidgetBase::InitRC(pRcPath);
    QList<CLOCKE_ELEMENT> tmpList = m_ClockWidget.keys();
    CLOCKE_ELEMENT tmpIndex;
    foreach(tmpIndex, tmpList)
    {
        WidgetBase *tmpWidget = m_ClockWidget.value(tmpIndex);
        if(tmpWidget)
        {
            tmpWidget->m_FileList = m_FileList;
            tmpWidget->m_ImgList = m_ImgList;
        }
    }
}

void WidgetTime::slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara)
{
    switch(pOperate)
    {
        case OPERATE_CHANGE:
        {
            QDateTime tmpDateTime = pPara.toDateTime();
            CLOCK_INFO tmpClock = tmpDateTime;
            if(tmpClock.mYear != m_SavedClock.mYear)
            {
                quint32 tmpNewYear2 = tmpClock.mYear%100/10;
                quint32 tmpOldYear2 = m_SavedClock.mYear%100/10;
                if(tmpNewYear2 != tmpOldYear2)
                {
                    WidgetBase *tmpWidget = m_ClockWidget.value(Element_Year2);
                    if(tmpWidget)
                        tmpWidget->slot_ExecOperate(OPERATE_CHANGE, tmpNewYear2);
                }
                quint32 tmpNewYear3 = tmpClock.mYear%10;
                quint32 tmpOldYear3 = m_SavedClock.mYear%10;
                if(tmpNewYear3 != tmpOldYear3)
                {
                    WidgetBase *tmpWidget = m_ClockWidget.value(Element_Year3);
                    if(tmpWidget)
                        tmpWidget->slot_ExecOperate(OPERATE_CHANGE, tmpNewYear3);
                }
            }
            if(tmpClock.mMonth != m_SavedClock.mMonth)
            {
                quint32 tmpNewMonth0 = tmpClock.mMonth/10;
                quint32 tmpOldMonth0 = m_SavedClock.mMonth/10;
                if(tmpNewMonth0 != tmpOldMonth0)
                {
                    WidgetBase *tmpWidget = m_ClockWidget.value(Element_Month0);
                    if(tmpWidget)
                        tmpWidget->slot_ExecOperate(OPERATE_CHANGE, tmpNewMonth0);
                }
                quint32 tmpNewMonth1 = tmpClock.mMonth%10;
                quint32 tmpOldMonth1 = m_SavedClock.mMonth%10;
                if(tmpNewMonth1 != tmpOldMonth1)
                {
                    WidgetBase *tmpWidget = m_ClockWidget.value(Element_Month1);
                    if(tmpWidget)
                        tmpWidget->slot_ExecOperate(OPERATE_CHANGE, tmpNewMonth1);
                }
            }
            if(tmpClock.mDay != m_SavedClock.mDay)
            {
                quint32 tmpNewDay0 = tmpClock.mDay/10;
                quint32 tmpOldDay0 = m_SavedClock.mDay/10;
                if(tmpNewDay0 != tmpOldDay0)
                {
                    WidgetBase *tmpWidget = m_ClockWidget.value(Element_Day0);
                    if(tmpWidget)
                        tmpWidget->slot_ExecOperate(OPERATE_CHANGE, tmpNewDay0);
                }
                quint32 tmpNewDay1 = tmpClock.mDay%10;
                quint32 tmpOldDay1 = m_SavedClock.mDay%10;
                if(tmpNewDay1 != tmpOldDay1)
                {
                    WidgetBase *tmpWidget = m_ClockWidget.value(Element_Day1);
                    if(tmpWidget)
                        tmpWidget->slot_ExecOperate(OPERATE_CHANGE, tmpNewDay1);
                }
            }
            if(tmpClock.mWeekDay != m_SavedClock.mWeekDay)
            {
                WidgetBase *tmpWidget = m_ClockWidget.value(Element_Week);
                if(tmpWidget)
                    tmpWidget->slot_ExecOperate(OPERATE_CHANGE, 10 + tmpClock.mWeekDay/*1~7*/ - 1);
            }
            if(tmpClock.mHour != m_SavedClock.mHour)
            {
                quint32 tmpNewHour0 = tmpClock.mHour/10;
                quint32 tmpOldHour0 = m_SavedClock.mHour/10;
                if(tmpNewHour0 != tmpOldHour0)
                {
                    WidgetBase *tmpWidget = m_ClockWidget.value(Element_Hour0);
                    if(tmpWidget)
                        tmpWidget->slot_ExecOperate(OPERATE_CHANGE, tmpNewHour0);
                }
                quint32 tmpNewHour1 = tmpClock.mHour%10;
                quint32 tmpOldHour1 = m_SavedClock.mHour%10;
                if(tmpNewHour1 != tmpOldHour1)
                {
                    WidgetBase *tmpWidget = m_ClockWidget.value(Element_Hour1);
                    if(tmpWidget)
                        tmpWidget->slot_ExecOperate(OPERATE_CHANGE, tmpNewHour1);
                }
            }
            if(tmpClock.mMinute != m_SavedClock.mMinute)
            {
                quint32 tmpNewMinute0 = tmpClock.mMinute/10;
                quint32 tmpOldMinute0 = m_SavedClock.mMinute/10;
                if(tmpNewMinute0 != tmpOldMinute0)
                {
                    WidgetBase *tmpWidget = m_ClockWidget.value(Element_Minute0);
                    if(tmpWidget)
                        tmpWidget->slot_ExecOperate(OPERATE_CHANGE, tmpNewMinute0);
                }
                quint32 tmpNewMinute1 = tmpClock.mMinute%10;
                quint32 tmpOldMinute1 = m_SavedClock.mMinute%10;
                if(tmpNewMinute1 != tmpOldMinute1)
                {
                    WidgetBase *tmpWidget = m_ClockWidget.value(Element_Minute1);
                    if(tmpWidget)
                        tmpWidget->slot_ExecOperate(OPERATE_CHANGE, tmpNewMinute1);
                }
            }
            if(tmpClock.mSecond != m_SavedClock.mSecond)
            {
                quint32 tmpNewSecond0 = tmpClock.mSecond/10;
                quint32 tmpOldSecond0 = m_SavedClock.mSecond/10;
                if(tmpNewSecond0 != tmpOldSecond0)
                {
                    WidgetBase *tmpWidget = m_ClockWidget.value(Element_Second0);
                    if(tmpWidget)
                        tmpWidget->slot_ExecOperate(OPERATE_CHANGE, tmpNewSecond0);
                }
                quint32 tmpNewSecond1 = tmpClock.mSecond%10;
                quint32 tmpOldSecond1 = m_SavedClock.mSecond%10;
                if(tmpNewSecond1 != tmpOldSecond1)
                {
                    WidgetBase *tmpWidget = m_ClockWidget.value(Element_Second1);
                    if(tmpWidget)
                        tmpWidget->slot_ExecOperate(OPERATE_CHANGE, tmpNewSecond1);
                }
            }
            m_SavedClock = tmpClock;
            break;
        }
        default:
            break;
    }
}
