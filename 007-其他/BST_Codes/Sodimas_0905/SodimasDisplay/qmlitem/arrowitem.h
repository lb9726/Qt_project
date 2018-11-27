#ifndef ARROWITEM_H
#define ARROWITEM_H

#include <QDeclarativeItem>
#include <QPainter>
#include <QPixmap>
#include "global/define.h"


class ArrowItem : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit ArrowItem(QDeclarativeItem *parent = 0);

    Q_INVOKABLE void tick();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool DrawPixmap(QPainter *p, QPixmap pPixmap, QSizeF pSize, QRectF pTargetRect, QRectF pSourceRect);
signals:

public slots:


public:
    qreal m_CurFrameNum;
    qreal m_FrameCount;
    QPixmap  curPix;
};

#endif // ARROWITEM_H
