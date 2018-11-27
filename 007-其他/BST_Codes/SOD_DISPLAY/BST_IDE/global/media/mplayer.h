#ifndef MPLAYER_H
#define MPLAYER_H

#include "playerbase.h"
#ifdef SETTING
#define D_MONITORDELAY  1000  //>@延时检测500ms
#else
#define D_MONITORDELAY  500  //>@延时检测500ms
#endif
#define D_PROCESSWAIT   200  //>@等待

/*
播放准则：
    1、音乐播放模式：单曲循环、单曲循环n次、顺序播放、优先播放
    2、不论优先播放还是非优先，都必须将当前音乐播放完后在播放下一首
    3、如果有新的音频到来，并且是优先播放的，则将当前音乐的播放模式强制设置为顺序播放
*/
class mplayer : public QThread
{
    Q_OBJECT
public:
    mplayer(QObject *parent = 0);
    ~mplayer();

    void ForceQuit() {m_Quit = true;}
    void Wait(int pTimeout);
    void run();

    void SetPlayer(QString pExeFile);

    void SetEnable(bool pEnable) {m_Enable = pEnable;}

    void SetTrackNum(quint32 pNum) {m_TrackNum = pNum;}
    quint32 GetTrackNum() {return m_TrackNum;}

    //>@进程函数
    bool FindPidInProc(int pPID);
    bool IsRunning();

    void CreatProcess();
    bool ReleaseProcess();
    bool WriteProcess(const char *pCmd, int pTimeout=D_PROCESSWAIT, bool pForce=false);
    bool WriteProcess(QString pCmd, int pTimeout=D_PROCESSWAIT, bool pForce=false);
    QString UpdateCmdString(PLAY_MEDIA_INFO *pMeidaInfo);  //>@用于生成播放指令

    //>@缓冲区处理
    void SetLoop(LOOP_TYPE pLoopType) {m_BufferLoop = pLoopType;}
    bool AddMedia(PLAY_MEDIA_INFO *pMeida, bool pPrior);
    PLAY_MEDIA_INFO *GetMedia();
    int  GetMediaBufferSize();
    bool FindInMediaBuffer(QString pAudioFile, QList<PLAY_MEDIA_INFO*> &pResultList, bool pFindall=false);
    bool CanPlay(QString pFile);
    bool UpdateMedia();   //>@每次音频播放完后需要执行此函数，用于从缓冲区中取出最新资源

    //>@音频处理
    void SetMixerDriver(QString pDriver);

    void AudioFinished();  //>@用户恢复音频现场
    void SetVoDriver(QString pDriver);
    void SetAoDriver(QString pDriver);

    void SetConfFile(QString pFile);
    void SetRect(QRect pRect, int pRotate = 0);
    void SetVolume(int pAoType, int pVolume, bool pSync=false);
    void SetBalance(int pBalance);
    void Abort();  //>@指中止当前音频，继续播放下一个
    void Restore() {}
    void Seek(int pMs);
    int  GetPlayPercent();
    bool Pause(bool pEnable);
    bool isPause() {return m_Pause;}
    bool Clear();
    bool Stop(bool pEnable, int pType = 0);  //>@指停止此声道
    void KillPlayer();

    bool TestMedia(int pAoType, QString pSrc, int pVolume);  //>@测试音频音量

    bool Add(int pAoType, QString pSrc, int pLoop, bool pPrior, int pVolume, bool pEmitSig=false);
    void Playback(LOOP_TYPE pLoop=LOOP_NONE);  //>@根据pLoop重新播放当前音频
    void PlayNext();
    void PlayHead();
    void PlayEnd();
    void StopOne(QString pFile);

signals:
    void sNull();   //>@只有在当前缓冲区全部播放完后，才发出此信号。
    void sKilled(); //>@通过kill方式杀死mplayer时发送
    void sStarted(); //>@单曲音频播放开始播放时发送
    void sFinished();   //>@单曲音频播放完毕后发送
    void sAborted();   //>@音频被停止时发送
    void sStopped();  //>@声道被停止时发送

public slots:
    void slot_StandardOutput();
    void slot_StandardError();

public:
    bool                       m_Enable;
    quint32                    m_TrackNum;  //>@声道标记
    quint32                    m_KillCount;

    QString                    m_ExePath;

    QList<int>                 m_StopBuffers;  //>@用于保存所有使音频中断的中断源，必须所有中断源都删除后才可以真正恢复音频

    //>@线程参数
    int                        m_DefDelay;
    bool                       m_Quit;
    bool                       m_AbortAudio;  //>@是否停止当前音频，注意，只针对当前播放中的音频

    //>@播放缓冲参数
    int                        m_BufferIndex;
    LOOP_TYPE                  m_BufferLoop;

    QList<PLAY_MEDIA_INFO*>    m_MediaBuffer;
    PLAY_MEDIA_INFO*           m_CurMedia;

    //>@播放器参数
    QProcess                  *m_PlayProcess;
    QString                    m_CurCmdString;

    QString                    m_ConfFile; //>@配置文件

    int                        m_PlayPos;  //>@ms

    bool                       m_Pause;

    QString                    m_MixerDriver;

    QString                    m_VoDriver; //>@视频驱动
    QRect                      m_VoRect;
    int                        m_VoRotate;

    QString                    m_AoDriver; //>@音频驱动
    int                        m_Volume[2];   //>@0:表示当前音量，1：用于保存当前系统设置的音量

    quint64                    m_ElapseTime;
};

#endif // MPLAYER_H

