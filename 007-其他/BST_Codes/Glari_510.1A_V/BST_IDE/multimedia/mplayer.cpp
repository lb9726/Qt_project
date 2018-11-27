#include "mplayer.h"

mplayer::mplayer(QObject *parent) :
    QObject(parent)
{
    m_MplayerThread = new MplayerThread(this);
    m_PlayProcess = 0;
    m_CurMedia = 0;

    m_Pause = false;
    m_PlayPos = 0;
    m_Volume = 0;
    m_VoRotate = 0;
    m_Mute = false;

    ReleaseMediaBuffer();
    CreatProcess();  //>@因为mplayer要放在一个线程中运行，所有new的东西都应在加入线程前创建，因此只能写在这里。
}

mplayer::~mplayer()
{
    if(m_MplayerThread)
        m_MplayerThread->deleteLater();
    ReleaseMediaBuffer();
    ReleaseProcess();
}

void mplayer::SetFIFO(QString pFifo)
{
#ifndef WINDOWS
    CreatPath(QString("/tmp/fifo/"));
    m_AvFIFO = QString("/tmp/fifo/%1").arg(pFifo);
    const char *tmpFIFO = m_AvFIFO.toAscii().data();
    //unlink(tmpFIFO);
    if(access(tmpFIFO, F_OK) != -1 || mkfifo(tmpFIFO, 0777))
    {
        m_AvFIFO.clear();
    }
#endif
}

void mplayer::CreatProcess()
{
    m_PlayProcess = new QProcess(this);
    connect(m_PlayProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(slot_StandardOutput()));
    connect(m_PlayProcess, SIGNAL(readyReadStandardError()), this, SLOT(slot_StandardError()));
    connect(m_PlayProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slot_Finished(int, QProcess::ExitStatus)));
    connect(m_PlayProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(slot_Error(QProcess::ProcessError)));
    m_PlayProcess->setProcessChannelMode(QProcess::SeparateChannels);
}

bool mplayer::ReleaseProcess()
{
    m_PlayMutex.unlock();
    if(m_PlayProcess)
    {
        disconnect(m_PlayProcess, SIGNAL(readyReadStandardOutput()),0,0);
        disconnect(m_PlayProcess, SIGNAL(readyReadStandardError()),0,0);
        disconnect(m_PlayProcess, SIGNAL(finished(int, QProcess::ExitStatus)),0,0);
        disconnect(m_PlayProcess, SIGNAL(error(QProcess::ProcessError)),0,0);
        Stop();
        delete m_PlayProcess;
        m_PlayProcess = 0;
    }
    ClearEnvir();
    return true;
}

void mplayer::ReleaseMediaBuffer()
{
    PLAY_MEDIA_INFO *tmpMedia = 0;
    foreach(tmpMedia, m_MediaBuffer)
    {
        if(tmpMedia)
            delete tmpMedia;
    }
    m_MediaBuffer.clear();
}

void mplayer::PlayMedia()
{
    if(!m_PlayProcess)
        CreatProcess();
    if(!m_CurCmdString.isEmpty())
        m_PlayProcess->start(m_CurCmdString);
}

bool mplayer::AddMedia(PLAY_MEDIA_INFO *pMeida, bool pPrior)
{
    if(!pMeida)
        return false;
    //>@插入音乐
    if(pPrior)
    {
        //>@如果有新的优先音乐来，则将当前播放的音乐强制设置为播放一次模式，以便于播放下一首
        if(m_CurMedia)
            m_CurMedia->mLoop = 0;
        m_MediaBuffer.push_front(pMeida);
    }
    else
    {
        m_MediaBuffer.push_back(pMeida);
    }
    int tmpMediacount = m_MediaBuffer.count();
    IDE_TRACE_INT(tmpMediacount);
    if(tmpMediacount > D_MAXBUFFERSIZE)  //>@当超过最大缓冲时认为是播放器出错，需要重启播放器。
    {
        IDE_TRACE();
        ReleaseProcess();
    }
    return true;
}

void mplayer::Add(QString pSrc, int pLoop, bool pPrior, int pVolume)
{
    if(QFile::exists(pSrc))
    {
        PLAY_MEDIA_INFO *tmpMeida = new PLAY_MEDIA_INFO;
        tmpMeida->mPath = pSrc;
        tmpMeida->mLoop = pLoop;
        AddMedia(tmpMeida, pPrior);
    }
}

void mplayer::Play(QString pSrc, int pLoop, bool pPrior, int pVolume)
{
    if(QFile::exists(pSrc))
    {
        PLAY_MEDIA_INFO *tmpMeida = new PLAY_MEDIA_INFO;
        tmpMeida->mPath = pSrc;
        tmpMeida->mLoop = pLoop;
        tmpMeida->mVolume = pVolume;
        if(AddMedia(tmpMeida, pPrior))
        {
            if(!IsRunning())
                Play();
        }
    }
}

//>@查询是否可以播放某个音频
bool mplayer::CanPlay(QString pFile)
{
    bool flag = true;
    //>@首先看当前音频缓冲队列是否为空，如果不为空，则查看队列以及当前媒体中是否包含当前到站钟，否则查询当前正在运行的是否为当前到站钟
    if(m_CurMedia && pFile.compare(m_CurMedia->mPath)==0 && IsRunning())
    {
        flag = false;
    }
    if(flag && !m_MediaBuffer.isEmpty())
    {
        PLAY_MEDIA_INFO *tmpMedaiInfo = 0;
        foreach(tmpMedaiInfo, m_MediaBuffer)
        {
            if(!tmpMedaiInfo)
                continue;
            if(pFile.compare(tmpMedaiInfo->mPath)==0)
            {
                flag = false;
                break;
            }
        }
    }
    return flag;
}

bool mplayer::CanPlay(int pAudioNum)
{
    bool flag = true;
    //>@首先看当前音频缓冲队列是否为空，如果不为空，则查看队列以及当前媒体中是否包含当前到站钟，否则查询当前正在运行的是否为当前到站钟
    quint32 tmpNum = GetCurMediaNum();
    if(tmpNum == pAudioNum && IsRunning())
    {
        flag = false;
    }
    if(flag && !m_MediaBuffer.isEmpty())
    {
        PLAY_MEDIA_INFO *tmpMedaiInfo = 0;
        foreach(tmpMedaiInfo, m_MediaBuffer)
        {
            if(!tmpMedaiInfo)
                continue;
            quint32 tmpNum = GetMediaNum(tmpMedaiInfo->mPath);
            if(tmpNum == pAudioNum)
            {
                flag = false;
                break;
            }
        }
    }
    return flag;
}

void mplayer::Play()
{
    m_PlayMutex.lock();
    if(m_CurMedia)  //>@已经有音乐播放了（但已经播放结束），需要根据此音乐的循环属性进行播放
    {
        if(m_CurMedia->mLoop < 0)  //>@单曲循环
        {
            goto PlayRun;
        }
        else if(m_CurMedia->mLoop > 0) //>@循环n次
        {
            m_CurMedia->mLoop--;
            goto PlayRun;
        }
        else                     //>@播放下一首
        {
            delete m_CurMedia;
            m_CurMedia = 0;
            emit sFinished();
        }
    }
    if(m_MediaBuffer.isEmpty())
    {
        m_PlayMutex.unlock();
        return;
    }
    m_CurMedia = m_MediaBuffer.takeFirst();
    if(!m_ConfFile.isEmpty())
    {
        int tmpVolume;
        if(m_CurMedia->mVolume == -1)
            tmpVolume = m_Volume;
        else
            tmpVolume = m_CurMedia->mVolume;
#ifdef WINDOWS
        m_CurCmdString = QString("%1 %2 -slave -volume %3")
                                .arg(D_MPLAYER_PATH)
                                .arg(m_CurMedia->mPath)
                                .arg(m_Mute?(0):(tmpVolume));
#elif UBUNTU
        m_CurCmdString = QString("%1 %2 -volume %3")
                                .arg(D_MPLAYER_PATH)
                                .arg(m_CurMedia->mPath)
                                .arg(m_Mute?(0):(m_Volume));
#else
        m_CurCmdString = QString("%1 -include %2 -volume %3 input=file=%4 %5")
                                .arg(D_MPLAYER_PATH)
                                .arg(m_ConfFile)
                                .arg(m_Mute?(0):(tmpVolume))
                                .arg(m_AvFIFO.toAscii().data())
                                .arg(m_CurMedia->mPath);
#endif
        IDE_DEBUG(m_CurCmdString);
    }
    else
    {
        m_CurCmdString = QString("%1 %2 -slave -framedrop quiet=1 osdlevel=0 volstep=1").arg(D_MPLAYER_PATH).arg(m_CurMedia->mPath);
        if(!m_AvFIFO.isEmpty())
            m_CurCmdString.append(QString(" input=file=%1").arg(m_AvFIFO));
        //>@视频
        if(m_VoRect.isNull())
        {
            m_CurCmdString.append(QString(" novideo=1"));
        }
        else
        {
            m_CurCmdString.append(QString(" noaspect=1"));
            m_CurCmdString.append(QString(" -zoom -x %1 -y %2 -geometry +%3+%4").arg(m_VoRect.width()).arg(m_VoRect.height()).arg(m_VoRect.x()).arg(m_VoRect.y()));
            if(!m_VoDriver.isEmpty())
                m_CurCmdString.append(QString(" vo=%1").arg(m_VoDriver));
            switch(m_VoRotate)
            {
                case 90:
                    m_CurCmdString.append(QString(" -vf rotate=1"));
                    break;
                case 180:
                    m_CurCmdString.append(QString(" -vf flip"));
                    break;
                case 270:
                    m_CurCmdString.append(QString(" -vf rotate=2"));
                    break;
                default:
                    break;
            }
        }
        //>@音频
        if(!m_AoDriver.isEmpty())
            m_CurCmdString.append(QString(" ao=%1").arg(m_AoDriver));
        if(m_Mute)
            m_CurCmdString.append(QString(" -volume 0"));
        else
            m_CurCmdString.append(QString(" -volume %1").arg(m_Volume));
    }
PlayRun:
    m_PlayMutex.unlock();
    m_MplayerThread->run();
}

bool mplayer::IsRunning()
{
    if(!m_PlayProcess)
        CreatProcess();
    bool tmpRunning = !(QProcess::NotRunning == m_PlayProcess->state());
    IDE_TRACE_INT(m_PlayProcess->state());
    return tmpRunning;
}

quint32 mplayer::GetMediaNum(QString pAudio)
{
    int index1 = pAudio.lastIndexOf("/RC");
    if(index1 < 0)
        return INVALIDRC;
    int index2 = pAudio.lastIndexOf('.');
    if(index2 < 0)
        return INVALIDRC;
    index1 += 2;
    return pAudio.mid(index1+1, index2-index1-1).toUInt();
}

quint32 mplayer::GetCurMediaNum()
{
    if(m_CurMedia)
    {
        return GetMediaNum(m_CurMedia->mPath);
    }
    return INVALIDRC;
}

void mplayer::SetConfFile(QString pFile)
{
    m_ConfFile = pFile;
}

void mplayer::SetVoDriver(QString pDriver)
{
    m_VoDriver = pDriver;
}

void mplayer::SetRect(QRect pRect, int pRotate)
{
    QDesktopWidget *tmpDesktop = QApplication::desktop();
    QRect tmpDesktopRect = tmpDesktop->rect();
    m_VoRotate = pRotate;
    switch(m_VoRotate)
    {
        case 90:
            m_VoRect = QRect(tmpDesktopRect.width() - pRect.y() - pRect.height(), pRect.x(), pRect.width(), pRect.height());
            break;
        case 180:
            m_VoRect = QRect(tmpDesktopRect.width() - pRect.x() - pRect.width(), tmpDesktopRect.height() - pRect.y() - pRect.height(), pRect.width(), pRect.height());
            break;
        case 270:
            m_VoRect = QRect(pRect.y(), tmpDesktopRect.height() - pRect.x() - pRect.width(), pRect.width(), pRect.height());
            break;
        default:
            m_VoRect = pRect;
            break;
    }
}

//>@设置音量、驱动
void mplayer::SetAoDriver(QString pDriver)
{
    m_AoDriver = pDriver;
}

void mplayer::SetVolume(int pVolume)
{
    if(pVolume < 0)
        m_Volume = 0;
    else if(pVolume > 100)
        m_Volume = 100;
    else
        m_Volume = pVolume;
    //>@如果当前处于静音状态，则直接退出
    if(m_Mute)
        return;
    if(IsRunning())
        m_PlayProcess->write(QString("volume %1 1\n").arg(m_Volume).toUtf8());
}

void mplayer::SetMute(bool pFlag)
{
    if(m_Mute == pFlag)
        return;
    //>@如果当前是静音模式
    if(m_Mute)
    {
        if(IsRunning())
            m_PlayProcess->write(QString("volume %1 1\n").arg(m_Volume).toUtf8());
    }
    else
    {
        if(IsRunning())
            m_PlayProcess->write(QString("volume 0 1\n").toUtf8());
    }
    m_Mute = pFlag;
}

void mplayer::Seek(int pMs)
{
    if(m_Pause)
        return;
    if(IsRunning())
        m_PlayProcess->write(QString("seek %1 2\n").arg(pMs).toUtf8());
}

void mplayer::Pause()
{
    if(m_Pause)
        return;
    if(IsRunning())
    {
        m_PlayProcess->write(QString("pause\n").toUtf8());
        m_Pause = true;
    }
}

void mplayer::Resume()
{
    if(!m_Pause)
        return;
    if(IsRunning())
    {
        m_PlayProcess->write(QString("pause\n").toUtf8());
        m_Pause = false;
    }
}

bool mplayer::Stop()
{
    quint32 count = 0;
    while(IsRunning())
    {
        m_PlayProcess->kill();
        if(++count > 5)
            return false;
        QtSleep(1);
    }
    m_PlayProcess->waitForFinished(D_WAITTIME);
    m_Pause = false;
    return true;
}

void mplayer::slot_StandardOutput()
{

}

void mplayer::slot_StandardError()
{

}

void mplayer::slot_Finished(int pExitCode, QProcess::ExitStatus pStatus)
{
    IDE_DEBUG(QString(">>>>>>>>>>> mplayer finished : pExitCode : %1, ExitStatus : %2").arg(pExitCode).arg(pStatus));
    Play();
}

void mplayer::slot_Error(QProcess::ProcessError error)
{
    IDE_DEBUG(QString(">>>>>>>>>>> mplayer Error : Error Code : %1").arg(error));
    ReleaseProcess();
    Play();
}

void mplayer::ClearEnvir()
{
#ifndef WINDOWS
#ifdef DISPLAY
    //SYSTEM_CMD::KillProcess("mplayer");
    system("killall mplayer");
    IDE_TRACE();
#endif
#endif
}

MplayerThread::MplayerThread(mplayer *qesp)
    :QThread()
{
    m_Mplayer = qesp;
}

MplayerThread::~MplayerThread()
{
}

void MplayerThread::run()
{
    m_Mplayer->PlayMedia();
}
