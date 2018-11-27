#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include "usbmanager.h"
#include "qdevicewatcher.h"
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
    QDeviceWatcher *q ;
public slots:
    void addusb(QString str);
    void rmusb(QString str);

};

#endif // WIDGET_H
