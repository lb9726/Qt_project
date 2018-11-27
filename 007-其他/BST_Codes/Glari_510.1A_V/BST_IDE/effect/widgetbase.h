#ifndef WIDGETBASE_H
#define WIDGETBASE_H

#include "global.h"

class EffectBase;

inline bool operator==(QPixmap b1, QPixmap b2) { return b1.data_ptr() == b2.data_ptr(); }

class RcBase
{
public:
    RcBase();
    ~RcBase();

    void LoadPixmap(int pKey, QPixmap pPixmap);
    QPixmap LoadPixmap(QString pPixmapName, QSizeF pSize=QSizeF());
    QPixmap GetPixmap(int pKey, QSizeF pSize=QSizeF());

    virtual void InitRC(QString pRcPath, bool pInititial = true) = 0;

    bool load(int pKey, QString pFile);
    bool load(int pKey,  QPixmap pImg);
    bool load(QHash<int, QString> pFileList);
    bool load(QHash<int, QPixmap> pImgList);

    QPixmap getNextImg();
    QPixmap getPrevImg();

    void SetRender(AREA_OPERATE pOperate, EffectBase *pRender);
    bool IsRendering(AREA_OPERATE pOperate);
    bool IsRendering();
    virtual void StartRender();
    void StopRender();

    virtual void Update();

public:
    QHash<int, QString>                 m_FileList;
    QHash<int, QPixmap>                 m_ImgList;
    PIXMAP_POINTER                      m_ImgPointer;

    QHash<AREA_OPERATE, EffectBase*>    m_EffectRenders;
    EffectBase*                         m_EffectRender;
};

class WidgetBase : public QWidget, public RcBase
{
    Q_OBJECT
public:
    WidgetBase(QWidget *parent=0, Qt::WindowFlags f = 0);
    ~WidgetBase();

    virtual void InitRC(QString pRcPath, bool pInititial = true);

    void Update() {update();}
    void paintEvent(QPaintEvent *e);

signals:
    void sReportOperate(AREA_OPERATE pOperate, QVariant pPara);

public slots:
    void paintEffect();
    virtual void slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara);

public:
    int                                 m_Index;
};

class GWidgetBase : public QGraphicsWidget, public RcBase
{
    Q_OBJECT
public:
    GWidgetBase(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    ~GWidgetBase();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:
    void sReportOperate(AREA_OPERATE pOperate, QVariant pPara);

public slots:
    void paintEffect();
    virtual void slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara)=0;
};


#endif // WIDGETBASE_H
