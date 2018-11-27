#include "fun.h"

QString GetStrBehindKey
    (QString pString, QString pKey,
     quint8 pStart, quint8 pLenth,
     Qt::CaseSensitivity pCaps)
{
    QString tmpString;
    tmpString.clear();

    if((pLenth==0)||(pString.isEmpty()))
        return tmpString;
    int tmpDex = pString.indexOf(pKey,pCaps);
    if(tmpDex==-1)
        return tmpString;
    if((tmpDex+pKey.length()+pStart,pLenth)>pString.length())
        return tmpString;

    tmpString = pString.mid(tmpDex+pKey.length()+pStart,pLenth);
    return tmpString;
}

QString GetStrBetweenPreAndAfter
    (QString pString,
     QString pPre, QString pAfter,
     int pPreindent, int pAfterindent,
     Qt::CaseSensitivity pCaps)
{
    QString tmpString;
    tmpString.clear();

    int tmpPreDex = pString.indexOf(pPre,pCaps);
    if(tmpPreDex==-1)
        return tmpString;
    int tmpAfterDex = pString.indexOf(pAfter,pCaps);
    if(tmpAfterDex==-1)
        return tmpString;

    tmpString = pString.mid(tmpPreDex+pPre.length()+pPreindent,
                            tmpAfterDex-tmpPreDex-pPre.length()-pAfterindent);
    return tmpString;
}

/************************************************************************
//>@
************************************************************************/
MULTIMEDIA_CONVERT::MULTIMEDIA_CONVERT(QObject *parent) :
    QObject(parent)
{
    InitPara();
    m_Process = 0;
}

MULTIMEDIA_CONVERT::~MULTIMEDIA_CONVERT()
{
    Release();
}

void MULTIMEDIA_CONVERT::InitPara()
{
    m_FileInfo.mType = Media_Unknow;
    m_FileInfo.mPath.clear();
    m_FileInfo.mName.clear();
    m_FileInfo.mSize = 0;
    m_FileInfo.mResolution.mW = 0;
    m_FileInfo.mResolution.mH = 0;
    m_FileInfo.mLenth = 0;

    m_CovertSet.mCover = false;
    m_CovertSet.mTimeout = 0;
    m_CovertSet.mDst.clear();
    m_CovertSet.mVideoPara.mEncoder.clear();
    m_CovertSet.mVideoPara.mResolution.clear();
    m_CovertSet.mVideoPara.mBitRate.clear();
    m_CovertSet.mVideoPara.mFrameRate.clear();
    m_CovertSet.mVideoPara.mGrabScreen.mEnable = false;
    m_CovertSet.mVideoPara.mGrabScreen.mPara.clear();
    m_CovertSet.mVideoPara.mWatermark.mEnable = false;
    m_CovertSet.mVideoPara.mWatermark.mPara.clear();
    m_CovertSet.mVideoPara.mText.mEnable = false;
    m_CovertSet.mVideoPara.mText.mPara.clear();
    m_CovertSet.mAudioPara.mEncoder.clear();
    m_CovertSet.mAudioPara.mSample.clear();
    m_CovertSet.mAudioPara.mBitRate.clear();
    m_CovertSet.mAudioPara.mChannel.clear();

    m_CovertInfo.mInfo.clear();
    m_CovertInfo.mSucess = false;
    m_CovertInfo.mProgress = 0;
}

void MULTIMEDIA_CONVERT::Release()
{
    InitPara();
    KillProcess();
}

void MULTIMEDIA_CONVERT::KillProcess()
{
    IDE_TRACE_STR("KillProcess");
    if(m_Process != 0)
    {
        disconnect(m_Process, SIGNAL(readyReadStandardOutput()), 0, 0);
        disconnect(m_Process, SIGNAL(finished(int, QProcess::ExitStatus)), 0, 0);
        m_Process->kill();
        static quint8 tmpState;
        while((tmpState = m_Process->state()))
        {
            IDE_TRACE_INT(tmpState);
            QtSleep(20);
        }
        m_Process->waitForFinished(1000);
        delete m_Process;
        m_Process = 0;
    }
}

Media_Type MULTIMEDIA_CONVERT::GetType(QString pStr)
{
    pStr = pStr.replace(" ","");
    IDE_TRACE_STR(pStr);
    QStringList tmpStrList = SplitString(pStr,",");
    quint8 tmpCnt = tmpStrList.count();
    if(tmpCnt < 2)
    {
        IDE_TRACE_STR("File_Unknow");
        return Media_Unknow;
    }
    for(int i=1;i<tmpCnt;i++)
    {
        QString tmpString = tmpStrList.at(i);
        IDE_TRACE_STR(tmpString);
        if((tmpString.compare("mp3",Qt::CaseInsensitive) == 0)||
           (tmpString.compare("wav",Qt::CaseInsensitive) == 0))
        {
            IDE_TRACE_STR("File_Audio");
            return Media_Audio;
        }
        if((tmpString.compare("mp4",Qt::CaseInsensitive) == 0)||
           (tmpString.compare("avi",Qt::CaseInsensitive) == 0)||
           (tmpString.compare("rmvb",Qt::CaseInsensitive) == 0)||
           (tmpString.compare("flv",Qt::CaseInsensitive)) == 0)
        {
            IDE_TRACE_STR("File_Video");
            return Media_Video;
        }
        else
        {
            continue;
        }
    }
    return Media_Unknow;
}

quint32 MULTIMEDIA_CONVERT::GetLenth(QString pLenth)
{
    QStringList tmpStrList = SplitString(pLenth,":");
    quint8  tmpSecond = 0,
            tmpMinute = 0,
            tmpHour = 0;
    quint32 tmpLenth = 0;
    if(tmpStrList.count() != 3)
        return 0;
    bool tmpOk = false;
    tmpSecond = tmpStrList.at(2).toInt(&tmpOk,10);
    if(!tmpOk)
        return 0;
    tmpMinute = tmpStrList.at(1).toInt(&tmpOk,10);
    if(!tmpOk)
        return 0;
    tmpHour = tmpStrList.at(0).toInt(&tmpOk,10);
    if(!tmpOk)
        return 0;
    tmpLenth = tmpHour*60*60 + tmpMinute*60 +tmpSecond;
    return tmpLenth;
}

void MULTIMEDIA_CONVERT::GetResolution(QString pResolution, quint16 &pW, quint16 &pH)
{
    pResolution = pResolution.replace(" ","");
    QStringList tmpStrList0 = pResolution.split(",", QString::KeepEmptyParts, Qt::CaseInsensitive);
    quint8 tmpCnt0 = tmpStrList0.count();
    for(int i=0;i<tmpCnt0;i++)
    {
        QString tmpString = tmpStrList0.at(i);
        QStringList tmpStrList1 = tmpString.split("x", QString::KeepEmptyParts, Qt::CaseInsensitive);
        quint8 tmpCnt1 = tmpStrList1.count();
        if(tmpCnt1 == 2)
        {
            pW = tmpStrList1.at(0).toInt();
            pH = tmpStrList1.at(1).toInt();
            break;
        }
    }
}

quint8 MULTIMEDIA_CONVERT::GetProgress(QString pCur, quint32 pLenth)
{
    QStringList tmpStrList = SplitString(pCur,".");
    if(tmpStrList.count() != 2)
        return 0;
    quint16 tmpCur = 0;
    bool tmpOk = false;
    tmpCur = tmpStrList.at(0).toInt(&tmpOk,10);
    if(!tmpOk)
        return 0;
    tmpCur = ((float)tmpCur/(float)pLenth)*100;
    if(tmpCur == 99)
        tmpCur = 100;
    return tmpCur;
}

bool MULTIMEDIA_CONVERT::GetFileInfo(QString pSrc, int pTimeout)
{
    if(pSrc.isEmpty())
    {
        IDE_TRACE_STR("PathErr");
        return false;
    }
    if(pTimeout <= 5)
    {
        IDE_TRACE_STR("Timeout");
        return false;
    }
    m_FileInfo.mPath = pSrc;
    QFile tmpFile(m_FileInfo.mPath);
    if(!tmpFile.exists())
    {
        IDE_TRACE_STR("FileUnexist");
        return false;
    }
    m_FileInfo.mName = getFileName(m_FileInfo.mPath);
    m_FileInfo.mSize = tmpFile.size();
    tmpFile.deleteLater();
    KillProcess();
    m_Process = new QProcess(this);
    m_Process->setProcessChannelMode(QProcess::MergedChannels);
    connect(m_Process, SIGNAL(readyReadStandardOutput()),this, SLOT(slot_CatchStandardOutput()));
    connect(m_Process, SIGNAL(finished(int, QProcess::ExitStatus)),this, SLOT(slot_Finished(int, QProcess::ExitStatus)));
    //    ./ffmpeg -i test.mp3
    QString tmpString = QString("%1 -i %2")
                                .arg((QString)D_FFMPEG_PATH)
                                .arg(pSrc);
    IDE_TRACE_STR(tmpString);
    m_Process->start(tmpString);
    m_Process->waitForFinished(pTimeout);
    KillProcess();
    return m_CovertInfo.mSucess;
}

bool MULTIMEDIA_CONVERT::ConvertAudio(Media_Info pAudioInfo, CONVERT_SET pConvertSet)
{
    if(pAudioInfo.mType != Media_Audio)
    {
        IDE_TRACE_STR("TypeErr");
        return false;
    }
    if(pAudioInfo.mPath.isEmpty()||pConvertSet.mDst.isEmpty())
    {
        IDE_TRACE_STR("PathErr");
        return false;
    }
    if(pConvertSet.mTimeout <= 10)
    {
        IDE_TRACE_STR("Timeout");
        return false;
    }
    KillProcess();
    m_Process = new QProcess(this);
    m_Process->setProcessChannelMode(QProcess::MergedChannels);
    connect(m_Process, SIGNAL(readyReadStandardOutput()),this, SLOT(slot_CatchStandardOutput()));
    connect(m_Process, SIGNAL(finished(int, QProcess::ExitStatus)),this, SLOT(slot_Finished(int, QProcess::ExitStatus)));
//    ./ffmpeg -y -i test.mp3 -ac 1 -ar 44100 -ab 32 -y test.wav
    QString tmpString = QString("%1 -y -i %2 -ac %3 -ar %4 -ab %5 %6.%7")
                                .arg((QString)D_FFMPEG_PATH)
                                .arg(pAudioInfo.mPath)
                                .arg(pConvertSet.mAudioPara.mChannel)
                                .arg(pConvertSet.mAudioPara.mSample)
                                .arg(pConvertSet.mAudioPara.mBitRate)
                                .arg(pConvertSet.mDst)
                                .arg(pConvertSet.mAudioPara.mEncoder);
    IDE_TRACE_STR(tmpString);
    m_Process->start(tmpString);
    m_Process->waitForFinished(pConvertSet.mTimeout);
    KillProcess();
    if(m_CovertInfo.mProgress == 100)
    {
        IDE_TRACE_STR("Sucess");
        return true;
    }
    else
    {
        IDE_TRACE_INT(m_CovertInfo.mProgress);
        return false;
    }
}

bool MULTIMEDIA_CONVERT::ConvertVideo(Media_Info pVideoInfo, CONVERT_SET pConvertSet)
{
    if(pVideoInfo.mType != Media_Video)
    {
        IDE_TRACE_STR("TypeErr");
        return false;
    }
    if(pVideoInfo.mPath.isEmpty()||pConvertSet.mDst.isEmpty())
    {
        IDE_TRACE_STR("PathErr");
        return false;
    }
    if(pConvertSet.mTimeout <= 10)
    {
        IDE_TRACE_STR("Timeout");
        return false;
    }
    KillProcess();
    m_Process = new QProcess(this);
    m_Process->setProcessChannelMode(QProcess::MergedChannels);
    connect(m_Process, SIGNAL(readyReadStandardOutput()),this, SLOT(slot_CatchStandardOutput()));
    connect(m_Process, SIGNAL(finished(int, QProcess::ExitStatus)),this, SLOT(slot_Finished(int, QProcess::ExitStatus)));
//    ./ffmpeg -y -i test.mp3 -ac 1 -ar 44100 -ab 32 -y test.wav
    QString tmpString = QString("%1 -y -i %2 -ac %3 -ar %4 -ab %5 %6.%7")
                                .arg((QString)D_FFMPEG_PATH)
                                .arg(pVideoInfo.mPath)
                                .arg(pConvertSet.mAudioPara.mChannel)
                                .arg(pConvertSet.mAudioPara.mSample)
                                .arg(pConvertSet.mAudioPara.mBitRate)
                                .arg(pConvertSet.mDst)
                                .arg(pConvertSet.mAudioPara.mEncoder);
    IDE_TRACE_STR(tmpString);
    m_Process->start(tmpString);
    m_Process->waitForFinished(pConvertSet.mTimeout);
    KillProcess();
    if(m_CovertInfo.mProgress == 100)
    {
        IDE_TRACE_STR("Sucess");
        return true;
    }
    else
    {
        IDE_TRACE_INT(m_CovertInfo.mProgress);
        return false;
    }
}

void MULTIMEDIA_CONVERT::slot_CatchStandardOutput()
{
    quint8 tmpCnt = 0;
    QString tmpString,tmpInfo,tmpKeyInfo,tmpKey0,tmpKey1;
    QStringList tmpStrList;
    tmpStrList.clear();
    if(m_Process != 0)
    {
        if(m_Process->state())
            tmpInfo = m_Process->readAllStandardOutput();
    }
    m_CovertInfo.mInfo.append(tmpInfo);
    qDebug()<<"---"<<tmpInfo;
//>@Type
    tmpKey0 = QString("Input #");
    tmpKey1 = QString(", from '");
    tmpKeyInfo = GetStrBetweenPreAndAfter(tmpInfo,tmpKey0,tmpKey1,0,0,Qt::CaseInsensitive);
    if(!tmpKeyInfo.isEmpty())
    {
        IDE_TRACE_STR(tmpKeyInfo);
        m_FileInfo.mType = GetType(tmpKeyInfo);
    }
//>@Lenth
    tmpKey0 = QString("Duration:");
    tmpKeyInfo = GetStrBehindKey(tmpInfo,tmpKey0,1,8,Qt::CaseSensitive);
    if(!tmpKeyInfo.isEmpty())
    {
//        IDE_TRACE_STR(tmpKeyInfo);
        m_FileInfo.mLenth = GetLenth(tmpKeyInfo);
    }
//>@Resolution
    tmpKey0 = QString("Stream #0.0");
    tmpKey1 = QString("[PAR");
    tmpKeyInfo = GetStrBetweenPreAndAfter(tmpInfo,tmpKey0,tmpKey1,0,0,Qt::CaseInsensitive);
    if(!tmpKeyInfo.isEmpty())
    {
        IDE_TRACE_STR(tmpKeyInfo);
        GetResolution(tmpKeyInfo, m_FileInfo.mResolution.mW, m_FileInfo.mResolution.mH);
    }
//>@Progress
    tmpKey0 = QString("time=");
    tmpKey1 = QString("bitrate=");
    tmpStrList = tmpInfo.split(tmpKey0, QString::KeepEmptyParts, Qt::CaseInsensitive);
    tmpCnt = tmpStrList.count();
    for(int i=1;i<tmpCnt;i++)
    {
        tmpString = QString(" time=%1").arg(tmpStrList.at(i));
        IDE_TRACE_STR(tmpString);
        tmpKeyInfo = GetStrBetweenPreAndAfter(tmpString,tmpKey0,tmpKey1,0,1,Qt::CaseSensitive);
        if(!tmpKeyInfo.isEmpty())
        {
            IDE_TRACE_STR(tmpKeyInfo);
            m_CovertInfo.mProgress = GetProgress(tmpKeyInfo,m_FileInfo.mLenth);
            emit sProgress(m_CovertInfo.mProgress);
        }
    }
}

void MULTIMEDIA_CONVERT::slot_Finished(int pID, QProcess::ExitStatus pState)
{
    IDE_TRACE_INT(pID);
    IDE_TRACE_INT(pState);
    m_CovertInfo.mInfo.clear();
    switch(pState)
    {
        case 0:
            IDE_TRACE_STR("Sucess");
            m_CovertInfo.mSucess = true;
            break;
        case 1:
            IDE_TRACE_STR("Falied");
            m_CovertInfo.mSucess = false;
            break;
    }
}

QPixmap GenerateSpectrum(QString pMediaName, QRectF pRect)
{
    QImage tmpImage(pRect.width(), pRect.height(), QImage::Format_ARGB32_Premultiplied);
    if(tmpImage.isNull())
        return false;
    tmpImage.fill(Qt::transparent); //>@´´½¨Í¸Ã÷Í¼²ã
    QPainter painter(&tmpImage);
    MULTIMEDIA_CONVERT tmpMediaCtrl;
    if(tmpMediaCtrl.GetFileInfo(pMediaName, 500))
    {
        qreal tmpDiffHeight = pRect.height() / 6.0;
        qreal tmpDiffWidth = tmpDiffHeight * 2.0 /3.0;

        QString tmpString = QString("[MediaType] : %1").arg(tmpMediaCtrl.m_FileInfo.mType);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), Qt::green, PRIOR_HEIGHT);

        tmpString = QString("[Path] : %1").arg(tmpMediaCtrl.m_FileInfo.mPath);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight, tmpDiffWidth*tmpString.length(), tmpDiffHeight), Qt::green, PRIOR_HEIGHT);

        tmpString = QString("[Name] : %1").arg(tmpMediaCtrl.m_FileInfo.mName);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight*2, tmpDiffWidth*tmpString.length(), tmpDiffHeight), Qt::green, PRIOR_HEIGHT);

        tmpString = QString("[TotalSize] : %1").arg(tmpMediaCtrl.m_FileInfo.mSize);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight*3, tmpDiffWidth*tmpString.length(), tmpDiffHeight), Qt::green, PRIOR_HEIGHT);

        tmpString = QString("[Resolution] : %1x%2").arg(tmpMediaCtrl.m_FileInfo.mResolution.mW).arg(tmpMediaCtrl.m_FileInfo.mResolution.mH);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight*4, tmpDiffWidth*tmpString.length(), tmpDiffHeight), Qt::green, PRIOR_HEIGHT);

        tmpString = QString("[TotalLength] : %1").arg(tmpMediaCtrl.m_FileInfo.mLenth);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight*5, tmpDiffWidth*tmpString.length(), tmpDiffHeight), Qt::green, PRIOR_HEIGHT);
    }
    painter.end();
    return QPixmap::fromImage(tmpImage);
}
