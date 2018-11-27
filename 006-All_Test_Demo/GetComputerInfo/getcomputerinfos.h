#ifndef GETCOMPUTERINFOS_H
#define GETCOMPUTERINFOS_H

#include <QObject>
#include <QString>

class GetComputerInfos : public QObject
{
    Q_OBJECT
public:
    explicit GetComputerInfos(QObject *parent = 0);
    QString getIp();
    QString getIp2();
    QString getMachineIp();
signals:

public slots:
};

#endif // GETCOMPUTERINFOS_H
