#ifndef BASEXAUDIO_H
#define BASEXAUDIO_H

#include "global.h"
#ifdef LINUX
    #include "lplay.h"
#else
    #include "qaduio.h"
#endif
#include <QThread>

class baseaudio : public QThread
{
    Q_OBJECT
public:
    baseaudio(QObject *parent);
    ~baseaudio();
    void run();
    void Play(QString pPath);
    void SetVoiceEnable(bool pEnale);
    void Pause();
    void Resume();
    void Stop();
public:
    QString             m_Path;
#ifdef LINUX
    QTimer              m_AlsaStateTimer;
#else
    qaduio              *m_QAudio;
    bool                m_Finished;
#endif

signals:
    void finished();
public slots:
    void slot_AlsaStateTimer();
    void slot_finished();
};

#endif // BASEXAUDIO_H
