#ifndef UIARROW_H
#define UIARROW_H
#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QTimeLine>
#include <QDomElement>
#include <QTimer>

class UiArrow : public QWidget
{
    Q_OBJECT

public:
    enum ArrowType{
        NoArrow,
        UpArrow,
        DownArrow,
        UpDownArrow,
        UpScrollArrow,
        DownScrollArrow,
        UpBlinkArrow,
        DownBlinkArrow
    };

public:
    UiArrow(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    void Init(QDomElement pElement);
    void setArrowType(ArrowType pType,int pScrollSpeed = 0);
    void paintEvent(QPaintEvent* e);
    void StartWork(int pScrollLevel);

public:
    QPixmap         mUpArrPix;
    QPixmap         mDownArrPix;
    QPixmap         mUpDownArrPix;
    ArrowType       mType;

    QTimeLine*      timeline;
    int             mFrame;
    QPixmap         mPixmap;
    int             mScrollLevel;
    QTimer          m_timers;
public slots:
    void slot_frameChanged(int pFrame);
    void setScrollLevels();
};

#endif // UIARROW_H
