#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mplayer.h"
#include <QString>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void init();

private:
    Ui::Widget *ui;
    Mplayer * m_player;
    QString musicPath;
};

#endif // WIDGET_H
