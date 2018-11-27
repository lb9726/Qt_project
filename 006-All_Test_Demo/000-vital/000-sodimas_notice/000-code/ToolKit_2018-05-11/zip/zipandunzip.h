#ifndef ZIPANDUNZIP_H
#define ZIPANDUNZIP_H

#define MAXFILEFRAME   2000   //>8k
#include <QThread>
#include <QProcess>
#include <QList>
#include <QTimer>

class ZipAndUnZip : public QThread
{
    Q_OBJECT
public:
    explicit ZipAndUnZip(QThread *parent = 0);
    virtual void run();
    ~ZipAndUnZip();
    void setZipFilePath(QString dirNamePath, QString zipFilePath);
    void setZipFlag(bool zipFlag, bool unZipFlag);

signals:
    void workStart(QString msg);
    void error();
    void zipProgress(quint32 ratio);
    void unZipProgress(quint32 ratio);
    void message(QString pInfo);
    void end(QString zip);

public slots:
    //>@ex:(d:/abc/src, d:/dfc/a.zip, true)
    bool Zip(QString pSrcPath, QString pZipFile, bool pCover, bool pIngore = false);   //pIngore表示忽略单文件进度
    //>@ex:(d:/dfc/a.zip, d:/abc/, true)
//    bool Unzip(QString pZipFile, QString pDstPath, bool pCover, bool pIngore = false);
    void Unzip(QString str  );
    void sltUnZipFinished(int code);
    void sltOcurrErrors(QProcess::ProcessError error);
    void sltTimeOut();
    void sltTimeOut2();

public:
    bool   mContainDir;  //>@压缩包中是否包含文件夹名
    int    mIsFinished;

private:
    QString GetDirectoryName(QString DirPath);
    QString getFileDirectory(QString FilePath);
    bool CreatPath(QString pPath);
    QStringList ErgodicDirectory(QString d);
    QString getFileName(QString FilePath);
    QString getParentDirectory(QString DirPath);
    QString  BiteorMega(int peso);
    void generateAscendRandomNumber();

private:
    bool m_ZipFlag;
    bool m_UnZipFlag;
    QString m_ZipFile;
    QString m_DirPath;
    QProcess *m_UnZipProcess;
    QString cmd;
    QList<int> numbersList;
    QTimer* m_UnzipTimer;
};

#endif // ZIPANDUNZIP_H
