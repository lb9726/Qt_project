#ifndef STACKED_H
#define STACKED_H

#include "globalfun.h"

class iStatckedItem : public QWidget
{
    Q_OBJECT
public:
    explicit iStatckedItem(QWidget *parent = 0);
    void setPixmap( const QPixmap& pixmap);
    QSize sizeHint();

    void setBgPixmap( const QPixmap& pixmap);

protected:
    void paintEvent( QPaintEvent *event);
    void resizeEvent( QResizeEvent *event);

public :
    QPixmap     mBgPixmap;

private:
    void updatePixmapOffset();
    QPixmap mPixmap;
    QPoint mPixmapOffset;
};

class iStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit iStackedWidget(QWidget *parent = 0);
    ~iStackedWidget();

    enum t_direction {
        LEFT2RIGHT,
        RIGHT2LEFT,
        TOP2BOTTOM,
        BOTTOM2TOP,
        AUTOMATIC
    };

protected:
    //! this is used for internal purposes in the class engine
    void slideInWgt(QWidget * nextWidget, enum t_direction direction);
    void resizeEvent( QResizeEvent *event);

public slots:
    //! Some basic settings API
    void setSpeed(int speed);   //animation duration in milliseconds
#ifndef N329
    void setAnimation(enum QEasingCurve::Type animationtype); //check out the QEasingCurve documentation for different styles
#endif
    void setVerticalMode(bool vertical=true);
    void setWrap(bool wrap);    //wrapping is related to slideInNext/Prev;it defines the behaviour when reaching last/first page

    //! The Animation / Page Change API
    void slideInNext();
    void slideInPrev();
    void slideInIdx(int idx, bool withanimation=true, enum t_direction direction=AUTOMATIC);

    virtual void slideToIdx(int idx);

signals:
    //! this is used for internal purposes in the class engine
    void animationFinished(void);

protected slots:
    //! this is used for internal purposes in the class engine
    void animationDoneSlot(void);

public:
    QWidget *m_mainwindow;

    int m_speed;
#ifndef N329
    enum QEasingCurve::Type m_animationtype;
#endif
    bool m_vertical;
    int m_now;
    int m_next;
    bool m_wrap;
    QPoint m_pnow;
    bool m_active;

    iStatckedItem *mBeginWidget, *mEndWidget;
    QPixmap mBeginPixmap, mEndPixmap;
    QColor mBackgroundColor;
};

#endif // GUI_H
