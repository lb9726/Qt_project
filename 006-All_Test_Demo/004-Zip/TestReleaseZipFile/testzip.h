#ifndef TESTZIP_H
#define TESTZIP_H

#include <QObject>
#include "zipfun.h"
#include "global.h"
#include <QString>

class TestZip : public QObject
{
    Q_OBJECT
public:
    explicit TestZip(QObject *parent = 0);
    //>@非缓冲解压、压缩，isLocal表示是否为本地解压、压缩
    bool UnZip(QString pSrc, QString pDst, qint64 pTimeout, bool isLocal);
    void init();
signals:
    void sZip(QString pSrc, QString pDir, bool pCover, bool pIngore);
    void sUnzip(QString pSrc, QString pDir, bool pCover, bool pIngore);

public:
    TaskState               m_ZipTaskState;
    bool                    m_runThread;
private:
    QString m_runPath;
    ZipTaskThread ztt;

public slots:
    void sProgress(quint32 ppro);
};

#endif // TESTZIP_H
