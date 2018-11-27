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
    void setScreenSaveHoldTime(int ptime);
    void setStandyByFlag(bool flag);
    void exitScreenSaver();

signals:
    void sigSetBlackPicture(bool flag);

public slots:
    void setStandByLuminance(int light);

private slots:
    void startCoverBlackGroundTimer();
    void removeBlackGround();

private:
    void remainStatus();

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
