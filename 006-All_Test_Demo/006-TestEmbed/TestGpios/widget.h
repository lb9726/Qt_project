#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "gpios.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    Gpios gpio;
};

#endif // WIDGET_H
