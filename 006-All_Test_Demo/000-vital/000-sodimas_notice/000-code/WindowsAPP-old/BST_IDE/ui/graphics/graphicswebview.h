#ifndef GRAPHICSWEBVIEW_H
#define GRAPHICSWEBVIEW_H

#include "graphicsoperate.h"

class GraphicsWebview : public GraphicsOperate
{
    Q_OBJECT
public:
    explicit GraphicsWebview(QGraphicsItem *parent);
    ~GraphicsWebview()
    {
        DestoryWebview();
    }
    bool Start();
    void updateEffect(int pIndex);
    bool CreateWebview();
    bool DestoryWebview();

    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }
public slots:
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);

public:
    int                     m_CurRcLabel;
#ifdef SURRPORT_WEBKIT
    QGraphicsWebView       *m_Webview;
#endif
};

class DesignWebview : public GraphicsWebview
{
    Q_OBJECT
public:
    explicit DesignWebview(QGraphicsItem *parent);

    QSize GetRcSize(QString pRcName) { return QSize(); }
    QList<QAction*> GetActionList();

    bool PaintEffect(QPainter *p)
    {
        GraphicsWebview::PaintEffect(p);
        //>@????¡À??¨°
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

#endif // GRAPHICSWEB
