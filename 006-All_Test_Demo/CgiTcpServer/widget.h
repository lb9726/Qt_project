#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "wificommunication.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void init();
    void sltGetAllParameter();
    void sltParameterSetUp(QString key, QVariant value);
    void sltRebootMachine(bool flag);
    void sltAppLogin(QString ssid, QString passwd);

    void sltWebUpload(QString fileName);
    void sltShowUploadFileMessage(bool pFlag);

private:
    Ui::Widget *ui;
    WifiCommunication          *mWiFi;
};

#endif // WIDGET_H
