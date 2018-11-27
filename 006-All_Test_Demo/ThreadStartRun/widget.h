#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "threadson.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void init();

private:
    Ui::Widget *ui;
    ThreadSon m_threadSon;
};

#endif // WIDGET_H
