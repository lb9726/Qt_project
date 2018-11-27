#ifndef BASEAUDIO_H
#define BASEAUDIO_H

#include "global.h"

#ifdef MPLAYER
    #include "mplayer.h"
#endif
#ifdef PHONON
    #include "qaduio.h"
#endif
#ifdef GSTREAMER
    #include "gstreamer.h"
#endif
#ifdef USERPLAY
    #include "alsa.h"
#endif

class baseaudio : public QObject
{
    Q_OBJECT
public:
    baseaudio(QObject *parent = 0);
    ~baseaudio();
    void Play(QString pSrc, QString pPara=QString(), bool pRepeat=false, bool pPrior=false);
    void SetVoiceEnable(bool pEnale);
    void SetVolume(int pValue);
    void SetRepeat(bool pRepeat);
    void Pause();
    void Resume();
    void Stop();

signals:
#ifdef USERPLAY
    void sInit();
#endif
    void sFinished();
    void sPlay(QString pSrc, QString pPara, bool pRepeat, bool pPrior);
    void sSetVoiceEnable(bool pEnale);
    void sPause();
    void sResume();
    void sStop();

public slots:
    void slot_Finished();

public:
    QThread     *m_Thread;
#ifdef MPLAYER
    mplayer     *m_Mplayer;
#endif
#ifdef PHONON
    qaduio      *m_QAudio;
#endif
#ifdef GSTREAMER
    gstreamer   *m_Gstreamer;
#endif
#ifdef USERPLAY
    alsa        *m_Alsa;
    QTimer      m_AlsaTimer;
#endif
};

inline void SystemAudio(QString pAudioPath, bool pPrior, bool pRepeat, quint32 pInterval)
{
    if(GlobalPara.m_SystemAudio == 0)
    {
        GlobalPara.m_SystemAudio = new baseaudio;
    }
    GlobalPara.m_SystemAudio->Play(pAudioPath, QString(), pRepeat, pPrior);
}

#endif // BASEAUDIO_H
