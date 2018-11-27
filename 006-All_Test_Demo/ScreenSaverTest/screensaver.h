#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include <QObject>
#include <QTimer>
#include "backlightctrl.h"

class ScreenSaver : public QObject
{
    Q_OBJECT
public:
    explicit ScreenSaver(QObject *parent = 0);
    void setWidgetIndex(int pwidgetIndex);
    void setScreenSaverTime(int ptime);
    void setChangeBgTime(int ptime);
signals:
    void sigSetBlackPicture(int index);
public slots:
    void enterScreenSaver(int light);
    void remainStatus();
    void startCoverBlackGroundTimer();
    void coverBlackGround();
    void exitScreenSaver();

private:
    QTimer              mTimerScreenSaver;
    QTimer              mTimerCoverBlack;
    BacklightCtrl       mLightCtrl;
    bool                mIsStanyBy;
    int                 mLastLightGrades;
    int                 mCurWidgetIndex;
    int                 mPreWidgetIndex;
    int                 mScreenSaverTime;
    int                 mChangeBackTime;
};

#endif // SCREENSAVER_H
