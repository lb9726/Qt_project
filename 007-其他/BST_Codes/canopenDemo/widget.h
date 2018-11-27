#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "open/openclient.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
public slots:
    void testSlot(int state);
private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
public:
    OpenClient *canOpenClient;
};

#endif // WIDGET_H
