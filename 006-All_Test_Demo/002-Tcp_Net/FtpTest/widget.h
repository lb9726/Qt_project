#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "ftpmanager.h"
#include <QNetworkReply>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    FtpManager mFtpManager;

public slots:
    void upload();
    void download();
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void error(QNetworkReply::NetworkError error);
};

#endif // WIDGET_H
