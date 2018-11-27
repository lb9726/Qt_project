#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QWidget>

class MenuButton : public QWidget
{
    Q_OBJECT
public:
    explicit MenuButton(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

    void SetPressed(bool pFlag);

signals:
    void sSelected();

public slots:

public:
    int mFlag;
    QPixmap mPixmap;
    QColor mColor;
};

#endif // MENUBUTTON_H
