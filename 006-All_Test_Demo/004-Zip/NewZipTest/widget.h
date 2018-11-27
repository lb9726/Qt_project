#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "zipfunction.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
public slots:
    void zipProgressUpdate(int value);
    void unZipProgressUpdate(int value);

private:
    Ui::Widget *ui;
    ZipFunction mZipFunction;
    QString mRunPath;
};

#endif // WIDGET_H
