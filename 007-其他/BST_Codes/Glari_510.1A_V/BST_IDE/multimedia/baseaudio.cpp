#include "baseaudio.h"

baseaudio::baseaudio(QObject *parent) :
    QObject(parent)
{
    m_Thread = 0;
    m_Thread = new QThread(this);
#ifdef MPLAYER
    m_Mplayer = 0;
    m_Mplayer =new mplayer(0);
    connect(this,SIGNAL(sPlay(QString, QString, bool, bool)),
            m_Mplayer,SLOT(slot_Play(QString, QString, bool, bool)),Qt::DirectConnection);
    connect(this,SIGNAL(sSetVolume(float)),m_Mplayer,SLOT(slot_SetVolume(float)));
    connect(this,SIGNAL(sSeek(qint64)),m_Mplayer,SLOT(slot_Seek(qint64)));
    connect(this,SIGNAL(sPause()),m_Mplayer,SLOT(slot_Pause()));
    connect(this,SIGNAL(sResume()),m_Mplayer,SLOT(slot_Resume()));
    connect(this,SIGNAL(sStop()),m_Mplayer,SLOT(slot_Stop()));
    connect(m_Mplayer,SIGNAL(sFinished()),this,SLOT(slot_finished()));
    //m_Mplayer->moveToThread(m_Thread);
#endif
#ifdef PHONON
    m_QAudio = 0;
    m_QAudio =new qaduio(0);
    connect(this,SIGNAL(sPlay(QString, QString, bool, bool)),
            m_QAudio,SLOT(slot_Play(QString, QString, bool, bool)),Qt::DirectConnection);
    connect(this,SIGNAL(sSetVoiceEnable(bool)),m_QAudio,SLOT(slot_SetVoiceEnable(bool)));
    connect(this,SIGNAL(sPause()),m_QAudio,SLOT(slot_Pause()));
    connect(this,SIGNAL(sResume()),m_QAudio,SLOT(slot_Resume()));
    connect(this,SIGNAL(sStop()),m_QAudio,SLOT(slot_Stop()),Qt::DirectConnection);
    connect(m_QAudio,SIGNAL(sFinished()),this,SLOT(slot_finished()));
    m_QAudio->moveToThread(m_Thread);
#endif
#ifdef GSTREAMER
    m_Gstreamer = 0;
    m_Gstreamer =new gstreamer(0);
    connect(this,SIGNAL(sPlay(QString, QString, bool, bool)),m_Gstreamer,SLOT(slot_Play(QString, QString, bool, bool)));
    connect(this,SIGNAL(sSetVolume(float)),m_Gstreamer,SLOT(slot_SetVolume(float)));
    connect(this,SIGNAL(sSeek(qint64)),m_Gstreamer,SLOT(slot_Seek(qint64)));
    connect(this,SIGNAL(sPause()),m_Gstreamer,SLOT(slot_Pause()));
    connect(this,SIGNAL(sResume()),m_Gstreamer,SLOT(slot_Resume()));
    connect(this,SIGNAL(sStop()),m_Gstreamer,SLOT(slot_Stop()));
    connect(m_Gstreamer,SIGNAL(sFinished()),this,SLOT(slot_finished()));
    //m_Gstreamer->moveToThread(m_Thread);
#endif
#ifdef USERPLAY
    m_Alsa = 0;
    m_Alsa =new alsa(0);
    connect(this,SIGNAL(sInit()),m_Alsa,SLOT(slot_Init()));
    connect(this,SIGNAL(sPlay(QString)),m_Alsa,SLOT(slot_Play(QString)),Qt::DirectConnection);
    connect(this,SIGNAL(sSetVoiceEnable(bool)),m_Alsa,SLOT(slot_SetVoiceEnable(bool)),Qt::DirectConnection);
    connect(this,SIGNAL(sPause()),m_Alsa,SLOT(slot_Pause()),Qt::DirectConnection);
    connect(this,SIGNAL(sResume()),m_Alsa,SLOT(slot_Resume()),Qt::DirectConnection);
    connect(this,SIGNAL(sStop()),m_Alsa,SLOT(slot_Stop()),Qt::DirectConnection);
    connect(m_Alsa,SIGNAL(sFinished()),this,SLOT(slot_Finished()));
    m_Alsa->moveToThread(m_Thread);
    connect(&m_AlsaTimer,SIGNAL(timeout()),m_Alsa,SLOT(slot_Finished()),Qt::DirectConnection);
#endif
    m_Thread->start(QThread::LowPriority);
    m_Thread->wait(10);
#ifdef USERPLAY
   emit sInit();
#endif
}

baseaudio::~baseaudio()
{
#ifdef MPLAYER
    if(m_Mplayer != 0)
    {
        disconnect(this,SIGNAL(sPlay(QString, QString, bool, bool)),0,0);
        disconnect(this,SIGNAL(sSetVolume(float)),0,0);
        disconnect(this,SIGNAL(sSeek(qint64)),0,0);
        disconnect(this,SIGNAL(sPause()),0,0);
        disconnect(this,SIGNAL(sResume()),0,0);
        disconnect(this,SIGNAL(sStop()),0,0);
        delete m_Mplayer;
        m_Mplayer = 0;
    }
#endif
#ifdef PHONON
    if(m_QAudio != 0)
    {
        disconnect(this,SIGNAL(sPlay(QString, QString, bool, bool)),0,0);
        disconnect(this,SIGNAL(sSetVoiceEnable(bool)),0,0);
        disconnect(this,SIGNAL(sPause()),0,0);
        disconnect(this,SIGNAL(sResume()),0,0);
        disconnect(this,SIGNAL(sStop()),0,0);
        delete m_QAudio;
        m_QAudio = 0;
    }
#endif
#ifdef GSTREAMER
    if(m_Gstreamer != 0)
    {
        disconnect(this,SIGNAL(sPlay(QString, QString, bool, bool)),0,0);
        disconnect(this,SIGNAL(sSetVolume(float)),0,0);
        disconnect(this,SIGNAL(sSeek(qint64)),0,0);
        disconnect(this,SIGNAL(sPause()),0,0);
        disconnect(this,SIGNAL(sResume()),0,0);
        disconnect(this,SIGNAL(sStop()),0,0);
        delete m_Gstreamer;
        m_Gstreamer = 0;
    }
#endif
#ifdef USERPLAY
    if(m_Alsa != 0)
    {
        disconnect(this,SIGNAL(sPlay(QString)),0,0);
        disconnect(this,SIGNAL(sSetVoiceEnable(bool)),0,0);
        disconnect(this,SIGNAL(sPause()),0,0);
        disconnect(this,SIGNAL(sResume()),0,0);
        disconnect(this,SIGNAL(sStop()),0,0);
        delete m_Alsa;
        m_Alsa = 0;
        m_AlsaTimer.stop();
        disconnect(&m_AlsaTimer,SIGNAL(timeout()),0,0);
    }
#endif
}

void baseaudio::Play(QString pSrc, QString pPara, bool pRepeat, bool pPrior)
{
#ifdef USERPLAY
    if(!m_AlsaTimer.isActive())
        m_AlsaTimer.start(100);
#endif
    emit sPlay(pSrc, pPara, pRepeat, pPrior);
}

void baseaudio::SetVoiceEnable(bool pEnale)
{
    emit sSetVoiceEnable(pEnale);
}

void baseaudio::SetVolume(int pValue)
{

}

void baseaudio::SetRepeat(bool pRepeat)
{

}

void baseaudio::Pause()
{
    emit sPause();
}

void baseaudio::Resume()
{
    emit sResume();
}

void baseaudio::Stop()
{
    emit sStop();
}

void baseaudio::slot_Finished()
{
#ifdef USERPLAY
    if(m_AlsaTimer.isActive())
        m_AlsaTimer.stop();
#endif
    emit sFinished();
}
