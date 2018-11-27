#ifndef WIDGETBUTTON_H
#define WIDGETBUTTON_H

#include "widgetbase.h"

class WidgetButton : public WidgetBase
{
    Q_OBJECT
public:
    WidgetButton(QWidget *parent = 0);
    ~WidgetButton();

    void InitButton(QString pDark, QString pLight);

    void SetValidRect(QRect pRect);

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    void ReleaseButton();

signals:
    void sPressed();
    void sReleased();

public slots:
    void slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara);
    void slot_PressDelay();

public:
    int                 m_BtnType;  //>@0:表示正常，1：表示按下无反应

    bool                m_Checked;
    bool                mIsPressed;

    QList<QPixmap>      mPixmaps;
    AREA_OPERATE        mOperate;

    //>@为0表示初试状态，1表示高亮
    int                 mStatues;

    int                 mPressDelay;  //>@按钮按下多少时间才算真正按下
    QTimer              mPressDelayTimer;

    QRect               mValidRect;
    bool                mValidFlag;
};


#endif // WIDGETBUTTON_H
