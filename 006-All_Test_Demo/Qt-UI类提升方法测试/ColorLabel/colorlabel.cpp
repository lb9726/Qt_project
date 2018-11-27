#include "colorlabel.h"

#include <QDebug>


ColorLabel::ColorLabel(QWidget *parent):QLabel(parent), color(Qt::white)
{
}

void ColorLabel::paintEvent(QPaintEvent *)
{
    qDebug() << "paintEvent";
    QPainter paint(this);
    paint.begin(this);
    paint.setBrush(QBrush(color,Qt::SolidPattern));
    paint.drawRect(0, 0, this->geometry().width(), this->geometry().height());
    paint.end();
}

void ColorLabel::setColor(QColor c)
{
    color = c;
}



