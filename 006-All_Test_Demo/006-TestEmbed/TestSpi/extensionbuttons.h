#ifndef EXTENSIONBUTTONS_H
#define EXTENSIONBUTTONS_H

#include <QThread>
#include <QMutex>

#define     SPI_PATH                    "/dev/spidev3.0"
#define     ONBOARD_BUTTON_NUMBER       12
#define     EXTEND_BUTTON_NUMBER        24
#define     BUTTON_NUMBER               36

class ExtensionButtons : public QThread
{
    Q_OBJECT
public:
    explicit ExtensionButtons(QThread *parent = 0);
    void run();
    bool initExtension();
    void initStatus();
    void parseSpiData();
    void setLightOn(int index);
    void setLightOff(int index);
signals:
    void sigExtensionBtnPressed(int bNum);
    void sigExtensionBtnCanceled(int bNum);

public slots:

private:
    int         m_extendfd;
    quint8      tx_buf_extend[3];  //0 _ light off, 1 _ light on
    quint8      rx_buf_extend[3];
    QMutex      m_mutex;
    int         m_BtnPressed[EXTEND_BUTTON_NUMBER];
    int         m_isLightOn[EXTEND_BUTTON_NUMBER];
};

#endif // EXTENSIONBUTTONS_H
