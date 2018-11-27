#ifndef FUN_H
#define FUN_H

#include "global.h"
#include <QCoreApplication>
#include <QProcess>
#include <QString>
#include <QTime>
#include <QDebug>
#include <QRect>
#include <QFile>
#include <QObject>

#define D_FFMPEG_PATH       QString("D:/MyProject/IDE/BST_IDE/video/Res/ffmpeg.exe")
#define D_MPLAY_ERPATH   QString("D:/MyProject/IDE/BST_IDE/video/Res/mplayer.exe");
#define D_TEMP_ERPATH   QString("D:/MyProject/IDE/BST_IDE/video/Res/temp");

QString GetStrBehindKey
    (QString pString, QString pKey,
     quint8 pStart, quint8 pLenth,
     Qt::CaseSensitivity pCaps);

QString GetStrBetweenPreAndAfter
    (QString pString,
     QString pPre, QString pAfter,
     int pPreindent, int pAfterindent,
     Qt::CaseSensitivity pCaps);

/************************************************************************
//>@
************************************************************************/
typedef enum{
    Media_Unknow,
    Media_Audio,
    Media_Video
} Media_Type;
typedef struct{
    quint16                     mW;
    quint16                     mH;
}VIDEO_RESOLUTION;
typedef struct{
    Media_Type                   mType;
    QString                     mPath;
    QString                     mName;
    quint32                     mSize;
    VIDEO_RESOLUTION            mResolution;
    quint32                     mLenth;
}Media_Info;
//>@
typedef struct{
    bool                        mEnable;
    QString                     mPara;
}GRABSCREEN_SET;
typedef struct{
    bool                        mEnable;
    QString                     mPara;
}WATERMARk_SET;
typedef struct{
    bool                        mEnable;
    QString                     mPara;
}TEXT_SET;
typedef struct{
    QString                     mEncoder;
    QString                     mResolution;
    QString                     mBitRate;
    QString                     mFrameRate;
    GRABSCREEN_SET              mGrabScreen;
    WATERMARk_SET               mWatermark;
    TEXT_SET                    mText;
}VIDEO_PARA_SET;
typedef struct{
    QString                     mEncoder;
    QString                     mSample;
    QString                     mBitRate;
    QString                     mChannel;
}AUDIO_PARA_SET;
typedef struct{
    bool                        mCover;
    quint64                     mTimeout;
    QString                     mDst;
    GRABSCREEN_SET              mGrabScreenPara;
    VIDEO_PARA_SET              mVideoPara;
    AUDIO_PARA_SET              mAudioPara;
}CONVERT_SET;
//>@
typedef struct{
    QString                     mInfo;
    bool                        mSucess;
    quint8                      mProgress;
}CONVERT_INFO;

class MULTIMEDIA_CONVERT : public QObject
{
    Q_OBJECT
public:
    MULTIMEDIA_CONVERT(QObject *parent = 0);
    ~MULTIMEDIA_CONVERT();
    void Release();
    void KillProcess();

    bool GetFileInfo(QString pSrc, int pTimeout);
    bool ConvertAudio(Media_Info pAudioInfo, CONVERT_SET pConvertSet);
    bool ConvertVideo(Media_Info pVideoInfo, CONVERT_SET pConvertSet);
    //rm/flv/avi/rmvb
private:
    void InitPara();
    Media_Type GetType(QString pStr);
    quint32 GetLenth(QString pLenth);
    void GetResolution(QString pResolution, quint16 &pW, quint16 &pH);
    quint8 GetProgress(QString pCur, quint32 pLenth);
public:
    Media_Info          m_FileInfo;
    CONVERT_SET         m_CovertSet;
    CONVERT_INFO        m_CovertInfo;
private:
    QProcess            *m_Process;
signals:
    void sProgress(quint8 pVaule);
private slots:
    void slot_CatchStandardOutput();
    void slot_Finished(int, QProcess::ExitStatus);
};

QPixmap GenerateSpectrum(QString pMediaName, QRectF pRect);

#endif // FUN_H
