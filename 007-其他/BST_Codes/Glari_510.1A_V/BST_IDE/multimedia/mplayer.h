#ifndef MPLAYER_H
#define MPLAYER_H

#include "global.h"
#include <QProcess>
#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QObject>
#include <QMutex>
#include <QThread>
#include <QQueue>

typedef struct _PLAY_MEDIA_INFO
{
    _PLAY_MEDIA_INFO()
    {
        mLoop = 0;
        mVolume = -1;
    }
    QString         mPath;
    int             mLoop;  //>@-1 表示无限循环
    int             mVolume;  //>@为-1时表示使用使用当前类默认音量
}PLAY_MEDIA_INFO;

typedef enum{
    LOOP_EVER = -1,
    LOOP_NONE,
    LOOP_LIST
} LOOP_TYPE;

class MplayerThread;

#define D_MAXBUFFERSIZE   3

/*
播放准则：
    1、音乐播放模式：单曲循环、单曲循环n次、顺序播放、优先播放
    2、不论优先播放还是非优先，都必须将当前音乐播放完后在播放下一首
    3、如果有新的音频到来，并且是优先播放的，则将当前音乐的播放模式强制设置为顺序播放
*/
class mplayer : public QObject
{
    Q_OBJECT
    friend class MplayerThread;
public:
    mplayer(QObject *parent = 0);
    ~mplayer();

    void SetFIFO(QString pFifo);
    void CreatProcess();
    bool ReleaseProcess();
    void ReleaseMediaBuffer();

    bool AddMedia(PLAY_MEDIA_INFO *pMeida, bool pPrior);
    void PlayMedia();
    void Add(QString pSrc, int pLoop=1, bool pPrior=false, int pVolume=-1);
    void Play(QString pSrc, int pLoop=1, bool pPrior=false, int pVolume=-1);
    void Play();

    bool CanPlay(QString pFile);
    bool CanPlay(int pAudioNum);
    bool IsRunning();
    quint32 GetMediaNum(QString pAudio);
    quint32 GetCurMediaNum();

    void SetTrackNum(quint32 pNum) {m_TrackNum = pNum;}
    quint32 GetTrackNum() {return m_TrackNum;}

    void SetConfFile(QString pFile);

    void SetVoDriver(QString pDriver);
    void SetRect(QRect pRect, int pRotate = 0);

    void SetAoDriver(QString pDriver);
    void SetVolume(int pVolume);
    void SetMute(bool pFlag);

    void Seek(int pMs);
    void Pause();
    void Resume();
    bool Stop();

    void ClearEnvir();

signals:
    void sFinished();

public slots:
    void slot_StandardOutput();
    void slot_StandardError();
    void slot_Finished(int pExitCode, QProcess::ExitStatus pStatus);
    void slot_Error(QProcess::ProcessError);

public:
    QMutex                     m_PlayMutex;
    QProcess                  *m_PlayProcess;
    MplayerThread             *m_MplayerThread;

    quint32                    m_TrackNum;

    QString                    m_CurCmdString;

    QString                    m_ConfFile;

    bool                       m_Pause;
    int                        m_PlayPos;  //>@ms
    QString                    m_AvFIFO;

    QString                    m_VoDriver; //>@视频驱动
    QRect                      m_VoRect;
    int                        m_VoRotate;

    QString                    m_AoDriver; //>@音频驱动
    int                        m_Volume;
    bool                       m_Mute;

    QList<PLAY_MEDIA_INFO*>    m_MediaBuffer;
    PLAY_MEDIA_INFO*           m_CurMedia;
};

class MplayerThread : QThread
{
    Q_OBJECT
    friend class mplayer;
public:
    MplayerThread(mplayer *qesp);
    ~MplayerThread();

    void run();

public:
    mplayer         *m_Mplayer;
};

#endif // MPLAYER_H
