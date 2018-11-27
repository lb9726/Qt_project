#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "usbaction.h"
#include <QStringListModel>
#include <QPixmap>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void setCopyFileList(QStringList list);
    void updateProgressBar(int num);

signals:

private:
    Ui::Widget *ui;
    UsbAction mUsbAct;
    QStringListModel model;
};

#endif // WIDGET_H
