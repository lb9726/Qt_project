#ifndef ZIPFUNCTION_H
#define ZIPFUNCTION_H

#define MAXFILEFRAME   2000   //>8k
#include <QObject>
#include <QThread>

typedef enum
{
    Failed = 0,
    Success,
    Start,
    Timeout,
    Stopped,
    Unstart
}TaskState;

class ZipFunction : public QObject
{
    Q_OBJECT
public:
    explicit ZipFunction(QObject *parent = 0);
    ~ZipFunction() {}

signals:
    void start();
    void error();
    void progress(quint32 ratio);
    void message(QString pInfo);
    void end();

    void startZip(QString pSrcPath, QString pZipFile, bool pCover, bool pIngore = false);
    void startUnZip(QString pZipFile, QString pDstPath, bool pCover, bool pIngore = false);

public slots:
//    bool Zip(QString pSrcPath, QString pDstPath);
    //>@ex:(d:/abc/src, d:/dfc/a.zip, true)
    bool Zip(QString pSrcPath, QString pZipFile, bool pCover, bool pIngore = false);   //pIngore表示忽略单文件进度
    //>@ex:(d:/dfc/a.zip, d:/abc/, true)
    bool Unzip(QString pZipFile, QString pDstPath, bool pCover, bool pIngore = false);
    void Stop(){}
public:
    bool   mContainDir;  //>@压缩包中是否包含文件夹名

private:
    QThread mZipTaskThread;
};

#endif // ZIPFUNCTION_H
