#ifndef ZIPFUNCTION_H
#define ZIPFUNCTION_H

#define MAXFILEFRAME   2000   //>8k
#include <QObject>
#include <QThread>

//typedef enum
//{
//    Failed = 0,
//    Success,
//    Start,
//    Timeout,
//    Unstart,
////    Stopped
//}TaskState;

class ZipFunction : public QThread
{
    Q_OBJECT
public:
    explicit ZipFunction(QThread *parent = 0);
    ~ZipFunction();
    virtual void run();
    void setZipFilePath(QString dirNamePath, QString zipFilePath);
    void setZipFlag(bool zipFlag, bool unZipFlag);

signals:
    void workStart(QString msg);
    void error();
    void zipProgress(quint32 ratio);
    void unZipProgress(quint32 ratio);
    void message(QString pInfo);
    void end(QString zip);

//    void startZip(QString pSrcPath, QString pZipFile, bool pCover, bool pIngore = false);
//    void startUnZip(QString pZipFile, QString pDstPath, bool pCover, bool pIngore = false);

public slots:
    //>@ex:(d:/abc/src, d:/dfc/a.zip, true)
    bool Zip(QString pSrcPath, QString pZipFile, bool pCover, bool pIngore = false);   //pIngore表示忽略单文件进度
    //>@ex:(d:/dfc/a.zip, d:/abc/, true)
    bool Unzip(QString pZipFile, QString pDstPath, bool pCover, bool pIngore = false);
    void Stop(){}
public:
    bool   mContainDir;  //>@压缩包中是否包含文件夹名

private:
    QString GetDirectoryName(QString DirPath);
    QString getFileDirectory(QString FilePath);
    bool CreatPath(QString pPath);
    QStringList ErgodicDirectory(QString d);
    QString getFileName(QString FilePath);
    QString getParentDirectory(QString DirPath);
    QString  BiteorMega(int peso);

private:
    bool m_ZipFlag;
    bool m_UnZipFlag;
    QString m_ZipFile;
    QString m_DirPath;
//    QThread mZipTaskThread;
};

#endif // ZIPFUNCTION_H
