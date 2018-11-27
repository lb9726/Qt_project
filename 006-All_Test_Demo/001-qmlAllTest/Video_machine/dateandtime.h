#ifndef DATEANDTIME_H
#define DATEANDTIME_H

#include <QObject>
#include <QString>
#include <QTimer>
#include "floor.h"

class DateAndTime : public QObject
{
    Q_OBJECT
public:
    explicit DateAndTime(QObject *parent = 0);

signals:
    void signalTimeAndDate(QString pTime);
    void sigAutoTestChanged(QString pFloor, bool pArrowUp);
    void sigWeekChanged(QString pWeekDay);

public slots:
    QString getTimeFormat(bool is12);
    void autoTest(int pFloorNum, bool upArrowFlag);
    QString getWeekDay();

private:
    void sendDateAndTime();

private:
    QString mStringTime;
    QTimer mTimer;
    Floor mFloor;
    QString mWeekString;
};

#endif // DATEANDTIME_H
