#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTime>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    bool RemoveDir(const QString path);

private:
    Ui::Widget *ui;
    QTime mTimeCount;
};

#endif // WIDGET_H
