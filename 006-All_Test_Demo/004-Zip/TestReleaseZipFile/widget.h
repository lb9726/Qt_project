#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//#include "testzip.h"
#include "zipfunction.h"
#include <QString>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void update_Progress(int value);

private:
    Ui::Widget *ui;
//    TestZip mZip;
    ZipFunction mZipFunction;
    QString mRunPath;
};

#endif // WIDGET_H
