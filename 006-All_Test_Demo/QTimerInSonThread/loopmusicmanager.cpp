#include "loopmusicmanager.h"
#include <QDebug>
const QString MediaPath = QString("/usr/bst/usrfs/Theme/sodimas/media/");

LoopMusicManager::LoopMusicManager(QThread *parent) : QThread(parent)
{
    mTimerLoopMusic = 0;
    mPlayLoopMusicCmd = QString();
    mLoopMusicFlag = false;
    mMusicNumber = 43;
    mBeepvol = 10;
}

void LoopMusicManager::run()
{
    if (mLoopMusicFlag)
    {
        initLoopMusicProcess();
    }
}

void LoopMusicManager::initLoopMusicProcess()
{
    mTimerLoopMusic = new QTimer;
    connect(mTimerLoopMusic, &QTimer::timeout, this, &LoopMusicManager::startLoopMusic);
}

void LoopMusicManager::startLoopMusic()
{
    QString path = MediaPath + QString("M%1.wav").arg(mMusicNumber);
    QString cmd = QString("mplayer -quiet -ao alsa -volume %1 %2 > /dev/null\n").arg(mBeepvol).arg(path);
    mPlayLoopMusicCmd = cmd;
}

void LoopMusicManager::playLoopMusic()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"finishCode = "<<finishCode;
    if (mLoopMusicFlag)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"startMusic again";
        system(mPlayLoopMusicCmd.toLatin1().data());
    }
}

void LoopMusicManager::stopLoopMusic()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;

}

void LoopMusicManager::setBeepvol(int beepvol)
{
    mBeepvol = beepvol;
}

void LoopMusicManager::setMusicNumber(int musicNumber)
{
    mMusicNumber = musicNumber;
}

bool LoopMusicManager::getLoopMusicFlag() const
{
    return mLoopMusicFlag;
}

void LoopMusicManager::setLoopMusicFlag(bool loopMusicFlag)
{
    mLoopMusicFlag = loopMusicFlag;
}

