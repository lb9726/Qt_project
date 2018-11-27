#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QByteArray>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QByteArray& getByteArrayReference(QByteArray& pArray);
    QByteArray& getByteArrayReference2(QByteArray& pArray);

private:
    Ui::Widget *ui;
    QByteArray mByteArray;
};

#endif // WIDGET_H
