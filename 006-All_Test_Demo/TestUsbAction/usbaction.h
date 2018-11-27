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
    QStringList readFile();
    int countAllFileSize(QString sourcePath);
    int GetListCounts(QStringList list);
    quint64 dirFileSize(const QString &path);
    void resetFileLenghtAndFileNum();
    int getFileNum();
    int getFileLength();
    int copyFile(QString src, QString dest);
    QStringList getFileList();

signals:
    void sigStartProgress();
    void sigUpdateProgress(int num);

public slots:
    void sendUpdateProgress();
private:
    QStringList mCpFileList;
    QStringList mFileList;
    QString runPath;
    quint64 fileLength;
    int fileNum;
    QThread mProgressThread;
};

#endif // USBACTION_H
