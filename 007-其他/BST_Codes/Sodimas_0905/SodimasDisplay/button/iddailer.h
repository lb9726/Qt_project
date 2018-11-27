#ifndef IDDAILER_H
#define IDDAILER_H

#include <QObject>
#include <QList>

class IdDailer : public QObject
{
    Q_OBJECT
public:
    explicit IdDailer(QObject *parent = 0);
    void getDailStatus();
    quint8 getNodeId();
    quint8 getBoutrate();
    QString getDailerStateString();
signals:

public slots:

public:
    QList<QString> m_dialGpio;
    int m_dailStatu[8];
};

#endif // IDDAILER_H
