#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QAbstractSlider>
#include <QMatrix>
#include <QGraphicsView>
#include <QGraphicsScene>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void initScence();
    void paintEvent(QPaintEvent *e);

private:
    Ui::Widget *ui;
    QString runPath;
    QMatrix leftmatrix;
    QGraphicsScene scene;
    QGraphicsView view;
};

#endif // WIDGET_H
