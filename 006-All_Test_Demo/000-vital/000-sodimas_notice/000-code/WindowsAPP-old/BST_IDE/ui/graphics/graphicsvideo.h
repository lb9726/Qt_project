#ifndef GraphicsVIDEO_H
#define GraphicsVIDEO_H

#include "graphicsoperate.h"
#include "physicalplayer.h"

class GraphicsVideo : public GraphicsOperate
{
    Q_OBJECT
public:
    GraphicsVideo(QGraphicsItem *parent);
    ~GraphicsVideo();
    bool Start();
    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }
    void updateEffect(int pIndex, int pLoop);
    int GetScreenRotate();

signals:
    void sPlay(int pAoType, QString pFile, int pLoop, bool pPrior, bool pTrack);

public slots:
    virtual void ExecAction(QAction *pAction) {}
    void UpdateCom(int pFrame) { GraphicsComponent::UpdateCom(pFrame); }
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);

public:
    QPointer<PhysicalPlayer> m_MediaPlayer;
};

class DesignVideo : public GraphicsVideo
{
    Q_OBJECT
public:
    DesignVideo(QGraphicsItem *parent);

    QSize GetRcSize(QString pRcName){ return size().toSize(); }
    QList<QAction*> GetActionList();

    bool PaintEffect(QPainter *p)
    {
        GraphicsVideo::PaintEffect(p);
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

    void updateEffect(int pIndex);
public slots:
    void ExecAction(QAction *pAction);

public:
    int m_CurRcLabel;
};

#endif // EFFECTVIDEO_H
