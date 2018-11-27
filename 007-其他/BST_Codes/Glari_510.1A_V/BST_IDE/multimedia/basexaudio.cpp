#include "basexaudio.h"

baseaudio::baseaudio(QObject *parent) :
    QThread(parent)
{
    m_Path.clear();
#ifdef LINUX
    connect(&m_AlsaStateTimer, SIGNAL(timeout()), this, SLOT(slot_AlsaStateTimer()));
#else
    m_Finished = false;
    m_QAudio = 0;
#endif
}

baseaudio::~baseaudio()
{
    m_Path.clear();
#ifdef LINUX
    if(m_AlsaStateTimer.isActive())
    {
        m_AlsaStateTimer.stop();
        disconnect(&m_AlsaStateTimer, SIGNAL(timeout()), this, SLOT(slot_AlsaStateTimer()));
    }
#else
    if(m_QAudio!=0)
    {
        disconnect(m_QAudio,SIGNAL(finished()),this,SLOT(slot_finished()));
        delete m_QAudio;
        m_QAudio = 0;
    }
#endif
}

void baseaudio::run()
{
#ifdef LINUX
    m_MusicPath = (const char*)m_Path.toLocal8Bit();
    Alsa_play(m_MusicPath,m_VoiceEnable);
#else
    if(m_QAudio!=0)
    {
        disconnect(m_QAudio,SIGNAL(finished()),this,SLOT(slot_finished()));
        delete m_QAudio;
        m_QAudio = 0;
    }
    m_QAudio = new qaduio(this);
    connect(m_QAudio,SIGNAL(finished()),this,SLOT(slot_finished()));
    m_QAudio->Play(m_MusicPath);
    do{
        QtSleep(10);
    }while(!m_Finished);
#endif
}

void baseaudio::Play(QString pPath)
{
    m_Finished = false;
#ifdef LINUX
    Stop();
    QtSleep(1000);
#endif
    if(isRunning())
        quit();
#ifdef LINUX
    if(!m_AlsaStateTimer.isActive())
        m_AlsaStateTimer.start(50);
#else
#endif
    m_Path = pPath;
    this->start(QThread::HighPriority);
}

void baseaudio::SetVoiceEnable(bool pEnale)
{
#ifdef LINUX
    m_VoiceEnable = pEnale;
#else
    switch(pEnale)
    {
        case true:Resume();break;
        case false:Pause();break;
    }
#endif
}

void baseaudio::Pause()
{
#ifdef LINUX

#else
    if(m_QAudio != 0)
    {
        if(m_QAudio->m_AudioOutput != 0)
        {
            if(m_QAudio->m_AudioOutput->state() == QAudio::ActiveState)
            {
                m_QAudio->m_AudioOutput->suspend();
            }
        }
    }
#endif
}

void baseaudio::Resume()
{
#ifdef LINUX

#else
    if(m_QAudio != 0)
    {
        if(m_QAudio->m_AudioOutput != 0)
        {
            if(m_QAudio->m_AudioOutput->state() == QAudio::SuspendedState)
            {
                m_QAudio->m_AudioOutput->resume();
            }
        }
    }
#endif
}

void baseaudio::Stop()
{
#ifdef LINUX
    m_Stop = true;
#else
    if(m_QAudio != 0)
    {
        if(m_QAudio->m_AudioOutput != 0)
        {
            if(m_QAudio->m_AudioOutput->state() != QAudio::IdleState)
            {
                m_QAudio->m_AudioOutput->stop();
            }
        }
    }
#endif
//    QtSleep(1000);
//    if(isRunning())
//    {
//        IDE_TRACE();
//        quit();
//    }
}

void baseaudio::slot_AlsaStateTimer()
{
#ifdef LINUX
    if(m_Finished)
    {
        if(m_AlsaStateTimer.isActive())
            m_AlsaStateTimer.stop();
        emit finished();
    }
#endif
}

void baseaudio::slot_finished()
{
#ifndef LINUX
    m_Finished = true;
    emit finished();
#endif
}

