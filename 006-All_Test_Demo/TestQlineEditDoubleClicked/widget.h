#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mylineedit.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void slot_Print();
private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
