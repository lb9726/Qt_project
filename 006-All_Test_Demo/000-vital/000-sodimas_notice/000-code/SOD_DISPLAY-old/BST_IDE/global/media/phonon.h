#ifndef PHONON_H
#define PHONON_H

#include "playerbase.h"
#include <QtMultimedia>

class phonon : public QThread
{
    Q_OBJECT
public:
    phonon(QObject *parent = 0);
    ~phonon();

    void ForceQuit() {m_Quit = true;}
    void Wait(int pTimeout);
    void run();

    void SetPlayer(QString pExe) {}

    void SetEnable(bool pEnable) {m_Enable = pEnable;}
    void SetMode(MD_MEDIA pMode) {m_MediaMode = pMode;}

    void SetTrackNum(quint32 pNum) {m_TrackNum = pNum;}
    quint32 GetTrackNum() {return m_TrackNum;}

    bool IsRunning();

    //>@缓冲区处理
    void SetLoop(LOOP_TYPE pLoopType) {m_BufferLoop = pLoopType;}
    bool AddMedia(PLAY_MEDIA_INFO *pMeida, bool pPrior);
    PLAY_MEDIA_INFO *GetMedia();
    int  GetMediaBufferSize();
    bool FindInMediaBuffer(QString pAudioFile, QList<PLAY_MEDIA_INFO*> &pResultList, bool pFindall=false);
    bool CanPlay(QString pFile);
    bool UpdateMedia();   //>@每次音频播放完后需要执行此函数，用于从缓冲区中取出最新资源

    void SetConfFile(QString pFile);
    void SetMixerDriver(QString pDriver);
    void SetVoDriver(QString pDriver);
    void SetAoDriver(QString pDriver);

    void AudioFinished();  //>@用户恢复音频现场

    void SetRect(QRect pRect, int pRotate = 0);
    void SetVolume(int pAoType, int pVolume, bool pSync=false);
    void SetBalance(int pBalance);

    void Abort();  //>@指中止当前音频，继续播放下一个
    void Restore() {}
    bool Clear();
    bool Stop(bool pEnable, int pType = 0);  //>@指停止此声道

    void Seek(int pMs);
    int  GetPlayPercent();

    bool Pause(bool pEnable);
    bool isPause() {return m_Pause;}

    bool Add(int pAoType, QString pSrc, int pLoop, bool pPrior, int pVolume, bool pEmitSig=false);
    void Playback(LOOP_TYPE pLoop=LOOP_NONE);  //>@根据pLoop重新播放当前音频
    void PlayNext();
    void PlayHead();
    void PlayEnd();
    void StopOne(QString pFile);
    bool TestMedia(int pAoType, QString pSrc, int pVolume);  //>@测试音频音量

signals:
    void sNull();   //>@只有在当前缓冲区全部播放完后，才发出此信号。
    void sKilled(); //>@通过kill方式杀死mplayer时发送
    void sStarted(); //>@单曲音频播放开始播放时发送
    void sFinished();   //>@单曲音频播放完毕后发送
    void sAborted();   //>@音频被停止时发送
    void sStopped();  //>@声道被停止时发送

public slots:
    void slot_StateChanged(QAudio::State pState);

public:
    bool                       m_Enable;
    quint32                    m_TrackNum;  //>@声道标记
    quint32                    m_KillCount;

    MD_MEDIA                   m_MediaMode;

    //>@线程参数
    int                        m_DefDelay;
    bool                       m_Quit;
    bool                       m_AbortAudio;    //>@用于中止当前播放的音乐，继而播放下一首

    //>@播放缓冲参数
    int                        m_BufferIndex;
    LOOP_TYPE                  m_BufferLoop;

    QList<PLAY_MEDIA_INFO*>    m_MediaBuffer;
    PLAY_MEDIA_INFO*           m_CurMedia;

    //>@用于保存所有使音频中断的中断源，必须所有中断源都删除后才可以真正恢复音频。
    QList<int>                 m_StopBuffers;

    //>@播放器参数
    bool                       m_Pause;
    int                        m_VoRotate;
    int                        m_Volume[2];   //>@0:表示当前音量，1：用于保存当前系统设置的音量

    QAudioOutput              *m_Player;
    QAudioFormat               m_PlayFormat;

    QAudioInput               *m_Record;
    QAudioFormat               m_RecoFormat;
    WAVContainer_t             m_WavHeader;
};

#endif // PHONON_H

