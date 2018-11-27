#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDomElement>
#include <QDomDocument>
#include <QDebug>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    bool readXml();
    void outPutAllXml(QDomNode doc);
    ~Widget();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
