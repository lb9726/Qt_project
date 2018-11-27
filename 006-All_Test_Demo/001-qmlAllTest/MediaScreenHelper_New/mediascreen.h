#ifndef MEDIASCREEN_H
#define MEDIASCREEN_H

#include <QObject>
#include "configureholder.h"
#include "ctranslator.h"
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QVariant>

#if defined(Q_OS_WIN)
#include <windows.h>
#include "UsbFormat/usbeventfilter_win32.h"
#elif defined(Q_OS_LINUX)
#include "Common/qdevicewatcher/qdevicewatcher.h"
#include "Common/qdevicewatcher/qdevicewatcher_p.h"
#include "Common/UsbFormat/usbhelper_linux.h"
#endif

class MediaScreen : public QObject
{
    Q_OBJECT
public:
    explicit MediaScreen(QObject *parent = 0);
    void init();
    ~MediaScreen();
    void getLocalIp();
#if defined(Q_OS_WIN)
    UsbEventFilter* getPoint();
#elif defined(Q_OS_LINUX)
    void usbDetected();
#endif

signals:
    void clearAudioParament(int lan_index);
    void getScrollTextLengthSignal();
    void hasUdiskInsertToComputer(QString uDiskName);

public slots:
    void changeUi();
    void setIndex(int i);
    QVariant getIndex();
    void getScrollTextLength();
    int getSystemLanguage();
    void setPreIndex(int i);
    QVariant getPreIndex();
    QString getIndexIpString(int index);
    QVariant getModel(int index);
    QString getSplitTime(int index);
    QString getSplitDate(int index);
    QString getDateFormat(bool isYmd);
    QString getTimeFormat(bool is12);
    void setUdiskIsInsertFlag(bool flag, QString udiskIndex);
    void sendUdiskInformation();

private:
    int index;
    int preIndex;
    QQmlContext *content;
    QQmlApplicationEngine* engine;
    ConfigureHolder *configureSerialer;
    QString ipString;
    bool isInsertUdiskFlag;
    QString udiskIndexStr;

#if defined(Q_OS_WIN)
    UsbEventFilter *nativeFilter;
#elif defined(Q_OS_LINUX)
    UsbHelper mUsbHelper;
#endif

};

#endif // MEDIASCREEN_H
