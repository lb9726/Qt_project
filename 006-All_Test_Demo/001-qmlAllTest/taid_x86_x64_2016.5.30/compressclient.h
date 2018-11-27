#ifndef COMPRESSCLIENT_H
#define COMPRESSCLIENT_H

#include <QObject>
#include <QThread>
#include "./include/quazip.h"
#include "./include/quazipfile.h"
#include "./include/JlCompress.h"

#include "uieditor.h"

class CompressClient : public QObject
{
    Q_OBJECT
public:
    explicit CompressClient(QObject *parent = 0);
    JlCompress* jscompress;
    QThread* thread;
    QTime time;
signals:
    void sig_compress(QString zipname, QString dirPath);
public slots:
    void slt_progress(int progress);

    void slt_Started();
    void slt_Finished();

public:
    void compressDir(QString zipname, QString dirPath);
    UiEditor* editor;
};
#endif // COMPRESSCLIENT_H
