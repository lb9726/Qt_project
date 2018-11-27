#include "copyjob.h"
#include "define.h"


CopyJob::CopyJob(QThread *parent) : QThread(parent)
{
    runningFlag = true;
    bufferLength = 1024;
    //src = NULL;
    //dst = NULL;
    offset = 0;
    len = 0;
    copyedBytes = 0;
    jobId = -1;
}


void CopyJob::setJob(int jobId,QString src, QString dst, qint64 offset, qint64 len)
{
    this->jobId = jobId;
    this->src = src;
    this->dst = dst;
    this->offset = offset;
    this->len = len;
}

void CopyJob::setBufferLength(qint32 bufLength)
{
    this->bufferLength = bufLength;
}

void CopyJob::run()
{
    //IDE_TRACE_INT(len);
    QFile srcfile(src);
    QFile dstfile(dst);
    if(!srcfile.open(QIODevice::ReadOnly)){
        IDE_TRACE();
        return;
    }

    if(!dstfile.open(QIODevice::WriteOnly)){
        IDE_TRACE();
        return;//18374378/18374379
    }

    srcfile.seek(offset);
    dstfile.seek(offset);
    char * buf = new char[bufferLength];
    //int count = len/bufferLength;
    qint64 readLength = 0;
    copyedBytes = 0;
    while(copyedBytes < len && runningFlag){
        readLength = srcfile.read(buf,bufferLength);
        if((copyedBytes + readLength) <= len){
            dstfile.write(buf,readLength);
            copyedBytes += readLength;
            //emit copyedbytes(jobId,readLength);
        }else{
            dstfile.write(buf,len - copyedBytes);
            copyedBytes = len;
            //emit copyedbytes(jobId,len - copyedBytes);
        }
        emit copyedbytes(jobId,copyedBytes);
        //qDebug()<<"id:"<<jobId <<"copyedBytes: "<<copyedBytes;
    }
    srcfile.close();
    dstfile.close();

    emit jobFinished(jobId);
}


void CopyJob::stopCopy()
{
    runningFlag = false;
}


