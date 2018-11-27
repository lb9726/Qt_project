#include "arrowitem.h"

ArrowItem::ArrowItem(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    setFlag(ItemHasNoContents,false);
    m_CurFrameNum = 0;
    curPix = QPixmap("T1.png");
    m_FrameCount = 100;
}

void ArrowItem::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *option,
                      QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);


    qreal hh = boundingRect().height();
    qreal hw = boundingRect().width();
    qreal tmpDiff;
    qreal index = m_CurFrameNum;
    qreal y;

    //首先将坐标原点移到当前需要绘制的区域。
    tmpDiff = hh / m_FrameCount;



    painter->save();
    if(0)
    {
        y = (qreal)(tmpDiff*index);
        painter->translate(boundingRect().left(), boundingRect().top()+y);
        DrawPixmap(painter, curPix, boundingRect().size(), QRectF(0,0,hw,hh-y), QRectF(0,0,hw,hh-y));
    }
    else
    {
        y = (qreal)(tmpDiff*index);
        if(y!=0){
            painter->translate(boundingRect().left(), boundingRect().top()-y);
            DrawPixmap(painter, curPix, boundingRect().size(), QRectF(0,y,hw,hh-y), QRectF(0,y,hw,hh-y));
        }
    }
    painter->restore();

    painter->save();
    if(0)
    {
        y = hh - y;
        painter->translate(boundingRect().left(), boundingRect().top()-y);
        DrawPixmap(painter, curPix, boundingRect().size(), QRectF(0,y,hw,hh-y), QRectF(0,y,hw,hh-y));
    }
    else
    {
        y = hh - y;
        if(y != hh){
            painter->translate(boundingRect().left(), boundingRect().top()+y);
            DrawPixmap(painter, curPix, boundingRect().size(), QRectF(0,0,hw,hh-y), QRectF(0,0,hw,hh-y));
        }
    }
    painter->restore();
}


bool ArrowItem::DrawPixmap(QPainter *p, QPixmap pPixmap, QSizeF pSize, QRectF pTargetRect, QRectF pSourceRect)
{
    if(pTargetRect.isValid() == false)
    {
        IDE_TRACE();
        return false;
    }
    if(pSourceRect.isValid() == false)
    {
        IDE_TRACE();
        return false;
    }
    if(pSize.isValid() == false)
    {
        IDE_TRACE();
        return false;
    }
    if(pPixmap.isNull() == false)
    {
        QPixmap tmpPixmap = pPixmap.scaled(pSize.toSize(),Qt::IgnoreAspectRatio);
        if(!tmpPixmap.isNull())
        {
            p->drawPixmap(pTargetRect, tmpPixmap, pSourceRect);
        }
        return true;
    }else{
        IDE_TRACE();
    }
    return false;
}


void ArrowItem::tick(){
    m_CurFrameNum ++;
    m_CurFrameNum = (qreal)((qlonglong)m_CurFrameNum % (qlonglong)m_FrameCount);
    update();
}
