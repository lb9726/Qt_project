#include "basevideo.h"

basevideo::basevideo(QObject *parent) :
    QObject(parent)
{
#ifdef PHONON
    m_QVideo = 0;
    m_QVideo = new qvideo(this);
    connect(this,SIGNAL(sSetVideoRect(QWidget *)),m_QVideo,SLOT(slot_SetVideoRect(QWidget *)));
    qRegisterMetaType<Phonon::MediaSource>("MediaSource");
    connect(this,SIGNAL(sPlay(QString, QString, bool, bool)),m_QVideo,SLOT(slot_Play(QString, QString, bool, bool)));
    connect(this,SIGNAL(sSetVolume(float)),m_QVideo,SLOT(slot_SetVolume(float)));
    connect(this,SIGNAL(sSeek(qint64)),m_QVideo,SLOT(slot_Seek(qint64)));
    connect(this,SIGNAL(sPause()),m_QVideo,SLOT(slot_Pause()));
    connect(this,SIGNAL(sResume()),m_QVideo,SLOT(slot_Resume()));
    connect(this,SIGNAL(sStop()),m_QVideo,SLOT(slot_Stop()));
    connect(m_QVideo,SIGNAL(sFinished()),this,SLOT(slot_finished()));
#endif
#ifdef MPLAYER
    m_Mplayer = 0;
    m_Mplayer =new mplayer(0);
    connect(this,SIGNAL(sSetVideoRect(QRect, qreal)),m_Mplayer,SLOT(slot_SetVideoRect(QRect, qreal))/*,Qt::BlockingQueuedConnection*/);
    connect(this,SIGNAL(sPlay(QString, QString, bool, bool)),m_Mplayer,SLOT(slot_Play(QString, QString, bool, bool)));
    connect(this,SIGNAL(sVideoToPic(QString,QString,QString)),m_Mplayer,SLOT(slot_VideoToPic(QString,QString,QString)));
    connect(this,SIGNAL(sSetVolume(float)),m_Mplayer,SLOT(slot_SetVolume(float)));
    connect(this,SIGNAL(sSeek(qint64)),m_Mplayer,SLOT(slot_Seek(qint64)));
    connect(this,SIGNAL(sPause()),m_Mplayer,SLOT(slot_Pause()));
    connect(this,SIGNAL(sResume()),m_Mplayer,SLOT(slot_Resume()));
    connect(this,SIGNAL(sStop()),m_Mplayer,SLOT(slot_Stop()));
    connect(m_Mplayer,SIGNAL(sFinished()),this,SLOT(slot_finished()));
#endif
#ifdef GSTREAMER
    m_Gstreamer = 0;
    m_Gstreamer =new gstreamer(0);
    connect(this,SIGNAL(sSetVideoRect(QRect, qreal)),m_Gstreamer,SLOT(slot_SetVideoRect(QRect, qreal))/*,Qt::BlockingQueuedConnection*/);
    connect(this,SIGNAL(sPlay(QString, QString, bool, bool)),m_Gstreamer,SLOT(slot_Play(QString, QString, bool, bool)));
    connect(this,SIGNAL(sVideoToPic(QString,QString,QString)),m_Gstreamer,SLOT(slot_VideoToPic(QString,QString,QString)));
    connect(this,SIGNAL(sSetVolume(float)),m_Gstreamer,SLOT(slot_SetVolume(float)));
    connect(this,SIGNAL(sSeek(qint64)),m_Gstreamer,SLOT(slot_Seek(qint64)));
    connect(this,SIGNAL(sPause()),m_Gstreamer,SLOT(slot_Pause()));
    connect(this,SIGNAL(sResume()),m_Gstreamer,SLOT(slot_Resume()));
    connect(this,SIGNAL(sStop()),m_Gstreamer,SLOT(slot_Stop()));
    connect(m_Gstreamer,SIGNAL(sFinished()),this,SLOT(slot_finished()));
#endif
}

basevideo::~basevideo()
{
#ifdef PHONON
    if(m_QVideo != 0)
    {
        disconnect(this,SIGNAL(sSetVideoRect(QWidget *)),0,0);
        disconnect(this,SIGNAL(sPlay(QString, QString, bool, bool)),0,0);
        disconnect(this,SIGNAL(sVideoToPic(QString)),0,0);
        disconnect(this,SIGNAL(sSetVolume(float)),0,0);
        disconnect(this,SIGNAL(sSeek(qint64)),0,0);
        disconnect(this,SIGNAL(sPause()),0,0);
        disconnect(this,SIGNAL(sResume()),0,0);
        disconnect(this,SIGNAL(sStop()),0,0);
        delete m_QVideo;
        m_QVideo = 0;
    }
#endif
#ifdef MPLAYER
    if(m_Mplayer != 0)
    {
        disconnect(this,SIGNAL(sSetVideoRect(QRect)),0,0);
        disconnect(this,SIGNAL(sPlay(QString, QString, bool, bool)),0,0);
        disconnect(this,SIGNAL(sVideoToPic(QString,QString,QString)),0,0);
        disconnect(this,SIGNAL(sSetVolume(float)),0,0);
        disconnect(this,SIGNAL(sSeek(qint64)),0,0);
        disconnect(this,SIGNAL(sPause()),0,0);
        disconnect(this,SIGNAL(sResume()),0,0);
        disconnect(this,SIGNAL(sStop()),0,0);
        delete m_Mplayer;
        m_Mplayer = 0;
    }
#endif
#ifdef GSTREAMER
    if(m_Gstreamer != 0)
    {
        disconnect(this,SIGNAL(sSetVideoRect(QRect)),0,0);
        disconnect(this,SIGNAL(sPlay(QString, QString, bool, bool)),0,0);
        disconnect(this,SIGNAL(sVideoToPic(QString,QString,QString)),0,0);
        disconnect(this,SIGNAL(sSetVolume(float)),0,0);
        disconnect(this,SIGNAL(sSeek(qint64)),0,0);
        disconnect(this,SIGNAL(sPause()),0,0);
        disconnect(this,SIGNAL(sResume()),0,0);
        disconnect(this,SIGNAL(sStop()),0,0);
        delete m_Gstreamer;
        m_Gstreamer = 0;
    }
#endif
}

void basevideo::SetVideoRect(QRect pRect, qreal pRotate)
{
    emit sSetVideoRect(pRect, pRotate);
}

void basevideo::Play(QString pPath, QString pPara, bool pRepeat, bool pPrior)
{
    emit sPlay(pPath, pPara, pRepeat, pPrior);
}

void basevideo::VideoToPic(QString pSrc, QString pSavepath, QString pSavetype)
{
    emit sVideoToPic(pSrc, pSavepath, pSavetype);
}

void basevideo::SetVolume(float pVolume)
{
    emit sSetVolume(pVolume);
}

void basevideo::Seek(qint64 pMs)
{
    emit sSeek(pMs);
}

void basevideo::Pause()
{
    emit sPause();
}

void basevideo::Resume()
{
    emit sResume();
}

void basevideo::Stop()
{
    emit sStop();
}

void basevideo::slot_finished()
{
    emit sFinished();
}
