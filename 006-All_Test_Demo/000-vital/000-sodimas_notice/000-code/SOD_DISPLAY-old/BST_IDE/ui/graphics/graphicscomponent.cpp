#include "graphicscomponent.h"

#ifdef SUPPORT_SCENE
#include "graphicsscene.h"
#include "devicemanager.h"
#elif SUPPORT_COMEDIT
#include "diagramscene.h"
#endif
#include "logicthememanager.h"

GraphicsComponent::GraphicsComponent(QGraphicsItem *parent, GraphicsScene *pScene, QRectF pRect) :
    QGraphicsWidget(parent, Qt::FramelessWindowHint), RcContainer(pScene, pRect)
{
    setAttribute(Qt::WA_DeleteOnClose);
    //setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);

#ifdef  SURRPORT_3D
    m_GLWidget = 0;
    m_3DXDiff = m_3DYDiff = m_3DZDiff = 0; //>@三个方向上的角度偏移
    m_3DXScale = m_3DYScale = m_3DZScale = 0; //>@三个方向上的缩放
    m_3DDeepDiff = 0;
#endif

    m_PaintOptimizer = false;

    m_AreaAnimate = 0;
    m_UpdateOPerate = OPERATE_NONE;
    m_UpdatePara = QVariant();

    m_ActionGroup = 0;
    m_ActionGroup = new QActionGroup(this);
    connect(m_ActionGroup, SIGNAL(triggered(QAction *)), this, SLOT(ExecAction(QAction *)));

    connect(&m_Animator, SIGNAL(frameChanged(int)), this, SLOT(UpdateCom(int)));

    connect(this, SIGNAL(sUpdateCom(int)), this, SLOT(UpdateCom(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(sRestart()), this, SLOT(slot_Restart()), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(sModifyGeomotery(QRectF)), this, SLOT(slot_ModifyGeomotery(QRectF)), Qt::BlockingQueuedConnection);
}

GraphicsComponent::~GraphicsComponent()
{
    m_Animator.stop();
}

void GraphicsComponent::ReleaseTextures()
{
#ifdef  SURRPORT_3D
    QList<int> tmpList;// = m_EffectTexture.keys();
    for(int i=0;i<tmpList.count();i++)
    {
        GLuint tmpTexture;// = m_EffectTexture.take(tmpList.at(i));
        if(m_GLWidget)
            m_GLWidget->deleteTexture(tmpTexture);
    }
#endif
}

void GraphicsComponent::UpdateCom(int pFrame)
{
    update(rect());
}

QRectF GraphicsComponent::boundingRect() const
{
    QRectF tmpRect = this->geometry().normalized();
    return QRectF(0,0,tmpRect.width(),tmpRect.height()).normalized();
}

QPainterPath GraphicsComponent::shape() const
{
    QPainterPath path;
    QRectF tmpRect = this->geometry().normalized();
    path.addRect(QRectF(0,0,tmpRect.width(),tmpRect.height()).normalized());
    return path;
}

void GraphicsComponent::Restart()
{
    if (QThread::currentThread() != thread())
        emit sRestart();
    else
        Start();
}

void GraphicsComponent::slot_Restart()
{
    Start();
}

bool GraphicsComponent::Start()
{
#ifdef DISPLAY
#ifdef SUPPORT_SCENE
    UiScene *tmpScene = VAR_CAST<UiScene*>(m_UiContainer.data());
    if(tmpScene)
        connect(this, SIGNAL(sSetDevices(bool,E_DEVMODE)), tmpScene->m_LogicThemeManager->m_DevManager, SLOT(SetDevicesEnable(bool,E_DEVMODE)), Qt::QueuedConnection);
#endif
#endif
    m_Animator.stop();
    m_EffectValid = false;
    m_RcList.clear();
    if(!RcContainer::Start())
    {
        IDE_TRACE();
        return false;
    }
    setData(COMTYPE, QVariant(m_ComType));
    SetComGeometory(GetCurRect());
    m_EffectValid = true;
    //>@用于制作循环播放效果
    if(hasResource())
        m_RcList = m_EffectRC.keys();
    if(m_RcList.isEmpty())
        return false;
    qSort(m_RcList.begin(), m_RcList.end());
    return true;
}

bool GraphicsComponent::ModifyGeomotery(QStringList pGeoList)
{
#ifdef SUPPORT_SCENE
#ifdef N329
    GraphicsComponent *tmpParBase = VAR_CAST<GraphicsComponent*>(parentItem());
#else
    GraphicsComponent *tmpParBase = VAR_CAST<GraphicsComponent*>(parent());
#endif
#elif SUPPORT_WIDGET
    WidgetBase *tmpParBase = VAR_CAST<WidgetBase*>(parent());
#elif SUPPORT_COMEDIT
    GraphicsComponent *tmpParBase = VAR_CAST<GraphicsComponent*>(parent());
#endif
    if(!tmpParBase)
        return false;
#ifdef SUPPORT_SCENE
    QRectF tmpRect = QRectF(pGeoList.at(0).toInt(), pGeoList.at(1).toInt(), pGeoList.at(2).toInt(), pGeoList.at(3).toInt());
#elif SUPPORT_WIDGET
    QRect tmpRect = QRect(pGeoList.at(0).toInt(), pGeoList.at(1).toInt(), pGeoList.at(2).toInt(), pGeoList.at(3).toInt());
#elif SUPPORT_COMEDIT
    QRectF tmpRect = QRectF(pGeoList.at(0).toInt(), pGeoList.at(1).toInt(), pGeoList.at(2).toInt(), pGeoList.at(3).toInt());
#endif
    if(tmpParBase->geometry().contains(tmpRect))
    {
        ModifyGeomotery(tmpRect);
    }
    return true;
}

void GraphicsComponent::ModifyGeomotery(QRectF pRect)
{
    if (QThread::currentThread() != thread())
        emit sModifyGeomotery(pRect);
    else
        SetComGeometory(pRect);
}

void GraphicsComponent::SetComGeometory(QRectF pRect)
{
    prepareGeometryChange();
    setGeometry(pRect);
}

void GraphicsComponent::slot_ModifyGeomotery(QRectF pRect)
{
    SetComGeometory(pRect);
}

void GraphicsComponent::StopAnimate()
{
    if(m_Animator.state() == QTimeLine::Running)
        m_Animator.stop();
    m_AreaAnimate = 0;
    UpdateCom(0);
}

void GraphicsComponent::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
#ifdef PIXQUALITY
    if(m_PaintOptimizer)
    {
        painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
        painter->setRenderHint(QPainter::Antialiasing);
    }
#endif
    PaintEffect(painter);
}

bool GraphicsComponent::PaintEffect(QPainter *p)
{
    if(m_EffectValid == false)
    {
        return false;
    }
    if(!m_AreaAnimate)
    {
        Paint2D_None(p, rect());
        return false;
    }
    switch(m_AreaAnimate->mEffectType)
    {
        case EffectTypeRoll:
            Paint2D_Roll(p, rect());
            break;
        case EffectTypeFlip:
            Paint2D_Flip(p, rect());
            break;
        case EffectTypeBlink:
            Paint2D_Blink(p, rect());
            break;
        case EffectTypeRotate:
            Paint2D_Rotate(p, rect());
            break;
        case EffectTypeTwirl:
            Paint2D_Twirl(p, rect());
            break;
        case EffectTypeZoom:
            Paint2D_Zoom(p, rect());
            break;
        case EffectTypeTransparent:
            Paint2D_Transparent(p, rect());
            break;
        case EffectTypeSpread:
            Paint2D_Spread(p, rect());
            break;
        case EffectTypeWheel:
            Paint2D_Wheel(p, rect());
            break;
        case EffectTypeSlipCycle:
            Paint2D_Slipcycle(p, rect());
            break;
#ifdef  SURRPORT_3D
        case EffectTypeCube:
            Paint3D_Cube(p, rect());
            break;
        case EffectTypeCylinder:
            Paint3D_Cylinder(p, rect());
            break;
#endif
        default:
            Paint2D_None(p, rect());
            break;
    }
    return true;
}

void GraphicsComponent::Play()
{
    updateEffect(OPERATE_PLAY, QVariant());
}

void GraphicsComponent::Stop()
{
    updateEffect(OPERATE_STOP, QVariant());
}

bool GraphicsComponent::SetEnvironment(QString pEnvirString)
{
    return true;
}

bool GraphicsComponent::GetEnvironment(QString &pEnvirString)
{
    pEnvirString = QString("Test");
    return true;
}

void GraphicsComponent::OperateTurn(QVariant pPara)
{
    SetComGeometory(GetCurRect());
}

void GraphicsComponent::OperateShow(QVariant pPara)
{
    if(m_ComType >= COM_FLOOR)
    {
        if(m_Animator.state() == QTimeLine::Paused)
            m_Animator.setPaused(false);
#if 0
        setOpacity(1);
#else
#if 0
        setGeometry(GetCurRect());
#else
        show();
#endif
#endif
    }
}

void GraphicsComponent::OperateHide(QVariant pPara)
{
    IDE_TRACE_INT(m_ComType);
    if(m_ComType >= COM_FLOOR)
    {
        if(m_Animator.state() == QTimeLine::Running)
            m_Animator.setPaused(true);
#if 0
        setOpacity(0);
#else
#if 0
        QRectF tmpRect = GetCurRect();
        tmpRect.setTopLeft(QPointF(-tmpRect.width(), -tmpRect.height()));
        setGeometry(tmpRect);
#else
        hide();
#endif
#endif
    }
}

void GraphicsComponent::OperateClear(QVariant pPara)
{
    if(m_ComType >= COM_FLOOR)
    {
        updateEffect(OPERATE_CHANGE, INVALIDRC);
    }
}

void GraphicsComponent::OperatePlay(QVariant pPara)
{
    if(m_ComType >= COM_FLOOR)
    {
        if(!isVisible())
#if 0
            show();
#else
            return;
#endif
        updateEffect(m_UpdateOPerate, m_UpdatePara);
    }
}

void GraphicsComponent::OperateStop(QVariant pPara)
{
    m_OperateInfo[STEP0].mValid = false;
    m_OperateInfo[STEP1].mValid = false;
    m_Animator.stop();
    m_AreaAnimate = 0;
    resetTransform();
#if 0
    if(m_ComType >= COM_FLOOR)
    {
        if(isVisible())
            hide();
    }
#endif
}

void GraphicsComponent::updateEffect(AREA_OPERATE pOperate, QVariant pPara)
{
    moveToThread(qApp->thread());
    if(m_UpdateOPerate != pOperate && pOperate != OPERATE_PLAY && pOperate != OPERATE_STOP)
    {
        m_UpdateOPerate = pOperate;
        m_UpdatePara = pPara;
    }
    if(pOperate == OPERATE_TURN)
    {
        OperateTurn(pPara);
    }
    else if(pOperate == OPERATE_SHOW)
    {
        OperateShow(pPara);
    }
    else if(pOperate == OPERATE_HIDE)
    {
        OperateHide(pPara);
    }
    else if(pOperate == OPERATE_CLEAR)
    {
        OperateClear(pPara);
    }
    else if(pOperate == OPERATE_PLAY)
    {
        OperatePlay(pPara);
    }
    else if(pOperate == OPERATE_STOP)
    {
        OperateStop(pPara);
    }
}

void GraphicsComponent::OperateStep0()
{
    if(m_EffectValid == false)
        return;
    UpdateCom(0);
    //>@执行STEP0中的内容，如果执行不成功，尝试执行STEP1中的内容
    if(m_OperateInfo[STEP0].mValid)
    {
        m_AreaAnimate = m_EffectGroup.value(m_OperateInfo[STEP0].mOperate);
        if(m_AreaAnimate)
        {
            connect(&m_Animator, SIGNAL(finished()), SLOT(OperateStep1()));
            StartTimeline();
            return;
        }
    }
    OperateStep1();
}

void GraphicsComponent::OperateStep1()
{
    if(m_EffectValid == false)
        return;
    //>@准备资源，两个必定为同一张图片
    m_PixmapPointer.mLastPointer = m_PixmapPointer.mCurPointer;
    UpdateCom(0);
    if(m_OperateInfo[STEP1].mValid)
    {
        m_AreaAnimate = m_EffectGroup.value(m_OperateInfo[STEP1].mOperate);
        if(m_AreaAnimate)
        {
            disconnect(&m_Animator, SIGNAL(finished()), 0, 0);
            StartTimeline();
            return;
        }
    }
}

void GraphicsComponent::StartTimeline()
{
    m_Animator.stop();
    m_Animator.setFrameRange(m_AreaAnimate->mFrameStart, m_AreaAnimate->mFrameEnd);
    m_AreaAnimate->mFrameCount = m_AreaAnimate->mFrameEnd - m_AreaAnimate->mFrameStart - 1;
    m_Animator.setDuration(m_AreaAnimate->mAnimateTime);
    m_Animator.setCurveShape(m_AreaAnimate->mAnimateType);
    m_Animator.setLoopCount(m_AreaAnimate->mLoopCount);
    m_Animator.start();
}

bool GraphicsComponent::DrawPixmap(QPainter *p, QPixmap pPixmap, QRectF pTargetRect, QRectF pSourceRect)
{
    if(pTargetRect.isValid() == false || pSourceRect.isValid() == false)
    {
        //IDE_TRACE();
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

bool GraphicsComponent::DrawPixmap(QPainter *p, QPixmap pPixmap, QRectF pTargetRect)
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
bool GraphicsComponent::DrawFlipPixmap(QPainter *p, QPixmap pPixmap, QRectF pTargetRect)
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
                        .rotate(180, Qt::XAxis)
                        .translate(-pTargetRect.width()/2.0, -pTargetRect.height()/2.0)));
        p->drawPixmap(pTargetRect.toRect(), pPixmap);
        p->restore();
        return true;
    }
    return false;
}

QPixmap GraphicsComponent::GetFlipPixmap(QPixmap pPixmap, QRectF pTargetRect)
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
                    .rotate(180, Qt::XAxis)
                    .translate(-pTargetRect.width()/2.0, -pTargetRect.height()/2.0)));
    DrawPixmap(&p, pPixmap, pTargetRect);
    p.end();
    return QPixmap::fromImage(tmpImage);
}

bool GraphicsComponent::DrawPixmap(QPainter *p, QPixmap pPixmap)
{
    if(pPixmap.isNull() == false)
    {
        p->drawPixmap(0, 0, pPixmap);
        return true;
    }
    return false;
}

bool GraphicsComponent::DrawPixmap(QPainter *p, QPixmap pPixmap, QSizeF pSize, QRectF pTargetRect, QRectF pSourceRect)
{
    if(pTargetRect.isValid() == false || pSourceRect.isValid() == false || pSize.isValid() == false)
    {
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
    }
    return false;
}

//>@平铺滚动
void GraphicsComponent::Paint2D_TileRoll(QPainter *p, QRectF targetRect)
{
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    qreal pixhh = m_PixmapPointer.mCurPointer.height();
    qreal pixhw = m_PixmapPointer.mCurPointer.width();
    qreal tmpDiff;
    if(m_Animator.state() != QTimeLine::Running)
        return;
    qreal FrameCount = m_AreaAnimate->mFrameEnd - m_AreaAnimate->mFrameStart - 1;
    if(FrameCount <= 0)
    {
        IDE_TRACE();
        return;
    }
    qreal index = m_Animator.currentFrame();
    qreal x, y;
    //>@首先将坐标原点移到当前需要绘制的区域。
    p->save();
    if(m_AreaAnimate->mDirection == Vertical)
    {
        tmpDiff = pixhh / FrameCount;
        y = (qreal)(tmpDiff*index);
        x = 0;
        p->translate(targetRect.left()+x, targetRect.top()-y);
        DrawPixmap(p, m_PixmapPointer.mLastPointer, QRect(x,y,pixhw-x,pixhh-y), QRect(x,y,pixhw-x,pixhh-y));
        int tmp, tmp1, i = 1;
        while((tmp = i*pixhh-y) < hh)
        {
            p->restore();
            p->save();
            x = 0;
            p->translate(targetRect.left()+x, targetRect.top()+tmp);
            tmp1 = (hh-tmp > pixhh)?(pixhh):(hh-tmp);
            DrawPixmap(p, m_PixmapPointer.mCurPointer,
                       QRect(0,0,pixhw-x,tmp1),
                       QRect(0,0,pixhw-x,tmp1));
            i++;
        }
    }
    else
    {
        tmpDiff = (qreal)(pixhw) / FrameCount;
        x = (qreal)(tmpDiff*index);
        y = 0;
        p->translate(targetRect.left()-x, targetRect.top()+y);
        DrawPixmap(p, m_PixmapPointer.mLastPointer, QRect(x,y,pixhw-x,pixhh-y), QRect(x,y,pixhw-x,pixhh-y));
        int tmp, tmp1, i = 1;
        while((tmp = i*pixhw-x) < hw)
        {
            p->restore();
            p->save();
            y = 0;
            p->translate(targetRect.left()+tmp, targetRect.top()+y);
            tmp1 = (hw-tmp > pixhw)?(pixhw):(hw-tmp);
            DrawPixmap(p, m_PixmapPointer.mCurPointer,
                       QRect(0,0,tmp1,pixhh-y),
                       QRect(0,0,tmp1,pixhh-y));
            i++;
        }
    }
    p->restore();
}

//>@需要绘制底图和特效图片
void GraphicsComponent::Paint2D_Roll(QPainter *p, QRectF targetRect)
{
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    qreal tmpDiff;
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, targetRect);
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
    {
        IDE_TRACE();
        return;
    }
    qreal index = m_Animator.currentFrame();
    qreal x, y;
    //>@首先将坐标原点移到当前需要绘制的区域。
    if(m_AreaAnimate->mDirection == Vertical)
    {
        p->save();
        tmpDiff = hh / m_AreaAnimate->mFrameCount;
        if(m_AreaAnimate->mOrientation == Positive)
        {
            y = (qreal)(tmpDiff*index);
            p->translate(targetRect.left(), targetRect.top()+y);
            DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(0,0,hw,hh-y), QRectF(0,0,hw,hh-y));
        }
        else
        {
            y = (qreal)(tmpDiff*index);
            p->translate(targetRect.left(), targetRect.top()-y);
            DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(0,y,hw,hh-y), QRectF(0,y,hw,hh-y));
        }
        p->restore();
        p->save();
        if(m_AreaAnimate->mOrientation == Positive)
        {
            y = hh - y;
            p->translate(targetRect.left(), targetRect.top()-y);
            DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(0,y,hw,hh-y), QRectF(0,y,hw,hh-y));
        }
        else
        {
            y = hh - y;
            p->translate(targetRect.left(), targetRect.top()+y);
            DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(0,0,hw,hh-y), QRectF(0,0,hw,hh-y));
        }
        p->restore();
    }
    else
    {
        p->save();
        tmpDiff = (qreal)(hw) / m_AreaAnimate->mFrameCount;
        if(m_AreaAnimate->mOrientation == Positive)
        {
            x = (qreal)(tmpDiff*index);
            p->translate(targetRect.left()+x, targetRect.top());
            DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(0,0,hw-x,hh), QRectF(0,0,hw-x,hh));
        }
        else
        {
            x = (qreal)(tmpDiff*index);
            p->translate(targetRect.left()-x, targetRect.top());
            DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(x,0,hw-x,hh), QRectF(x,0,hw-x,hh));
        }
        p->restore();
        p->save();
        if(m_AreaAnimate->mOrientation == Positive)
        {
            x = hw - x;
            p->translate(targetRect.left()-x, targetRect.top());
            DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(x,0,hw-x,hh), QRectF(x,0,hw-x,hh));
        }
        else
        {
            x = hw - x;
            p->translate(targetRect.left()+x, targetRect.top());
            DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(0,0,hw-x,hh), QRectF(0,0,hw-x,hh));
        }
        p->restore();
    }
}

void GraphicsComponent::Paint2D_None(QPainter *p, QRectF targetRect)
{
    DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect);
}

//>@需要绘制底图和特效图片
void GraphicsComponent::Paint2D_Flip(QPainter *p, QRectF targetRect)
{
    qreal w = targetRect.width();
    qreal h = targetRect.height();
    qreal hw = w / 2.0;
    qreal hh = h / 2.0;
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, targetRect);
        return;
    }
    // behind is the new pixmap
    DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect);
    if(m_AreaAnimate->mFrameCount <= 0)
        return;
    qreal halfcount =  m_AreaAnimate->mFrameCount / 2.0;
    qreal index = m_Animator.currentFrame();

    p->save();
    if (index <= halfcount)
    {
        // the top part of the old pixmap is flipping
        qreal angle;
        if(m_AreaAnimate->mOrientation == Positive)
        {
            angle = -180.0 * index / m_AreaAnimate->mFrameCount;
        }
        else
        {
            angle = 180.0 * index / m_AreaAnimate->mFrameCount;
        }
        p->translate(targetRect.left() + hw, targetRect.top() + hh);
        if(m_AreaAnimate->mDirection == Horizontal)
            p->setTransform(QTransform().rotate(angle, Qt::YAxis), true);
        else
            p->setTransform(QTransform().rotate(angle, Qt::XAxis), true);
        DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(-hw, -hh, w, h), QRectF(0,0,w,h));

        // the bottom part is still the old pixmap
        p->restore();
        p->save();
        p->translate(targetRect.left(), targetRect.top());
        if(m_AreaAnimate->mDirection == Horizontal)
            if(m_AreaAnimate->mOrientation == Positive)
                p->setClipRect(hw, 0, hw, targetRect.height());
            else
                p->setClipRect(0, 0, hw, targetRect.height());
        else
            if(m_AreaAnimate->mOrientation == Positive)
                p->setClipRect(0, hh, targetRect.width(), hh);
            else
                p->setClipRect(0, 0, targetRect.width(), hh);
        DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(0,0,w,h), QRectF(0,0,w,h));
    }
    else
    {
        p->translate(targetRect.left(), targetRect.top());
        if(m_AreaAnimate->mDirection == Horizontal)
            if(m_AreaAnimate->mOrientation == Positive)
                p->setClipRect(hw, 0, hw, targetRect.height());
            else
                p->setClipRect(0, 0, hw, targetRect.height());
        else
            if(m_AreaAnimate->mOrientation == Positive)
                p->setClipRect(0, hh, targetRect.width(), hh);
            else
                p->setClipRect(0, 0, targetRect.width(), hh);

        // the bottom part is still the old pixmap
        DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(0,0,w,h), QRectF(0,0,w,h));
        // the bottom part of the new pixmap is flipping
        qreal angle;
        if(m_AreaAnimate->mOrientation == Positive)
        {
            angle = 180.0 - 180.0 * index / m_AreaAnimate->mFrameCount;
        }
        else
        {
            angle = - 180.0 * (m_AreaAnimate->mFrameCount-index) / m_AreaAnimate->mFrameCount;
        }
        p->translate(hw, hh);
        if(m_AreaAnimate->mDirection == Horizontal)
            p->setTransform(QTransform().rotate(angle, Qt::YAxis), true);
        else
            p->setTransform(QTransform().rotate(angle, Qt::XAxis), true);
        DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(-hw, -hh, w, h), QRectF(0,0,w,h));
    }
    p->restore();
}

//>@需要绘制底图和特效图片
void GraphicsComponent::Paint2D_Zoom(QPainter *p, QRectF targetRect)
{
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, targetRect);
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
    {
        IDE_TRACE();
        return;
    }
    qreal halfcount =  m_AreaAnimate->mFrameCount / 2.0;
    qreal index = m_Animator.currentFrame();

    qreal x,y,w,h;
    p->save();
    p->translate(targetRect.left()+hw/2, targetRect.top()+hh/2);
    if(index < halfcount)
    {
        index = halfcount - index;
        x = -hw*index/m_AreaAnimate->mFrameCount;
        y = -hh*index/m_AreaAnimate->mFrameCount;
        w = 2*hw*index/m_AreaAnimate->mFrameCount;
        h = 2*hh*index/m_AreaAnimate->mFrameCount;
        DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(x,y,w,h), QRectF(0,0,hw,hh));
    }
    else
    {
        index = index - halfcount;
        x = -hw*index/m_AreaAnimate->mFrameCount;
        y = -hh*index/m_AreaAnimate->mFrameCount;
        w = 2*hw*index/m_AreaAnimate->mFrameCount;
        h = 2*hh*index/m_AreaAnimate->mFrameCount;
        DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(x,y,w,h), QRectF(0,0,hw,hh));
    }
    p->restore();
}

void GraphicsComponent::Paint2D_Blink(QPainter *p,QRectF targetRect)
{
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, targetRect);
        return;
    }
    p->save();
    int index = m_Animator.currentFrame();
    if(index % 2)
        p->setOpacity(0.2);
    else
        p->setOpacity(1);
    Paint2D_None(p, targetRect);
    p->restore();
}

//>@翻转
void GraphicsComponent::Paint2D_Twirl(QPainter *p, QRectF targetRect)
{
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, rect());
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
    {
        IDE_TRACE();
        return;
    }
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    qreal index = m_Animator.currentFrame();
    p->save();
    p->translate(targetRect.left()+hw/2, targetRect.top()+hh/2);
    qreal angle = 0;
    if(m_AreaAnimate->mOrientation == Positive)
    {
        angle = 360.0 * index / m_AreaAnimate->mFrameCount;
    }
    else if(m_AreaAnimate->mOrientation == Negative)
    {
        angle = - 360.0 * index / m_AreaAnimate->mFrameCount;
    }
    p->rotate(angle);
    p->restore();
}

//>@需要绘制底图和特效图片
void GraphicsComponent::Paint2D_Rotate(QPainter *p, QRectF targetRect)
{
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, rect());
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
    {
        IDE_TRACE();
        return;
    }
    qreal halfCount =  m_AreaAnimate->mFrameCount / 2.0;
    qreal index = m_Animator.currentFrame();

    p->save();
    p->translate(targetRect.left()+hw/2.0, targetRect.top()+hh/2.0);
    QPixmap pix = (index <= halfCount) ? m_PixmapPointer.mLastPointer : m_PixmapPointer.mCurPointer;

    qreal angle = (index <= halfCount) ? (-180.0 * index / m_AreaAnimate->mFrameCount) : (180.0 - 180.0 * index / m_AreaAnimate->mFrameCount);
    if(m_AreaAnimate->mDirection == Horizontal)
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

//>@需要绘制底图和特效图片
void GraphicsComponent::Paint2D_Slipcycle(QPainter *p, QRectF targetRect)
{
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, targetRect);
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
        return;
    qreal halfCount =  m_AreaAnimate->mFrameCount / 2.0;
    if(halfCount <= 0)
        return;
    qreal index = m_Animator.currentFrame();

    p->save();
    QPixmap tmpPixmap;
    qreal x, y, tmpDiff;
    if(m_AreaAnimate->mDirection == Vertical)
    {
        tmpDiff = (qreal)(hh) / halfCount;
        if(index <= halfCount)
        {
            tmpPixmap = m_PixmapPointer.mLastPointer;
            y = (int)(tmpDiff*index);
            if(y < 0)
                y = 0;
            if(m_AreaAnimate->mOrientation == Positive)
            {
                p->translate(targetRect.left(), targetRect.top()+y);
                //>@如果使用这种方法，则会刷出按钮范围，不过依然是一个比较好看的特效。
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, targetRect.size(), QRectF(0,0,hw,hh-y), QRectF(0,0,hw,hh-y));
            }
            else
            {
                p->translate(targetRect.left(), targetRect.top()-y);
                //>@如果使用这种方法，则会刷出按钮范围，不过依然是一个比较好看的特效。
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, targetRect.size(), QRectF(0,y,hw,hh-y), QRectF(0,y,hw,hh-y));
            }
        }
        else
        {
            y = (int)(tmpDiff*(m_AreaAnimate->mFrameEnd - index));
            tmpPixmap = m_PixmapPointer.mCurPointer;
            if(y < 0)
                y = 0;
            if(m_AreaAnimate->mOrientation == Positive)
            {
                p->translate(targetRect.left(), targetRect.top()+y);
                //>@如果使用这种方法，则会刷出按钮范围，不过依然是一个比较好看的特效。
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, targetRect.size(), QRectF(0,0,hw,hh-y), QRectF(0,0,hw,hh-y));
            }
            else
            {
                p->translate(targetRect.left(), targetRect.top()-y);
                //>@如果使用这种方法，则会刷出按钮范围，不过依然是一个比较好看的特效。
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, targetRect.size(), QRectF(0,y,hw,hh-y), QRectF(0,y,hw,hh-y));
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
            tmpPixmap = m_PixmapPointer.mLastPointer;
            if(m_AreaAnimate->mOrientation == Positive)
            {
                p->translate(targetRect.left()+x, targetRect.top());
                //>@如果使用这种方法，则会刷出按钮范围，不过依然是一个比较好看的特效。
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, targetRect.size(), QRectF(0,0,hw-x,hh), QRectF(0,0,hw-x,hh));
            }
            else
            {
                p->translate(targetRect.left()-x, targetRect.top());
                //>@如果使用这种方法，则会刷出按钮范围，不过依然是一个比较好看的特效。
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, targetRect.size(), QRectF(x,0,hw-x,hh), QRectF(x,0,hw-x,hh));
            }
        }
        else
        {
            x = (int)(tmpDiff*(m_AreaAnimate->mFrameEnd - index));
            if(x < 0)
                x = 0;
            tmpPixmap = m_PixmapPointer.mCurPointer;
            if(m_AreaAnimate->mOrientation == Positive)
            {
                p->translate(targetRect.left()+x, targetRect.top());
                //>@如果使用这种方法，则会刷出按钮范围，不过依然是一个比较好看的特效。
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, targetRect.size(), QRectF(0,0,hw-x,hh), QRectF(0,0,hw-x,hh));
            }
            else
            {
                p->translate(targetRect.left()-x, targetRect.top());
                //>@如果使用这种方法，则会刷出按钮范围，不过依然是一个比较好看的特效。
                //painter->drawPixmap(0, 0, m_PixList[PressEffect]);
                DrawPixmap(p, tmpPixmap, targetRect.size(), QRectF(x,0,hw-x,hh), QRectF(x,0,hw-x,hh));
            }
        }
    }
    p->restore();
}

void GraphicsComponent::Paint2D_Fireworks(QPainter *p, QRectF targetRect)
{

}

//>@需要绘制底图和特效图片
void GraphicsComponent::Paint2D_Transparent(QPainter *p, QRectF targetRect)
{
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, targetRect);
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
    {
        IDE_TRACE();
        return;
    }
    qreal Stage1 = m_AreaAnimate->mFrameCount*3.0/10.0;
    qreal Stage2 = m_AreaAnimate->mFrameCount*4.0/10.0;
    //qreal Stage3 = (qreal)(m_AreaAnimate->mFrameCount)*3.0/10.0;

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
    p->translate(targetRect.left(), targetRect.top());
    p->setOpacity(tmpLastAlpha);
    DrawPixmap(p, m_PixmapPointer.mLastPointer, targetRect.size(), QRectF(0,0,hw,hh), QRectF(0,0,hw,hh));
    p->setOpacity(tmpAlpha);
    DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(0,0,hw,hh), QRectF(0,0,hw,hh));
    p->restore();
}

//>@只需要绘制特效图片即可
void GraphicsComponent::Paint2D_Spread(QPainter *p, QRectF targetRect)
{
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    qreal x,y,h,w;
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, targetRect);
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
        return;
    qreal halfCount =  m_AreaAnimate->mFrameCount / 2.0;
    qreal index = m_Animator.currentFrame();

    if(index < halfCount)
    {
        index = halfCount - index;
        x = -hw*index/m_AreaAnimate->mFrameCount;
        y = -hh*index/m_AreaAnimate->mFrameCount;
        w = 2*hw*index/m_AreaAnimate->mFrameCount;
        h = 2*hh*index/m_AreaAnimate->mFrameCount;
    }
    else
    {
        index = index - halfCount;
        x = -hw*index/m_AreaAnimate->mFrameCount;
        y = -hh*index/m_AreaAnimate->mFrameCount;
        w = 2*hw*index/m_AreaAnimate->mFrameCount;
        h = 2*hh*index/m_AreaAnimate->mFrameCount;
    }
    p->save();
    p->translate(targetRect.left()+hw/2, targetRect.top()+hh/2);
    DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(x,y,w,h), QRectF(0,0,hw,hh));
    p->restore();
}

//>@只需要绘制特效图片即可
void GraphicsComponent::Paint2D_Wheel(QPainter *p, QRectF targetRect)
{
    qreal hh = targetRect.height();
    qreal hw = targetRect.width();
    if(m_Animator.state() != QTimeLine::Running)
    {
        Paint2D_None(p, targetRect);
        return;
    }
    if(m_AreaAnimate->mFrameCount <= 0)
        return;
    qreal index = m_Animator.currentFrame();

    qreal tmpDiff = (qreal)(2.0 * PI) / (qreal)(m_AreaAnimate->mFrameCount) * 2.0;
    qreal rotate = 360.0 * normalizeAngle(tmpDiff*index) / 2.0 /PI;

    p->save();
    p->translate(targetRect.left()+hw/2, targetRect.top()+hh/2);
    p->rotate(rotate);
    DrawPixmap(p, m_PixmapPointer.mCurPointer, targetRect.size(), QRectF(-hw/2.0, -hh/2.0, hw, hh), QRectF(0,0,hw,hh));
    p->restore();
}

void GraphicsComponent::PressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event && event->button() == Qt::LeftButton)
    {
        if(m_EffectValid)
        {
            updateEffect(OPERATE_KEYPRESS, QVariant(mapFromScene(event->scenePos())));
        }
    }
    //QGraphicsWidget::mousePressEvent(event);
}

void GraphicsComponent::MoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event && event->button() == Qt::LeftButton)
    {
        if(m_EffectValid)
        {
            updateEffect(OPERATE_KEYMOVE, QVariant(mapFromScene(event->scenePos())));
        }
    }
    //QGraphicsWidget::mouseMoveEvent(event);
}

void GraphicsComponent::ReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event && event->button() == Qt::LeftButton)
    {
        if(m_EffectValid)
        {
            updateEffect(OPERATE_KEYRELEASE, QVariant(mapFromScene(event->scenePos())));
        }
        emit MouseRelease(event->scenePos());
    }
    //QGraphicsWidget::mouseReleaseEvent(event);
}

void GraphicsComponent::keyPressEvent(QKeyEvent *event)
{
    if(m_EffectValid == false)
        return;
    switch (event->key())
    {
#ifdef  SURRPORT_3D
        case Qt::Key_Up:
            if(event->modifiers() == Qt::ControlModifier)
                m_3DYDiff += 2;
            else if(event->modifiers() == Qt::AltModifier)
                m_3DYScale += 2;
            else if(event->modifiers() == Qt::ShiftModifier)
                m_3DDeepDiff += 0.1;
            break;
        case Qt::Key_Down:
            if(event->modifiers() == Qt::ControlModifier)
                m_3DYDiff -= 2;
            else if(event->modifiers() == Qt::AltModifier)
                m_3DYScale -= 2;
            else if(event->modifiers() == Qt::ShiftModifier)
                m_3DDeepDiff -= 0.1;
            break;
        case Qt::Key_Left:
            if(event->modifiers() == Qt::ControlModifier)
                m_3DXDiff += 2;
            else if(event->modifiers() == Qt::AltModifier)
                m_3DXScale += 2;
            else if(event->modifiers() == Qt::ShiftModifier)
                m_3DDeepDiff += 0.1;
            break;
        case Qt::Key_Right:
            if(event->modifiers() == Qt::ControlModifier)
                m_3DXDiff -= 2;
            else if(event->modifiers() == Qt::AltModifier)
                m_3DXScale -= 2;
            else if(event->modifiers() == Qt::ShiftModifier)
                m_3DDeepDiff -= 0.1;
            break;
#endif
        default:
            return;
    }
    QGraphicsItem::keyPressEvent(event);
}

#ifdef  SURRPORT_3D
void GraphicsComponent::setGeometry3D(qreal x, qreal y, qreal width, qreal height)
{
    setPos(x,y);
    m_3DWidth = width;
    m_3DHeight = height;
}

void GraphicsComponent::setGeometry3D(QRectF rect)
{
    setPos(rect.left(),rect.top());
    m_3DWidth = rect.width();
    m_3DHeight = rect.height();
}

void GraphicsComponent::BindTexture(GLuint L_Texture,GLfloat R,GLfloat G,GLfloat B)
{
    glPushMatrix();
    glMatrixMode(GL_TEXTURE);
    //>@物体自身发光
    GLfloat matDiff[] = {R, G, B, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiff);
    //>@绑定纹理
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, L_Texture);

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void GraphicsComponent::UnbindTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void GraphicsComponent::OpenGL_PaintBegin(QPainter *painter)
{
    //>@创建GLWidget资源
    if(m_GLWidget == 0)
        m_GLWidget = new GraphicsGLWidget;

    QRectF rect = boundingRect().translated(pos()).normalized();
    float width = float(painter->device()->width());
    float height = float(painter->device()->height());
    float left = 2.0f * float(rect.left()) / width - 1.0f;
    float right = 2.0f * float(rect.right()) / width - 1.0f;
    float top = 1.0f - 2.0f * float(rect.top()) / height;
    float bottom = 1.0f - 2.0f * float(rect.bottom()) / height;
    float moveToRectMatrix[] = {
                                    0.5f* (right - left), 0.0f, 0.0f, 0.0f,
                                    0.0f, 0.5f* (bottom - top), 0.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f, 0.0f,
                                    0.5f* (right + left), 0.5f* (bottom + top), 0.0f, 1.0f
                                };
    painter->beginNativePainting();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(moveToRectMatrix);
    //>@视角不支持
    //glFrustum(-1, 1, -1, 1, 20.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    //>Enable  Disable
    //>@视角不支持，Z轴深度不支持，采用修改投影矩阵/glscale实现缩放
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_FLAT);
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    //>@非可见面删除，如果关闭则极大影响性能
    glEnable(GL_CULL_FACE);
    //>@GL_CCW 表示窗口坐标上投影多边形的顶点顺序为逆时针方向的表面为正面。
    //>@GL_CW 表示顶点顺序为顺时针方向的表面为正面。顶点的方向又称为环绕。
    glFrontFace(GL_CCW);
    //>@混色不支持，采用物体自发光方式绕过
    //glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    //glEnable(GL_DEPTH_TEST);
    //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_LIGHTING);
    //>@透明度支持存在问题
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);
    //glDisable(GL_DEPTH_TEST);
    //glColor4f( 1.0, 1.0, 1.0, 0.13 );
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //glEnable(GL_BLEND);
    //>@禁用抖动特效，以提高性能。
    glDisable(GL_DITHER);
    float lightColour0[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float lightDir0[] = {0.0f, 0.0f, 10.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightDir0);
    glEnable(GL_LIGHT0);

    float lightColour1[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float lightDir1[] = {0.0f, 0.0f, -10.0f, 1.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColour1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightDir1);
    glEnable(GL_LIGHT1);

    float lightColour2[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float lightDir2[] = {0.0f, 10.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColour2);
    glLightfv(GL_LIGHT2, GL_POSITION, lightDir2);
    glEnable(GL_LIGHT2);

    float lightColour3[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float lightDir3[] = {0.0f, -10.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lightColour3);
    glLightfv(GL_LIGHT3, GL_POSITION, lightDir3);
    glEnable(GL_LIGHT3);

    float lightColour4[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float lightDir4[] = {10.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT4, GL_DIFFUSE, lightColour4);
    glLightfv(GL_LIGHT4, GL_POSITION, lightDir4);
    glEnable(GL_LIGHT4);

    float lightColour5[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float lightDir5[] = {-10.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT5, GL_DIFFUSE, lightColour5);
    glLightfv(GL_LIGHT5 ,GL_POSITION, lightDir5);
    glEnable(GL_LIGHT5);

    //GLfloat light_ambient[]={0.7,0.8,0.9,1.0};
    //GLfloat light_diffuse[]={1.0,1.0,1.0,1.0};
    //GLfloat light_specular[]={1.0,1.0,1.0,1.0};
    //GLfloat light_position[]={2.0,2.0,2.0,0.0};
    //GLfloat mat_diffuse[]={0.8,0.8,0.8,1.0};

    //glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
    //glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
    //glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
    //glLightfv(GL_LIGHT0,GL_POSITION,light_position);

    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_AUTO_NORMAL);
    //glEnable(GL_NORMALIZE);
    //glEnable(GL_CULL_FACE);
    //启用双面光照
    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
    //glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
}

void GraphicsComponent::OpenGL_PaintEnd(QPainter *painter)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glDisable(GL_NORMALIZE);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHT0);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    painter->endNativePainting();
}

void GraphicsComponent::OpenGL_Effect(QPainter *painter)
{
    //>@旋转
    //glTranslatef(0.0f,0.5f,0.0f);
    glRotatef(-m_3DXDiff,1.0f,0.0f,0.0f);
    glRotatef(-m_3DYDiff,0.0f,1.0f,0.0f);
    glRotatef(-m_3DZDiff,0.0f,0.0f,1.0f);
    //>@缩放特效
    glScalef(m_3DXScale, m_3DYScale, m_3DZScale);
}

void GraphicsComponent::Set3DEffect(EffectType pType)
{

}
#endif

