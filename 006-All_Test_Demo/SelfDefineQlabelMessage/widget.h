#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <qlabelselfdefine.h>
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
private:
    QlabelSelfDefine mylabel;
};

#endif // WIDGET_H
