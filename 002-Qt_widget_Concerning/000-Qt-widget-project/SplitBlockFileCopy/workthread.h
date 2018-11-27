#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>
#include <QDebug>
#include <QFile>
#include <QString>
#include "log.h"
#include <QStringList>

//#define MAXSIZE 10
#define MAXSIZE 4
typedef enum{
   COPY_START = 0,
   COPY_STOP,
   COPY_FILE_NAME,
   COPY_TOTAL_RATE,
   COPY_SINGLE_RATE,
   COPY_ERROR_MEM_FULL
}COPY_STATION;

typedef enum{
   SUCCESS = 0,
   ERROR_SRC_PATH_NULL,
   ERROR_DES_PATH_NULL,
   ERROR_NO_FILES,
   ERROR_MEM_FULL
}RETURN_VALUE;

typedef struct _THREADCOPY
{
    int id;                // 线程的id
//    WorkThread *wThread;   // 线程
    int size;              // 文件大小
    int offset_filehead;   // 文件偏移的位置
}THREADCOPY;

class WorkThread : public QThread
{
    Q_OBJECT
public:
//    static THREADCOPY copyThreadLength[MAXSIZE];
    explicit WorkThread();
    void run();
    void splitFileLength(const QString & filename, int Max);
    void fileCopyStar();
    ~WorkThread();
    void setJob(int jobId, QString src, QString dst/*, qint64 offset, qint64 len*/);
    void setFileStringList(QStringList m_list);
    void setFileTotalSize(qint64 size);
    void setEndFlag(bool flag);
    bool sendBytesCopyed(qint64 cplen, qint64 totallen);

public:
    volatile bool selfCopyEndFlag;
signals:
    void copyedbytes(int jobId, qint64 bytes);
    void jobFinished(int jobId);

public slots:
private:
    bool m_IsAverage;
    qint32 bufferLength;
    volatile bool runningFlag;
    int jobId;
    QString src;
    QString dst;
    qint64 offset;
    qint64 len;
    qint64 copyedBytes;
    QStringList m_wthreadFileList;
    qint64 m_FileTotalSize;
};



#endif // WORKTHREAD_H
