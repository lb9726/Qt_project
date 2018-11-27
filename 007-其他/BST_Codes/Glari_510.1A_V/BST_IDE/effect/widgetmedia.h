#ifndef WINDGETMEDIA_H
#define WINDGETMEDIA_H

#include "widgetbase.h"
#ifdef IMX6X
#include <gst/gst.h>
#endif

class ThreadMedia : public QThread
{
public:
    ThreadMedia(QObject *parent = 0);
    ~ThreadMedia(){}

    void Stop(bool pFlag);
    void SetPlayMode(quint8 pMode);

    void SetVolume(int pVolume);

    void Abort();
    void Change(QString pMedia, QRect pRect, int pRotate);

protected:
    void run();

public:
#ifdef IMX6X
    GstElement  *m_Pipeline;

    GstBus      *bus;
    GstMessage  *msg;
#endif
    QString m_MediaName;
    QRect   m_MediaRect;
    int     m_MediaRotate;
    bool    m_Abort;
    bool    m_Stop;
    quint8  m_PlayMode;  //>@0:???????  //>@1:?????????

    qreal     m_Volume;
};

class ThreadAudio : public QThread
{
public:
    ThreadAudio(QObject *parent = 0);
    ~ThreadAudio(){}

    void SetVolume(int pVolume);
    void Abort();
    void Play(QString pMedia);

protected:
    void run();

public:
#ifdef IMX6X
   GstElement *m_Pipeline;

   GstBus *bus;
   GstMessage *msg;
#endif
    QString m_MediaName;
    qreal     m_Volume;
    bool    m_Abort;
};

class WidgetMedia : public WidgetBase
{
    Q_OBJECT
public:
    WidgetMedia(QWidget *parent = 0);
    ~WidgetMedia();

    void InitRC(int pRotate);
    void InitRC(QString pRcPath, bool pInititial, int pRotate);

    void SetBgImage(QString pBgImage);
    void SetPlayMode(quint8 pMode);

public slots:
    void slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara);

public:
    ThreadMedia     *m_MediaPlayer;
    QRect            m_VoRect;
    int              m_VoRotate;
};

#endif // WIDGETMEDIA_H
