#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "canerrorcatch.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    CanErrorCatch mcanErrorCatch;
};

#endif // WIDGET_H
