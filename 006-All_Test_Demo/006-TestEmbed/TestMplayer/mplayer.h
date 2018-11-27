#ifndef MPLAYER_H
#define MPLAYER_H

#include "define.h"

#include <QThread>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QTimer>

typedef enum{
    LOOP_EVER = -1,
    LOOP_NONE = 0,
    LOOP_LIST = 1
} LOOP_TYPE;

typedef enum{
    AO_LIFTFLR=0,
    AO_LIFTARV=1,
    AO_BEEP=2,
    AO_ADMUSIC=3,
    AO_NONE
} AO_TYPE;

typedef struct _PLAY_MEDIA_INFO
{
    _PLAY_MEDIA_INFO()
    {
        mType = AO_NONE;
        mLoop = 0;
        mVolume = -1;
        mEmitSig = false;
    }
    AO_TYPE         mType;
    QString         mPath;
    QString         mPara;
    int             mLoop;      //>-1 表示无限循环
    int             mVolume;    //>为-1时表示使用使用当前类默认音量
    bool            mEmitSig;   //>表示是否发送开始播放、结束播放、停止播放信号
} PLAY_MEDIA_INFO;


class Mplayer : public QThread
{
    Q_OBJECT
public:
    Mplayer();
    ~Mplayer();
    void startWork();
    void run();
    bool addMediaToBuffer(PLAY_MEDIA_INFO *pMeida, bool pPrior);
    void delayTimeMs(quint32 pTimeout);
    void pauseBackgroundPlayer();
    void playBackgroundPlayer();
    void setBackgroundPlayerVolume(int volume);
    void setBackgroundMuisc(QString  musicPath,int volume);
    bool addMedia(int pAoType, QString pSrc, int pLoop, bool pPrior, int pVolume);

    bool updateMediaBuffer();
    bool clearMediaBuffer();
public slots:
    void backgroundVolumeKeeper();


private:
    int                     m_DefDelay;
    bool                    m_AbortAudio;
    int                     m_BufferIndex;
    LOOP_TYPE               m_BufferLoop;
    QList<PLAY_MEDIA_INFO*> m_MediaBuffer;
    PLAY_MEDIA_INFO*        m_CurMedia;
    QString                 m_CurCmdString;
    //int                     m_Volume[2];
    //volatile bool m_mainPlayerAlive;
    volatile bool m_backgroundPlayerAlive;
    volatile bool m_backgroundPlayerPaused;

    QTimer* m_TestTimer;
    int m_backgroundVolume;
//    int m_AdmusicVol;
//    int m_BeepVol;
//    int m_LiftArvVol;
//    int m_LiftFlrVol;
};

#endif // MPLAYER_H
