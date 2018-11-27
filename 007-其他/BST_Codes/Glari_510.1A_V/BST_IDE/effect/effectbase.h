#ifndef EFFECTBASE_H
#define EFFECTBASE_H

#include "global.h"

class WidgetBase;
class GWidgetBase;

class EFFECT_ANIMATE
{
public:
    EFFECT_ANIMATE()
    {
        mDirection = Horizontal;
        mOrientation = Positive;
        mFrameStart = 0;
        mFrameEnd = 10;
        mFrameCount = mFrameEnd - mFrameStart -1;
        mAnimateTime = 200;
        mLoopCount = 1;        //>@循环次数，如果为0，则表示无限循环
        mDutyRatio = 50;        //>@占空比：0~100
        mAnimateType = QTimeLine::LinearCurve;
    }
public:
    DIRECTION               mDirection;    //>@图片特效方向
    ORIENTATION             mOrientation;  //>@特效正逆序
    int                     mLoopCount;    //>@为0表示永远循环
    int                     mFrameStart, mFrameEnd, mFrameCount;
    int                     mAnimateTime;
    int                     mDutyRatio;
    QTimeLine::CurveShape   mAnimateType;
};
Q_DECLARE_METATYPE(EFFECT_ANIMATE)

inline bool DrawPixmap(QPainter *p, QPixmap pPixmap, QRectF pTargetRect, QRectF pSourceRect)
{
    if(pTargetRect.isValid() == false || pSourceRect.isValid() == false)
    {
        IDE_TRACE();
        return false;
    }
    if(pPixmap.isNull() == false)
    {
        p->drawPixmap(pTargetRect,
                     pPixmap,
                     pSourceRect);
        return true;
    }
    return false;
}

inline bool DrawPixmap(QPainter *p, QPixmap pPixmap, QRectF pTargetRect)
{
    if(pTargetRect.isValid() == false)
    {
        IDE_TRACE();
        return false;
    }
    if(pPixmap.isNull() == false)
    {
        p->drawPixmap(pTargetRect.toRect(), pPixmap);
        return true;
    }
    return false;
}

//>@只能用于整幅图片的翻转显示
inline bool DrawFlipPixmap(QPainter *p, QPixmap pPixmap, QRectF pTargetRect, Qt::Axis pAxis)
{
    if(pTargetRect.isValid() == false)
    {
        IDE_TRACE();
        return false;
    }
    if(pPixmap.isNull() == false)
    {
        p->save();
        p->translate(pTargetRect.width()/2.0, pTargetRect.height()/2.0);
        p->setTransform((QTransform().translate(pTargetRect.width()/2.0, pTargetRect.height()/2.0)
                        .rotate(180, pAxis)
                        .translate(-pTargetRect.width()/2.0, -pTargetRect.height()/2.0)));
        p->drawPixmap(pTargetRect.toRect(), pPixmap);
        p->restore();
        return true;
    }
    return false;
}

inline QPixmap GetFlipPixmap(QPixmap pPixmap, QRectF pTargetRect, Qt::Axis pAxis)
{
    if(pPixmap.isNull())
    {
        IDE_TRACE();
        return QPixmap();
    }
    QImage tmpImage(pTargetRect.width(), pTargetRect.height(), QImage::Format_ARGB32_Premultiplied);
    if(tmpImage.isNull())
        return QPixmap();
    tmpImage.fill(Qt::transparent); //>@创建透明图层
    QPainter p(&tmpImage);
    p.translate(pTargetRect.width()/2.0, pTargetRect.height()/2.0);
    p.setTransform((QTransform().translate(pTargetRect.width()/2.0, pTargetRect.height()/2.0)
                    .rotate(180, pAxis)
                    .translate(-pTargetRect.width()/2.0, -pTargetRect.height()/2.0)));
    DrawPixmap(&p, pPixmap, pTargetRect);
    p.end();
    return QPixmap::fromImage(tmpImage);
}

inline bool DrawPixmap(QPainter *p, QPixmap pPixmap)
{
    if(pPixmap.isNull() == false)
    {
        p->drawPixmap(0, 0, pPixmap);
        return true;
    }
    return false;
}

inline bool DrawPixmap(QPainter *p, QPixmap pPixmap, QSizeF pSize, QRectF pTargetRect, QRectF pSourceRect)
{
    if(pTargetRect.isValid() == false || pSourceRect.isValid() == false || pSize.isValid() == false)
    {
        return false;
    }
    if(pPixmap.isNull() == false)
    {
        p->drawPixmap(pTargetRect,
                      pPixmap.scaled(pSize.toSize(),Qt::IgnoreAspectRatio),
                      pSourceRect);
        return true;
    }
    return false;
}

class EffectBase : public QObject
{
    Q_OBJECT
public:
    EffectBase(QObject *parent=0);
    ~EffectBase();

    bool isValid();

    QSize defaultSize();
    QRect viewBox();
    QRectF viewBoxF();
    void setViewBox(QRect viewbox);
    void setViewBox(QRectF viewbox);

    bool animated();
    int framesPerSecond();
    void setFramesPerSecond(int num);
    int currentFrame();
    void setCurrentFrame(int);
    int animationDuration();//in seconds

    void setOrientation(ORIENTATION pOrientation)  {if(m_EffectAnimate) m_EffectAnimate->mOrientation = pOrientation;}

    QPixmap GetPixmap(int pKey) { return m_EffectImgList.value(pKey); }

    virtual void Start();
    virtual void Stop();

    virtual void paint(QPainter *p, QRectF pRect=QRectF());

public Q_SLOTS:
    void setImage(int pKey, QPixmap pImg);
    void setEffect(int pDuration, int pLoop, DIRECTION pDirection, ORIENTATION pOrientation);

    void updateEffect(int pFrameNum);

    void render(WidgetBase *pWidget, AREA_OPERATE pOperate);
    void render(GWidgetBase *pWidget, AREA_OPERATE pOperate);

    void unRender(WidgetBase *pWidget, AREA_OPERATE pOperate);
    void unRender(GWidgetBase *pWidget, AREA_OPERATE pOperate);

Q_SIGNALS:
    void repaintNeeded();

public:
    QList<WidgetBase*>                  m_OwnWidgets;
    QList<GWidgetBase*>                 m_OwnGraphicsWidgets;

    QHash<int, QPixmap>                 m_EffectImgList;

    QTimeLine                           m_Animator;
    EFFECT_ANIMATE                     *m_EffectAnimate;

    bool                                m_Valid;
};

inline void EffectBase::Start()
{
    m_Animator.stop();
    if(m_EffectAnimate)
    {
        m_Animator.setFrameRange(m_EffectAnimate->mFrameStart, m_EffectAnimate->mFrameEnd);
        m_EffectAnimate->mFrameCount = m_EffectAnimate->mFrameEnd - m_EffectAnimate->mFrameStart - 1;
        m_Animator.setDuration(m_EffectAnimate->mAnimateTime);
        m_Animator.setCurveShape(m_EffectAnimate->mAnimateType);
        m_Animator.setLoopCount(m_EffectAnimate->mLoopCount);
    }
    m_Animator.start();
}

inline void EffectBase::Stop()
{
    if(m_Animator.state() == QTimeLine::Running)
        m_Animator.stop();
    if(m_EffectAnimate)
        m_EffectAnimate = 0;
    updateEffect(0);
}

inline void EffectBase::paint(QPainter *p, QRectF pRect)
{
    DrawPixmap(p, GetPixmap(0), pRect);
}

class EffectFlip : public EffectBase
{
    Q_OBJECT
public:
    EffectFlip(QObject *parent=0);
    virtual void paint(QPainter *p, QRectF pRect=QRectF());
};

class EffectRoll : public EffectBase
{
    Q_OBJECT
public:
    EffectRoll(QObject *parent=0);
    virtual void paint(QPainter *p, QRectF pRect=QRectF());
};


class EffectRotate : public EffectBase
{
    Q_OBJECT
public:
    EffectRotate(QObject *parent=0);
    virtual void paint(QPainter *p, QRectF pRect=QRectF());
};

class EffectSlipCycle : public EffectBase
{
    Q_OBJECT
public:
    EffectSlipCycle(QObject *parent=0);
    virtual void paint(QPainter *p, QRectF pRect=QRectF());
};

class EffectTransparent : public EffectBase
{
    Q_OBJECT
public:
    EffectTransparent(QObject *parent=0);
    virtual void paint(QPainter *p, QRectF pRect=QRectF());
};

class EffectZoom : public EffectBase
{
    Q_OBJECT
public:
    EffectZoom(QObject *parent=0);
    virtual void paint(QPainter *p, QRectF pRect=QRectF());
};

class EffectBtnZoom : public EffectBase
{
    Q_OBJECT
public:
    EffectBtnZoom(QObject *parent=0);

    virtual void Start();
    virtual void Stop();
public Q_SLOTS:
    void render(WidgetBase *pWidget, AREA_OPERATE pOperate);

public:
#ifndef N329
    QPropertyAnimation  m_Animation;
#endif
    QRect               m_Rect[2];
    WidgetBase         *m_Widget;
};

class EffectTileRoll : public EffectBase
{
    Q_OBJECT
public:
    EffectTileRoll(QObject *parent=0);
    virtual void paint(QPainter *p, QRectF pRect=QRectF());
};

#endif // EFFECTBASE_H
