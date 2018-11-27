#include "toolhttp.h"

ToolHttp::ToolHttp(QObject *parent) :
    QObject(parent)
{
#ifndef QT5
    http = new QHttp(this);
    connect(http, SIGNAL(requestFinished(int,bool)), this, SLOT(slot_requestFinished(int , bool)));
#endif
    flag = 0;
    mBuffer = new QBuffer();
}

bool ToolHttp::UploadFile(QString pSrcFile, QString pDstFile, bool pCover, int pTimeout)
{
    return true;
}

bool ToolHttp::DownloadFile(QString pSrcFile, QString pDstFile, bool pCover, int pTimeout)
{
    return false;
}

void ToolHttp::GetPixmapUrl(const QUrl &url)
{
    if(!flag)
    {
#ifndef QT5
        http->setHost(url.host(), url.port(80));
        http->get("/?action=snapshot", mBuffer);
#endif
        flag = 1;
    }
}

void ToolHttp::slot_requestFinished(int , bool)
{
    QByteArray byteArray =  mBuffer->buffer();
    m_Pixmap.loadFromData(byteArray);
    flag = 0;
    mBuffer->seek(0);
    mBuffer->buffer().clear();
}

