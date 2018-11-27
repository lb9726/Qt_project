#ifndef UNZIPFUNCTION_H
#define UNZIPFUNCTION_H

#include <QObject>
#include <QProcess>
//#include <QElapsedTimer>
#include <QByteArray>
#include <QList>
#include <QTimer>

class UnZipFunction : public QObject
{
    Q_OBJECT
public:
    explicit UnZipFunction(QObject *parent = 0);
    void startUnZip(QString str);
    void generateAscendRandomNumber();
//    virtual void run();
signals:
    void sigProgress(int num);

public slots:
    void sltStarted();
    void sltUnZipFinished(int code);
    void sltOcurrErrors(QProcess::ProcessError error);
    void sltTimeOut();
//    void sltReadRunTimeInfo();

private:
    QProcess m_ZipProcess;
    QString cmd;
    QList<int> numbersList;
    QTimer m_timer;
};

#endif // UNZIPFUNCTION_H
