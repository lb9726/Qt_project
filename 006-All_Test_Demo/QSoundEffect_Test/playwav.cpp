#include "playwav.h"
#include <QDebug>

const QString MediaPath = QString("/usr/bst/usrfs/Theme/sodimas/media/");

PlayWav::PlayWav(QObject *parent) : QObject(parent)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    mPlayWav = NULL;
}

void PlayWav::initPlayer()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    mPlayWav = new QSoundEffect();
    if (mPlayWav)
    {
        mPlayWav->setSource(MediaPath + "M43.wav");
    }
}

void PlayWav::play()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    if (mPlayWav)
    {
        mPlayWav->play();
    }
}

void PlayWav::stop()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    if (mPlayWav)
    {
        mPlayWav->stop();
    }
}

void PlayWav::setMuted()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    if (mPlayWav)
    {
        mPlayWav->setMuted(true);
    }
}

void PlayWav::setLoopCount(int loopCount)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"loopCount = "<<loopCount;
    if (loopCount < 0)
    {
        if (mPlayWav)
        {
            mPlayWav->setLoopCount(QSoundEffect::Infinite);
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"will loop forever";
        }
    }
    else
    {
        if (mPlayWav)
        {
            mPlayWav->setLoopCount(loopCount);
        }
    }
}

void PlayWav::setVolume(qreal volume)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"volume = "<<volume;
    if (mPlayWav)
    {
        mPlayWav->setVolume(volume);
    }
}
