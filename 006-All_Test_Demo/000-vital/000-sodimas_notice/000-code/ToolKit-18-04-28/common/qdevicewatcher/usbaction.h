#ifndef USBACTION_H
#define USBACTION_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QFile>
#include <QThread>

class UsbAction : public QObject
{
    Q_OBJECT
public:
    explicit UsbAction(QObject *parent = 0);
//    int countAllFileSize(QString sourcePath);
//    int GetListCounts(QStringList list);
    quint64 dirFileSize(const QString &path);
    void resetCopyParameter();
    int getFileNum();
    int getFileLength();
    int copyFile(QString src, QString dest);
    QStringList getFileList(bool isBackUp);
    void removeFile(QString src);

signals:
    void sigStartProgress(bool isBackUp);
    void sigUpdateProgress(int num);
    void sigBackUpEnded();
    void sigUpdateEnded();

public slots:
    void sendUpdateProgress(bool isBackUp);

private:
    QString runPath;
    quint64 fileLength;
    int fileNum;
    QStringList mFileList;
    QStringList mCpFileList;
    QThread mProgressThread;
};

#endif // USBACTION_H
