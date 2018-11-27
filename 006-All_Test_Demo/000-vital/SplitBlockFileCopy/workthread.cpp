#include "workthread.h"
#include <QCoreApplication>
#include "define.h"

THREADCOPY cpThreadFileLen[MAXSIZE];

WorkThread::WorkThread()
{
    qDebug()<< __PRETTY_FUNCTION__;
    m_FileTotalSize = 0;
    m_wthreadFileList = QStringList();
    runningFlag = true;
    selfCopyEndFlag = false;
    bufferLength = 1024*1024;
    offset = 0;
    len = 0;
    copyedBytes = 0;
    jobId = -1;
    for(int i = 0; i < MAXSIZE; ++i)
    {
        cpThreadFileLen[i].id = -1;
        cpThreadFileLen[i].size = 0;
        cpThreadFileLen[i].offset_filehead = -1;
    }
}

WorkThread::~WorkThread()
{
    qDebug()<< __PRETTY_FUNCTION__<<"is xigou";
}

void WorkThread::run()
{
    QFile srcfile(src);
    QFile dstfile(dst);
    if(!srcfile.open(QIODevice::ReadOnly))
    {
        IDE_TRACE();
        return;
    }

    if(!dstfile.open(QIODevice::ReadWrite))
    {
        IDE_TRACE();
        return;
    }
    dstfile.resize(srcfile.size());
    srcfile.seek(offset);
    dstfile.seek(offset);

    char buf[bufferLength];
    qint64 readLength = 0;
    copyedBytes = 0;
    this->selfCopyEndFlag = false;
    while(copyedBytes < len && runningFlag)
    {
//        if (this->jobId == 0)
//        {
//            qDebug()<< "copyBytes = "<< copyedBytes << "len = "<< len << "offset = "<< offset;

//        }
        readLength = srcfile.read(buf, bufferLength);
        if((copyedBytes + readLength) <= len)
        {// 当已经拷贝的文件长度加上当前读到的文件长度小于等于需要拷贝的长度的时候
            dstfile.write(buf, readLength);  // 因为文件没有结束，可能会读到超过既定长度的文件
            copyedBytes += readLength;
        }
        else
        {
            dstfile.write(buf, len - copyedBytes);
            copyedBytes = len;
        }
        if (copyedBytes == len || sendBytesCopyed(copyedBytes, len))
        {
            emit hasCopyedBytes(jobId, copyedBytes);
//            qDebug()<<__PRETTY_FUNCTION__<<"emit hasCopyedBytes(jobId, copyedBytes)";
        }
        QtSleep(2);
    }
    this->selfCopyEndFlag = true;
    copyedBytes = 0;
    srcfile.close();
    dstfile.close();
    qDebug()<< "run ended!";
}

void WorkThread::setJob(int jobId, QString src, QString dst)
{
    this->jobId = jobId;
    this->src = src;
    this->dst = dst;
    this->offset = cpThreadFileLen[jobId].offset_filehead;
    this->len = cpThreadFileLen[jobId].size;
    qDebug()<< __PRETTY_FUNCTION__;
    qDebug()<< "the copy len = " << len;
}

void WorkThread::setFileStringList(QStringList m_list)
{
    m_wthreadFileList = m_list;
    qDebug()<< "m_wthreadFileList = "<< m_wthreadFileList;
}

void WorkThread::setFileTotalSize(qint64 size)
{
    m_FileTotalSize = size;
    qDebug()<< "m_FileTotalSize = "<< m_FileTotalSize;
}

void WorkThread::setEndFlag(bool flag)
{
    selfCopyEndFlag = flag;
}

bool WorkThread::sendBytesCopyed(qint64 cplen, qint64 totallen)
{
    qint64 totaltemp = totallen / 1024;
    qint64 copytemp = cplen / 1024;
    qint64 tmp = totaltemp / copytemp/10 ;
    // 每1%更新一次进度
    if (tmp == 1 || tmp == 2 || tmp == 3
        || tmp == 4 || tmp == 5 || tmp == 6
        || tmp == 7 || tmp == 8 || tmp == 9 || 10 == tmp)
    {
        return true;
    }
    else if (tmp < 1)
    {
        int tmp1 = totaltemp / copytemp ;
        if (1 == tmp1 || 2 == tmp1 || 3 == tmp1
            || 4 == tmp1 || 5 == tmp1 || 6 == tmp1
            || 7 == tmp1 || 8 == tmp1 || 9 == tmp1 || 10 == tmp1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


void WorkThread::splitFileLength(const QString &filename, int Max)
{
    qDebug()<< __PRETTY_FUNCTION__;
    QFile file(filename);
    int flen = file.size();
    int blocksize = flen / Max;  // Max 是最大的线程数
    if (0 == flen - blocksize * Max) // 刚好能整除
    {
        int i = 0;
        for(i = 0; i < Max; ++i)
        {
            cpThreadFileLen[i].id = i;
            cpThreadFileLen[i].size = blocksize;
            cpThreadFileLen[i].offset_filehead = cpThreadFileLen[i].id * cpThreadFileLen[i].size;
        }
        m_IsAverage = true;
    }
    else if (0 <= flen - blocksize * Max) // 不能整除，最后一个块将比其他的多一点
    {
        int i = 0;
        for(i = 0; i < Max-1; ++i)
        {
            cpThreadFileLen[i].id = i;
            cpThreadFileLen[i].size = blocksize;
            cpThreadFileLen[i].offset_filehead = cpThreadFileLen[i].id * cpThreadFileLen[i].size;
        }
        cpThreadFileLen[i].id = i;
        cpThreadFileLen[i].size = flen - blocksize * (Max-1);
        cpThreadFileLen[i].offset_filehead = cpThreadFileLen[i].id * cpThreadFileLen[i-1].size;
        m_IsAverage = false;
    }
}
