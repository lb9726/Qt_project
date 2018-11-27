#include "mplayer.h"

#if (defined(UBUNTU) || defined(LINUX))
#include <signal.h>
#include <sys/types.h>
#endif

mplayer::mplayer(QObject *parent) :
    QThread(parent)
{
    m_DefDelay = 300;
    m_Quit = false;
    m_AbortAudio = false;
    m_PlayProcess = NULL;
    m_CurMedia = 0;
    m_BufferIndex = -1;
    m_BufferLoop = LOOP_NONE;

    m_KillCount = 0;

    m_Enable = false;
    m_Pause = false;
    m_PlayPos = 0;
    m_Volume[0] = m_Volume[1] = 0;
    m_VoRotate = 0;

    start(HighPriority);
}

mplayer::~mplayer()
{
    ForceQuit();
    wait(1000);
    Clear();
}

void mplayer::Wait(int pTimeout)
{
    quint32 i=0;
    while((i+=10) < pTimeout && !m_Quit)
        msleep(10);
}

void mplayer::SetPlayer(QString pExeFile)
{
    if(pExeFile.isEmpty())
        return;
    if(!QFile::exists(pExeFile))
        return;
    m_ExePath = pExeFile;
}

void mplayer::run()
{
    while(!m_Quit)
    {
        if(!m_PlayProcess)
            CreatProcess();  //>@因为QProcess的建立与使用必须在一个线程中
        if(m_Pause || !m_StopBuffers.isEmpty())
        {
            Wait(m_DefDelay);
            continue;
        }

        //>@恢复音频现场
        AudioFinished();

        //>@获取缓冲区中最新资源
        if(!UpdateMedia())  //>@如果没有获取新资源，表示没有新资源
        {
            Wait(m_DefDelay);
            continue;
        }

        if(!m_Enable)
        {
            if(!m_CurMedia->mPath.contains("beep.wav"))
            {
                Wait(m_DefDelay);
                continue;
            }
        }

        m_AbortAudio = false;

        //>@开始播放音频
        m_PlayProcess->start(m_CurCmdString);
        if(!m_PlayProcess->waitForStarted(1000))
        {
            Abort();
            IDE_DEBUG(QString("Track[%1] : audio start error!").arg(m_TrackNum));
            continue;
        }
        if(m_CurMedia->mEmitSig)
            emit sStarted();
        IDE_DEBUG(QString("Track[%1] PID:%2 - %3!").arg(m_TrackNum).arg((int)(m_PlayProcess->pid())).arg(m_CurCmdString));

        //>@等待其结束
        if(m_CurMedia->mLoop == LOOP_EVER)
        {
            while(!m_AbortAudio)
            {
                Wait(50);
            }
            m_CurMedia->mLoop == LOOP_NONE;
        }
        else
        {
            if(m_CurMedia->mPath.endsWith(".wav", Qt::CaseInsensitive))
                m_ElapseTime = GetWavTime(m_CurMedia->mPath) + D_MONITORDELAY;  //>@多出100ms进行计算
            else if(m_CurMedia->mPath.endsWith(".mp3", Qt::CaseInsensitive))
                m_ElapseTime = GetMp3Time(m_CurMedia->mPath) + D_MONITORDELAY;  //>@多出100ms进行计算
            //IDE_TRACE_INT(m_ElapseTime);
            quint64 tmpElapseTime = m_ElapseTime * (m_CurMedia->mLoop + 1);
            //IDE_TRACE_INT(tmpElapseTime);
            int count = tmpElapseTime / 100;
            while(!m_AbortAudio && IsRunning())
            {
                count--;
                Wait(100);
                if(count == 0)
                {
                    break;
                }
            }
        }

        if(IsRunning())
        {
            Abort();
            IDE_DEBUG(QString("Track[%1] : audio finish error!").arg(m_TrackNum));
        }

        if(m_CurMedia && m_CurMedia->mEmitSig)  //>@成功发送结束信号，失败发送中止信号
        {
            IDE_DEBUG(QString("Track[%1] : audio finished!").arg(m_TrackNum));
            emit sFinished();
        }
    }
    Abort();
}

void mplayer::CreatProcess()
{
    m_PlayProcess = new QProcess;
    m_PlayProcess->closeReadChannel(QProcess::StandardOutput);
    m_PlayProcess->closeReadChannel(QProcess::StandardError);
    //connect(m_PlayProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(slot_StandardOutput()));
    //connect(m_PlayProcess, SIGNAL(readyReadStandardError()), this, SLOT(slot_StandardError()));
    //m_PlayProcess->setProcessChannelMode(QProcess::SeparateChannels);
}

bool mplayer::ReleaseProcess()
{
    if(m_PlayProcess)
    {
        m_KillCount = 0;
        //disconnect(m_PlayProcess, SIGNAL(readyReadStandardOutput()),0,0);
        //disconnect(m_PlayProcess, SIGNAL(readyReadStandardError()),0,0);
        int count = 0;
        while((count++ < 10) && FindPidInProc((int)(m_PlayProcess->pid())))
        {
            m_PlayProcess->write("stop\n");
            m_PlayProcess->waitForFinished(2000);
            if(!IsRunning())
                break;
            m_PlayProcess->terminate();   //>@QProcess::close() 调用 QProcess::kill(),使用此句会在播放43帧（重复）音频时阻塞住。
        }
        m_PlayProcess->closeWriteChannel();
        //KillPlayer();
        QtSleep(100);

        //IDE_DEBUG(QString("Track[%1] : error %2 state %3!").arg(m_TrackNum).arg(m_PlayProcess->error()).arg(m_PlayProcess->state()));

        m_PlayProcess->deleteLater();
        m_PlayProcess = 0;
    }
    return true;
}

bool mplayer::WriteProcess(const char *pCmd, int pTimeout, bool pForce)
{
    if(pForce)
    {
        m_PlayProcess->write(pCmd);
        if(pTimeout)
            return m_PlayProcess->waitForBytesWritten(pTimeout);
        return true;
    }
    else if(IsRunning())
    {
        m_PlayProcess->write(pCmd);
        if(pTimeout)
            return m_PlayProcess->waitForBytesWritten(pTimeout);
        return true;
    }
    return false;
}

bool mplayer::WriteProcess(QString pCmd, int pTimeout, bool pForce)
{
    return WriteProcess(pCmd.toUtf8().data(), pTimeout, pForce);
}

bool mplayer::AddMedia(PLAY_MEDIA_INFO *pMeida, bool pPrior)
{
    if(!pMeida)
        return false;

    //>@插入音乐
    //>@查看是否超出最大缓冲区
    int tmpMediacount = m_MediaBuffer.count();
    if(tmpMediacount > D_MAXBUFFERSIZE)  //>@当超过最大缓冲时认为是播放器出错，则清空缓冲区
    {
        IDE_DEBUG(QString("Track[%1] : Media buffer is overload!").arg(m_TrackNum));
        Clear();
    }

    if(pPrior) //>@如果有新的优先音乐来
    {
        m_MediaBuffer.push_front(pMeida);
    }
    else
    {
        //>@如果插入的是个循环播放的音频，则需要确认之前的一个音频是否也为相同的循环音频。不允许插入两个相邻的相同循环音乐
        if(pMeida->mLoop == LOOP_EVER)
        {
            PLAY_MEDIA_INFO *tmpMeida = 0;
            if(!m_MediaBuffer.isEmpty())
                tmpMeida = m_MediaBuffer.last();
            else
                tmpMeida = m_CurMedia;
            if(tmpMeida && (tmpMeida->mLoop == LOOP_EVER) && (!tmpMeida->mPath.compare(pMeida->mPath)))
                return false;
            m_MediaBuffer.push_back(pMeida);
        }
        else
        {
            m_MediaBuffer.push_back(pMeida);
        }
    }

    //>@当有新音频插入时，如果当前播放的音频为循环播放，则退出循环播放模式，以便于播放下一首
    if(m_CurMedia)
    {
        if(m_CurMedia->mLoop != LOOP_NONE)
        {
            m_AbortAudio = true;
        }
    }

    IDE_DEBUG(QString("Track[%1] : Media buffer size is %2 !").arg(m_TrackNum).arg(m_MediaBuffer.count()));
    return true;
}

bool mplayer::UpdateMedia()
{
    if(m_CurMedia)  //>@已经有音乐播放了（但已经播放结束），需要根据此音乐的循环属性进行播放
    {
        if(m_BufferLoop == LOOP_LIST)
        {
            int count = m_MediaBuffer.count();
            if(count == 0)
                m_BufferIndex = -1;
            else if(m_BufferIndex == (count-1))
                m_BufferIndex = 0;
            else
                m_BufferIndex++;
        }
        else
        {
            delete m_CurMedia;
            m_CurMedia = 0;
        }
    }
    if(m_MediaBuffer.isEmpty())
    {
        //IDE_DEBUG(QString("Track[%1] : Media buffer is empty, emit finish signals!").arg(m_TrackNum));
        emit sNull();
        return false;
    }
    //>@获取最新资源
    if(m_BufferLoop == LOOP_LIST)
    {
        if(m_BufferIndex < 0)
            m_BufferIndex = 0;
        m_CurMedia = m_MediaBuffer.at(m_BufferIndex);
    }
    else
    {
        m_CurMedia = m_MediaBuffer.takeFirst();
    }
    if(!m_CurMedia)
    {
        IDE_DEBUG(QString("Track[%1] : Current media is invalid!").arg(m_TrackNum));
        return false;
    }
    //>@更新播放指令
    m_CurCmdString = UpdateCmdString(m_CurMedia);
    if(m_CurCmdString.isEmpty())
    {
        IDE_DEBUG(QString("Track[%1] : Current command is invalid!").arg(m_TrackNum));
        return false;
    }
    return true;
}

QString mplayer::UpdateCmdString(PLAY_MEDIA_INFO *pMeidaInfo)
{
    if(!pMeidaInfo)
        return QString();
    if(m_ExePath.isEmpty())
        m_ExePath = D_MPLAYER_PATH;
    QString tmpCmdString;
    if(!m_ConfFile.isEmpty())
    {
        if(pMeidaInfo->mVolume >= 0)
            m_Volume[0] = pMeidaInfo->mVolume;
        int loop = 1;
        if(pMeidaInfo->mLoop == LOOP_EVER)
            loop = 0;
        else
            loop = pMeidaInfo->mLoop+1;
#ifdef WINDOWS
        tmpCmdString = QString("%1 -slave -volume %2 -loop %3 %4")
                                .arg(m_ExePath)
                                .arg(m_Volume[0])
                                .arg(loop)
                                .arg(pMeidaInfo->mPath);
#elif UBUNTU
        //>@音量从-40到10
        int tmpNewVolume = (m_Volume[0]/2)-40;
        QString tmpMplayer = "mplayer";
        tmpCmdString = QString("%1 -af volume=%2 -loop %3 %4")
                                .arg(tmpMplayer)
                                .arg(loop)
                                .arg(tmpNewVolume)
                                .arg(pMeidaInfo->mPath);
#else
        tmpCmdString = QString("%1 -include %2 -volume %3 -loop %4 %5 &")
                                .arg(m_ExePath)
                                .arg(m_ConfFile)
                                .arg(m_Volume[0])
                                .arg(loop)
                                .arg(pMeidaInfo->mPath);
#endif
    }
    else
    {
        tmpCmdString = QString("%1 %2 -slave -framedrop quiet=1 osdlevel=0 volstep=1").arg(m_ExePath).arg(pMeidaInfo->mPath);
        //>@视频
        if(m_VoRect.isNull())
        {
            tmpCmdString.append(QString(" novideo=1"));
        }
        else
        {
            tmpCmdString.append(QString(" noaspect=1"));
            tmpCmdString.append(QString(" -zoom -x %1 -y %2 -geometry +%3+%4").arg(m_VoRect.width()).arg(m_VoRect.height()).arg(m_VoRect.x()).arg(m_VoRect.y()));
            if(!m_VoDriver.isEmpty())
                tmpCmdString.append(QString(" vo=%1").arg(m_VoDriver));
            switch(m_VoRotate)
            {
                case 90:
                    tmpCmdString.append(QString(" -vf rotate=1"));
                    break;
                case 180:
                    tmpCmdString.append(QString(" -vf flip"));
                    break;
                case 270:
                    tmpCmdString.append(QString(" -vf rotate=2"));
                    break;
                default:
                    break;
            }
        }
        //>@音频
        if(!m_AoDriver.isEmpty())
            tmpCmdString.append(QString(" ao=%1").arg(m_AoDriver));
        if(m_CurMedia->mVolume < 0)
            m_CurMedia->mVolume = m_Volume[0];
        tmpCmdString.append(QString(" -volume %1").arg(m_CurMedia->mVolume));
    }
    return tmpCmdString;
}

bool mplayer::FindInMediaBuffer(QString pAudioFile, QList<PLAY_MEDIA_INFO*> &pResultList, bool pFindall)
{
    pResultList.clear();
    //>@首先看当前音频缓冲队列是否为空，如果不为空，则查看队列以及当前媒体中是否包含当前到站钟，否则查询当前正在运行的是否为当前到站钟
    if(IsRunning() && m_CurMedia && !pAudioFile.compare(m_CurMedia->mPath))
    {
        pResultList.append(m_CurMedia);
        return true;
    }
    bool flag = false;  //>@默认没有找到
    if(!m_MediaBuffer.isEmpty())
    {
        PLAY_MEDIA_INFO *tmpMedaiInfo = 0;
        foreach(tmpMedaiInfo, m_MediaBuffer)
        {
            if(!tmpMedaiInfo)
                continue;
            if(pAudioFile.compare(tmpMedaiInfo->mPath)==0)
            {
                pResultList.append(tmpMedaiInfo);
                flag = true;
                if(!pFindall)
                    break;
            }
        }
    }
    return flag;
}

//>@查询是否可以播放某个音频
bool mplayer::CanPlay(QString pFile)
{
    QList<PLAY_MEDIA_INFO*> tmpResultList;
    return !FindInMediaBuffer(pFile, tmpResultList);
}

PLAY_MEDIA_INFO *mplayer::GetMedia()
{
    PLAY_MEDIA_INFO *tmpMedia = m_CurMedia;
    return tmpMedia;
}

int mplayer::GetMediaBufferSize()
{
    int count = m_MediaBuffer.count();
    return count;
}

bool mplayer::Add(int pAoType, QString pSrc, int pLoop, bool pPrior, int pVolume, bool pEmitSig)
{
    if(QFile::exists(pSrc))
    {
        IDE_TRACE_INT(pVolume);
        PLAY_MEDIA_INFO *tmpMeida = new PLAY_MEDIA_INFO;
        tmpMeida->mType = (AO_TYPE)pAoType;
        tmpMeida->mPath = pSrc;
        tmpMeida->mLoop = pLoop;
        tmpMeida->mVolume = pVolume;
        tmpMeida->mEmitSig = pEmitSig;
        if(AddMedia(tmpMeida, pPrior))
            return true;
        delete tmpMeida;
    }
    return false;
}

bool mplayer::TestMedia(int pAoType, QString pSrc, int pVolume)
{
    return Add(pAoType, pSrc, LOOP_NONE, true, pVolume, false);
}

//>@停止某一个音频文件的播放
void mplayer::StopOne(QString pFile)
{
    //>@首先看当前音频缓冲队列是否为空，如果不为空，则查看队列以及当前媒体中是否包含当前到站钟，否则查询当前正在运行的是否为当前到站钟
    if(!m_MediaBuffer.isEmpty())
    {
        for(int i=m_MediaBuffer.count()-1;i>=0;i--)
        {
            PLAY_MEDIA_INFO *tmpMedia = m_MediaBuffer.at(i);
            if(tmpMedia && !(pFile.compare(tmpMedia->mPath)))
            {
                m_MediaBuffer.takeAt(i);
                delete tmpMedia;
            }
        }
    }
    if(m_CurMedia && !(pFile.compare(m_CurMedia->mPath)) && IsRunning())
    {
        m_AbortAudio = true;
    }
}

void mplayer::Playback(LOOP_TYPE pLoop)
{
    if(m_CurMedia)
        m_CurMedia->mLoop = pLoop;
}

bool mplayer::FindPidInProc(int pPID)
{
    //IDE_TRACE_INT(pPID);
    if(pPID < 300)
        return false;
    //>@查询proc下有无pid文件即可
    QFile tmpFile(QString("/proc/") + QString::number(pPID));
    return tmpFile.exists();
}

bool mplayer::IsRunning()
{
    if(!m_PlayProcess)
        return false;
    bool tmpRunning = !(QProcess::NotRunning == m_PlayProcess->state());
    //IDE_DEBUG(QString("Track[%1] PID[%2] : state %3, running: %4").arg(m_TrackNum).arg((int)(m_PlayProcess->pid())).arg(m_PlayProcess->state()).arg(tmpRunning));
    return tmpRunning;
}

int mplayer::GetPlayPercent()
{
    if(!/*m_PlayProcess*/IsRunning())
    {
        IDE_TRACE();
        return -1;
    }
    IDE_DEBUG(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  GetPlayPercent:begin  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    WriteProcess("get_time_pos\n", D_PROCESSWAIT, true);  //>@返回ANS_TIME_POSITION=14.8，表示当前运行了多少秒
    WriteProcess("get_time_length\n", D_PROCESSWAIT, true);  //>@返回ANS_LENGTH=220.00，表示总共有多少秒
    WriteProcess("get_percent_pos\n", D_PROCESSWAIT, true);
    m_PlayProcess->waitForReadyRead(1000);
    char *tmpKeyString = "ANS_PERCENT_POSITION=";
    int tmpPercent = -1;
    while(m_PlayProcess->canReadLine())
    {
        QByteArray tmpLine = m_PlayProcess->readLine(1024);
        if(tmpLine.startsWith(tmpKeyString))
        {
            QList<QByteArray> tmpList = tmpLine.split('=');
            if(tmpList.count() == 2)
            {
                QString tmpInt = tmpList.at(1);
                tmpInt.remove('\n');
                tmpPercent = tmpInt.toInt();
            }
            break;
        }
    }

    return tmpPercent;
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

void mplayer::SetMixerDriver(QString pDriver)
{
    m_MixerDriver = pDriver;
}

void mplayer::SetVolume(int pAoType, int pVolume, bool pSync) //>@pSync表示是否同步修改系统音量
{
    IDE_TRACE_INT(pVolume);
    if(pVolume < 0)
        m_Volume[0] = 0;
    else if(pVolume > 100)
        m_Volume[0] = 100;
    else
        m_Volume[0] = pVolume;
#ifdef UBUNTU
    m_Volume[0] = (m_Volume[0]/2)-40;
#endif
    if(pSync)
        m_Volume[1] = m_Volume[0];
    //>@同步设置当前所有缓冲区中的音量，否则会出现当下一首是在设置音量前加入的缓冲区，还是按照之前设置的音量播放的bug
    for(int i=0;i<m_MediaBuffer.count();i++)
    {
        PLAY_MEDIA_INFO *tmpInfo = m_MediaBuffer.at(i);
        if(tmpInfo && ((pAoType == AO_NONE) || (pAoType == tmpInfo->mType)))  //>@当pAoType为None或者与音频类型相同时才进行更改
            tmpInfo->mVolume = m_Volume[0];
    }
    if(m_CurMedia && ((pAoType == AO_NONE) || (pAoType == m_CurMedia->mType)))
    {
        m_CurMedia->mVolume = m_Volume[0];
    }
    WriteProcess(QString("volume %1 1\n").arg(m_Volume[0]));
}

void mplayer::Seek(int pMs)
{
    if(m_Pause)
        return;
    WriteProcess(QString("seek %1 2\n").arg(pMs));
}

/*process->write("balance 1 1\n");   右声道
  process->write("balance -1 1\n");  左声道*/
void mplayer::SetBalance(int pBalance)
{
    WriteProcess(QString("balance %1 1\n").arg(pBalance));
}

bool mplayer::Pause(bool pEnable)
{
    SetEnable(!pEnable);
    if(!IsRunning())
    {
        m_Pause = false;
        return false;
    }
    if(m_Pause == pEnable)
    {
        IDE_DEBUG(QString("Track[%1] : Repeat pause is ignored!").arg(m_TrackNum));
        return true;
    }
    if(pEnable)
    {
        IDE_DEBUG(QString("Track[%1] : Pause!").arg(m_TrackNum));
#if 1
        SetVolume(AO_NONE, 0);
#else
        WriteProcess("pause\n", true);
#endif
    }
    else
    {
        IDE_DEBUG(QString("Track[%1] : Resume!").arg(m_TrackNum));
#if 1
        SetVolume(AO_NONE, m_Volume[1]);
#else
        WriteProcess("pause\n", true);
#endif
    }
    m_Pause = pEnable;
    return true;
}

void mplayer::Abort()  //>@只在线程中使用
{
    AudioFinished();
    if(IsRunning())
    {
        ReleaseProcess();
        emit sAborted();
    }
}

void mplayer::PlayHead()
{
    IDE_TRACE_STR();
    Stop(true);//>@关闭当前音乐
    m_BufferIndex = -1;
    Stop(false);
}

void mplayer::PlayEnd()
{
    IDE_TRACE_STR();
    Stop(true);//>@关闭当前音乐
    int count = m_MediaBuffer.count();
    if(count >= 1)
        m_BufferIndex = count - 2;
    Stop(false);
}

void mplayer::PlayNext()
{
    IDE_TRACE_STR();
    Stop(true);//>@关闭当前音乐
    Stop(false);
}

bool mplayer::Stop(bool pEnable, int pType)
{
    if(pEnable)
    {
        if(!m_StopBuffers.contains(pType))
            m_StopBuffers.append(pType);
        SetEnable(false);
        if(m_PlayProcess)
            m_PlayProcess->write("stop\n");
        m_AbortAudio = true;
    }
    else
    {
        m_StopBuffers.removeOne(pType);
        SetEnable(true);
    }
    return true;
}

//>@Clear执行后会将新的音频继续播放下去，因此不需要执行slot_finished.
bool mplayer::Clear()
{
    IDE_DEBUG(QString("Track[%1] : Stop and clear Media buffer!").arg(m_TrackNum));
    PLAY_MEDIA_INFO *tmpMedia = 0;
    foreach(tmpMedia, m_MediaBuffer)
    {
        if(tmpMedia)
            delete tmpMedia;
    }
    m_MediaBuffer.clear();
    if(m_CurMedia)
        m_CurMedia->mLoop = LOOP_NONE;
    return true;
}

void mplayer::slot_StandardOutput()
{
    //IDE_DEBUG(m_PlayProcess->readAllStandardOutput());
}

void mplayer::slot_StandardError()
{
    //IDE_DEBUG(m_PlayProcess->readAllStandardError());
}

void mplayer::AudioFinished()
{
    m_Pause = false;
}

void mplayer::KillPlayer()
{
    m_KillCount++;
#ifdef DISPLAY
#if (defined(UBUNTU) || defined(LINUX))
    int pid = -1;
    if(m_PlayProcess)
        pid = m_PlayProcess->pid();  //>@表示m_PlayProcess已经被杀死了。
    if(pid == 0)
    {
        IDE_DEBUG("Track[%1] : audio has finished!");
        return;
    }
#if 1
    QString tmpCmd;
    if(pid < 300)
        tmpCmd = QString("killall mplayer");
    else
        tmpCmd = QString("kill -9 %1").arg(pid);
    IDE_DEBUG(QString("Track[%1] : kill mplayer [%2 times] - %3").arg(m_TrackNum).arg(m_KillCount).arg(tmpCmd));
    system(tmpCmd.toLatin1().data());
#else
    IDE_DEBUG(QString("Track[%1] : kill mplayer [%2 times] - kill(pid, 2)").arg(m_TrackNum).arg(m_KillCount));
    kill((pid_t)pid, 2);
#endif
#else
    SYSTEM_CMD::KillProcess("mplayer.exe");
#endif
#endif
}


