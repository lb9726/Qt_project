#include "widgetmedia.h"

void ThreadMedia::Abort()  //>@只在线程中使用
{
    m_Abort = true;
#ifdef IMX6X
    if(m_Pipeline)
        gst_element_set_state(m_Pipeline, GST_STATE_PAUSED);
#endif
}

void ThreadMedia::Stop(bool pFlag)
{
    if(pFlag)
    {
        Abort();
        m_Stop = true;
    }
    else
    {
        m_Stop = false;
    }
}

void ThreadMedia::SetPlayMode(quint8 pMode)
{
    m_PlayMode = pMode;
    if((pMode == 0) && (m_Stop == true))
        Stop(false);
}

void ThreadMedia::Change(QString pMedia, QRect pRect, int pRotate)
{
    if(!m_MediaName.compare(pMedia))
    {
        IDE_TRACE();
        return;
    }
    Abort();
    QtSleep(100);
    IDE_TRACE_STR(pMedia);
    m_MediaName = pMedia;
    m_MediaRect = pRect;
    m_MediaRotate = pRotate;
    Stop(false);
}

void ThreadMedia::SetVolume(int pVolume)
{
#ifdef IMX6X
    if(pVolume < 0)
        pVolume = 0;
    else if(pVolume > 100)
        pVolume = 100;
    qreal tmpVolume = (qreal)(pVolume)*4.0/100.0;
    if(m_Pipeline)
    {
        IDE_TRACE();
        GstElement *tmpVolBin = gst_bin_get_by_name(GST_BIN(m_Pipeline), "volume");
        if(tmpVolBin)
        {
            IDE_TRACE_INT(tmpVolume);
            g_object_set(tmpVolBin, "volume", tmpVolume, NULL);
        }
    }
    m_Volume = tmpVolume;
#endif
}

ThreadMedia::ThreadMedia(QObject *parent) : QThread(parent)
{
    m_MediaName = QString("/usr/Storage/UDisk/Media/1.avi");
    m_MediaRotate = 0;
    m_Abort = false;
    m_Stop = false;
    m_PlayMode = 0;
#ifdef IMX6X
    m_Pipeline=0;

    gst_init(0,0);
    QString nano_str;
    guint major, minor, micro, nano;
    gst_version (&major,&minor,&micro,&nano);
    if (nano==1)
        nano_str ="(CVS)";
    else if (nano ==2)
        nano_str ="(Prerelease)";
    else
        nano_str ="";
    IDE_DEBUG(QString("This program is linked against GStreamer %1.%2.%3 %4").arg(major).arg(minor).arg(micro).arg(nano_str));
#endif
}

void ThreadMedia::run()
{
#ifdef IMX6X
    while(1)
    {
        if(m_Stop)
        {
            usleep(100);
            continue;
        }
        if(m_MediaName.isEmpty() || m_MediaRect.isEmpty() || !QFile::exists(m_MediaName))
        {
            usleep(100);
            continue;
        }

        QString tmpCmd = QString("playbin2 uri=file://%1 volume=%7 video-sink=\"mfw_v4lsink axis-top=%3 axis-left=%2 disp-width=%4 disp-height=%5 rotate=%6\"")
                                .arg(m_MediaName)
                                .arg(m_MediaRect.x())
                                .arg(m_MediaRect.y())
                                .arg(m_MediaRect.width())
                                .arg(m_MediaRect.height())
                                .arg(m_MediaRotate)
                                .arg(m_Volume);
        IDE_TRACE_STR(tmpCmd);
        /* Build the pipeline */
        m_Pipeline = gst_parse_launch (tmpCmd.toLatin1().data(), NULL);

        m_Abort = false;

        /* Start playing */
        gst_element_set_state (m_Pipeline, GST_STATE_PLAYING);

        /* Wait until error or EOS */
        bus = gst_element_get_bus (m_Pipeline);
        while(!m_Abort)
        {
            msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, (GstMessageType)(GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

            /* Free resources */
            if(msg != NULL)
            {
                int type = GST_MESSAGE_TYPE (msg);
                if(type == GST_MESSAGE_STATE_CHANGED)
                {
                    GstState old_state, new_state, pending_state;
                    gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
                    //IDE_DEBUG(QString("Pipeline state changed from %1 to %2").arg(QString(gst_element_state_get_name (old_state))).arg(QString(gst_element_state_get_name (new_state))));
                    if((old_state == GST_STATE_PLAYING) && (new_state != GST_STATE_PLAYING))
                    {
                        if(m_Abort == true)
                        {
                            gst_message_unref (msg);
                            break;
                        }
                    }
                }
                else if(type == GST_MESSAGE_ERROR)
                {
                    gst_message_unref (msg);
                    break;
                }
                else if(type == GST_MESSAGE_EOS)
                {
                    gst_message_unref (msg);
                    break;
                }
                gst_message_unref (msg);
            }
        }
        m_Abort = false;
        gst_object_unref (bus);
        gst_element_set_state (m_Pipeline, GST_STATE_NULL);
        gst_object_unref (m_Pipeline);
        system("sync");
        system("echo 3 > /proc/sys/vm/drop_caches");
        if(m_PlayMode == 1)
        {
            m_Stop = true;
        }
        sleep(1);
    }
#endif
#ifdef IMX53
    sleep(1);
    QStringList tmpFileList;
    int index = -1;
    while(1)
    {
        if(tmpFileList.isEmpty())
        {
            tmpFileList = SearchFiles("/mnt/usb/MEDIA/", "avi");
            if(!tmpFileList.isEmpty())
            {
                m_MediaName = tmpFileList.first();
                index = 0;
            }
        }
        if(!m_MediaName.isEmpty())
        {
            index++;
            if(index < tmpFileList.count())
                m_MediaName = tmpFileList.at(index);
            else
            {
                index = 0;
                m_MediaName = tmpFileList.first();
            }
            QString tmpCmd = QString("./myplay %1 %2 %3 %4 0 10 %5")
                                    .arg(m_MediaRect.x())
                                    .arg(m_MediaRect.y())
                                    .arg(m_MediaRect.width())
                                    .arg(m_MediaRect.height())
                                    .arg(m_MediaName);
            system(tmpCmd.toLatin1().data());
        }
        sleep(1);
    }
#endif
}

ThreadAudio::ThreadAudio(QObject *parent) : QThread(parent)
{
#ifdef IMX6X
  m_Pipeline=0;

  gst_init(0,0);
  QString nano_str;
  guint major, minor, micro, nano;
  gst_version (&major,&minor,&micro,&nano);
  if (nano==1)
      nano_str ="(CVS)";
  else if (nano ==2)
      nano_str ="(Prerelease)";
  else
      nano_str ="";
  IDE_DEBUG(QString("This program is linked against GStreamer %1.%2.%3 %4").arg(major).arg(minor).arg(micro).arg(nano_str));
#endif
}

void ThreadAudio::SetVolume(int pVolume)
{
#ifdef IMX6X
    if(pVolume < 0)
        pVolume = 0;
    else if(pVolume > 100)
        pVolume = 100;
    qreal tmpVolume = (qreal)(pVolume)*4.0/100.0;
    if(m_Pipeline)
    {
        IDE_TRACE();
        GstElement *tmpVolBin = gst_bin_get_by_name(GST_BIN(m_Pipeline), "volume");
        if(tmpVolBin)
        {
            IDE_TRACE_INT(tmpVolume);
            g_object_set(tmpVolBin, "volume", tmpVolume, NULL);
        }
    }
    m_Volume = tmpVolume;
#endif
}

void ThreadAudio::Play(QString pMedia)
{
    m_MediaName = pMedia;
    start();
}

void ThreadAudio::Abort()  //>@只在线程中使用
{
    m_Abort = true;
#ifdef IMX6X
    if(m_Pipeline)
        gst_element_set_state(m_Pipeline, GST_STATE_PAUSED);
#endif
}

void ThreadAudio::run()
{
#ifdef IMX6X
    if(m_MediaName.isEmpty() || !QFile::exists(m_MediaName))
    {
        //usleep(100);
        return;
    }

    QString tmpCmd = QString("playbin2 uri=file://%1 volume=%2")
                            .arg(m_MediaName).arg(m_Volume);
    IDE_TRACE_STR(tmpCmd);
    /* Build the pipeline */
    m_Pipeline = gst_parse_launch (tmpCmd.toLatin1().data(), NULL);

    m_Abort = false;
    /* Start playing */
    gst_element_set_state (m_Pipeline, GST_STATE_PLAYING);

    /* Wait until error or EOS */
    bus = gst_element_get_bus (m_Pipeline);
    while(1)
    {
        msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, (GstMessageType)(GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

        /* Free resources */
        if(msg != NULL)
        {
            int type = GST_MESSAGE_TYPE (msg);
            if(type == GST_MESSAGE_STATE_CHANGED)
            {
                GstState old_state, new_state, pending_state;
                gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
                //IDE_DEBUG(QString("Pipeline state changed from %1 to %2").arg(QString(gst_element_state_get_name (old_state))).arg(QString(gst_element_state_get_name (new_state))));
                if((old_state == GST_STATE_PLAYING) && (new_state != GST_STATE_PLAYING))
                {
                    if(m_Abort == true)
                    {
                        gst_message_unref (msg);
                        break;
                    }
                }
            }
            else if(type == GST_MESSAGE_ERROR)
            {
                gst_message_unref (msg);
                break;
            }
            else if(type == GST_MESSAGE_EOS)
            {
                gst_message_unref (msg);
                break;
            }
            gst_message_unref (msg);
        }
    }
    m_Abort = false;
    gst_object_unref (bus);
    gst_element_set_state (m_Pipeline, GST_STATE_NULL);
    gst_object_unref (m_Pipeline);
    system("sync");
    system("echo 3 > /proc/sys/vm/drop_caches");
#endif
}

WidgetMedia::WidgetMedia(QWidget *parent) :
    WidgetBase(parent)
{
    m_VoRotate = 0;
    m_MediaPlayer = new ThreadMedia();
    m_MediaPlayer->start();
}

WidgetMedia::~WidgetMedia()
{
    m_MediaPlayer->Abort();
    m_MediaPlayer->wait(2000);
    m_MediaPlayer->deleteLater();
}

void WidgetMedia::SetBgImage(QString pBgImage)
{
    pBgImage.replace("\\", "/");
    pBgImage.replace("//", "/");
    m_ImgPointer.mCurPointer = QPixmap(pBgImage);
    Update();
}

void WidgetMedia::InitRC(int pRotate)
{
    QDesktopWidget *tmpDesktop = QApplication::desktop();
    QRect tmpDesktopRect = tmpDesktop->rect();
    m_VoRotate = pRotate;
    QPoint tmpPoint = mapToGlobal(QPoint(0,0));
    QRect pRect = QRect(tmpPoint, size());

    int tmpXDiff = pRect.x() % 8;
    if(tmpXDiff != 0)
        pRect.setX(pRect.x() - tmpXDiff);
    int tmpYDiff = pRect.y() % 8;
    if(tmpYDiff != 0)
        pRect.setY(pRect.y() - tmpYDiff);

    int width = pRect.width() + tmpXDiff;
    int tmpWDiff = width % 8;
    if(tmpWDiff != 0)
        pRect.setWidth(width + (8-tmpWDiff));
    else
        pRect.setWidth(width);
    int height = pRect.height() + tmpYDiff;
    int tmpHDiff = height % 8;
    if(tmpHDiff != 0)
        pRect.setHeight(height + (8-tmpHDiff));
    else
        pRect.setHeight(height);

    switch(m_VoRotate)
    {
        case 90:
            m_VoRotate = 0;
            m_VoRect = QRect(pRect.y(), tmpDesktopRect.width() - pRect.x() - pRect.width(), pRect.height(), pRect.width());
            break;
        case 180:
            m_VoRect = QRect(tmpDesktopRect.width() - pRect.x() - pRect.width(), tmpDesktopRect.height() - pRect.y() - pRect.height(), pRect.width(), pRect.height());
            break;
        case 270:
            m_VoRotate = 180;
            m_VoRect = QRect(tmpDesktopRect.height() - pRect.y() - pRect.height(), pRect.x(), pRect.height(), pRect.width());
            break;
        default:
            m_VoRect = pRect;
            break;
    }
}

void WidgetMedia::InitRC(QString pRcPath, bool pInititial, int pRotate)
{
    if(!pRcPath.isEmpty())
    {
        if(!pRcPath.endsWith('/'))
            pRcPath.append('/');
        pRcPath.replace("//", "/");
        QStringList tmpFileList = getAllFileInFloders(pRcPath);
        QString tmpFile;
        foreach(tmpFile, tmpFileList)
        {
            QString tmpFileName = getFileName(tmpFile);
            tmpFileName.remove("RC");
            if(tmpFileName.endsWith(".avi", Qt::CaseInsensitive) ||
               tmpFileName.endsWith(".mp4", Qt::CaseInsensitive))
            {
                int index = tmpFileName.indexOf('.');
                if(index)
                    tmpFileName = tmpFileName.mid(0, index);
                int tmpKey = GetKey(tmpFileName);
                if(tmpKey != -1)
                    load(tmpKey, tmpFile);
                IDE_TRACE_INT(tmpKey);
            }
        }
    }

    InitRC(pRotate);
    if(pInititial)
    {
        QList<int> tmpList = m_FileList.keys();
        if(!tmpList.isEmpty())
        {
            qSort(tmpList.begin(), tmpList.end());
            slot_ExecOperate(OPERATE_CHANGE, QVariant(tmpList.first()));
        }
    }
}

void WidgetMedia::SetPlayMode(quint8 pMode)
{
    if(m_MediaPlayer)
        m_MediaPlayer->SetPlayMode(pMode);
}

void WidgetMedia::slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara)
{
    switch(pOperate)
    {
        case OPERATE_CHANGE:
        {
            QString tmpFile = m_FileList.value(pPara.toInt());
            if(QFile::exists(tmpFile) && m_MediaPlayer)
            {
                m_MediaPlayer->Change(tmpFile, m_VoRect, m_VoRotate);
            }
            break;
        }
        default:
            break;
    }
}
