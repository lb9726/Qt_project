#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "gifplayer.h"
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
    void ShowBusy(bool pFlag, quint64 pTimeout = 60000);  //>@如果pTimeout为0，则表示一直等待直到退出。

public slots:
    void slot_CloseBusy();

private:
    Ui::Widget *ui;
    GifPlayer *mBusyFlagIcon;
    QTimer  mBusyTimeout;
};

#endif // WIDGET_H
