#ifndef GRAPHICSREGION_H
#define GRAPHICSREGION_H

#include "graphicsoperate.h"

class GraphicsRegion : public GraphicsOperate
{
    Q_OBJECT
public:
    GraphicsRegion(QGraphicsItem *parent);

    bool Start();
    virtual void updateEffect(int pState, AREA_OPERATE pOperate);

    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }

public slots:
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    void UpdateCom(int pFrame);

public:
    int            m_RegionState;
};

//>@################################################################################
//>@################################################################################
#ifdef IDE
    #define ComDesktop  DesignDesktop
#elif DISPLAY
    #define ComDesktop  GraphicsDesktop
#endif

class GraphicsDesktop : public GraphicsRegion
{
    Q_OBJECT
public:
    GraphicsDesktop(QGraphicsItem *parent);
    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }

    void OperateTurn(QVariant pPara);
    bool Start();

public slots:
    virtual void ExecAction(QAction *pAction) {}  

public:   //>@只有Desktop横竖版图片不一样，《100的为横版图片，》100的为竖版图片
    QList<int>    m_HRcList;       //>@
    QList<int>    m_VRcList;       //>@
};

class DesignDesktop : public GraphicsDesktop
{
    Q_OBJECT
public:
    DesignDesktop(QGraphicsItem *parent) :
        GraphicsDesktop(parent)
    {
        QAction *tmpDefault = m_ActionGroup->addAction(tr("0"));
        for(int i=1;i<10;i++)
        {
            m_ActionGroup->addAction(QString("%1").arg(i));
        }
        tmpDefault->setChecked(true);
    }
    bool Start()
    {
        if(GraphicsComponent::Start() == false)
            return false;
        updateEffect(OPERATE_CHANGE, QVariant(m_RcList.first()));
        return true;
    }

    QSize GetRcSize(QString pRcName){ return size().toSize(); }
    QList<QAction*> GetActionList()
    {
        QList<QAction*> tmpList;
        if(m_ActionGroup)
        {
            tmpList = m_ActionGroup->actions();
            //>@根据当前已有的资源使能响应功能Action
            for(int i=0;i<tmpList.count();i++)
            {
                QAction *tmpAction = tmpList.at(i);
                if(tmpAction == 0)
                    continue;
                int tmpState = tmpAction->text().toInt();
                if(m_EffectRC.contains(tmpState))
                    tmpAction->setEnabled(true);
                else
                    tmpAction->setDisabled(true);
            }
        }
        return tmpList;
    }

    bool PaintEffect(QPainter *p)
    {
        GraphicsDesktop::PaintEffect(p);
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
    void ExecAction(QAction *pAction)
    {
        updateEffect(pAction->text().toInt(), OPERATE_STEADY);
    }
};

#ifdef IDE
    #define ComImage  DesignImage
#elif DISPLAY
    #define ComImage  GraphicsImage
#endif

class GraphicsImage : public GraphicsRegion
{
    Q_OBJECT
public:
    GraphicsImage(QGraphicsItem *parent);

    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }
public slots:
    virtual void ExecAction(QAction *pAction) {}
};

class DesignImage : public GraphicsImage
{
    Q_OBJECT
public:
    DesignImage(QGraphicsItem *parent) :
        GraphicsImage(parent)
    {
        QAction *tmpDefault = m_ActionGroup->addAction(tr("0"));
        for(int i=1;i<10;i++)
        {
            m_ActionGroup->addAction(QString("%1").arg(i));
        }
        tmpDefault->setChecked(true);
    }
    bool Start()
    {
        if(GraphicsComponent::Start() == false)
            return false;
        updateEffect(OPERATE_CHANGE, QVariant(m_RcList.first()));
        return true;
    }
    QSize GetRcSize(QString pRcName){ return size().toSize(); }
    QList<QAction*> GetActionList()
    {
        QList<QAction*> tmpList;
        if(m_ActionGroup)
        {
            tmpList = m_ActionGroup->actions();
            //>@根据当前已有的资源使能响应功能Action
            for(int i=0;i<tmpList.count();i++)
            {
                QAction *tmpAction = tmpList.at(i);
                if(tmpAction == 0)
                    continue;
                int tmpState = tmpAction->text().toInt();
                if(m_EffectRC.contains(tmpState))
                    tmpAction->setEnabled(true);
                else
                    tmpAction->setDisabled(true);
            }
        }
        return tmpList;
    }

    bool PaintEffect(QPainter *p)
    {
        GraphicsImage::PaintEffect(p);
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
    void ExecAction(QAction *pAction)
    {
        updateEffect(pAction->text().toInt(), OPERATE_STEADY);
    }
};

#ifdef IDE
    #define ComLogo  DesignLogo
#elif DISPLAY
    #define ComLogo  GraphicsLogo
#endif

class GraphicsLogo : public GraphicsRegion
{
    Q_OBJECT
public:
    GraphicsLogo(QGraphicsItem *parent);
    QPixmap LoadIcon(QPixmap pPixmap, QSizeF pSize);

    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }
public slots:
    virtual void ExecAction(QAction *pAction) {}
    void slot_ShowIcon(QString pIconName);
    void slot_HideIcon(QString pIconName);
public:
#ifdef N329
    QImage          m_TranparentImg;
#endif
    QString         m_CurIcon;
};

class DesignLogo : public GraphicsLogo
{
    Q_OBJECT
public:
    DesignLogo(QGraphicsItem *parent) :
        GraphicsLogo(parent)
    {
        QAction *tmpDefault = m_ActionGroup->addAction(tr("0"));
        for(int i=1;i<10;i++)
        {
            m_ActionGroup->addAction(QString("%1").arg(i));
        }
        tmpDefault->setChecked(true);
    }
    bool Start()
    {
        if(GraphicsComponent::Start() == false)
            return false;
        updateEffect(OPERATE_CHANGE, QVariant(m_RcList.first()));
        return true;
    }
    QSize GetRcSize(QString pRcName){ return size().toSize(); }
    QList<QAction*> GetActionList()
    {
        QList<QAction*> tmpList;
        if(m_ActionGroup)
        {
            tmpList = m_ActionGroup->actions();
            //>@根据当前已有的资源使能响应功能Action
            for(int i=0;i<tmpList.count();i++)
            {
                QAction *tmpAction = tmpList.at(i);
                if(tmpAction == 0)
                    continue;
                int tmpState = tmpAction->text().toInt();
                if(m_EffectRC.contains(tmpState))
                    tmpAction->setEnabled(true);
                else
                    tmpAction->setDisabled(true);
            }
        }
        return tmpList;
    }

    bool PaintEffect(QPainter *p)
    {
        GraphicsLogo::PaintEffect(p);
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
    void ExecAction(QAction *pAction)
    {
        updateEffect(pAction->text().toInt(), OPERATE_STEADY);
    }
};

#ifdef IDE
    #define ComWeather  DesignWeather
#elif DISPLAY
    #define ComWeather  GraphicsWeather
#endif

class GraphicsWeather : public GraphicsRegion
{
    Q_OBJECT
public:
    GraphicsWeather(QGraphicsItem *parent);

    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }
public slots:
    virtual void ExecAction(QAction *pAction) {}
};

class DesignWeather : public GraphicsWeather
{
    Q_OBJECT
public:
    DesignWeather(QGraphicsItem *parent) :
        GraphicsWeather(parent)
    {
        QAction *tmpDefault = m_ActionGroup->addAction(tr("0"));
        for(int i=1;i<10;i++)
        {
            m_ActionGroup->addAction(QString("%1").arg(i));
        }
        tmpDefault->setChecked(true);
    }
    bool Start()
    {
        if(GraphicsComponent::Start() == false)
            return false;
        updateEffect(OPERATE_CHANGE, QVariant(m_RcList.first()));
        return true;
    }
    QSize GetRcSize(QString pRcName){ return size().toSize(); }
    QList<QAction*> GetActionList()
    {
        QList<QAction*> tmpList;
        if(m_ActionGroup)
        {
            tmpList = m_ActionGroup->actions();
            //>@根据当前已有的资源使能响应功能Action
            for(int i=0;i<tmpList.count();i++)
            {
                QAction *tmpAction = tmpList.at(i);
                if(tmpAction == 0)
                    continue;
                int tmpState = tmpAction->text().toInt();
                if(m_EffectRC.contains(tmpState))
                    tmpAction->setEnabled(true);
                else
                    tmpAction->setDisabled(true);
            }
        }
        return tmpList;
    }

    bool PaintEffect(QPainter *p)
    {
        GraphicsWeather::PaintEffect(p);
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
    void ExecAction(QAction *pAction)
    {
        updateEffect(pAction->text().toInt(), OPERATE_STEADY);
    }
};

#ifdef IDE
    #define ComNamePlate  DesignNamePlate
#elif DISPLAY
    #define ComNamePlate  GraphicsNamePlate
#endif

class GraphicsNamePlate : public GraphicsRegion
{
    Q_OBJECT
public:
    GraphicsNamePlate(QGraphicsItem *parent);

    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }
public slots:
    virtual void ExecAction(QAction *pAction) {}
};

class DesignNamePlate : public GraphicsNamePlate
{
    Q_OBJECT
public:
    DesignNamePlate(QGraphicsItem *parent) :
        GraphicsNamePlate(parent)
    {
        QAction *tmpDefault = m_ActionGroup->addAction(tr("0"));
        for(int i=1;i<10;i++)
        {
            m_ActionGroup->addAction(QString("%1").arg(i));
        }
        tmpDefault->setChecked(true);
    }
    bool Start()
    {
        if(GraphicsComponent::Start() == false)
            return false;
        updateEffect(OPERATE_CHANGE, QVariant(m_RcList.first()));
        return true;
    }
    QSize GetRcSize(QString pRcName){ return size().toSize(); }
    QList<QAction*> GetActionList()
    {
        QList<QAction*> tmpList;
        if(m_ActionGroup)
        {
            tmpList = m_ActionGroup->actions();
            //>@根据当前已有的资源使能响应功能Action
            for(int i=0;i<tmpList.count();i++)
            {
                QAction *tmpAction = tmpList.at(i);
                if(tmpAction == 0)
                    continue;
                int tmpState = tmpAction->text().toInt();
                if(m_EffectRC.contains(tmpState))
                    tmpAction->setEnabled(true);
                else
                    tmpAction->setDisabled(true);
            }
        }
        return tmpList;
    }

    bool PaintEffect(QPainter *p)
    {
        GraphicsNamePlate::PaintEffect(p);
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
    void ExecAction(QAction *pAction)
    {
        updateEffect(pAction->text().toInt(), OPERATE_STEADY);
    }
};

#ifdef IDE
    #define ComFunction  DesignFunction
#elif DISPLAY
    #define ComFunction  GraphicsFunction
#endif

class GraphicsText;
class GraphicsFunction : public GraphicsRegion
{
    Q_OBJECT
public:
    GraphicsFunction(QGraphicsItem *parent);
    bool Start();
    bool hasText();
    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }
    void updateEffect(int pState, AREA_OPERATE pOperate);
public slots:
    virtual void ExecAction(QAction *pAction) {}
public:
    QPointer<GraphicsText>  m_GraphicsText;
    bool                    m_ShowText;
};

class DesignFunction : public GraphicsFunction
{
    Q_OBJECT
public:
    DesignFunction(QGraphicsItem *parent) :
        GraphicsFunction(parent)
    {
        QAction *tmpDefault = m_ActionGroup->addAction(tr("0"));
        for(int i=1;i<10;i++)
        {
            m_ActionGroup->addAction(QString("%1").arg(i));
        }
        tmpDefault->setChecked(true);
    }
    bool Start()
    {
        if(GraphicsComponent::Start() == false)
            return false;
        updateEffect(m_RcList.first(), OPERATE_CHANGE);
        return true;
    }
    QSize GetRcSize(QString pRcName){ return size().toSize(); }
    QList<QAction*> GetActionList()
    {
        QList<QAction*> tmpList;
        if(m_ActionGroup)
        {
            tmpList = m_ActionGroup->actions();
            //>@根据当前已有的资源使能响应功能Action
            for(int i=0;i<tmpList.count();i++)
            {
                QAction *tmpAction = tmpList.at(i);
                if(tmpAction == 0)
                    continue;
                int tmpState = tmpAction->text().toInt();
                if(m_EffectRC.contains(tmpState))
                    tmpAction->setEnabled(true);
                else
                    tmpAction->setDisabled(true);
            }
        }
        return tmpList;
    }

    bool PaintEffect(QPainter *p)
    {
        GraphicsFunction::PaintEffect(p);
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
    void ExecAction(QAction *pAction)
    {
        updateEffect(pAction->text().toInt(), OPERATE_STEADY);
    }
};

#endif // GraphicsREGION_H
