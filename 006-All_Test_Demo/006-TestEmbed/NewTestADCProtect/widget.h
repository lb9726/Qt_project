#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//#include "protectadc.h"
#include "gpios.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
//    ProtectADC mADC;
    Gpios mGpio;
};

#endif // WIDGET_H
