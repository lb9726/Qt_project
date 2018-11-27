#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "datahandle.h"
#include <QMap>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    typedef struct __UserGroup
    {
        int seriesNum;
        int groupNum;
        QString privateKey;
    }UserGroup;

private:
    Ui::Widget *ui;
    DataHandle mDataHandle;
    QMap<int, UserGroup*> mList;
};

#endif // WIDGET_H
