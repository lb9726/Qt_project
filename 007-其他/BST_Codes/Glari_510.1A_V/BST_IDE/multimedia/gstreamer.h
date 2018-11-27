#ifndef GSTREAMER_H
#define GSTREAMER_H

#include "global.h"
#include <QProcess>
#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QObject>
#include <QMutex>
#include <QThread>

class gstreamer : public QObject
{
    Q_OBJECT
public:
    gstreamer(QObject *parent = 0);
    ~gstreamer();
    void CreatProcess();
    void ReleaseProcess();
public:
    QProcess    *m_PlayProcess;
    QString      m_RectString;
    bool         m_Pause;
signals:
    void sFinished();
public slots:
    void slot_SetVideoRect(QRect pRect, qreal pRotate = 0);
    void slot_Play(QString pSrc, QString pPara, bool pRepeat, bool pPrior);
    void slot_VideoToPic(QString pSrc, QString pSavepath, QString pSavetype);
    void slot_SetVolume(float pVolume);
    void slot_Seek(qint64 pMs);
    void slot_Pause();
    void slot_Resume();
    void slot_Stop();
    void slot_Finished(int pExitCode, QProcess::ExitStatus pStatus);
};

#endif // GSTREAMER_H
