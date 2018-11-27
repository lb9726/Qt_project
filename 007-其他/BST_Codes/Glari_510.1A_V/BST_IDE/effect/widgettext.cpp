#include "widgettext.h"

WidgetText::WidgetText(QWidget *parent) :
    WidgetBase(parent)
{
}

WidgetText::~WidgetText()
{
}

void WidgetText::ShowText(int pIndex, QString pText, QColor pColor, QFont pFont)
{
    EffectBase *tmpEffect = m_EffectRenders.value(OPERATE_CHANGE);
    if(tmpEffect && tmpEffect->m_EffectAnimate)
    {
        QRect tmpRect;

        DIRECTION tmpDirection = tmpEffect->m_EffectAnimate->mDirection;
        if(tmpDirection == Horizontal)
        {
            int tmpPixH = height();
            int fontsize=6;
            while(fontsize < 200) //>@确保fontsize最大为200，防止死循环
            {
                int newfontsize;
                newfontsize = fontsize+1;
                pFont.setPointSize(newfontsize);
                QFontMetricsF fm(pFont);
                qreal pixelsH = fm.height();
                if(pixelsH > tmpPixH)
                {
                    pFont.setPointSize(fontsize);
                    break;
                }
                fontsize = newfontsize;
            }
            QFontMetricsF fm(pFont);
            qreal tmpPixW = fm.width(pText);
            tmpRect = QRect(0, 0, tmpPixW, tmpPixH);
        }

        QImage tmpImage(tmpRect.width(), tmpRect.height(), QImage::Format_ARGB32_Premultiplied);
        if(tmpImage.isNull())
            return;
#ifndef N329
        tmpImage.fill(Qt::transparent); //>@创建透明图层
#endif
        QPainter p(&tmpImage);
        p.save();
        p.setFont(pFont);
        p.setPen(pColor);
        p.drawText(tmpRect, Qt::AlignLeft | Qt::AlignVCenter, pText);
        p.end();
        load(pIndex, QPixmap::fromImage(tmpImage));

        slot_ExecOperate(OPERATE_CHANGE, pIndex);
    }
}

void WidgetText::slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara)
{
    switch(pOperate)
    {
        case OPERATE_CHANGE:
        {
            int tmpKey = pPara.toInt();
            if(tmpKey == m_Index)
                return;
            m_Index = tmpKey;
            m_ImgPointer.mCurPointer = GetPixmap(tmpKey);

            m_EffectRender = m_EffectRenders.value(OPERATE_CHANGE);
            StartRender();
            break;
        }
        default:
            break;
    }
}
