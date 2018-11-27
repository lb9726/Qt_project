#include "gstreamer.h"

gstreamer::gstreamer(QObject *parent) :
    QObject(parent)
{
    m_RectString.clear();
    m_PlayProcess = 0;
    m_Pause = false;
}

gstreamer::~gstreamer()
{
    m_RectString.clear();
    m_Pause = false;
    ReleaseProcess();
}

void gstreamer::CreatProcess()
{
    ReleaseProcess();
    m_PlayProcess = new QProcess(this);
    m_PlayProcess->setProcessChannelMode(QProcess::MergedChannels);
    connect(m_PlayProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(slot_Finished(int, QProcess::ExitStatus)));
}

void gstreamer::ReleaseProcess()
{
    IDE_TRACE();
    if(m_PlayProcess !=0)
    {
        disconnect(m_PlayProcess, SIGNAL(finished(int, QProcess::ExitStatus)),0,0);
        m_PlayProcess->kill();
        static quint8 tmpState;
        while((tmpState = m_PlayProcess->state()))
        {
            IDE_TRACE_INT(tmpState);
            QtSleep(20);
        }
        m_PlayProcess->waitForFinished(D_WAITTIME);
        delete m_PlayProcess;
        m_PlayProcess = 0;
    }
}

void gstreamer::slot_SetVideoRect(QRect pRect, qreal pRotate)
{ 
    QDesktopWidget *tmpDesktop = QApplication::desktop();
    QRect tmpDesktopRect = tmpDesktop->rect();
    int x, y, w, h;
    QString tmpRotatePara;
    switch((int)pRotate)
    {
        case 90:
            x = tmpDesktopRect.width() - pRect.y() - pRect.height();
            y = pRect.x();
            w = pRect.width();
            h = pRect.height();
            tmpRotatePara = QString("rotate=%1 overlay-width=%2 overlay-height=%3 overlay-top=%4 overlay-left=%5").arg(pRotate).arg(w).arg(h).arg(x).arg(y);
            break;
        case 180:
            x = tmpDesktopRect.width() - pRect.x() - pRect.width();
            y = tmpDesktopRect.height() - pRect.y() - pRect.height();
            w = pRect.width();
            h = pRect.height();
            tmpRotatePara = QString("rotate=%1 overlay-width=%2 overlay-height=%3 overlay-top=%4 overlay-left=%5").arg(pRotate).arg(w).arg(h).arg(x).arg(y);
            break;
        case 270:
            x = pRect.y();
            y = tmpDesktopRect.height() - pRect.x() - pRect.width();
            w = pRect.width();
            h = pRect.height();
            tmpRotatePara = QString("rotate=%1 overlay-width=%2 overlay-height=%3 overlay-top=%4 overlay-left=%5").arg(pRotate).arg(w).arg(h).arg(x).arg(y);
            break;
        default:
            x = pRect.x();
            y = pRect.y();
            w = pRect.width();
            h = pRect.height();
            tmpRotatePara = QString("rotate=%1 overlay-width=%2 overlay-height=%3 overlay-top=%4 overlay-left=%5").arg(pRotate).arg(w).arg(h).arg(x).arg(y);
            break;
    }
    m_RectString.clear();
    m_RectString = QString("-quiet -framedrop %1").arg(tmpRotatePara);
    IDE_TRACE_STR(m_RectString);
}

void gstreamer::slot_Play(QString pSrc, QString pPara, bool pRepeat, bool pPrior)
{
    QString tmpPath = D_GSTREAMER_PATH;
    QString tmpString = QString("%1 filesrc location=%2 ! qtdemux name=demux demux.audio_00 ! queue max-size-buffers=8000 max-size-time=0 max-size-bytes=0 ! TIAuddec1 ! alsasink demux.video_00 ! queue ! TIViddec2 padAllocOutbufs=TRUE ! queue ! tidisplaysink2 %3 display-output=lcd").arg(tmpPath).arg(pSrc).arg(m_RectString);
    CreatProcess();
    IDE_TRACE_STR(tmpString);
    m_PlayProcess->start(tmpString);
}

void gstreamer::slot_VideoToPic(QString pSrc, QString pSavepath, QString pSavetype)
{
    IDE_DEBUG("Gstreamer don't support Converting video to pic!");
}

void gstreamer::slot_SetVolume(float pVolume)
{
    if(m_PlayProcess != 0)
    {
        QString tmpString;
        tmpString.clear();
        tmpString = QString("volume %1 1\n").arg(pVolume);
        m_PlayProcess->write(tmpString.toUtf8());
    }
}

void gstreamer::slot_Seek(qint64 pMs)
{
    if(m_PlayProcess != 0)
    {
        QString tmpString;
        tmpString.clear();
        tmpString = QString("seek %1 2\n").arg(pMs);
        m_PlayProcess->write(tmpString.toUtf8());
    }
}

void gstreamer::slot_Pause()
{
    if(m_PlayProcess != 0)
    {
        if(m_Pause)
            return;
        QString tmpString;
        tmpString.clear();
        tmpString = QString("pause\n");
        m_PlayProcess->write(tmpString.toUtf8());
        m_Pause = true;
    }
}

void gstreamer::slot_Resume()
{
    if(m_PlayProcess != 0)
    {
        if(!m_Pause)
            return;
        QString tmpString;
        tmpString.clear();
        tmpString = QString("pause\n");
        m_PlayProcess->write(tmpString.toUtf8());
        m_Pause = false;
    }
}

void gstreamer::slot_Stop()
{
    ReleaseProcess();
    m_Pause = false;
}

void gstreamer::slot_Finished(int pExitCode, QProcess::ExitStatus pStatus)
{
    emit sFinished();
}
