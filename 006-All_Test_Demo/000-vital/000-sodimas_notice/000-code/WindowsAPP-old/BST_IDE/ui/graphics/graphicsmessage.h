#ifndef GraphicsMESSAGE_H
#define GraphicsMESSAGE_H

#include "graphicsoperate.h"

#ifdef IDE
    #define ComMessage  DesignMessage
#elif DISPLAY
    #define ComMessage  GraphicsMessage
#endif

inline QColor GetColor(MSG_LEVEL pLevel)
{
    QColor tmpColor;
    switch(pLevel)
    {
        case LEVEL_INFO:
            tmpColor = Qt::white;
            break;
        case LEVEL_WARN:
            tmpColor = Qt::green;
            break;
        case LEVEL_ERROR:
            tmpColor = Qt::red;
            break;
        default:
            tmpColor = Qt::white;
    }
    return tmpColor;
}

class GraphicsMessage : public GraphicsOperate
{
    Q_OBJECT
public:
    GraphicsMessage(QGraphicsItem *parent);
    bool InitSubRcPara(AREA_STYLE* pAreaStyle, QString pLabel, QString pContent);
    bool Start();

    bool SetMode(E_SHOWMODE pShowMode, E_DEVMODE pDevMode);
    QPixmap LoadMessage(MSG_INFO pMsgInfo);
    void AppendMessage(MSG_INFO pMsg);
    bool ProcessMessage(MSG_LEVEL pLevel, QString pContent, int pDelay, E_DEVMODE pDevMode);

    QPixmap RefreshMessage();
    void updateFont();

    void OperateTurn(QVariant pPara);

    void updateEffect();
    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }

    bool PaintEffect(QPainter *p);

public slots:
    void slot_CloseMessage();
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    void UpdateCom(int pFrame) { GraphicsComponent::UpdateCom(pFrame); }
    void info(QString pContent, int pDelay, E_DEVMODE pDevMode=DM_NORMAL);
    void warn(QString pContent, int pDelay, E_DEVMODE pDevMode=DM_NORMAL);
    void error(QString pContent, int pDelay, E_DEVMODE pDevMode=DM_NORMAL);
    void slot_Finished();

    void slot_UpdateFont();

public:
    E_DEVMODE           mDevMode;
    E_SHOWMODE          mShowMode;
    int                 mOldDelay;

    bool                m_ShowTitle;
    QRectF              m_MsgRect;
    int                 m_TextHeight;
    QFont               m_TextFont[2];  //>@横显与竖显时设置相同的字体大小，但实际上不一样大小，0横显，1竖显
    int                 m_MsgMaxCount;
    QList<MSG_INFO>     m_MessageQueue;
    QTimer              m_DelayTimer;
};

class DesignMessage : public GraphicsMessage
{
    Q_OBJECT
public:
    DesignMessage(QGraphicsItem *parent);

    QSize GetRcSize(QString pRcName){ return size().toSize(); }
    QList<QAction*> GetActionList();

    bool PaintEffect(QPainter *p)
    {
        GraphicsMessage::PaintEffect(p);
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

#endif // EFFECTMESSAGE_H
