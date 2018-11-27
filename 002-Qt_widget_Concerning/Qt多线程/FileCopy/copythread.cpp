#include "copythread.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>

CopyThread::CopyThread()
{

}

int CopyThread::copyStart()
{
    // loading filelist
    QString fileType;
    QString fileName;
    QDir fileDir;
    QFileInfoList fileInfoList;
    QFileInfo fileInfo;
    int i=0;

    // check path
    fileInfo.setFile(srcPath);
    if(!fileInfo.isDir()) return ERROR_SRC_PATH_NULL;
    fileInfo.setFile(desPath);
    if(!fileInfo.isDir()) return ERROR_DES_PATH_NULL;

    fileList.clear();
    fileTotalSize=0;
    curSize=0;

    fileDir.setPath(srcPath);
    fileDir.setFilter(QDir::Files);             // 只选择文件其他的过滤掉
    fileInfoList = fileDir.entryInfoList();     // 获取文件信息列表

    do{
        fileInfo = fileInfoList.at(i);
        fileType = fileInfo.fileName().split(".").last();

        if(fileType=="png" || fileType=="mp4")   // jpg、avi
        {
            fileTotalSize+=fileInfo.size()/1024;
            fileName=srcPath+"/"+fileInfo.fileName();
            fileList<<fileName;
        }
        else
        {
            fileInfoList.removeOne(fileInfo);
            continue;
        }
        i++;

    }while(i<fileInfoList.size());

    if(0==fileList.count()) return ERROR_NO_FILES;

    bStop=false;
    emit copyStationSig(COPY_START,NULL);
    this->start();
    emit copyStationSig(COPY_STOP,NULL);
    return SUCCESS;
}

void CopyThread::copyStop()
{
    bStop=true;
}

void CopyThread::run()
{
    int i;
    QString fileName;

    for(i=0;i<fileList.count();++i)
    {
        fileName = fileList.at(i);
        emit copyStationSig(COPY_FILE_NAME,fileName.split("/").last());
        if(ERROR_MEM_FULL==fileCopy(fileName))
        {
            bStop=true;
            emit copyStationSig(COPY_ERROR_MEM_FULL,NULL);
            this->exit();
            return;
        }

        if(bStop) break;
    }

    emit copyStationSig(COPY_STOP,fileName);
}

int CopyThread::fileCopy(QString fileName)
{
    QFileInfo fileInfo;
    QString desFileName;
    QByteArray byteArray;
    unsigned long fileSize;
    QString qsStaText;
    unsigned long count=0;
    unsigned long freeSpace= 5*1024*1024;

    desFileName=desPath + "/" + fileName.split("/").last();

    fileInfo.setFile(fileName);
    fileSize=fileInfo.size()/1024;

    if(fileSize>freeSpace) return ERROR_MEM_FULL;

    fileInfo.setFile(desFileName);
    if(fileInfo.isFile())                       // 存在则删除
    {
        qDebug()<<desFileName<<" 该文件已存在！";
        QFile::remove(desFileName);
    }

    // 创建目标文件
    QFile desFile(desFileName);
    desFile.open(QIODevice::WriteOnly);

    // 打开原文件
    QFile srcFile(fileName);
    srcFile.open(QIODevice::ReadOnly);

    while(!srcFile.atEnd())
    {
        count++;
        byteArray=srcFile.read(1024);
        desFile.write(byteArray);

        qsStaText = QString::number(100*count/fileSize);
        emit copyStationSig(COPY_SINGLE_RATE,qsStaText);

        qsStaText = QString::number(100*(curSize+count)/fileTotalSize);
        emit copyStationSig(COPY_TOTAL_RATE,qsStaText);

        if(bStop)
        {
            desFile.close();
            srcFile.close();
            QFile::remove(desFileName);
            return SUCCESS;
        }
    }

    desFile.close();
    srcFile.close();

    curSize+=fileSize;

    return SUCCESS;
}
