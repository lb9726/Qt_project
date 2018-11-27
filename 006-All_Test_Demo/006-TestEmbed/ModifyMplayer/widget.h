#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mplayer.h"
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void slot_TimeOut();

private:
    Ui::Widget *ui;
    Mplayer* m_player;
    QTimer m_TimePlay;
    int m_liftarvvol;
    int m_liftflrvol;
};

#endif // WIDGET_H
