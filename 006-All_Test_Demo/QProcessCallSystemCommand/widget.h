#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "unzipfunction.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    UnZipFunction m_unZip;
};

#endif // WIDGET_H
