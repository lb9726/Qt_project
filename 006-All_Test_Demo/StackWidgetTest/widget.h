#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QTimer>
#include "screensaver.h"

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
    void testChangeWidget();
    void ChangeWidget(int index);

private:
    Ui::Widget          *ui;
    QString             runPath;
    QTimer              timer;
    ScreenSaver         mScreenSaver;
};

#endif // WIDGET_H
