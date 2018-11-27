#ifndef UIICON_H
#define UIICON_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QListView>
#include <QProgressBar>
#include <QStringListModel>
#include "usbaction.h"


#include "QEventLoop"
#include <QTimer>
#include <QTime>
#include <unistd.h>
#include <QCoreApplication>

inline void QtSleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
#if defined(Q_OS_WIN)
        QEventLoop eventloop;
        QTimer::singleShot(msec, &eventloop, SLOT(quit()));
        eventloop.exec();
#else
        usleep(1000);//sleep和usleep都已经obsolete，建议使用nanosleep代替
#endif
    }
}

namespace Ui {
class UiIcon;
}

class UiIcon : public QWidget
{
    Q_OBJECT

public:
    explicit UiIcon(QWidget *parent = 0);
    ~UiIcon();
    void initComponent();
    void setHaveCopyFlag(bool flag);

private:
    void setUsbUI(bool flag, QString str);
public:

//private slots:
//    void setUsbConnected();
    void showCopyScreen();
    void updateProgress(int num);
    void promptUpdateComplete();

public slots:
    void showBackUpScreen();
    void setUsbRemoveFlag();

private:
    Ui::UiIcon *ui;
    QLabel                  *mUsbPicture;
    QLabel                  *mLabelWaring;
    QListView               *mTextView;
    QProgressBar            *mCopyProgress;

    QStringListModel        model;

    bool                    mHaveCopy;
    bool                    mBackUp;
    bool                    mUpdateComplete;
    UsbAction               mUsbAct;
};

#endif // UIICON_H
