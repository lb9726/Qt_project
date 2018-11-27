#include "screensaver.h"
#include "define.h"

ScreenSaver::ScreenSaver(QObject *parent) : QObject(parent)
  , mLastLightGrades(70)
  , mPreWidgetIndex(0)
  , mScreenSaverTime(1800000)
  , mChangeBackTime(5000)
  , mIsStanyBy(false)
{
}

void ScreenSaver::setPreWidgetIndex(int pwidgetIndex)
{   
    mPreWidgetIndex = pwidgetIndex;
    IDE_TRACE_INT(mPreWidgetIndex);
}

void ScreenSaver::setScreenSaverTime(int ptime)
{
    mScreenSaverTime = ptime;
    IDE_TRACE_INT(mScreenSaverTime);
}

void ScreenSaver::setChangeBgTime(int ptime)
{
    mChangeBackTime = ptime;
    IDE_TRACE_INT(mChangeBackTime);
}

void ScreenSaver::setStandyByFlag(bool flag)
{
    IDE_TRACE_INT(mIsStanyBy);
    mIsStanyBy = flag;
    IDE_TRACE_INT(mIsStanyBy);
}

void ScreenSaver::remainStatus()
{
    if (mIsStanyBy)
    {
        QTimer::singleShot(mScreenSaverTime, this, SLOT(startCoverBlackGroundTimer()));
        IDE_TRACE_STR(QString("mTimerScreenSaver is start"));
    }
}

void ScreenSaver::enterScreenSaver(int light)
{
    if (mIsStanyBy)
    {
//        mLastLightGrades = mLightCtrl.getLightGrade();
//        IDE_TRACE_INT(mLastLightGrades);
        mLightCtrl.setBacklightValue(light);
        remainStatus();
    }
}

void ScreenSaver::startCoverBlackGroundTimer()
{
    if (mIsStanyBy)
    {
        IDE_TRACE_STR(QString("mTimerScreenSaver is timeout call"));
        emit sigSetBlackPicture(3);     // 切换为index为3的黑背景画面
        QTimer::singleShot(mChangeBackTime, this, SLOT(coverBlackGround()));
        IDE_TRACE_STR(QString("mTimerCoverBlack is start"));
    }
}

void ScreenSaver::coverBlackGround()
{
    if (mIsStanyBy)
    {
        IDE_TRACE_STR(QString("remove blackGround"));
        emit sigSetBlackPicture(mPreWidgetIndex);       //  切换为index为上一个的背景画面
        remainStatus();                                 //  再次循环盖黑画面和原来的画面
    }
}

void ScreenSaver::exitScreenSaver()
{
    if (mIsStanyBy)
    {
        IDE_TRACE_STR(QString("stop all timers"));
        emit sigSetBlackPicture(mPreWidgetIndex);       // 切换为index为上一个的背景画面
        mLightCtrl.setBacklightValue(mLastLightGrades);
        mIsStanyBy = false;
    }
}
