#ifndef MYDAEMON_H
#define MYDAEMON_H

#include <QObject>
#include <QString>
#include <QProcess>

class MyDaemon : public QObject
{
    Q_OBJECT
public:
    explicit MyDaemon(QObject *parent = 0);
    void startExternProcess(); // start with Ui's EXE
signals:

public slots:

private:
    void initDaemon();
    void killExternProcess();  // stop with Ui's EXE
    void rebootSystem();
    QString getExternProcessRunStatus();

private:
    QProcess * mProcess;
    QString mRunStatus;
    QString mRunPath;
};

#endif // MYDAEMON_H
