#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QtQuickWidgets/QQuickWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QQuickWidget* setQmlIntoWidget();

private:
//    Ui::Widget *ui;
    QGraphicsView               *mView;
    QQuickWidget *m_quickWidget;
};

#endif // WIDGET_H
