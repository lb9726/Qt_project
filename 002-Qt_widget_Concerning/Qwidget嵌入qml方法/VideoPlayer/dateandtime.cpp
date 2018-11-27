#include "dateandtime.h"
#include <QDebug>

DateAndTime::DateAndTime(QObject *parent) : QObject(parent)
{
    connect(&m_time, &QTimer::timeout, this, &DateAndTime::sendDateAndTime);
    m_time.start(1000);
}

QString DateAndTime::getDateFormat(bool isYmd)
{
    QString date = "";
    date.clear();
    if (isYmd)
    {
        date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    }
    else
    {
        date = QDateTime::currentDateTime().toString("MM-dd-yyyy");
    }
//    qDebug()<< "date = "<< date;
    return date;
}

QString DateAndTime::getTimeFormat(bool is12)
{
    QString timelocal = "";
    timelocal.clear();
    if (is12)
    {
        timelocal = QDateTime::currentDateTime().toString("hh:mm a");
        timelocal =  timelocal.split(" ").at(0);
    }
    else
    {
        timelocal = QDateTime::currentDateTime().toString("HH:mm");
    }
//    qDebug()<< "timelocal = "<< timelocal;
    return timelocal;
}

void DateAndTime::sendDateAndTime()
{
    m_stringTime = getTimeFormat(false);
    m_stringDate = getDateFormat(false);
    emit signalTimeAndDate(m_stringTime, m_stringDate);
}
