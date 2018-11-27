#ifndef BASEVIDEO_H
#define BASEVIDEO_H

#include "global.h"

#ifdef MPLAYER
    #include "mplayer.h"
#endif
#ifdef PHONON
    #include "qvideo.h"
#endif
#ifdef GSTREAMER
    #include "gstreamer.h"
#endif
#ifdef QtAV
    #include "QtAV/AVPlayer.h"
    #include "QtAV/GraphicsItemRenderer.h"
#endif

class basevideo : public QObject
{
    Q_OBJECT
public:
    basevideo(QObject *parent = 0);
    ~basevideo();
    void SetVideoRect(QRect pRect, qreal pRotate = 0);
    void Play(QString pPath, QString pPara=QString(), bool pRepeat=false, bool pPrior=false);
    void VideoToPic(QString pSrc, QString pSavepath, QString pSavetype);
    void SetVolume(float pVolume);
    void Seek(qint64 pMs);
    void Pause();
    void Resume();
    void Stop();

signals:
    void sFinished();
    void sSetVideoRect(QRect pRect, qreal pRotate = 0);
    void sPlay(QString pPath, QString pPara, bool pRepeat, bool pPrior);
    void sVideoToPic(QString, QString, QString);
    void sSetVolume(float pVolume);
    void sSeek(qint64 pMs);
    void sPause();
    void sResume();
    void sStop();

public slots:
    void slot_finished();

public:
#ifdef MPLAYER
    mplayer     *m_Mplayer;
#endif
#ifdef PHONON
    qvideo      *m_QVideo;
#endif
#ifdef GSTREAMER
    gstreamer    *m_Gstreamer;
#endif
};

#endif // BASEVIDEO_H
