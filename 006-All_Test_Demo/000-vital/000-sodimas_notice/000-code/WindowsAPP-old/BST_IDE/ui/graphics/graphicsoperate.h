#ifndef GRAPHICSOPERATE_H
#define GRAPHICSOPERATE_H

#include "graphicscomponent.h"

//>@MVC结构，驱动scene、property、frame、digramsetting
class GraphicsOperate : public GraphicsComponent
{
    Q_OBJECT
public:
    GraphicsOperate(QGraphicsItem *parent, GraphicsScene *pScene=0, QRectF pRect=QRectF());
    ~GraphicsOperate() {}

    bool RefreshParameter();   //>@更新自己的坐标以及可编辑的最大范围以及特效/参数

    qreal ModifyComLeft(qreal pValue, bool pSync);
    qreal ModifyComTop(qreal pValue, bool pSync);
    qreal ModifyComRight(qreal pValue, bool pSync);
    qreal ModifyComBottom(qreal pValue, bool pSync);
    qreal ModifyComWidth(qreal pValue, bool pSync);
    qreal ModifyComHeight(qreal pValue, bool pSync);
    QPointF ModifyComPos(QRectF pComRect, bool pSync);

    QRectF GetVirtualSceneRect();  //>@获取缓冲中的scene下的区域大小，也即还没有映射到最终组件上的坐标

    bool ModifySceneRect(QRectF pRect);
    bool ModifyStateEffect(QString pStateName, QString pEffectName);
    bool ModifyEffectPara(QString pStateName, QString pParaName, QString pParaValue);
    bool ModifyRcFile(QString pStateName, QString pRcName, QString pRcFile);
    bool DelRcFile(QString pStateName, QString pRcName);

    //>@实现远程在线编辑操作


signals:
    //>@一般来讲，Property以及Frame窗口只有区域坐标需要同步，因此只要下面Rect信号即可
    //>@并且，对于此种需要多个设置窗口同步的数据，在某一个设置窗口修改后，不要立即修改此窗口中的数据，而是通过统一发送的改变信号出发修改操作。
    void sSceneRectChanged(QRectF pRect);
    //void sStateEffectChanged(QString pStateName, QString pEffectName);
    //void sEffectParaChanged(QString pStateName, QString pParaName, QString pParaValue);
    //void sRcFileChanged(QString pStateName, QString pRcName, QString pRcFile);
    //void sRcFileDeleted(QString pStateName, QString pRcName);

public slots:

public:
    bool                        mDragValid;

    GraphicsComponent          *mParComponet;

    //>@初始化控件的坐标参数
    QRectF                      m_CurRect;  //>@当前横竖方向下的Scene坐标
    qreal   m_LeftLimitMin,   m_LeftLimitMax,     m_CurLeft;
    qreal   m_TopLimitMin,    m_TopLimitMax,      m_CurTop;
    qreal   m_RightLimitMin,  m_RightLimitMax,    m_CurRight;
    qreal   m_BottomLimitMin, m_BottomLimitMax,   m_CurBottom;

    //>@相反横竖方向下的Scene坐标
    //>@对于相反方向，只需要判断宽度以及高度是否超过父即可
    qreal   m_OppMaxWidth,   m_OppMaxHeight;
};


#endif // GRAPHICSOPERATE_H
