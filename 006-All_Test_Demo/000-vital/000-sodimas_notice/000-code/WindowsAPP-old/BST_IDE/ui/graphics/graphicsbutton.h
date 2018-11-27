#ifndef GraphicsBUTTON_H
#define GraphicsBUTTON_H

#include "graphicsoperate.h"

#ifdef IDE
    #define ComButton  DesignButton
#elif DISPLAY
    #define ComButton  GraphicsButton
#endif

class GraphicsButton : public GraphicsOperate
{
    Q_OBJECT
public:
    GraphicsButton(QGraphicsItem *parent);

    void SetType(quint32 pType) { m_CurType = pType; }
    quint32 GetCurType() { return m_CurType; }

    bool Start();
    virtual bool PaintEffect(QPainter *p);
    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }
public slots:
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    void UpdateCom(int pFrame) { GraphicsComponent::UpdateCom(pFrame); }
signals:
    void sButtonPressed(quint32 pKeyType);
    void sButtonCancel(quint32 pKeyType);
    void sButtonReleased(quint32 pKeyType);

public:
    BUTTON_RC           m_ButtonState;
private:
    quint32             m_CurType;
};

class DesignButton : public GraphicsButton
{
    Q_OBJECT
public:
    DesignButton(QGraphicsItem *parent);

    QSize GetRcSize(QString pRcName){ return size().toSize(); }
    QList<QAction*> GetActionList();

    bool PaintEffect(QPainter *p)
    {
        GraphicsButton::PaintEffect(p);
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

//>@楼层的映射关系：1~42个标号映射出对应显示的FLOOR_INFO
class GraphicsFloorButton : public GraphicsOperate
{
    Q_OBJECT
public:
    GraphicsFloorButton(QGraphicsItem *parent);
    ~GraphicsFloorButton()
    {
        ReleaseFloorBtnInfoList();
    }
    bool InitSubEffectPara(AREA_STYLE* pAreaStyle, QString pLabel, QString pContent);
    bool Start();
    virtual bool PaintEffect(QPainter *p) { return  true; }

    bool InitFloorBtnInfoList();
    void ReleaseFloorBtnInfoList();

    quint32 GetMappedType(quint32 pType);
    bool AddButton(quint32 pType, GraphicsButton *&pButton, QRectF pRect);
    int PrepareBtnChar(char pAscii, bool pLight);
    QPixmap GetBtnRc(QSizeF pSize, FLOOR_INFO pFloorInfo, bool pLight);

    bool SetRow(int pCount){}
    bool SetColumn(int pCount){}

    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }
public slots:
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);

public:
    class FloorBtnInfo
    {
    public:
        FloorBtnInfo()
        {
            mButtonItem = 0;
        }
        ~FloorBtnInfo()
        {
            if(mButtonItem)
            {
                QGraphicsScene *tmpScene = mButtonItem->scene();
                if(tmpScene)
                    tmpScene->removeItem(mButtonItem);
                mButtonItem->deleteLater();
            }
        }
    public:
        GraphicsButton             *mButtonItem;
        QRectF                      mRect;
    };

    QList<FloorBtnInfo*>       m_FloorBtnInfoList;  //>@
    FB_PATTERN                 m_Pattern;

    DIRECTION                 m_SliderDirection;
    QSize                     m_BtnSize;
    int                       m_Row;
    int                       m_Column;
    int                       m_RowSpace;
    int                       m_ColumnSpace;

    int   mRow, mColunm, mRSpace, mCSpace, mCount;
    qreal mWidth, mHeight, mTextWidth, mTextHeight;
    qreal mItemLSpace, mItemRSpace;
};

class DesignFloorButton : public GraphicsFloorButton
{
    Q_OBJECT
public:
    DesignFloorButton(QGraphicsItem *parent);

    QSize GetRcSize(QString pRcName);
    QList<QAction*> GetActionList();

    bool PaintEffect(QPainter *p)
    {
        GraphicsFloorButton::PaintEffect(p);
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

#endif // EFFECTBUTTONGRID_H
