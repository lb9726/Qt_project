#ifndef OSSPLAYER_H
#define OSSPLAYER_H

#include "playerbase.h"

#ifdef LINUX
#include <sys/soundcard.h>
#include <sys/poll.h>
#endif
#if (defined(UBUNTU) || defined(LINUX))
#include <signal.h>
#include <sys/types.h>
#endif
#include <math.h>

class MplayerThread;

#define D_MONITORDELAY  500   //>@延时检测100ms
#define D_PROCESSWAIT   200   //>@等待

#define SPU_ANALOG_VOLUME		32

/*
播放准则：
    1、音乐播放模式：单曲循环、单曲循环n次、顺序播放、优先播放
    2、不论优先播放还是非优先，都必须将当前音乐播放完后在播放下一首
    3、如果有新的音频到来，并且是优先播放的，则将当前音乐的播放模式强制设置为顺序播放
*/
class ossplayer : public QThread
{
    Q_OBJECT
public:
    ossplayer(QObject *parent = 0);
    ~ossplayer();

    void ForceQuit() {m_Quit = true;}
    void Wait(int pTimeout);
    void run();

    void SetPlayer(QString pExeFile) {}

    void SetEnable(bool pEnable) {m_Enable = pEnable;}

    void SetTrackNum(quint32 pNum) {m_TrackNum = pNum;}
    quint32 GetTrackNum() {return m_TrackNum;}

    //>@
    bool IsRunning();

    bool OpenPlayer();
    bool ClosePlayer();

    //>@
    void SetLoop(LOOP_TYPE pLoopType) {m_BufferLoop = pLoopType;}
    bool AddMedia(PLAY_MEDIA_INFO *pMeida, bool pPrior);
    PLAY_MEDIA_INFO *GetMedia();
    int  GetMediaBufferSize();
    bool FindInMediaBuffer(QString pAudioFile, QList<PLAY_MEDIA_INFO*> &pResultList, bool pFindall=false);
    bool CanPlay(QString pFile);
    bool UpdateMedia();   //>@每次音频播放完后需要执行此函数，用于从缓冲区中取出最新资源

    //>@音频处理
    void AudioFinished();   //>@用户恢复音频现场

    void SetMixerDriver(QString pDriver); //>@"/dev/dsp"  "/dev/mixer"
    void SetAoDriver(QString pDriver);

    void SetConfFile(QString pConf) {}
    void SetVoDriver(QString pDriver) {}
    void SetRect(QRect pRect, int pRotate = 0) {}

    void SetVolume(int pVolume);
    void SetVolume(int pAoType, int pVolume, bool pSync=false);
    void Abort();  //>@指中止当前音频，继续播放下一个
    void Seek(int pMs);
    int  GetPlayPercent();
    bool Pause(bool pEnable);
    bool isPause() {return m_Pause;}
    bool Clear();
    bool Stop(bool pEnable, int pType = 0);  //>@指停止此声道，不继续播放了

    bool TestMedia(int pAoType, QString pSrc, int pVolume);  //>@测试音频音量

    bool Add(int pAoType, QString pSrc, int pLoop, bool pPrior, int pVolume, bool pEmitSig=false);
    void Playback(LOOP_TYPE pLoop=LOOP_NONE);  //>@根据pLoop重新播放当前音频
    void StopOne(QString pFile);
    void PlayNext();
    void PlayHead();
    void PlayEnd();

signals:
    void sNull();   //>@只有在当前缓冲区全部播放完后，才发出此信号。
    void sStarted(); //>@单曲音频播放开始播放时发送
    void sFinished();   //>@单曲音频播放完毕后发送

public:
    bool                       m_Enable;
    quint32                    m_TrackNum;  //>@

    int                        m_DefDelay;
    bool                       m_Quit;
    bool                       m_AbortAudio;  //>@用于中止当前播放的音乐，继而播放下一首

    AO_MODE                    m_AoMode; //>@

    //>@用于保存所有使音频中断的中断源，必须所有中断源都删除后才可以真正恢复音频。
    QList<int>                 m_StopBuffers;

    //>@播放缓冲参数
    int                        m_BufferIndex;
    LOOP_TYPE                  m_BufferLoop;

    QList<PLAY_MEDIA_INFO*>    m_MediaBuffer;
    PLAY_MEDIA_INFO*           m_CurMedia;

    //>@播放器参数
    int                        m_PlayerFd;
    int                        m_MixerFd;
    QFile                      m_AudioFile;

    int                        m_PlayPos;  //>@ms
    bool                       m_Pause;

    QString                    m_MixerDriver; //>@

    QString                    m_AoDriver; //>
    int                        m_Volume[2]; //>@0:表示当前音量，1：用于保存当前系统设置的音量

    quint64                    m_ElapseTime;
};

#endif // OSSPLAYER_H

