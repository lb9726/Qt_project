#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QThread>

#include <math.h>

#ifdef LINUX
#include <sys/soundcard.h>
#include <sys/poll.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#include "playerbase.h"

class MediaPlayer : public QThread
{
    Q_OBJECT
public:
    MediaPlayer(QObject *parent = 0);
    ~MediaPlayer();

    bool openPlayer();
    bool closePlayer();

    void run();

    bool add(int pAoType, QString pSrc, int pLoop, bool pPrior, int pVolume, bool pEmitSig=false);
    void playback(LOOP_TYPE pLoop = LOOP_NONE);  ///根据pLoop重新播放当前音频
    void stopOne(QString pFile);
    void playNext();
    void playHead();
    void playEnd();
    bool testMedia(int pAoType, QString pSrc, int pVolume);  ///测试音频音量

    ///
    bool isRunning();
    void forceQuit(){m_Quit = true;}
    void wait(int pTimeout);

    ///
    void setLoop(LOOP_TYPE pLoopType) {m_BufferLoop = pLoopType;}
    bool addMedia(PLAY_MEDIA_INFO *pMeida, bool pPrior);

    PLAY_MEDIA_INFO *getMedia();
    int  getMediaBufferSize();
    bool findInMediaBuffer(QString pAudioFile, QList<PLAY_MEDIA_INFO*> &pResultList, bool pFindall=false);
    bool canPlay(QString pFile);
    bool updateMedia();   ///每次音频播放完后需要执行此函数，用于从缓冲区中取出最新资源

    ///音频处理
    void audioFinished();   ///用户恢复音频现场

    void setMixerDriver(QString pDriver); ///"/dev/dsp"  "/dev/mixer"
    void setAoDriver(QString pDriver);

    void setVolume(int pVolume);
    void setVolume(int pAoType, int pVolume, bool pSync=false);

    void abort();  ///指中止当前音频，继续播放下一个
    void seek(int pMs);
    int  getPlayPercent();
    bool pause(bool pEnable);
    bool isPause() {return m_Pause;}
    bool clear();
    bool stop(bool pEnable, int pType = 0);  ///指停止此声道，不继续播放了




    void setEnable(bool pEnable) {m_Enable = pEnable;}
    void setTrackNum(quint32 pNum) {m_TrackNum = pNum;}
    quint32 getTrackNum() {return m_TrackNum;}

signals:
    void sNull();
    void sStarted();
    void sFinished();   ///单曲音频播放完毕后发送

public:
    bool                       m_Enable;
    bool                       m_Quit;
    bool                       m_AbortAudio;  ///用于中止当前播放的音乐，继而播放下一首

    ///用于保存所有使音频中断的中断源，必须所有中断源都删除后才可以真正恢复音频。
    QList<int>                 m_StopBuffers;
    int                        m_BufferIndex;///播放缓冲参数
    LOOP_TYPE                  m_BufferLoop;

    PLAY_MEDIA_INFO*           m_CurMedia;
    QList<PLAY_MEDIA_INFO*>    m_MediaBuffer;

    AO_MODE                    m_AoMode; ///
    quint32                    m_TrackNum;  ///
    int                        m_DefDelay;
    int                        m_PlayPos;  ///ms
    bool                       m_Pause;
    int                        m_Volume[2]; ///0:表示当前音量，1：用于保存当前系统设置的音量
    quint64                    m_ElapseTime;

    QString                    m_MixerDriver;
    QString                    m_AoDriver;
    int                        m_PlayerFd;///播放器参数
    int                        m_MixerFd;
    QFile                      m_AudioFile;
};


#endif // MEDIAPLAYER_H
