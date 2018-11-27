#ifndef GraphicsARROW_H
#define GraphicsARROW_H

#include "graphicsoperate.h"

#ifdef IDE
    #define ComArrow  DesignArrow
#elif DISPLAY
    #define ComArrow  GraphicsArrow
#endif

//>@Arrow只支持滚动特效
class ArrowThread;
class GraphicsArrow : public GraphicsOperate
{
    Q_OBJECT
public:
    GraphicsArrow(QGraphicsItem *parent);
    ~GraphicsArrow();
    bool Start();
    bool PaintEffect(QPainter *p);
    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }
    void UpdateAnimate();

    void OperateShow(QVariant pPara);
    void OperateHide(QVariant pPara);

signals:
    void sUpdate();
    void sRcChanged();
    void sShowNull();

public slots:
    void slot_ShowNull();
    void slot_Update();
    void slot_RcChanged();
    void slot_Monitor();
    virtual void ExecAction(QAction *pAction) {}
    void UpdateCom(int pFrame) { GraphicsComponent::UpdateCom(pFrame); }
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);

private:
    int             m_CurRcLabel[2]; //>@0:表示当前正在用的，1：表示下次将要用的
    AREA_OPERATE    m_CurOperate[2];

    ArrowThread    *m_MonitorThread;
    int             m_CurFrameNum;  //>@当前帧计数
    int             m_OperateStatus; //>@0:没有操作 1：正在操作 2：切换过程，保持为空图片

    int             m_Status2Count; //>@保持为空的时间
    int             m_Status2Num;

    bool            m_ShowFlag;
};

class ArrowThread : QThread
{
    Q_OBJECT
    friend class GraphicsArrow;
public:
    ArrowThread(GraphicsArrow *qesp);
    ~ArrowThread();
    void ForceQuit() {m_Quit = true;}
    void run();
    void Wait(int &pTimeout)
    {
        quint32 i=0;
        while((i+=10) < pTimeout && !m_Quit)
            msleep(10);
    }
public:
    int                     m_DefDelay;
    bool                    m_Quit;
    QPointer<GraphicsArrow> m_Arrow;
};

class DesignArrow : public GraphicsArrow
{
    Q_OBJECT
public:
    DesignArrow(QGraphicsItem *parent);

    bool Start()
    {
        if(GraphicsComponent::Start() == false)
            return false;
        updateEffect(OPERATE_CHANGE, QVariant(m_RcList.first()));
        return true;
    }

    QSize GetRcSize(QString pRcName){ return size().toSize(); }
    QList<QAction*> GetActionList();

    bool PaintEffect(QPainter *p)
    {
        GraphicsArrow::PaintEffect(p);
        //>@绘制边框
        QRectF tmpRect = geometry();
        qreal Width = tmpRect.width();
        qreal Height = tmpRect.height();
        p->translate(Width / 2, Height / 2);
        p->setPen(Qt::DashDotDotLine);
        p->setPen(QColor(10,10,255));
        p->drawRect(QRect(-Width/2, -Height/2, Width, Height));
        p->drawRect(QRect(-Width/2+1, -Height/2+1, Width-2, Height-2));
        return true;
    }

public slots:
    void ExecAction(QAction *pAction);
};

#endif // GraphicsARROW_H

