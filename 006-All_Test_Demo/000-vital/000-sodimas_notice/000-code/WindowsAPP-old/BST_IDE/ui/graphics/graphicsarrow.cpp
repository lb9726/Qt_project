#include "graphicsarrow.h"
#include "logicbase.h"
#ifdef IDE
    #include "devicescene.h"
#elif DISPLAY
    #include "devicemanager.h"
#elif SETTING
    #include "devicesetting.h"
#endif

GraphicsArrow::GraphicsArrow(QGraphicsItem *parent) :
    GraphicsOperate(parent)
{
    m_AreaAnimate = 0;
    m_Status2Count = 0;
    m_Status2Num = 0;
    m_OperateStatus = 0;
    m_CurFrameNum = 0;
    m_CurRcLabel[0] = INVALIDRC;
    m_CurRcLabel[1] = BGRC;
    m_CurOperate[0] = m_CurOperate[1] = OPERATE_NONE;
    m_MonitorThread = new ArrowThread(this);
    connect(this, SIGNAL(sUpdate()), this, SLOT(slot_Update()), Qt::AutoConnection);
    connect(this, SIGNAL(sRcChanged()), this, SLOT(slot_RcChanged()), Qt::AutoConnection);
    connect(this, SIGNAL(sShowNull()), this, SLOT(slot_ShowNull()), Qt::AutoConnection);
}

GraphicsArrow::~GraphicsArrow()
{
    m_MonitorThread->ForceQuit();
    m_MonitorThread->wait(3000);
    m_MonitorThread->deleteLater();
    m_MonitorThread = 0;
}

bool GraphicsArrow::Start()
{
    m_OperateStatus = -1;
    if(GraphicsOperate::Start() == false)
        return false;
    if(!m_MonitorThread->isRunning())
        m_MonitorThread->start(QThread::TimeCriticalPriority);
    m_CurOperate[0] = OPERATE_NONE;
    m_CurRcLabel[0] = INVALIDRC;
    m_OperateStatus = 0;
    m_Status2Count = 2000/m_MonitorThread->m_DefDelay;
    return true;
}

bool GraphicsArrow::PaintEffect(QPainter *p)
{
    if(m_EffectValid == false)
    {
        return false;
    }
    if(m_PixmapPointer.mLastPointer.isNull() && m_PixmapPointer.mCurPointer.isNull())
    {
        return false;
    }
    //IDE_TRACE();
    if(!(m_AreaAnimate && (m_AreaAnimate->mFrameCount > 0) && (m_CurFrameNum > 0)))
    {
        Paint2D_None(p, rect());
        return false;
    }
    qreal hh = size().height();
    qreal hw = size().width();
    qreal tmpDiff;
    qreal index = m_CurFrameNum;
    qreal y;
    //>@首先将坐标原点移到当前需要绘制的区域。
    p->save();
    tmpDiff = hh / m_AreaAnimate->mFrameCount;
    if(m_AreaAnimate->mOrientation == Positive)
    {
        y = (qreal)(tmpDiff*index);
        p->translate(rect().left(), rect().top()+y);
        DrawPixmap(p, m_PixmapPointer.mLastPointer, rect().size(), QRectF(0,0,hw,hh-y), QRectF(0,0,hw,hh-y));
    }
    else
    {
        y = (qreal)(tmpDiff*index);
        p->translate(rect().left(), rect().top()-y);
        DrawPixmap(p, m_PixmapPointer.mLastPointer, rect().size(), QRectF(0,y,hw,hh-y), QRectF(0,y,hw,hh-y));
    }
    p->restore();
    p->save();
    if(m_AreaAnimate->mOrientation == Positive)
    {
        y = hh - y;
        p->translate(rect().left(), rect().top()-y);
        DrawPixmap(p, m_PixmapPointer.mCurPointer, rect().size(), QRectF(0,y,hw,hh-y), QRectF(0,y,hw,hh-y));
    }
    else
    {
        y = hh - y;
        p->translate(rect().left(), rect().top()+y);
        DrawPixmap(p, m_PixmapPointer.mCurPointer, rect().size(), QRectF(0,0,hw,hh-y), QRectF(0,0,hw,hh-y));
    }
    p->restore();
    return true;
}

void GraphicsArrow::slot_Update()
{
    update(rect());
}

void GraphicsArrow::slot_ShowNull()
{
    m_PixmapPointer.mLastPointer = m_PixmapPointer.mCurPointer = QPixmap();
    slot_Update();
}

void GraphicsArrow::slot_RcChanged()
{
    static int oldLabel = INVALIDRC;
    if(oldLabel != m_CurRcLabel[0] || (m_PixmapPointer.mLastPointer.isNull() && m_PixmapPointer.mCurPointer.isNull()))
    {
        oldLabel = m_CurRcLabel[0];
        m_PixmapPointer.mLastPointer = m_PixmapPointer.mCurPointer = GetPixmap(m_CurRcLabel[0], size());
    }
}

void GraphicsArrow::UpdateAnimate()
{
    m_AreaAnimate = m_EffectGroup.value(m_CurOperate[0]);
    if(m_AreaAnimate && (m_CurOperate[0] == OPERATE_RUN))
    {
        if(m_CurRcLabel[0] == ArrowUp)
        {
            m_AreaAnimate->mOrientation = Negative;
        }
        else if(m_CurRcLabel[0] == ArrowDown)
        {
            m_AreaAnimate->mOrientation = Positive;
        }
        m_AreaAnimate->mDirection = Vertical;
        m_AreaAnimate->mLoopCount = 0;
        m_AreaAnimate->mFrameCount = m_AreaAnimate->mAnimateTime/m_MonitorThread->m_DefDelay; //>@不能+1，否则会出现花屏现象。
    }
}

//>@不论遇到任何变化，都要把此次操作完成后再执行新操作。
void GraphicsArrow::slot_Monitor()
{
    if(!isVisible())
    {
        //IDE_TRACE();
        return;
    }
    if(m_OperateStatus == 0)  //>@只有在没有操作的情况下采取检测是否有新的动作来了
    {
        bool tmpArrowStateChange = false;
        bool tmpOperateChange = false;
        if(m_CurRcLabel[0] != m_CurRcLabel[1])
            tmpArrowStateChange = true;
        if(m_CurOperate[0] != m_CurOperate[1])
            tmpOperateChange = true;
        //>@根据新的动作设置参数
        //>@如果箭头资源发生变化，则进入运行模式
        if(tmpArrowStateChange)
        {
            m_CurOperate[0] = m_CurOperate[1];
            m_CurRcLabel[0] = m_CurRcLabel[1];
            if((m_CurOperate[0] == OPERATE_CHANGE) && (m_CurRcLabel[0] == BGRC))
            {
                m_Status2Num = 0;
                m_OperateStatus = 2;
            }
            else
            {
                UpdateAnimate();
                emit sRcChanged();
                m_OperateStatus = 1;
            }
        }
        else if(tmpOperateChange)
        {
            m_CurOperate[0] = m_CurOperate[1];
            UpdateAnimate();
            m_OperateStatus = 1;
        }
        else if(m_CurOperate[0] == OPERATE_RUN)
        {
            //UpdateAnimate();
            m_OperateStatus = 1;
        }
        m_CurFrameNum = 0;
    }
    if(m_OperateStatus == 1)
    {
        if(m_CurOperate[0] == OPERATE_RUN)
        {
            if(m_AreaAnimate)
            {
                m_CurFrameNum ++;
                if(m_CurFrameNum >= m_AreaAnimate->mFrameCount) //>@要用》=号，否则会出现停顿现象。
                {
                    m_CurFrameNum = 0;
                    m_OperateStatus = 0;
                }
                emit sUpdate();
            }
            else
            {
                m_CurFrameNum = 0;
                emit sUpdate();
                m_OperateStatus = 0;
            }
        }
        else
        {
            m_CurFrameNum = 0;
            emit sUpdate();
            m_OperateStatus = 0;
        }
    }
    if(m_OperateStatus == 2)
    {
        emit sShowNull();
        m_Status2Num ++;
        if(m_Status2Num >= m_Status2Count)
        {
            m_Status2Num = 0;
            UpdateAnimate();
            emit sRcChanged();
            m_OperateStatus = 1;
        }
    }
}

void GraphicsArrow::OperateShow(QVariant pPara)
{
    if(m_ComType >= COM_FLOOR)
    {
        //if(m_OperateStatus == -1)
        //    m_OperateStatus = 0;
        show();
    }
}

void GraphicsArrow::OperateHide(QVariant pPara)
{
    if(m_ComType >= COM_FLOOR)
    {
        //m_OperateStatus = -1;
        hide();
    }
}

void GraphicsArrow::updateEffect(AREA_OPERATE pOperate, QVariant pPara)
{
    GraphicsComponent::updateEffect(pOperate, pPara);
    if(pOperate != OPERATE_CHANGE)
        return;
    int tmpArrowState = pPara.toInt();
    switch(tmpArrowState)
    {
        case ArrowUp:
        {
            m_CurOperate[1] = OPERATE_CHANGE;
            m_CurRcLabel[1] = ArrowUp;
            if(m_OperateStatus == 2)
                m_OperateStatus = 0;
            break;
        }
        case ArrowUproll:
        {
            m_CurOperate[1] = OPERATE_RUN;
            m_CurRcLabel[1] = ArrowUp;
            if(m_OperateStatus == 2)
                m_OperateStatus = 0;
            break;
        }
        case ArrowDown:
        {
            m_CurOperate[1] = OPERATE_CHANGE;
            m_CurRcLabel[1] = ArrowDown;
            if(m_OperateStatus == 2)
                m_OperateStatus = 0;
            break;
        }
        case ArrowDownroll:
        {
            m_CurOperate[1] = OPERATE_RUN;
            m_CurRcLabel[1] = ArrowDown;
            if(m_OperateStatus == 2)
                m_OperateStatus = 0;
            break;
        }
        case ArrowUpDown:
        case ArrowUpDownroll:
        {
            m_CurOperate[1] = OPERATE_CHANGE;
            m_CurRcLabel[1] = ArrowUpDown;
            if(m_OperateStatus == 2)
                m_OperateStatus = 0;
            break;
        }
        default:
        {
            m_CurOperate[1] = OPERATE_CHANGE;
            m_CurRcLabel[1] = BGRC;
            break;
        }
    }
}

ArrowThread::ArrowThread(GraphicsArrow *qesp)
    :QThread()
{
    m_DefDelay = 80;
    m_Arrow = qesp;
    m_Quit = false;
}

ArrowThread::~ArrowThread()
{
    m_Quit = true;
}

void ArrowThread::run()
{
    while(!m_Quit)
    {
        GraphicsArrow *tmpArrow = m_Arrow.data();
        if(tmpArrow)
            tmpArrow->slot_Monitor();
        msleep(m_DefDelay);
        //Wait(m_DefDelay);
    }
}

DesignArrow::DesignArrow(QGraphicsItem *parent) :
    GraphicsArrow(parent)
{
    QAction *tmpDefault = m_ActionGroup->addAction(tr("UP"));
    m_ActionGroup->addAction(tr("UPROLL"));
    m_ActionGroup->addAction(tr("UPHALT"));
    m_ActionGroup->addAction(tr("DOWN"));
    m_ActionGroup->addAction(tr("DOWNROLL"));
    m_ActionGroup->addAction(tr("DOWNHALT"));
    m_ActionGroup->addAction(tr("UPDOWN"));
    m_ActionGroup->addAction(tr("UPDOWNROLL"));
    m_ActionGroup->addAction(tr("UPDOWNHALT"));
    tmpDefault->setChecked(true);
}

QList<QAction*> DesignArrow::GetActionList()
{
    QList<QAction*> tmpList;
    if(m_ActionGroup)
    {
        bool hasUP = m_EffectRC.contains(ArrowUp);
        bool hasDOWN = m_EffectRC.contains(ArrowDown);
        bool hasUPDOWN = m_EffectRC.contains(ArrowUpDown);
        tmpList = m_ActionGroup->actions();
        //>@根据当前已有的资源使能响应功能Action
        for(int i=0;i<tmpList.count();i++)
        {
            QAction *tmpAction = tmpList.at(i);
            if(tmpAction == 0)
                continue;
            ARROWSTATE tmpState = GetArrowState(tmpAction->text());
            if(tmpState == ArrowUnknow)
                continue;
            if((tmpState == ArrowUp || tmpState == ArrowUproll || tmpState == ArrowUphalt) && hasUP)
                tmpAction->setEnabled(true);
            else if((tmpState == ArrowDown || tmpState == ArrowDownroll || tmpState == ArrowDownhalt) && hasDOWN)
                tmpAction->setEnabled(true);
            else if((tmpState == ArrowUpDown || tmpState == ArrowUpDownroll || tmpState == ArrowUpDownhalt) && hasUPDOWN)
                tmpAction->setEnabled(true);
            else
                tmpAction->setDisabled(true);
        }
    }
    return tmpList;
}

void DesignArrow::ExecAction(QAction *pAction)
{
    updateEffect(OPERATE_CHANGE, QVariant(GetArrowState(pAction->text())));
}
