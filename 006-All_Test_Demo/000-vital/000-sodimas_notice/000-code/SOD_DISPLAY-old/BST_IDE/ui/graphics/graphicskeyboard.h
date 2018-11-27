#ifndef GraphicsKEYBOARD_H
#define GraphicsKEYBOARD_H

#include "graphicsoperate.h"

#ifdef IDE
    #define ComKeyboard  DesignKeyboard
#elif DISPLAY
    #define ComKeyboard  GraphicsKeyboard
#endif

class GraphicsKey : public GraphicsOperate
{
    Q_OBJECT
public:
    GraphicsKey(QGraphicsItem *parent);

    //void SetTypeList(QList<quint32> pType);
    //void SetTypeList(quint32 pType);
    //quint32 GetCurType();

    bool Start();
    virtual bool PaintEffect(QPainter *p);
    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }

public slots:
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    void UpdateCom(int pFrame) { GraphicsComponent::UpdateCom(pFrame); }
    void SetAlternateType(int index)
    {
        if(index < 0 || index >= m_KeyTypeList.count())
            m_CurTypeIndex = -1;
        else
            m_CurTypeIndex = index;
    }
    void SetAlternateType()
    {
        if(m_CurTypeIndex < m_KeyTypeList.count()-1)
            m_CurTypeIndex ++;
        else
            m_CurTypeIndex = 0;
    }

public:
    BUTTON_RC           m_KeyState;
    QList<quint32>      m_KeyTypeList;
    int                 m_CurTypeIndex;
};

#define MAX_DIALCOUNT  3 //>@dial界面最多输入的字符数

class GraphicsLCD : public GraphicsOperate
{
    Q_OBJECT
public:
    GraphicsLCD(QGraphicsItem *parent);

    bool Start();
    virtual bool PaintEffect(QPainter *p);

    void SetPattern(KB_PATTERN pPattern) {m_Pattern = pPattern;}

    QPixmap GetBgImage();
    QPixmap GetFloorPixmap();
    QPixmap GetDialPixmap();
    QPixmap GetIconPixmap();
    quint32 GetMappedFloor(quint32 pType);
    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }
public slots:
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    void UpdateCom(int pFrame) { GraphicsComponent::UpdateCom(pFrame); }
private:
    KB_PATTERN          m_Pattern;
    //>@楼层显示参数
    QRectF              m_FloorRect;
    char               *m_FloorAsciiList;
    int                 m_FloorAsciiMax, m_FloorAsciiIndex;
    QFont               m_FloorFont;
    //>@回显参数
    QRectF              m_DialNumRect;
    char                m_DialNumList[MAX_DIALCOUNT+1];
    int                 m_DialNumIndex;
    QFont               m_DialNumFont;
    //>@特殊图标显示区域
    bool                m_IconRectValid;
    QRectF              m_IconRect;
};

class GraphicsKeyboard : public GraphicsOperate
{
    Q_OBJECT
public:
typedef struct{
    QString  mLabel;
    qreal    mWidthRatio;  //>@此字符的宽度或者高度比例
    qreal    mHeightRatio;  //>@此字符的宽度或者高度比例
} KEYINFO;

class KeyboardBtnInfo
{
public:
    KeyboardBtnInfo()
    {
        mButtonItem = 0;
    }
    ~KeyboardBtnInfo()
    {
        if(mButtonItem)
        {
            if(mButtonItem->scene())
                mButtonItem->scene()->removeItem(mButtonItem);
            mButtonItem->deleteLater();
        }
    }
public:
    GraphicsKey             *mButtonItem;
    QRectF                      mRect;
};
public:
    GraphicsKeyboard(QGraphicsItem *parent);
    ~GraphicsKeyboard()
    {
        ReleaseKeyboardBtnInfoList();
    }
    bool Start();
    bool InitSubRcPara(AREA_STYLE* pAreaStyle, QString pLabel, QString pContent);
    virtual bool PaintEffect(QPainter *p) { return true; }

    bool InitKeyboardBtnInfoList();
    void ReleaseKeyboardBtnInfoList();

    bool AddLCD(QRectF pRect);
    bool AddButton(QString pKeys, GraphicsKey *&pButton, QRectF pRect);

    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }

signals:
    void sAlternateType();

public slots:
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    void collectReport(AREA_OPERATE pOperate, QVariant pPara);
    void HideLCD();

public:
    QList<KeyboardBtnInfo*>    m_KeyboardBtnInfoList;  //>@
    GraphicsLCD               *m_LCD;
    KB_PATTERN                 m_Pattern;
    int                        mRSpace, mCSpace;

    QTimer                     m_LCDHideTimer;
};

class DesignKeyboard : public GraphicsKeyboard
{
    Q_OBJECT
public:
    DesignKeyboard(QGraphicsItem *parent);

    QSize GetRcSize(QString pRcName);
    QList<QAction*> GetActionList() { QList<QAction*> tmpList; return tmpList; }

    bool PaintEffect(QPainter *p)
    {
        GraphicsKeyboard::PaintEffect(p);
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
    void ExecAction(QAction *pAction) {}
};

#endif // EFFECTKEYBOARD_H

