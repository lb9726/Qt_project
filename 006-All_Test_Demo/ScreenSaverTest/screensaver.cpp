#include "screensaver.h"
#include "define.h"

ScreenSaver::ScreenSaver(QObject *parent) : QObject(parent)
{
    mLastLightGrades = 70;
    mPreWidgetIndex = 0;
    mScreenSaverTime = 1800000;
    mChangeBackTime = 5000;
}

void ScreenSaver::setWidgetIndex(int pwidgetIndex)
{
    mPreWidgetIndex = pwidgetIndex;
}

void ScreenSaver::setScreenSaverTime(int ptime)
{
    mScreenSaverTime = ptime;
}

void ScreenSaver::setChangeBgTime(int ptime)
{
    mChangeBackTime = ptime;
}

void ScreenSaver::remainStatus()
{
    disconnect(&mTimerScreenSaver, SIGNAL(timeout()), this, SLOT(startCoverBlackGroundTimer()));
    connect(&mTimerScreenSaver, SIGNAL(timeout()), this, SLOT(startCoverBlackGroundTimer()));
    mTimerScreenSaver.start(mScreenSaverTime);
    IDE_TRACE_STR(QString("mTimerScreenSaver is start"));
}

void ScreenSaver::enterScreenSaver(int light)
{
    mLastLightGrades = mLightCtrl.getLightGrade();
    mLightCtrl.setBacklightValue(light);
    remainStatus();
}

void ScreenSaver::startCoverBlackGroundTimer()
{
    IDE_TRACE_STR(QString("mTimerScreenSaver is timeout call"));
    disconnect(&mTimerCoverBlack, SIGNAL(timeout()), this, SLOT(coverBlackGround()));
    connect(&mTimerCoverBlack, SIGNAL(timeout()), this, SLOT(coverBlackGround()));
    emit sigSetBlackPicture(4);     // 切换为index为4的黑背景画面
    mTimerCoverBlack.start(mChangeBackTime);
    IDE_TRACE_STR(QString("mTimerCoverBlack is start"));
}

void ScreenSaver::coverBlackGround()
{
    IDE_TRACE_STR(QString("remove blackGround"));
    emit sigSetBlackPicture(mPreWidgetIndex);       //  切换为index为上一个的背景画面
    remainStatus();                                 //  再次循环盖黑画面和原来的画面
}

void ScreenSaver::exitScreenSaver()
{
    IDE_TRACE_STR(QString("stop all timers"));
    mTimerScreenSaver.stop();
    mTimerScreenSaver.stop();
    emit sigSetBlackPicture(mPreWidgetIndex);       // 切换为index为上一个的背景画面
    mLightCtrl.setBacklightValue(mLastLightGrades);
}
