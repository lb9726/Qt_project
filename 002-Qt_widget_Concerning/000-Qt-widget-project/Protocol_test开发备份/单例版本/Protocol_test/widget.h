#ifndef WIDGET_H
#define WIDGET_H
#define LCD_DEVICE ("/dev/ttyUSB0")

#include <QWidget>
#include "protocoldeal.h"
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
    void DealData(char str[]);

public slots:
    void sett();
    void setstring(QString str);

private:
    Ui::Widget *ui;
    Protocoldeal *pro;

};

#endif // WIDGET_H
