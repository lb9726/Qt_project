#include "widgetarrow.h"
#include "effectbase.h"

WidgetArrow::WidgetArrow(QWidget *parent) :
    WidgetBase(parent)
{
}

WidgetArrow::~WidgetArrow()
{

}

void WidgetArrow::slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara)
{
    switch(pOperate)
    {
        case OPERATE_CHANGE:
        {
            m_ImgPointer.mLastPointer = m_ImgPointer.mCurPointer;
            ARROWSTATE tmpArrowState = (ARROWSTATE)(pPara.toInt());
            switch(tmpArrowState)
            {
                case ArrowUp:
                case ArrowDown:
                case ArrowUpDown:
                case ArrowUpDownroll:  //>@don't roll
                {
                    m_EffectRender = m_EffectRenders.value(OPERATE_CHANGE);
                    m_ImgPointer.mCurPointer = GetPixmap(tmpArrowState);
                    break;
                }
                case ArrowUproll:
                {
                    m_EffectRender = m_EffectRenders.value(OPERATE_RUN);
                    if(m_EffectRender)
                        m_EffectRender->setOrientation(Negative);
                    m_ImgPointer.mLastPointer = m_ImgPointer.mCurPointer = GetPixmap(ArrowUp);
                    break;
                }
                case ArrowDownroll:
                {
                    m_EffectRender = m_EffectRenders.value(OPERATE_RUN);
                    if(m_EffectRender)
                        m_EffectRender->setOrientation(Positive);
                    m_ImgPointer.mLastPointer = m_ImgPointer.mCurPointer = GetPixmap(ArrowDown);
                    break;
                }
                default:
                {
                    m_ImgPointer.mLastPointer = m_ImgPointer.mCurPointer = QPixmap();
                    break;
                }
            }
            StartRender();
            break;
        }
        default:
            break;
    }
}


