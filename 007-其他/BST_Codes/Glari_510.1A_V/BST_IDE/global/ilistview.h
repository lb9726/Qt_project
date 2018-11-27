#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include "define.h"

class UiClass
{
public:
    virtual void SetFlag(quint32 pFlag, QVariant pPara = QVariant()) { m_Flag.insert(pFlag, pPara); }
public:
    QHash<quint32, QVariant>   m_Flag;
};

class FlickCharmPrivate;
class FlickCharm: public QObject
{
    Q_OBJECT
public:
    FlickCharm(QObject *parent = 0);
    ~FlickCharm();
    void activateOn(QWidget *widget, int rate = 1);
    void deactivateFrom(QWidget *widget);
    bool eventFilter(QObject *object, QEvent *event);

signals:
    void sFinished();

protected:
    void timerEvent(QTimerEvent *event);

private:
    FlickCharmPrivate *d;
};

#define D_ITEM_LINE_LEN     1

class ItemScrollContent : public QWidget
{
    Q_OBJECT
public:
    ItemScrollContent(QWidget *parent = 0);
    ~ItemScrollContent();

    void Highlight(QPainter *p, QString pString, QRect pRect, bool pEnable);
    void paintEvent(QPaintEvent *);

    void Init(QStringList pList, int pIndex);
    void SetCurrentIndex(int pIndex, bool pSync = true);

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    void Resize(QSize pSize);

signals:
    void sClicked(int pIndex);  //>@发送给ItemScrollArea的信号

public slots:
    void slot_Clicked(int pIndex);  //ItemScrollArea发来的信号插槽

public:
    int                  mIndex;
    QStringList          mStringList;
    QList<QRect>         mRectList;

    int                  mItemHeight;
    QImage               mBuffer;  //一级缓冲
    QPixmap              mBuffer2;

    QFont                mFont[2];
    QColor               mColor[2];  //>@字体颜色，0表示选中的，1表示未选中的
    //自绘制背景
    QColor               mBgColor[2];
    QColor               mFrameColor;
    //使用图片作为背景
    QPixmap              mBgImg[2];

    //使用图片作为每个标签图片
    QPixmap              mImage[2];

    QPoint               mPressPos;
    QPoint               mReleasePos;

    bool                 mButtonMode;
};

class ItemScrollArea : public QScrollArea, public UiClass
{
    Q_OBJECT
public:
    ItemScrollArea(QWidget *parent = 0);
    ~ItemScrollArea();

    enum{
        FLAG_SELECTOR,      //>@单选模式，默认三选一，必须等待其它参数设置完毕后才能后设置
        FLAG_ITEMBUTTON,    //>@按钮模式，此模式下，可以点击某个item并使之高亮，否则Item不高亮
        FLAG_ITEMHEIGHT,
        //
        FLAG_FOCUS_FONTTYPE,
        FLAG_UNFOCUS_FONTTYPE,
        //
        FLAG_FOCUS_FONTSIZE,
        FLAG_UNFOCUS_FONTSIZE,
        //
        FLAG_RECT_COLOR,    //>@item边框颜色
        //
        FLAG_FOCUS_COLOR,
        FLAG_UNFOCUS_COLOR,
        //
        FLAG_FOCUS_BGCOLOR,
        FLAG_UNFOCUS_BGCOLOR,
        //
        FLAG_FOCUS_BGIMG,
        FLAG_UNFOCUS_BGIMG
    };

    void Init();                                //执行完new后必须立即执行一次
    virtual void Init(QStringList pList, int pIndex);   //调用所有的setflag后立即执行一次
    void SetFlag(quint32 pFlag, QVariant pPara = QVariant());

    QString GetCurrentText();
    int GetCurrentIndex();
    void SetCurrentIndex(int pIndex);  //>@仅仅设置某一项为高亮，不做其它事

    void resizeEvent(QResizeEvent *e);

signals:
    void sClicked(int pIndex); //发送给ItemScrollContent的信号
    void sItemClicked(int);

public slots:
    void slot_Clicked(int pIndex);  //ItemScrollContent发来的信号插槽
    void slot_ScrollFinished();

public:
    ItemScrollContent    *mWidget;
    FlickCharm           mFlickCharm;

    bool                 mSelectorMode;
};

class TextTerminal : public QWidget, public UiClass
{
    Q_OBJECT
public:
    TextTerminal(QWidget *parent = 0);
    ~TextTerminal();

    enum{
        FLAG_TITLE_EN = 0,
        FLAG_FONT_SIZE,
        FLAG_FONT_FAMILY,
        FLAG_FONT_WEIGHT,
        FLAG_INFO_COLOR,
        FLAG_WARN_COLOR,
        FLAG_ERROR_COLOR
    };

    struct TEXT_INFO{
        E_MSG_LEVEL  mLevel;
        QString      mText;
        int          mCount;
    };

    void SetFlag(quint32 pFlag, QVariant pPara = QVariant());

    void Append(E_MSG_LEVEL pLevel, QString pText);
    void Replace(int pIndex, QString pText);  //>@修改最后一行的字符串

    void Refresh();

    void UpdatePara(QSize pSize);
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);

public:
    QList<TEXT_INFO*>   mTextList;
    int                 mMaxCount;
    int                 mMaxRows;
    int                 mDiffHeight;
    int                 mItemHeight;

    QFont               mFont;
    Qt::GlobalColor     mColor[3];
    QRect               mTextRect;

    QImage              mImage;
    QImage              mBuffer;

    //背景图片
    QBrush       m_BgBrush, m_FgBrush,
                 m_BGlowBrush,                      //>@底部光晕
                 m_TGlassBrush, m_TGlassBrush2,     //>@顶部玻璃磨砂效果
                 m_LightBrush;      //>@选中高亮效果
    QColor       m_BgPenColor, m_FgPenColor;
    QRect        m_BgRect, m_FgRect,
                 m_BGlowRect,      //>@底部光晕
                 m_TGlassRect,     //>@顶部玻璃磨砂效果
                 m_LightRect;      //>@选中高亮效果
    QString      m_Title;         //标题
    QFont        m_TitleFont;
    QColor       m_TitleColor;
    bool         m_ShowTitle;
};

#endif // GUI_H
