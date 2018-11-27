#ifndef BLOCKFILECOPY_H
#define BLOCKFILECOPY_H

#include <QObject>
#include "workthread.h"
#include <QFile>
#include <QDebug>
#define MAXSIZE 20

typedef struct BLOCKSIZE
{
    int blockSize;
}BLOCKSIZE;

//typedef struct _THREADCOPY
//{
//    int id;                // 线程的id
//    WorkThread *wThread;   // 线程
//    int size;              // 文件大小
//    int offset_filehead;   // 文件偏移的位置
//}THREADCOPY;

class BlockFileCopy : public QObject
{
    Q_OBJECT
public:
    explicit BlockFileCopy(QObject *parent = 0);
    ~BlockFileCopy();
    void splitFileLength(const char* filename, int Max);
    void fileCopyStar();
signals:

public slots:

private:
//    WorkThread *wThread[MAXSIZE];
    BLOCKSIZE block[MAXSIZE];
//    THREADCOPY wCopyThread[MAXSIZE];
//    bool m_IsAverage;
};

#endif // BLOCKFILECOPY_H
