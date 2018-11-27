#include "mplayer.h"
#include <QString>
#include <QByteArray>
#include <QDebug>
#ifdef LINUX
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/types.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <QProcess>
#endif

#define D_MONITORDELAY  500
#define D_PROCESSWAIT   200

#define D_MAXBUFFERSIZE   10

#define MPLAYER_STD_BUFFER_LENGHT 1024

const  char* GET_CURRENT_POSITION = "get_percent_pos\n";
const  char* PAUSE_PLAYER = "pause\n";

int backgroundPlayer_fd;
int backgroundPlayerPipe[2];
QString backgroundMusicPath = "/usr/bst/usrfs/Theme/sodimas/music/M0.wav";
QString backgroundVolume = "10";

bool initBackgroundplayerProcess()
{
#ifdef LINUX
    pid_t pid;
    unlink("/tmp/musicplayer_fifo");
    if (mkfifo("/tmp/musicplayer_fifo", O_CREAT|0666) == -1)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
        return false;
    }
    if (pipe(backgroundPlayerPipe) == -1)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
        return false;
    }
    if ((pid = fork()) == -1)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
        return false;
    }
    else if (pid == 0)
    {
        close(backgroundPlayerPipe[0]);
        dup2(backgroundPlayerPipe[1], 1);
        execlp("mplayer", "mplayer", "-slave", "-quiet", "-input", "file=/tmp/musicplayer_fifo", \
               "-ao", "alsa", "-loop", "0", "-volume", backgroundVolume.toStdString().c_str(), \
               backgroundMusicPath.toStdString().c_str(), (char *)NULL);
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
        if ((backgroundPlayer_fd = open("/tmp/musicplayer_fifo", O_RDWR)) == -1)
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
            return false;
        }
    }
#endif
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    return true;
}

Mplayer::Mplayer()
{
    m_DefDelay = 1000;
    m_AbortAudio = false;
    m_CurMedia = 0;
    m_BufferIndex = -1;
    m_BufferLoop = LOOP_NONE;
    m_backgroundPlayerAlive = false;
    m_backgroundPlayerPaused = true;
    m_TestTimer = new QTimer;
    QObject::connect(m_TestTimer, SIGNAL(timeout()), this, SLOT(backgroundVolumeKeeper()));
    m_TestTimer->setInterval(2000);
    m_backgroundVolume = 0;
}

Mplayer::~Mplayer()
{
    if (m_TestTimer)
    {
        m_TestTimer->stop();
        m_TestTimer->deleteLater();
        m_TestTimer = 0;
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"delete m_TestTimer";
    }
}

void Mplayer::setBackgroundMuisc(QString  musicPath, int volume)
{
    if (QFile::exists(musicPath))
    {
        backgroundMusicPath = musicPath;
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<musicPath<<"is not exists";
    }
    if (volume > 0 && volume < 100)
    {
        m_backgroundVolume = volume;
        backgroundVolume = QString("%1").arg(volume);
    }
}

void Mplayer::startWork()
{
    if (QFile::exists(backgroundMusicPath))
    {
        m_backgroundPlayerAlive = initBackgroundplayerProcess();
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"startwork OK";
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"file is not exists!";
    }
    start();
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"after start";
    m_TestTimer->start();
}

void Mplayer::run()
{
    while(1)
    {
        if (!updateMediaBuffer())   //如果没有获取新资源，表示没有新资源
        {
            msleep(m_DefDelay);
            continue;
        }
        system(m_CurCmdString.toStdString().c_str());
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"after system m_CurCmdString = "<<m_CurCmdString;
    }
}

bool Mplayer::updateMediaBuffer()
{
    if (m_CurMedia)  //>@已经有音乐播放了（但已经播放结束），需要根据此音乐的循环属性进行播放
    {
        if (m_BufferLoop == LOOP_LIST)
        {
            int count = m_MediaBuffer.count();
            if (0 == count)
            {
                m_BufferIndex = -1;
            }
            else if (m_BufferIndex == (count - 1))
            {
                m_BufferIndex = 0;
            }
            else
            {
                m_BufferIndex++;
            }
        }
        else
        {
            delete m_CurMedia;
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"delete m_CurMedia";
            m_CurMedia = 0;
        }
    }
    if (m_MediaBuffer.isEmpty())
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"file is empty!";
        return false;
    }
    //获取最新资源
    if (m_BufferLoop == LOOP_LIST)
    {
        if (m_BufferIndex < 0)
        {
            m_BufferIndex = 0;
        }
        m_CurMedia = m_MediaBuffer.at(m_BufferIndex);
    }
    else
    {
        int count0 = m_MediaBuffer.count();
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"before len = "<<count0;
        m_CurMedia = m_MediaBuffer.takeFirst();
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"after len = "<<count0;
    }

    if (!m_CurMedia)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"Current media is invalid!";
        return false;
    }
    if (!QFile::exists(m_CurMedia->mPath))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<m_CurMedia->mPath<<"is not exist";
        return false;
    }
    m_CurCmdString = QString("mplayer -quiet -ao alsa -volume %1 %2 > /dev/null\n").arg(m_CurMedia->mVolume).arg(m_CurMedia->mPath);

    if (m_CurCmdString.isEmpty())
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"Current command is invalid!";
        return false;
    }
    return true;
}

bool Mplayer::addMediaToBuffer(PLAY_MEDIA_INFO *pMeida, bool pPrior)
{
    if (!pMeida)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"pMeida is NULL return false";
        return false;
    }

    int tmpMediacount = m_MediaBuffer.count();
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"tmpMediacount = "<<tmpMediacount;
    if (tmpMediacount > D_MAXBUFFERSIZE)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"Media buffer is overload will delete!";
        clearMediaBuffer();
    }

    if (pPrior)
    {
        m_MediaBuffer.push_front(pMeida);
    }
    else
    {
        ///如果插入的是个循环播放的音频，则需要确认之前的一个音频是否也为相同的循环音频。
        ///不允许插入两个相邻的相同循环音乐
        if (pMeida->mLoop == LOOP_EVER)
        {
            PLAY_MEDIA_INFO *tmpMeida = 0;
            if (!m_MediaBuffer.isEmpty())
            {
                tmpMeida = m_MediaBuffer.last();
            }
            else
            {
                tmpMeida = m_CurMedia;
            }
            if (tmpMeida && (tmpMeida->mLoop == LOOP_EVER) && (!tmpMeida->mPath.compare(pMeida->mPath)))
            {
                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"will return false";
                return false;
            }
            m_MediaBuffer.push_back(pMeida);
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"LOOP_EVER m_MediaBuffer.push_back(pMeida)";
        }
        else
        {
            m_MediaBuffer.push_back(pMeida);
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"Not LOOP_EVER m_MediaBuffer.push_back(pMeida)";
            int lenght = m_MediaBuffer.length();
            int lencount = m_MediaBuffer.count();
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"lenth = "<<lenght;
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"lencount = "<<lencount;
        }
    }
    ///当有新音频插入时，如果当前播放的音频为循环播放，则退出循环播放模式，以便于播放下一首
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"will return true";
    return true;
}

bool Mplayer::clearMediaBuffer()
{
    IDE_DEBUG(QString("Stop and clear Media buffer!"));
    PLAY_MEDIA_INFO *tmpMedia = 0;
    foreach(tmpMedia, m_MediaBuffer)
    {
        if (tmpMedia)
        {
            delete tmpMedia;
            tmpMedia = 0;
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<"delete tmpMeida";
        }
    }
    m_MediaBuffer.clear();
    if (m_CurMedia)
    {
        m_CurMedia->mLoop = LOOP_NONE;
    }
    return true;
}

void Mplayer::pauseBackgroundPlayer()
{
    if (m_backgroundPlayerAlive && !m_backgroundPlayerPaused)
    {
        m_backgroundPlayerPaused = true;
        write(backgroundPlayer_fd, PAUSE_PLAYER, strlen(PAUSE_PLAYER));
    }
}

void Mplayer::playBackgroundPlayer()
{
    if (m_backgroundPlayerAlive && m_backgroundPlayerPaused)
    {
        m_backgroundPlayerPaused = false;
        write(backgroundPlayer_fd, PAUSE_PLAYER, strlen(PAUSE_PLAYER));
    }
}

void Mplayer::setBackgroundPlayerVolume(int volume)
{
    if (volume < 0 || volume > 100)
    {
        return;
    }
    m_backgroundVolume = volume;
    QString volumeCmd = QString("volume %1 1\n").arg(volume);
    write(backgroundPlayer_fd, volumeCmd.toStdString().c_str(), volumeCmd.size());
}

void Mplayer::delayTimeMs(quint32 pTimeout)
{
    quint32 i = 0;
    while(i < pTimeout)
    {
        msleep(10);
        i += 10;
    }
}

void Mplayer::backgroundVolumeKeeper()
{
    setBackgroundPlayerVolume(m_backgroundVolume);
}

bool Mplayer::addMedia(int pAoType, QString pSrc, int pLoop, bool pPrior, int pVolume)
{
    if (QFile::exists(pSrc))
    {
        PLAY_MEDIA_INFO *tmpMeida = new PLAY_MEDIA_INFO;
        if (tmpMeida)
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"new PLAY_MEDIA_INFO = "<<tmpMeida;
            tmpMeida->mType = (AO_TYPE)pAoType;
            tmpMeida->mPath = pSrc;
            tmpMeida->mLoop = pLoop;
            tmpMeida->mVolume = pVolume;
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"new PLAY_MEDIA_INFO is NULL ";
            return false;
        }
        if (addMediaToBuffer(tmpMeida, pPrior))
        {
            return true;
        }
        delete tmpMeida;
        tmpMeida = 0;
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<"delete tmpMeida";
    }
    return false;
}

