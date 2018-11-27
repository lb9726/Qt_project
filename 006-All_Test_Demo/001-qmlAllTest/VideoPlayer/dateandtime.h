#ifndef DATEANDTIME_H
#define DATEANDTIME_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QTimer>

class DateAndTime : public QObject
{
    Q_OBJECT
public:
    explicit DateAndTime(QObject *parent = 0);

    void sendDateAndTime();

signals:
    void signalTimeAndDate(QString pStringTime, QString pStringDate);

public slots:
    QString getDateFormat(bool isYmd);
    QString getTimeFormat(bool is12);

private:
    QTimer m_time;
    QString m_stringTime;
    QString m_stringDate;
};

#endif // DATEANDTIME_H
