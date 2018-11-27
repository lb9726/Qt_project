#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QString>
#include "aloneprogessbar.h"

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();
    void init();
    void paintEvent(QPaintEvent *event);

private:
    Ui::MyWidget *ui;
    QPixmap  mHorizontalPixmap;
    QPixmap  mVerticalPixmap;
    QString mRunPath;
    AloneProgessBar* progressBar;
};

#endif // MYWIDGET_H
