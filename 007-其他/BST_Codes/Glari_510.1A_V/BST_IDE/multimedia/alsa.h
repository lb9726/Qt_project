#ifndef ALSA_H
#define ALSA_H

#include "global.h"
#include "lplay.h"
#include <QDebug>
#include <QTimer>
#include <QMutex>
#include <QObject>

class alsa : public QObject
{
    Q_OBJECT
public:
    alsa(QObject *parent = 0);
    ~alsa();
public:
signals:
    void sFinished();
public slots:
    void slot_Init();
    void slot_Play(QString pPath);
    void slot_SetVoiceEnable(bool pEnale);
    void slot_Pause();
    void slot_Resume();
    void slot_Stop();
    void slot_Finished();
};

#endif // ALSA_H
