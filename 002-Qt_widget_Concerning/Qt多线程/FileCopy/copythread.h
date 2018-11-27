#ifndef COPYTHREAD_H
#define COPYTHREAD_H
#include <QThread>
#include <QStringList>

enum{
    COPY_START=0,
    COPY_STOP,
    COPY_FILE_NAME,
    COPY_TOTAL_RATE,
    COPY_SINGLE_RATE,
    COPY_ERROR_MEM_FULL
}COPY_STATION;

enum{
    SUCCESS=0,
    ERROR_SRC_PATH_NULL,
    ERROR_DES_PATH_NULL,
    ERROR_NO_FILES,
    ERROR_MEM_FULL
}RETURN_VALUE;

class CopyThread : public QThread
{
    Q_OBJECT

public:
    CopyThread();

    void run();
    int copyStart();
    void copyStop();

    void setSrcDirPath(QString path) { srcPath=path; }
    void setDesDirPath(QString path) { desPath=path; }

private:
    QStringList fileList;

    QString srcPath;
    QString desPath;
    bool bStop;
    unsigned long fileTotalSize;
    unsigned long curSize;

    int fileCopy(QString fileName);

signals:
    void copyStationSig(int flag,QString qsText);
};

#endif // COPYTHREAD_H
