#include "effectbase.h"
#include "widgetbase.h"

EffectBase::EffectBase(QObject *parent) :
    QObject(parent)
{
    m_Valid = false;
    m_EffectAnimate = new EFFECT_ANIMATE;
    connect(&m_Animator, SIGNAL(frameChanged(int)), this, SLOT(updateEffect(int)));
}

EffectBase::~EffectBase()
{
    m_Valid = false;
    m_Animator.stop();
    if(m_EffectAnimate)
        delete m_EffectAnimate;
}

bool EffectBase::isValid()
{
    return m_Valid;
}

QSize EffectBase::defaultSize()
{
    return QSize();
}

QRect EffectBase::viewBox()
{
    return QRect();
}

QRectF EffectBase::viewBoxF()
{
    return QRectF();
}

void EffectBase::setViewBox(QRect viewbox)
{

}

void EffectBase::setViewBox(QRectF viewbox)
{

}

bool EffectBase::animated()
{
    return false;
}

int EffectBase::framesPerSecond()
{
    return -1;
}

void EffectBase::setFramesPerSecond(int num)
{

}

int EffectBase::currentFrame()
{
    return -1;
}

void EffectBase::setCurrentFrame(int)
{

}

int EffectBase::animationDuration()
{
    return -1;
}

void EffectBase::setImage(int pKey, QPixmap pImg)
{
    m_EffectImgList.insert(pKey, pImg);
}

void EffectBase::setEffect(int pDuration, int pLoop, DIRECTION pDirection, ORIENTATION pOrientation)
{
    m_EffectAnimate->mDirection = pDirection;
    m_EffectAnimate->mOrientation = pOrientation;
    m_EffectAnimate->mLoopCount = pLoop;
    //>@10ms需要刷新一次
    m_EffectAnimate->mFrameStart = 0;
    m_EffectAnimate->mFrameEnd = pDuration / 10;
    m_EffectAnimate->mAnimateTime = pDuration;
}

void EffectBase::updateEffect(int pFrameNum)
{
    emit repaintNeeded();
}

void EffectBase::render(WidgetBase *pWidget, AREA_OPERATE pOperate)
{
    if(!m_OwnWidgets.contains(pWidget))
    {
        pWidget->SetRender(pOperate, this);
        connect(this, SIGNAL(repaintNeeded()), pWidget, SLOT(paintEffect()));
        m_OwnWidgets.append(pWidget);
    }
}

void EffectBase::render(GWidgetBase *pWidget, AREA_OPERATE pOperate)
{
    if(!m_OwnGraphicsWidgets.contains(pWidget))
    {
        pWidget->SetRender(pOperate, this);
        connect(this, SIGNAL(repaintNeeded()), pWidget, SLOT(paintEffect()));
        m_OwnGraphicsWidgets.append(pWidget);
    }
}

void EffectBase::unRender(WidgetBase *pWidget, AREA_OPERATE pOperate)
{
    if(pWidget)
    {
        disconnect(this, SIGNAL(repaintNeeded()), pWidget, SLOT(paintEffect()));
        pWidget->SetRender(pOperate, 0);
        m_OwnWidgets.removeOne(pWidget);
    }
}

void EffectBase::unRender(GWidgetBase *pWidget, AREA_OPERATE pOperate)
{
    if(pWidget)
    {
        disconnect(this, SIGNAL(repaintNeeded()), pWidget, SLOT(paintEffect()));
        pWidget->SetRender(pOperate, 0);
        m_OwnGraphicsWidgets.removeOne(pWidget);
    }
}

EffectFlip::EffectFlip(QObject *parent) :
    EffectBase(parent)
{
}

void EffectFlip::paint(QPainter *p, QRectF pRect)
{
    qreal w = pRect.width();
    qreal h = pRect.height();
    qreal hw = w / 2.0;
    qreal hh = h / 2.0;
    if(m_Animator.state() != QTimeLine::Running)
    {
        EffectBase::paint(p, pRect);
        return;
    }
    // behind is the new pixmap
    DrawPixmap(p, GetPixmap(0), pRect);
    if(m_EffectAnimate->mFrameCount <= 0)
        return;
    qreal halfcount =  m_EffectAnimate->mFrameCount / 2.0;
    qreal index = m_Animator.currentFrame();

    p->save();
    if (index <= halfcount)
    {
        // the top part of the old pixmap is flipping
        qreal angle;
        if(m_EffectAnimate->mOrientation == Positive)
        {
            angle = -180.0 * index / m_EffectAnimate->mFrameCount;
        }
        else
        {
            angle = 180.0 * index / m_EffectAnimate->mFrameCount;
        }
        p->translate(pRect.left() + hw, pRect.top() + hh);
        if(m_EffectAnimate->mDirection == Horizontal)
            p->setTransform(QTransform().rotate(angle, Qt::YAxis), true);
        else
            p->setTransform(QTransform().rotate(angle, Qt::XAxis), true);
        DrawPixmap(p,GetPixmap(1), pRect.size(), QRectF(-hw, -hh, w, h), QRectF(0,0,w,h));

        // the bottom part is still the old pixmap
        p->restore();
        p->save();
        p->translate(pRect.left(), pRect.top());
        if(m_EffectAnimate->mDirection == Horizontal)
            if(m_EffectAnimate->mOrientation == Positive)
                p->setClipRect(hw, 0, hw, pRect.height());
            else
                p->setClipRect(0, 0, hw, pRect.height());
        else
            if(m_EffectAnimate->mOrientation == Positive)
                p->setClipRect(0, hh, pRect.width(), hh);
            else
                p->setClipRect(0, 0, pRect.width(), hh);
        DrawPixmap(p,GetPixmap(1), pRect.size(), QRectF(0,0,w,h), QRectF(0,0,w,h));
    }
    else
    {
        p->translate(pRect.left(), pRect.top());
        if(m_EffectAnimate->mDirection == Horizontal)
            if(m_EffectAnimate->mOrientation == Positive)
                p->setClipRect(hw, 0, hw, pRect.height());
            else
                p->setClipRect(0, 0, hw, pRect.height());
        else
            if(m_EffectAnimate->mOrientation == Positive)
                p->setClipRect(0, hh, pRect.width(), hh);
            else
                p->setClipRect(0, 0, pRect.width(), hh);

        // the bottom part is still the old pixmap
        DrawPixmap(p,GetPixmap(1), pRect.size(), QRectF(0,0,w,h), QRectF(0,0,w,h));
        // the bottom part of the new pixmap is flipping
        qreal angle;
        if(m_EffectAnimate->mOrientation == Positive)
        {
            angle = 180.0 - 180.0 * index / m_EffectAnimate->mFrameCount;
        }
        else
        {
            angle = - 180.0 * (m_EffectAnimate->mFrameCount-index) / m_EffectAnimate->mFrameCount;
        }
        p->translate(hw, hh);
        if(m_EffectAnimate->mDirection == Horizontal)
            p->setTransform(QTransform().rotate(angle, Qt::YAxis), true);
        else
            p->setTransform(QTransform().rotate(angle, Qt::XAxis), true);
        DrawPixmap(p, GetPixmap(0), pRect.size(), QRectF(-hw, -hh, w, h), QRectF(0,0,w,h));
    }
    p->restore();
}

EffectRoll::EffectRoll(QObject *parent) :
    EffectBase(parent)
{
}

void EffectRoll::paint(QPainter *p, QRectF pRect)
{
    qreal hh = pRect.height();
    qreal hw = pRect.width();
    qreal tmpDiff;
    if(m_Animator.state() != QTimeLine::Running)
    {
        EffectBase::paint(p, pRect);
        return;
    }
    if(m_EffectAnimate->mFrameCount <= 0)
    {
        IDE_TRACE();
        return;
    }
    qreal index = m_Animator.currentFrame();
    qreal x, y;
    //>@首先将坐标原点移到当前需要绘制的区域。
    if(m_EffectAnimate->mDirection == Vertical)
    {
        p->save();
        tmpDiff = hh / m_EffectAnimate->mFrameCount;
        if(m_EffectAnimate->mOrientation == Positive)
        {
            y = (qreal)(tmpDiff*index);
            p->translate(pRect.left(), pRect.top()+y);
            DrawPixmap(p,GetPixmap(1), pRect.size(), QRectF(0,0,hw,hh-y), QRectF(0,0,hw,hh-y));
        }
        else
        {
            y = (qreal)(tmpDiff*index);
            p->translate(pRect.left(), pRect.top()-y);
            DrawPixmap(p,GetPixmap(1), pRect.size(), QRectF(0,y,hw,hh-y), QRectF(0,y,hw,hh-y));
        }
        p->restore();
        p->save();
        if(m_EffectAnimate->mOrientation == Positive)
        {
            y = hh - y;
            p->translate(pRect.left(), pRect.top()-y);
            DrawPixmap(p, GetPixmap(0), pRect.size(), QRectF(0,y,hw,hh-y), QRectF(0,y,hw,hh-y));
        }
        else
        {
            y = hh - y;
            p->translate(pRect.left(), pRect.top()+y);
            DrawPixmap(p, GetPixmap(0), pRect.size(), QRectF(0,0,hw,hh-y), QRectF(0,0,hw,hh-y));
        }
        p->restore();
    }
    else
    {
        p->save();
        tmpDiff = (qreal)(hw) / m_EffectAnimate->mFrameCount;
        if(m_EffectAnimate->mOrientation == Positive)
        {
            x = (qreal)(tmpDiff*index);
            p->translate(pRect.left()+x, pRect.top());
            DrawPixmap(p,GetPixmap(1), pRect.size(), QRectF(0,0,hw-x,hh), QRectF(0,0,hw-x,hh));
        }
        else
        {
            x = (qreal)(tmpDiff*index);
            p->translate(pRect.left()-x, pRect.top());
            DrawPixmap(p,GetPixmap(1), pRect.size(), QRectF(x,0,hw-x,hh), QRectF(x,0,hw-x,hh));
        }
        p->restore();
        p->save();
        if(m_EffectAnimate->mOrientation == Positive)
        {
            x = hw - x;
            p->translate(pRect.left()-x, pRect.top());
            DrawPixmap(p, GetPixmap(0), pRect.size(), QRectF(x,0,hw-x,hh), QRectF(x,0,hw-x,hh));
        }
        else
        {
            x = hw - x;
            p->translate(pRect.left()+x, pRect.top());
            DrawPixmap(p, GetPixmap(0), pRect.size(), QRectF(0,0,hw-x,hh), QRectF(0,0,hw-x,hh));
        }
        p->restore();
    }
}

EffectRotate::EffectRotate(QObject *parent) :
    EffectBase(parent)
{
}

void EffectRotate::paint(QPainter *p, QRectF pRect)
{
    qreal hh = pRect.height();
    qreal hw = pRect.width();
    if(m_Animator.state() != QTimeLine::Running)
    {
        EffectBase::paint(p, pRect);
        return;
    }
    if(m_EffectAnimate->mFrameCount <= 0)
    {
        IDE_TRACE();
        return;
    }
    qreal halfCount =  m_EffectAnimate->mFrameCount / 2.0;
    qreal index = m_Animator.currentFrame();

    p->save();
    p->translate(pRect.left()+hw/2.0, pRect.top()+hh/2.0);
    QPixmap pix = (index <= halfCount) ?GetPixmap(1) : GetPixmap(0);

    qreal angle = (index <= halfCount) ? (-180.0 * index / m_EffectAnimate->mFrameCount) : (180.0 - 180.0 * index / m_EffectAnimate->mFrameCount);
    if(m_EffectAnimate->mDirection == Horizontal)
    {
        p->setTransform(QTransform().rotate(angle, Qt::YAxis), true);
        DrawPixmap(p, pix, QRect(-hw / 2.0, -hh / 2.0, hw, hh));
    }
    else
    {
        p->setTransform(QTransform().rotate(angle, Qt::XAxis), true);
        DrawPixmap(p, pix, QRect(-hw / 2.0, -hh / 2.0, hw, hh));
    }
    p->restore();
}

EffectSlipCycle::EffectSlipCycle(QObject *parent) :
    EffectBase(parent)
{
}

void EffectSlipCycle::paint(QPainter *p, QRectF pRect)
{
    qreal hh = pRect.height();
    qreal hw = pRect.width();
    if(m_Animator.state() != QTimeLine::Running)
    {
        EffectBase::paint(p, pRect);
        return;
    }
    if(m_EffectAnimate->mFrameCount <= 0)
        return;
    qreal halfCount =  m_EffectAnimate->mFrameCount / 2.0;
    if(halfCount <= 0)
        return;
    qreal index = m_Animator.currentFrame();

    p->save();
    QPixmap tmpPixmap;
    qreal x, y, tmpDiff;
    if(m_EffectAnimate->mDirection == Vertical)
    {
        tmpDiff = (qreal)(hh) / halfCount;
        if(index <= halfCount)
        {
            tmpPixmap =GetPixmap(1);
            y = (int)(tmpDiff*index);
            if(y < 0)
                y = 0;
            if(m_EffectAnimate->mOrientation == Positive)
            {
                p->translate(pRect.left(), pRect.top()+y);
                //>@如果使用这种方法，则会刷出按钮范围，不过依然是一个比较好看的特效。
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, pRect.size(), QRectF(0,0,hw,hh-y), QRectF(0,0,hw,hh-y));
            }
            else
            {
                p->translate(pRect.left(), pRect.top()-y);
                //>@如果使用这种方法，则会刷出按钮范围，不过依然是一个比较好看的特效。
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, pRect.size(), QRectF(0,y,hw,hh-y), QRectF(0,y,hw,hh-y));
            }
        }
        else
        {
            y = (int)(tmpDiff*(m_EffectAnimate->mFrameEnd - index));
            tmpPixmap = GetPixmap(0);
            if(y < 0)
                y = 0;
            if(m_EffectAnimate->mOrientation == Positive)
            {
                p->translate(pRect.left(), pRect.top()+y);
                //>@如果使用这种方法，则会刷出按钮范围，不过依然是一个比较好看的特效。
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, pRect.size(), QRectF(0,0,hw,hh-y), QRectF(0,0,hw,hh-y));
            }
            else
            {
                p->translate(pRect.left(), pRect.top()-y);
                //>@如果使用这种方法，则会刷出按钮范围，不过依然是一个比较好看的特效。
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, pRect.size(), QRectF(0,y,hw,hh-y), QRectF(0,y,hw,hh-y));
            }
        }
    }
    else
    {
        tmpDiff = (qreal)(hw) / halfCount;
        if(index <= halfCount)
        {
            x = (int)(tmpDiff*index);
            if(x < 0)
                x = 0;
            tmpPixmap =GetPixmap(1);
            if(m_EffectAnimate->mOrientation == Positive)
            {
                p->translate(pRect.left()+x, pRect.top());
                //>@如果使用这种方法，则会刷出按钮范围，不过依然是一个比较好看的特效。
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, pRect.size(), QRectF(0,0,hw-x,hh), QRectF(0,0,hw-x,hh));
            }
            else
            {
                p->translate(pRect.left()-x, pRect.top());
                //>@如果使用这种方法，则会刷出按钮范围，不过依然是一个比较好看的特效。
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, pRect.size(), QRectF(x,0,hw-x,hh), QRectF(x,0,hw-x,hh));
            }
        }
        else
        {
            x = (int)(tmpDiff*(m_EffectAnimate->mFrameEnd - index));
            if(x < 0)
                x = 0;
            tmpPixmap = GetPixmap(0);
            if(m_EffectAnimate->mOrientation == Positive)
            {
                p->translate(pRect.left()+x, pRect.top());
                //>@如果使用这种方法，则会刷出按钮范围，不过依然是一个比较好看的特效。
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, pRect.size(), QRectF(0,0,hw-x,hh), QRectF(0,0,hw-x,hh));
            }
            else
            {
                p->translate(pRect.left()-x, pRect.top());
                //>@如果使用这种方法，则会刷出按钮范围，不过依然是一个比较好看的特效。
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, pRect.size(), QRectF(x,0,hw-x,hh), QRectF(x,0,hw-x,hh));
            }
        }
    }
    p->restore();
}

EffectTransparent::EffectTransparent(QObject *parent) :
    EffectBase(parent)
{
}

void EffectTransparent::paint(QPainter *p, QRectF pRect)
{
    qreal hh = pRect.height();
    qreal hw = pRect.width();
    if(m_Animator.state() != QTimeLine::Running)
    {
        EffectBase::paint(p, pRect);
        return;
    }
    if(m_EffectAnimate->mFrameCount <= 0)
    {
        IDE_TRACE();
        return;
    }
    qreal Stage1 = m_EffectAnimate->mFrameCount*3.0/10.0;
    qreal Stage2 = m_EffectAnimate->mFrameCount*4.0/10.0;
    //qreal Stage3 = (qreal)(m_EffectAnimate->mFrameCount)*3.0/10.0;

    qreal index = m_Animator.currentFrame();
    //>@循环改变透明度图片
    qreal tmpLastAlpha, tmpAlpha;
    if(index <= Stage1)
    {
        tmpLastAlpha = (qreal)(Stage1 - index) / (qreal)Stage1;;
        tmpAlpha = (qreal)(index) / (qreal)Stage1;
    }
    else if(index <= Stage2 + Stage1)
    {
        tmpLastAlpha = 0;
        tmpAlpha = 1;
    }
    else  //>@必须得添加，否则一次动画完成后也即index>Stage2 + Stage1时会将两幅图片都进行透明处理
    {
        tmpLastAlpha = 0;
        tmpAlpha = 1;
    }

    p->save();
    p->translate(pRect.left(), pRect.top());
    p->setOpacity(tmpLastAlpha);
    DrawPixmap(p,GetPixmap(1), pRect.size(), QRectF(0,0,hw,hh), QRectF(0,0,hw,hh));
    p->setOpacity(tmpAlpha);
    DrawPixmap(p, GetPixmap(0), pRect.size(), QRectF(0,0,hw,hh), QRectF(0,0,hw,hh));
    p->restore();
}

EffectZoom::EffectZoom(QObject *parent) :
    EffectBase(parent)
{
}

void EffectZoom::paint(QPainter *p, QRectF pRect)
{
    qreal hh = pRect.height();
    qreal hw = pRect.width();
    if(m_Animator.state() != QTimeLine::Running)
    {
        EffectBase::paint(p, pRect);
        return;
    }
    if(m_EffectAnimate->mFrameCount <= 0)
    {
        IDE_TRACE();
        return;
    }
    qreal halfcount =  m_EffectAnimate->mFrameCount / 2.0;
    qreal index = m_Animator.currentFrame();

    qreal x,y,w,h;
    p->save();
    p->translate(pRect.left()+hw/2, pRect.top()+hh/2);
    if(index < halfcount)
    {
        index = halfcount - index;
        x = -hw*index/m_EffectAnimate->mFrameCount;
        y = -hh*index/m_EffectAnimate->mFrameCount;
        w = 2*hw*index/m_EffectAnimate->mFrameCount;
        h = 2*hh*index/m_EffectAnimate->mFrameCount;
        DrawPixmap(p,GetPixmap(1), pRect.size(), QRectF(x,y,w,h), QRectF(0,0,hw,hh));
    }
    else
    {
        index = index - halfcount;
        x = -hw*index/m_EffectAnimate->mFrameCount;
        y = -hh*index/m_EffectAnimate->mFrameCount;
        w = 2*hw*index/m_EffectAnimate->mFrameCount;
        h = 2*hh*index/m_EffectAnimate->mFrameCount;
        DrawPixmap(p, GetPixmap(0), pRect.size(), QRectF(x,y,w,h), QRectF(0,0,hw,hh));
    }
    p->restore();
}


EffectBtnZoom::EffectBtnZoom(QObject *parent) :
    EffectBase(parent)
{
    m_Widget = 0;
}

void EffectBtnZoom::Start()
{
#ifndef N329
    m_Animation.stop();
//    if(m_Widget)
//        m_Widget->setGeometry(m_Rect[1]);
    m_Animation.start();
#endif
}

void EffectBtnZoom::Stop()
{
#ifndef N329
    if(m_Animation.state() == QTimeLine::Running)
        m_Animation.stop();
#endif
}

void EffectBtnZoom::render(WidgetBase *pWidget, AREA_OPERATE pOperate)
{
#ifndef N329
    if(!m_OwnWidgets.contains(pWidget))
    {
        m_Widget = pWidget;
        pWidget->SetRender(pOperate, this);
        m_Animation.setTargetObject(pWidget);
        m_Animation.setPropertyName("geometry");
        m_Animation.setDuration(400);
        m_Animation.setEasingCurve(QEasingCurve::Linear);
        m_Rect[0] = pWidget->geometry();
        int xDiff = m_Rect[0].width()*0.1;
        int yDiff = m_Rect[0].height()*0.1;
        int x = m_Rect[0].x() - xDiff;
        int y = m_Rect[0].y() - yDiff;
        int w = m_Rect[0].width() + xDiff*2;
        int h = m_Rect[0].height() + yDiff*2;
        m_Rect[1] = QRect(x,y,w,h);
        m_Animation.setStartValue(m_Rect[1]);
        m_Animation.setKeyValueAt(0.2, QRect(x,y,w,h));
        m_Animation.setEndValue(m_Rect[0]);
        m_OwnWidgets.append(pWidget);
    }
#endif
}

EffectTileRoll::EffectTileRoll(QObject *parent) :
    EffectBase(parent)
{
}

//>@平铺滚动
void EffectTileRoll::paint(QPainter *p, QRectF pRect)
{
    if(m_Animator.state() != QTimeLine::Running)
        return;
    if(m_EffectAnimate->mFrameCount <= 0)
        return;

    qreal hh = pRect.height();
    qreal hw = pRect.width();
    QPixmap tmpPixmap = GetPixmap(0);
    if(tmpPixmap.isNull())
        return;

    qreal pixhh = tmpPixmap.height();
    qreal pixhw = tmpPixmap.width();
    qreal tmpDiff;

    qreal index = m_Animator.currentFrame();
    qreal x, y;
    //>@首先将坐标原点移到当前需要绘制的区域。
    p->save();
    if(m_EffectAnimate->mDirection == Vertical)
    {
        tmpDiff = pixhh / m_EffectAnimate->mFrameCount;
        y = (qreal)(tmpDiff*index);
        x = 0;
        p->translate(pRect.left()+x, pRect.top()-y);
        DrawPixmap(p, tmpPixmap, QRect(x,y,pixhw-x,pixhh-y), QRect(x,y,pixhw-x,pixhh-y));
        int tmp, tmp1, i = 1;
        while((tmp = i*pixhh-y-1) < hh)
        {
            p->restore();
            p->save();
            x = 0;
            p->translate(pRect.left()+x, pRect.top()+tmp);
            tmp1 = (hh-tmp > pixhh)?(pixhh):(hh-tmp);
            DrawPixmap(p, tmpPixmap,
                       QRect(0,0,pixhw-x,tmp1),
                       QRect(0,0,pixhw-x,tmp1));
            i++;
        }
    }
    else
    {
        tmpDiff = (qreal)(pixhw) / m_EffectAnimate->mFrameCount;
        x = (qreal)(tmpDiff*index);
        y = 0;
        p->translate(pRect.left()-x, pRect.top()+y);
        DrawPixmap(p, tmpPixmap, QRect(x,y,pixhw-x,pixhh-y), QRect(x,y,pixhw-x,pixhh-y));
        int tmp, tmp1, i = 1;
        while((tmp = i*pixhw-x-1) < hw)   //>@i*pixhw-x-1如果不-1，那么会出现一条白线。
        {
            p->restore();
            p->save();
            y = 0;
            p->translate(pRect.left()+tmp, pRect.top()+y);
            tmp1 = (hw-tmp > pixhw)?(pixhw):(hw-tmp);
            DrawPixmap(p, tmpPixmap,
                       QRect(0,0,tmp1,pixhh-y),
                       QRect(0,0,tmp1,pixhh-y));
            i++;
        }
    }
    p->restore();
}

