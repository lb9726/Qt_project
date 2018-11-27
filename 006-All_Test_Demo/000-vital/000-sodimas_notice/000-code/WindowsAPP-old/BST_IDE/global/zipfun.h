#ifndef ZIPFUN_H
#define ZIPFUN_H

#include "globalfun.h"
#ifdef GCC
#include "quazipfile.h"
#endif

#define MAXFILEFRAME   2000   //>8k

class ZipTask : public QObject
{
    Q_OBJECT
public:
    ZipTask() { mContainDir = true; }
    ~ZipTask() {}
signals:
    void start();
    void error();
    void progress(quint32 ratio);
    void message(QString pInfo);
    void end();
public slots:
    bool Zip(QString pSrcPath, QString pDstPath);
    //>@ex:(d:/abc/src, d:/dfc/a.zip, true)
    bool Zip(QString pSrcPath, QString pZipFile, bool pCover, bool pIngore=false);   //pIngore表示忽略单文件进度
    //>@ex:(d:/dfc/a.zip, d:/abc/, true)
    bool Unzip(QString pZipFile, QString pDstPath, bool pCover, bool pIngore=false);
    void Stop(){}
public:
    bool   mContainDir;  //>@压缩包中是否包含文件夹名
};

class ZipTaskThread : public QThread
{
    Q_OBJECT
public:
    ZipTaskThread(QObject *parent = 0);
    void stop()
    {
        if (QThread::currentThread() != this)
            wait();
        exit();
    }
public slots:
    bool Zip(QString pSrcPath, QString pZipFile, bool pCover);
    bool Unzip(QString pZipFile, QString pDstPath, bool pCover);
public:
    ZipTask m_ZipTask;
};

#endif // ZIPFUN_H
