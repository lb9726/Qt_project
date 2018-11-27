#ifndef UICLOCK_H
#define UICLOCK_H
#include <QWidget>

class UiClock : public QWidget
{
    Q_OBJECT
public:
    UiClock(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
};

#endif // UICLOCK_H
