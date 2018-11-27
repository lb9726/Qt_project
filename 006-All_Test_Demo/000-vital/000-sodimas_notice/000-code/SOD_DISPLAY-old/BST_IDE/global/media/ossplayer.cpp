#include "ossplayer.h"

ossplayer::ossplayer(QObject *parent) :
    QThread(parent)
{
    m_DefDelay = 300;
    m_Quit = false;

    m_PlayerFd = m_MixerFd = 0;
    m_CurMedia = 0;
    m_BufferIndex = -1;
    m_BufferLoop = LOOP_NONE;
    m_Pause = false;
    m_Enable = false;
    m_PlayPos = 0;
    m_Volume[0] = m_Volume[1] = 0;

    m_StopBuffers.clear();

    AudioFinished();
    start(LowestPriority);
}

ossplayer::~ossplayer()
{
    ForceQuit();
    wait(1000);
    Clear();
}

void ossplayer::Wait(int pTimeout)
{
    quint32 i=0;
    while((i+=10) < pTimeout && !m_Quit)
        msleep(10);
}

void ossplayer::run()
{
    if(!QString::compare(m_MixerDriver,"/dev/mixer2",Qt::CaseSensitive)){
        m_DefDelay = 1000;
    }

    while(!m_Quit)
    {
        //IDE_TRACE_STR(m_MixerDriver);
        if(m_Pause)
        {
            Wait(m_DefDelay);
            continue;
        }
        //>@获取缓冲区中最新资源
        if(!UpdateMedia())  //>@如果没有获取新资源，表示没有新资源
        {
            Wait(m_DefDelay);
            continue;
        }
        //IDE_TRACE_STR(m_MixerDriver);

        if(!QString::compare(m_MixerDriver,"/dev/mixer2",Qt::CaseSensitive)){
            if(!m_Enable)
            {
                //IDE_TRACE_STR(m_MixerDriver);
                Wait(m_DefDelay);
                continue;
                //                if(!m_CurMedia->mPath.contains("beep"))
                //                {
                //                    Wait(m_DefDelay);
                //                    continue;
                //                }
            }

//            if(!m_StopBuffers.isEmpty())
//            {
//                IDE_TRACE_STR(m_MixerDriver);
//                Wait(m_DefDelay);
//                continue;

//                if(!m_CurMedia->mPath.contains("beep"))
//                {
//                    Wait(m_DefDelay);
//                    continue;
//                }
//            }
        }

        //>@打开音频驱动
        if(!m_PlayerFd || !m_MixerFd)
        {
            if(!OpenPlayer())
            {
                IDE_DEBUG(QString("Track[%1] : open driver error!").arg(m_TrackNum));
                m_AoMode = AO_DRVERROR;
                Wait(m_DefDelay);
                continue;
            }
        }
        //IDE_TRACE();
        //>@恢复音频现场
        AudioFinished();

        //>@打开音频
        if(m_AudioFile.isOpen())
            m_AudioFile.close();
        m_AudioFile.setFileName(m_CurMedia->mPath);
        if(!m_AudioFile.open(QIODevice::ReadOnly))
        {
            IDE_DEBUG(QString("Track[%1] : open %2 error!").arg(m_TrackNum).arg(m_CurMedia->mPath));
            m_AoMode = AO_RCERROR;
            Wait(m_DefDelay);
            continue;
        }
        //IDE_TRACE_STR(m_CurMedia->mPath);

        //>@提取参数
        int tmpWavHeaderSize = sizeof(WAVContainer_t);
        //IDE_TRACE_INT(tmpWavHeaderSize);
        WAVContainer_t *tmpAudioInfo = (WAVContainer_t*)malloc(tmpWavHeaderSize);
        if(WAV_ReadHeader(m_AudioFile.handle(), tmpAudioInfo) == -1)
        {
            m_AoMode = AO_FMTERROR;
            IDE_DEBUG(QString("Track[%1] : %2 is invalid wav file!").arg(m_TrackNum).arg(m_CurMedia->mPath));
            free(tmpAudioInfo);
            m_AudioFile.close();
            Wait(m_DefDelay);
            continue;
        }
        //WAV_P_PrintHeader(tmpAudioInfo);

        ioctl(m_PlayerFd, SNDCTL_DSP_SETFMT, &tmpAudioInfo->format.format);     //AFMT_S16_LE;/*standard 16bit little endian format, support this format only*/
        ioctl(m_PlayerFd, SNDCTL_DSP_SPEED, &tmpAudioInfo->format.sample_rate);
        //ioctl(m_PlayerFd, SNDCTL_DSP_CHANNELS, &tmpAudioInfo->format.channels);
        //IDE_TRACE_INT(tmpAudioInfo->format.channels);

        //设置音量 //0~0x1f1f(分别为左右声道);
        if(m_CurMedia->mVolume < 0)
            m_CurMedia->mVolume = m_Volume[0];
        //IDE_TRACE_INT(m_CurMedia->mVolume);
        SetVolume(m_CurMedia->mVolume);

        //>@提取参数
        int tmpDspBlkSize;
        ioctl(m_PlayerFd, SNDCTL_DSP_GETBLKSIZE, &tmpDspBlkSize);
        if(tmpDspBlkSize < 1024)
        {
            m_AoMode = AO_UNKNOWERROR;
            IDE_DEBUG(QString("Track[%1] : can't get dsp blk size!").arg(m_TrackNum));
            free(tmpAudioInfo);
            m_AudioFile.close();
            Wait(m_DefDelay);
            continue;
        }
        char *buffer = (char *)malloc(tmpDspBlkSize);
        if(buffer == NULL)
        {
            m_AoMode = AO_MEMERROR;
            IDE_DEBUG(QString("Track[%1] : Can't alloc memory!").arg(m_TrackNum));
            free(tmpAudioInfo);
            m_AudioFile.close();
            Wait(m_DefDelay);
            continue;
        }
        //IDE_TRACE_INT(tmpDspBlkSize);

        int tmpSize = 0;
        //>@开始播放音频
        m_AoMode = AO_PLAYING;
        if(m_CurMedia->mEmitSig)
        {
            //IDE_DEBUG(QString("Track[%1] : %2 started!").arg(m_TrackNum).arg(m_CurMedia->mPath));
            emit sStarted();
        }
        //>@查找pCM数据段，只要查找头信息中的data字段，然后6个字节后面的数据即为PCM数据
        m_AudioFile.seek(0);
        QByteArray tmpArray = m_AudioFile.read(1024);
        int index = tmpArray.indexOf("data")+6;
        while(!m_AbortAudio)
        {
            IDE_DEBUG(QString("Track[%1] : playing %2 times!").arg(m_TrackNum).arg(m_CurMedia->mLoop));
            bool ret = m_AudioFile.seek(index);
            while(!m_AudioFile.atEnd() && !m_AbortAudio)
            {
                while(m_Pause && !m_AbortAudio)
                {
                    //IDE_DEBUG(QString("Track[%1] : pause ...").arg(m_TrackNum));
                    m_AoMode = AO_PAUSE;
                    msleep(10);
                }
                m_AoMode = AO_PLAYING;
                if(m_AbortAudio)
                    break;

                memset(buffer, 0, tmpDspBlkSize);
                if((tmpSize = m_AudioFile.read(buffer, tmpDspBlkSize)) <= 0)
                    break;
                //IDE_DEBUG(QString("Track[%1] : playing size %2 ...").arg(m_TrackNum).arg(tmpSize));

                int count = 0;
                audio_buf_info info;
                do{
                    ioctl(m_PlayerFd , SNDCTL_DSP_GETOSPACE , &info);
                    usleep(100);
                    //IDE_DEBUG(QString("Track[%1]").arg(m_TrackNum));
                }while((info.bytes < tmpDspBlkSize) && (count++ < 100));

                fd_set writefds;
                struct timeval tv;
                tv.tv_sec       = 0;
                tv.tv_usec      = 0;
                FD_ZERO( &writefds );
                FD_SET( m_PlayerFd , &writefds );
                tv.tv_sec       = 0;
                tv.tv_usec      = 0;
                //IDE_DEBUG(QString("Track[%1]").arg(m_TrackNum));
                select( m_PlayerFd + 1 , NULL , &writefds , NULL, &tv );
                //IDE_DEBUG(QString("Track[%1]").arg(m_TrackNum));
                if( FD_ISSET( m_PlayerFd, &writefds ))
                {
                    //IDE_DEBUG(QString("Track[%1]").arg(m_TrackNum));
                    write(m_PlayerFd, buffer, tmpDspBlkSize);
                }
                usleep(100);
            }

            int bytes;
            ioctl(m_PlayerFd,SNDCTL_DSP_GETODELAY,&bytes);
            int delay = bytes / (tmpAudioInfo->format.sample_rate * 2 * tmpAudioInfo->format.channels);
            //IDE_TRACE_INT(delay);
            sleep(delay);

            IDE_DEBUG(QString("Track[%1] : %2 is end!").arg(m_TrackNum).arg(m_CurMedia->mPath));

            //>@循环播放参数
            if(m_CurMedia->mLoop == -1)
            {
                usleep(10000);
                continue;
            }
            else if(m_CurMedia->mLoop == 0)
            {
                break;
            }
            else
            {
                usleep(10000);
                m_CurMedia->mLoop--;
            }
        }

        free(buffer);
        free(tmpAudioInfo);
        m_AudioFile.close();

        if(m_AbortAudio)
            m_AoMode = AO_ABORT;
        else
            m_AoMode = AO_FINISHED;
        if(m_CurMedia->mEmitSig)  //>@成功发送结束信号，失败发送中止信号
        {
            IDE_DEBUG(QString("Track[%1] : %2 finished!").arg(m_TrackNum).arg(m_CurMedia->mPath));
            emit sFinished();
        }

    }
}

bool ossplayer::AddMedia(PLAY_MEDIA_INFO *pMeida, bool pPrior)
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

bool ossplayer::UpdateMedia()
{
    //IDE_TRACE();
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
    return true;
}

bool ossplayer::OpenPlayer()
{
    ClosePlayer();
    m_PlayerFd = open(m_AoDriver.toLatin1().data(), O_RDWR);
    if(m_PlayerFd < 0)
    {
        IDE_DEBUG("Open dsp error");
        return false;
    }
    m_MixerFd = open(m_MixerDriver.toLatin1().data(), O_RDWR);
    if(m_MixerFd < 0)
    {
        IDE_DEBUG("Open mixer error");
        return false;
    }
    return true;
}

bool ossplayer::ClosePlayer()
{
    if(m_PlayerFd)
    {
        close(m_PlayerFd);
        m_PlayerFd = 0;
    }
    else if(m_MixerFd)
    {
        close(m_MixerFd);
        m_MixerFd = 0;
    }
    return true;
}

bool ossplayer::FindInMediaBuffer(QString pAudioFile, QList<PLAY_MEDIA_INFO*> &pResultList, bool pFindall)
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
bool ossplayer::CanPlay(QString pFile)
{
    QList<PLAY_MEDIA_INFO*> tmpResultList;
    return !FindInMediaBuffer(pFile, tmpResultList);
}

PLAY_MEDIA_INFO *ossplayer::GetMedia()
{
    PLAY_MEDIA_INFO *tmpMedia = m_CurMedia;
    return tmpMedia;
}

int ossplayer::GetMediaBufferSize()
{
    int count = m_MediaBuffer.count();
    return count;
}

bool ossplayer::Add(int pAoType, QString pSrc, int pLoop, bool pPrior, int pVolume, bool pEmitSig)
{
    if(QFile::exists(pSrc))
    {
        //IDE_TRACE_INT(pVolume);
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

bool ossplayer::TestMedia(int pAoType, QString pSrc, int pVolume)
{
    return Add(pAoType, pSrc, LOOP_NONE, true, pVolume, false);
}

//>@停止某一个音频文件的播放
void ossplayer::StopOne(QString pFile)
{
    ///IDE_TRACE();
    //>@首先看当前音频缓冲队列是否为空，如果不为空，则查看队列以及当前媒体中是否包含当前到站钟，否则查询当前正在运行的是否为当前到站钟
    if(!m_MediaBuffer.isEmpty())
    {
        //IDE_TRACE();
        for(int i=m_MediaBuffer.count()-1;i>=0;i--)
        {
            //IDE_TRACE_INT(i);
            PLAY_MEDIA_INFO *tmpMedia = m_MediaBuffer.at(i);
            if(tmpMedia && !(pFile.compare(tmpMedia->mPath)))
            {
                m_MediaBuffer.takeAt(i);
                delete tmpMedia;
            }
        }
    }
    //IDE_TRACE();
    if(m_CurMedia && !(pFile.compare(m_CurMedia->mPath)) && IsRunning())
    {
        m_AbortAudio = true;
    }
}

void ossplayer::Playback(LOOP_TYPE pLoop)
{
    if(m_CurMedia)
        m_CurMedia->mLoop = pLoop;
}

void ossplayer::PlayNext()
{
    //IDE_TRACE();
    Stop(true);//>@关闭当前音乐
    Stop(false);
}

void ossplayer::PlayHead()
{
    //IDE_TRACE();
    Stop(true);//>@关闭当前音乐
    m_BufferIndex = -1;
    Stop(false);
}

void ossplayer::PlayEnd()
{
    //IDE_TRACE();
    Stop(true);//>@关闭当前音乐
    int count = m_MediaBuffer.count();
    if(count >= 1)
        m_BufferIndex = count - 2;
    Stop(false);
}

bool ossplayer::IsRunning()
{
    if(m_AoMode == AO_PLAYING)
    {
        IDE_DEBUG(QString("Track[%1] : Running!").arg(m_TrackNum));
        return true;
    }
    IDE_DEBUG(QString("Track[%1] : NoRunning - %2!").arg(m_TrackNum).arg(m_AoMode));
    return false;
}

int ossplayer::GetPlayPercent()
{
    int tmpPercent = -1;
    if(IsRunning())
    {
        if(m_AudioFile.isOpen())
        {
            return (qreal)m_AudioFile.pos() * 100.0 / (qreal)m_AudioFile.size();
        }
    }
    return tmpPercent;
}

void ossplayer::SetMixerDriver(QString pDriver)
{
    m_MixerDriver = pDriver;
}

//>@设置音量、驱动
void ossplayer::SetAoDriver(QString pDriver)
{
    m_AoDriver = pDriver;
}

void ossplayer::SetVolume(int pVolume)
{
    if(m_MixerFd == NULL)
    {
        IDE_DEBUG(QString("Track[%1] : mixer is invalide!").arg(m_TrackNum));
        return;
    }
#ifdef N329
    //    quint32 tmpPcmVol = 0x5050;
    //    ioctl(m_MixerFd, MIXER_WRITE(SOUND_MIXER_PCM), &tmpPcmVol);
    //>@SPU_ANALOG_VOLUME指的是Codec的音量，每个声道范围为0~0x1f
    quint32 tmpCodecVol = 0x1f1f;
    ioctl(m_MixerFd, MIXER_WRITE(SPU_ANALOG_VOLUME), &tmpCodecVol);
    //>@使用127.5*(tan((x/200-0.25) * pi)+1)
    //quint32 tmpSoloVol = 127.5 * (tan(((qreal)pVolume/200 - 0.25)*PI) + 1);
    quint32 tmpSoloVol = (qreal)pVolume * 0.6375;  //>@最大值为64，很奇怪
    quint32 tmpVolume = tmpSoloVol + (tmpSoloVol<<8);
    int channels;
    //>@SOUND_MIXER_LINE/SOUND_MIXER_VOLUME指的是每个声道的的音量，每个声道范围为0~0x7f
    if(!m_MixerDriver.compare("/dev/mixer", Qt::CaseInsensitive))
    {
        channels = 0;
        ioctl(m_MixerFd, MIXER_WRITE(SOUND_MIXER_VOLUME), &tmpVolume);  //>@执行后，会将tmpVolume清零。
        ioctl(m_PlayerFd, SNDCTL_DSP_CHANNELS, channels);
    }
    else// if(!m_MixerDriver.compare("/dev/mixer2", Qt::CaseInsensitive))
    {
        channels = 2;
        ioctl(m_MixerFd, MIXER_WRITE(SOUND_MIXER_VOLUME), &tmpVolume);  //>@执行后，会将tmpVolume清零。
        ioctl(m_PlayerFd, SNDCTL_DSP_CHANNELS, channels);
    }
    IDE_DEBUG(QString("Track[%1] : set volume %2, realvalue = %3!").arg(m_TrackNum).arg(pVolume).arg(tmpSoloVol));
#endif
}

void ossplayer::SetVolume(int pAoType, int pVolume, bool pSync) //>@pSync表示是否同步修改系统音量，最大值为0x1f1f
{
    if(pVolume < 0)
        m_Volume[0] = 0;
    else if(pVolume > 100)
        m_Volume[0] = 100;
    else
        m_Volume[0] = pVolume;
    if(pSync)
        m_Volume[1] = m_Volume[0];
    //>@同步设置当前所有缓冲区中的音量，否则会出现当下一首是在设置音量前加入的缓冲区，还是按照之前设置的音量播放的bug
    for(int i=0;i<m_MediaBuffer.count();i++)
    {
        PLAY_MEDIA_INFO *tmpInfo = m_MediaBuffer.at(i);
        if(tmpInfo && ((pAoType == AO_NONE) || (pAoType == tmpInfo->mType)))  //>@当pAoType为None或者与音频类型相同时才进行更改
        {
            if(!tmpInfo->mPath.endsWith("M43.wav") && !tmpInfo->mPath.endsWith("M48.wav"))
                tmpInfo->mVolume = m_Volume[0];
        }
    }
    if(m_CurMedia && ((pAoType == AO_NONE) || (pAoType == m_CurMedia->mType)))
    {
        if(!m_CurMedia->mPath.endsWith("M43.wav") && !m_CurMedia->mPath.endsWith("M48.wav"))
            m_CurMedia->mVolume = m_Volume[0];
    }
    if(m_CurMedia && !m_CurMedia->mPath.endsWith("M43.wav") && !m_CurMedia->mPath.endsWith("M48.wav"))
    {
        SetVolume(m_Volume[0]);
    }
}

void ossplayer::Seek(int pMs)
{
    if(m_Pause)
        return;
}

bool ossplayer::Pause(bool pEnable)
{
    SetEnable(!pEnable);
    m_Pause = pEnable;
    return true;
}

void ossplayer::Abort()  //>@只在线程中使用
{
    m_AbortAudio = true;
}

bool ossplayer::Stop(bool pEnable, int pType)
{
    if(pEnable)
    {
        //IDE_TRACE_STR(m_MixerDriver);
        //IDE_TRACE_INT(pType);
        if(!m_StopBuffers.contains(pType))
            m_StopBuffers.append(pType);
        //IDE_TRACE_INT(m_StopBuffers.count());
        SetEnable(false);
        Abort();
    }
    else
    {
        //IDE_TRACE_STR(m_MixerDriver);
        //IDE_TRACE_INT(pType);
        //        if(!QString::compare(m_MixerDriver,"/dev/mixer2",Qt::CaseSensitive)){
        //            m_StopBuffers.clear();
        //        }

        //IDE_TRACE_INT(m_StopBuffers.count());
        SetEnable(true);
    }
    return true;
}

//>@Clear执行后会将新的音频继续播放下去，因此不需要执行slot_finished.
bool ossplayer::Clear()
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

void ossplayer::AudioFinished()
{
    m_AoMode = AO_IDEL;
    m_AbortAudio = false;
    m_Pause = false;
}


