#ifndef QVIDEO_H
#define QVIDEO_H

#include "global.h"
#include <phonon/MediaObject>
#include <phonon/VideoPlayer>
#include <QHBoxLayout>
#include <QObject>

class qvideo : public QObject
{
    Q_OBJECT
public:
    qvideo(QObject *parent = 0);
    ~qvideo();
public:
    QHBoxLayout             *m_HLayout;
    Phonon::VideoPlayer     *m_Player;
    bool                    m_Stop;
signals:
    void sFinished();
public slots:
    void slot_SetVideoRect(QWidget *pWidget);
    void slot_Play(QString pSrc, QString pPara, bool pRepeat, bool pPrior);
    void slot_SetVolume(float pVolume);
    void slot_Seek(qint64 pMs);
    void slot_Pause();
    void slot_Resume();
    void slot_Stop();
    void slot_Finished();
};

#endif // QVIDEO_H
