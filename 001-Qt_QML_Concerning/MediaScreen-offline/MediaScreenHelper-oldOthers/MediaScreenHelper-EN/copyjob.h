#ifndef COPYJOB_H
#define COPYJOB_H

#include <QObject>
#include <QThread>
#include <QFile>

class CopyJob : public QThread
{
    Q_OBJECT
public:
    explicit CopyJob(QThread *parent = 0);
    void setJob(int jobId, QString src, QString dst, qint64 offset, qint64 len);
    void setBufferLength(qint32 bufLength);
    void run();

signals:
    void copyedbytes(int jobId,qint64 bytes);
    void jobFinished(int jobId);
public slots:
    void stopCopy();
private:
    qint32 bufferLength;
    volatile bool runningFlag;
    int jobId;
    //QFile* src;
    //QFile* dst;
    QString src;
    QString dst;
    qint64 offset;
    qint64 len;
    qint64 copyedBytes;
};


#endif // COPYJOB_H
