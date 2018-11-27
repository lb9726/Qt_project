#ifndef FILECLIENT_H
#define FILECLIENT_H

#include <QtNetwork/QtNetwork>
#include <QDebug>

class FileClient : public QIODevice
{
    Q_OBJECT

public:
    FileClient(QString, quint16, QObject *parent = 0);
    void send(QString FullPath);
    void get(QString FullPath);

private slots:
    void readData();

private:
    QString IP;
    quint16 port;
    QTcpSocket *socket;
};

#endif // FILECLIENT_H
