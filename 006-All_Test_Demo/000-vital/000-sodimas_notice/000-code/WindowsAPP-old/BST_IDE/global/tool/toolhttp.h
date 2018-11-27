#ifndef TOOLHTTP_H
#define TOOLHTTP_H

#include "global.h"
#include <QtNetwork>
#include <QBuffer>

class ToolHttp : public QObject
{
    Q_OBJECT
public:
    ToolHttp(QObject *parent=0);

    bool UploadFile(QString pSrcFile, QString pDstFile, bool pCover, int pTimeout=60000);
    bool DownloadFile(QString pSrcFile, QString pDstFile, bool pCover, int pTimeout=60000);

    //>@http://%1:8080/?action=snapshot
    void GetPixmapUrl(const QUrl &url);

private slots:
    void slot_requestFinished(int , bool);

public:
#ifndef QT5
    QHttp         *http;
#endif
    QPixmap        m_Pixmap;
    bool           flag;
    QBuffer       *mBuffer;
};

#endif // TOOLHTTP_H
