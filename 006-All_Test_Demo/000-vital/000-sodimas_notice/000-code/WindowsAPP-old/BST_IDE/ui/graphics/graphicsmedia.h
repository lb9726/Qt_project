#ifndef GraphicsMEDIA_H
#define GraphicsMEDIA_H

#include "graphicsoperate.h"
#include "physicalplayer.h"

#ifdef IDE
    #define ComMedia  DesignMedia
#elif DISPLAY
    #define ComMedia  GraphicsMedia
#endif

class GraphicsMedia : public GraphicsOperate
{
    Q_OBJECT
public:
    GraphicsMedia(QGraphicsItem *parent);
    ~GraphicsMedia();

    int StrToEnum(QString pString);
    QVariant StrToValue(int pEnum, QString pString);

    bool Start();
    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }

    void Abort(int pAoType);

signals:
    void sPlay(int pAoType, QString pFile, int pLoop, bool pPrior, bool pTrack);  //>pMultiTrack是否允许多声道同时播放
    void sPlay(int pAoType, QString pFile, int pVolume, int pLoop, bool pPrior, bool pTrack);  //>pMultiTrack是否允许多声道同时播放

public slots:
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);

public:
    QPointer<PhysicalPlayer> m_MediaPlayer;
    REPEAT_MODE     m_RepeatMode;
};

class DesignMedia : public GraphicsMedia
{
    Q_OBJECT
public:
    DesignMedia(QGraphicsItem *parent);

    QSize GetRcSize(QString pRcName){ return size().toSize(); }
    QList<QAction*> GetActionList();

    bool PaintEffect(QPainter *p)
    {
        GraphicsMedia::PaintEffect(p);
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

    void updateEffect(int pIndex);

public slots:
    void ExecAction(QAction *pAction);

public:
    int m_CurRcLabel;
};

#endif // GraphicsMedia_H
