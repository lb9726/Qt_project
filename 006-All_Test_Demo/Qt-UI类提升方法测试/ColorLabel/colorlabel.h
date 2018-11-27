#ifndef COLORLABEL_H
#define COLORLABEL_H

#include <QLabel>
#include <QPainter>

class ColorLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ColorLabel(QWidget *parent=0);
    void setColor(QColor c);
private:
    QColor color;
    QPainter *paint;
    void paintEvent(QPaintEvent *);
};


#endif // COLORLABEL_H
