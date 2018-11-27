#ifndef FLOOR_H
#define FLOOR_H

#include <QObject>

class Floor : public QObject
{
    Q_OBJECT
public:
    explicit Floor(QObject *parent = 0);
    void autoTestFloorChanage();

signals:
    void floorChanged(int pFloorNum, bool upArrowFlag);
public slots:

private:
    int mCurrentFloor;
    bool mArrowUp;
};

#endif // FLOOR_H
