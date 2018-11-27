#ifndef USBACTION_H
#define USBACTION_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>

class UsbAction : public QObject
{
    Q_OBJECT
public:
    explicit UsbAction(QObject *parent = 0);
    void initProcess();
    void startWork();
    void setIsBackUpFlag(bool value);
    int copyFile(QString src, QString dest);

signals:
    void currentFileName(QString fileName);
    void cmdError(QString pBackUp);
    void cmdFinished(QString pBackUp);
    void cmdStart(QString pBackUp);

public slots:
    void sltError(QProcess::ProcessError error);
    void sltFinished(int code);
    void sltStart();
    void sltReadStandardOutPut();

private:
    QProcess * cpProcess;
    bool isBackUpFlag;
    QString mCmd;
    QStringList mList;
};

#endif // USBACTION_H
