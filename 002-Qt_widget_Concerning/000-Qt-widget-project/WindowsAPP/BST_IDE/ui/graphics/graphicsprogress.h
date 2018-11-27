#ifndef GraphicsPROGRESS_H
#define GraphicsPROGRESS_H

#include "graphicsoperate.h"

#ifdef IDE
    #define ComProgress  DesignProgress
#elif DISPLAY
    #define ComProgress  GraphicsProgress
#endif

class GraphicsProgress : public GraphicsOperate
{
    Q_OBJECT
public:
    GraphicsProgress(QGraphicsItem *parent);
    ~GraphicsProgress();

    bool Start();
    virtual bool PaintEffect(QPainter *p);
    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }

    void UpdateExtraValue(quint32 pValue);
    void UpdateValue(quint32 pValue);

public slots:
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    void UpdateCom(int pFrame) { GraphicsComponent::UpdateCom(pFrame); }
    void slot_CloseProgress();
    void slot_Progress(quint32 pPrg);
    void slot_Progress(quint32 pPrg, quint32 pExtraPrg);

public:
    void updateEffect(quint32 pValue, AREA_OPERATE pOperate);

public:
    QTimer              m_DelayTimer;
    int                 m_ProgressValue;

    QPixmap             m_BgPix;
    QRect               m_TextRect;
    QFont               m_Font;
    QColor              m_Color;

    GraphicsProgress   *m_ExtraProgress;
};

class DesignProgress : public GraphicsProgress
{
    Q_OBJECT
public:
    DesignProgress(QGraphicsItem *parent);

    QSize GetRcSize(QString pRcName);
    QList<QAction*> GetActionList();

    bool PaintEffect(QPainter *p)
    {
        GraphicsProgress::PaintEffect(p);
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

#endif // EFFECTPROGRESS_H
