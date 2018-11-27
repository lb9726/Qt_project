#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
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
    void changePicture();
    void paintEvent(QPaintEvent *event);

private:
    Ui::Widget *ui;
    QTimer mTimer;
};

#endif // WIDGET_H
