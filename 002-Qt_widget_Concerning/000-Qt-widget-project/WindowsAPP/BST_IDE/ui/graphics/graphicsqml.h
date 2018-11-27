#ifndef GRAPHICSQML_H
#define GRAPHICSQML_H

#include "graphicsoperate.h"

#ifdef IDE
    #define ComQML  DesignQML
#elif DISPLAY
    #define ComQML  GraphicsQML
#endif

class GraphicsQML : public GraphicsOperate
{
    Q_OBJECT
public:
    GraphicsQML(QGraphicsItem *parent);
    ~GraphicsQML()
    {
        UnloadQML();
    }

    bool Start();
    void UnloadQML();
    bool LoadQML(QString pFileName);
    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }
public slots:
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    void UpdateCom(int pFrame) { GraphicsComponent::UpdateCom(pFrame); }
public:
    void updateEffect(int pState);
private:
    int                 m_CurRcLabel;
#ifdef SURRPORT_QML
    QDeclarativeItem   *m_QMLItem;
    QDeclarativeEngine *m_QMLEngine;
#endif
};

class DesignQML : public GraphicsQML
{
    Q_OBJECT
public:
    DesignQML(QGraphicsItem *parent);

    QSize GetRcSize(QString pRcName){ return size().toSize(); }
    QList<QAction*> GetActionList();

    bool PaintEffect(QPainter *p)
    {
        GraphicsQML::PaintEffect(p);
        //>@»æÖÆ±ß¿ò
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

#endif // GRAPHICSQML_H
