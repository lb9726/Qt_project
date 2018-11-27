#include "screensaver.h"
#include "define.h"
/*
 * 用法 *
        mScreenSaver.setStandyByFlag(true);
        mScreenSaver.setScreenSaveHoldTime(m_screensavehold);
        mScreenSaver.setScreenSaverTime(m_screensave);
        mScreenSaver.setStandByLuminance(m_luminance);
*/

ScreenSaver::ScreenSaver(QObject *parent) : QObject(parent)
  , mLastLightGrades(70)
  , mPreWidgetIndex(0)
  , mScreenSaverTime(1800000)
  , mChangeBackTime(5000)
  , mIsStanyBy(false)
{
    qtimer_mScreenSaverTime.setSingleShot(true);
    qtimer_mChangeBackTime.setSingleShot(true);
    connect(&qtimer_mScreenSaverTime,SIGNAL(timeout()),this,SLOT(startCoverBlackGroundTimer()));
    connect(&qtimer_mChangeBackTime,SIGNAL(timeout()),this,SLOT(removeBlackGround()));
}

void ScreenSaver::setPreWidgetIndex(int pwidgetIndex)
{   
    mPreWidgetIndex = pwidgetIndex;
    IDE_TRACE_INT(mPreWidgetIndex);
}

void ScreenSaver::setScreenSaverTime(int ptime)
{
    if (ptime < 0)  // 小于0维持默认值
    {
        ptime = mScreenSaverTime;
    }
    mScreenSaverTime = ptime;
    IDE_TRACE_INT(mScreenSaverTime);
}

void ScreenSaver::setScreenSaveHoldTime(int ptime)
{
    if (ptime < 0)  // 小于0维持默认值
    {
        ptime = mChangeBackTime;
    }
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
        qtimer_mScreenSaverTime.start(mScreenSaverTime);
        IDE_TRACE_STR(QString("mTimerScreenSaver is start"));
    }
}

void ScreenSaver::setStandByLuminance(int light)
{
    if (mIsStanyBy)
    {
        mLastLightGrades = mLightCtrl.getLightGrade();
        IDE_TRACE_INT(mLastLightGrades);
        mLightCtrl.setBacklightValue(light);
        remainStatus();
    }
}

void ScreenSaver::startCoverBlackGroundTimer()
{
    if (mIsStanyBy)
    {
        IDE_TRACE_STR(QString("mTimerScreenSaver is timeout will coverblack"));
        emit sigSetBlackPicture(true);
        qtimer_mChangeBackTime.start(mChangeBackTime);
        IDE_TRACE_STR(QString("mTimer removeBlack is start"));
    }
}

void ScreenSaver::removeBlackGround()
{
    if (mIsStanyBy)
    {
        IDE_TRACE_STR(QString("remove blackGround"));
        emit sigSetBlackPicture(false);
        remainStatus();                                 //  再次循环盖黑画面和原来的画面
    }
}

void ScreenSaver::exitScreenSaver()
{   
    IDE_TRACE_STR(QString("exit screenSaver"));
    mLightCtrl.setBacklightValue(mLastLightGrades);
    mIsStanyBy = false;
    qtimer_mChangeBackTime.stop();
    qtimer_mScreenSaverTime.stop();
}
