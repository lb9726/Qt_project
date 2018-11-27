#include "compressclient.h"
#include <QDebug>

CompressClient::CompressClient(QObject *parent) : QObject(parent)
{
    editor = (UiEditor*)parent;
    thread = new QThread(parent);
    jscompress = new  JlCompress();
    connect(jscompress,SIGNAL(compressProgress(int)),this,SLOT(slt_progress(int)));
    connect(jscompress,&JlCompress::compressStart,this,&CompressClient::slt_Started);
    connect(jscompress,&JlCompress::compressFinish,this,&CompressClient::slt_Finished);
    connect(this,&CompressClient::sig_compress,jscompress,&JlCompress::slt_compressDir);

    jscompress->moveToThread(thread);

}

void CompressClient::slt_progress(int progress)
{    
    QVariant retVal;
    QMetaObject::invokeMethod(editor->rootQmlObject, "compressProgress",
                              Q_RETURN_ARG(QVariant, retVal),
                              Q_ARG(QVariant, progress));
    //qDebug()<<progress;
}

void CompressClient::compressDir(QString zipname,QString dirPath)
{
    if(!thread->isRunning()){
        thread->start();
    }
    emit sig_compress(zipname,dirPath);
}

void CompressClient::slt_Started()
{
    //qDebug()<<"slt_Started";
}

void CompressClient::slt_Finished()
{
    QVariant retVal;
    QMetaObject::invokeMethod(editor->rootQmlObject, "compressfinish",
                              Q_RETURN_ARG(QVariant, retVal),
                              Q_ARG(QVariant, ""));
    thread->quit();
}
