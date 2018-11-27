#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
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
    void initScreen();

private:
    Ui::Widget *ui;
    QString runPath;
    QTimer mtimer;
};

#endif // WIDGET_H
