#ifndef SHOWFLOOR_H
#define SHOWFLOOR_H

#include <QObject>
#include <QDebug>
#include <QTimer>
class ShowFloor : public QObject
{
    Q_OBJECT
public:
    explicit ShowFloor(QObject *parent = 0);

signals:
    void sendSignalFloor(int index);
    void sendShowPicture(bool Up);
public slots:
    void SetFloorNumber();
};

#endif // SHOWFLOOR_H
