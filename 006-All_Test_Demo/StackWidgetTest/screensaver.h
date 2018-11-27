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
    void setPreWidgetIndex(int pwidgetIndex);
    void setScreenSaverTime(int ptime);
    void setChangeBgTime(int ptime);
    void setStandyByFlag(bool flag);

signals:
    void sigSetBlackPicture(int index);

public slots:
    void enterScreenSaver(int light);
    void remainStatus();
    void startCoverBlackGroundTimer();
    void coverBlackGround();
    void exitScreenSaver();

private:
    BacklightCtrl                  mLightCtrl;
    int                            mLastLightGrades;
    int                            mCurWidgetIndex;
    int                            mPreWidgetIndex;
    int                            mScreenSaverTime;
    int                            mChangeBackTime;
    volatile   bool                mIsStanyBy;
};

#endif // SCREENSAVER_H
