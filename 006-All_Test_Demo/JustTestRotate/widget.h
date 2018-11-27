#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QString>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>

namespace Ui {
class Widget;
}

typedef enum{
    Vertical = 0,
    Horizontal = 1,
    HVBoth = 2,
    HVNone
} DIRECTION;///横向 纵向

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void init();
    QString getTimeFormat(bool is12);
    QString getFloor();
    void setFloorNumber(int num);
    void setViews(QGraphicsView *view);

private:
    Ui::Widget *ui;
    QTimer mChangeLayout;
    QTimer mUpdateTimeAndFloor;
    QString runpath;
    int floorsNumber;
    QGraphicsView *mtmpviews;
    DIRECTION currentDirection;

public slots:
    void setMyDirection();
    void updateTimeAndFloor();
    void changeLayouts(QGraphicsView *view);
};

#endif // WIDGET_H
