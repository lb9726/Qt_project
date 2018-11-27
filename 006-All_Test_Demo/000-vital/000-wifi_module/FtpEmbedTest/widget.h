#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "ftpmanager.h"
#include <QNetworkReply>
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void upload();
    void download();
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void error(QNetworkReply::NetworkError error);
    void removeFile(QString path);

private:
    Ui::Widget *ui;
    FtpManager mFtpManager;
    QTimer mTimer;
    QTimer mTimerDownLoad;
};

#endif // WIDGET_H
