#ifndef QADUIO_H
#define QADUIO_H

/*
    RIFF WAVE Chun 12
    ==================================
    |       |所占字节数|  具体内容   |
    ==================================
    | ID    |  4 Bytes |   'RIFF'    |
    ----------------------------------
    | Size  |  4 Bytes |             |
    ----------------------------------
    | Type  |  4 Bytes |   'WAVE'    |
    ----------------------------------
    */
    /*
    Format Chunk 26
    ====================================================================
    |               |   字节数  |              具体内容                |
    ====================================================================
    | ID            |  4 Bytes  |   'fmt '                             |
    --------------------------------------------------------------------
    | Size          |  4 Bytes  | 数值为16或18，18则最后又附加信息     |
    --------------------------------------------------------------------  ----
    | FormatTag     |  2 Bytes  | 编码方式，一般为0x0001               |     |
    --------------------------------------------------------------------     |
    | Channels      |  2 Bytes  | 声道数目，1--单声道；2--双声道       |     |
    --------------------------------------------------------------------     |
    | SamplesPerSec |  4 Bytes  | 采样频率                             |     |
    --------------------------------------------------------------------     |
    | AvgBytesPerSec|  4 Bytes  | 每秒所需字节数                       |     |===> WAVE_FORMAT
    --------------------------------------------------------------------     |
    | BlockAlign    |  2 Bytes  | 数据块对齐单位(每个采样需要的字节数) |     |
    --------------------------------------------------------------------     |
    | BitsPerSample |  2 Bytes  | 每个采样需要的bit数                  |     |
    --------------------------------------------------------------------     |
    |         bsp;    |  2 Bytes  | 附加信息（可选，通过Size来判断有无） |     |
    --------------------------------------------------------------------  ----
    */
    /*
    Fact Chunk (可选)
    ==================================
    |       |所占字节数|  具体内容   |
    ==================================
    | ID    |  4 Bytes |   'fact'    |
    ----------------------------------
    | Size  |  4 Bytes |   数值为4   |
    ----------------------------------
    | data  |  4 Bytes |             |
    ----------------------------------
    */
    /*
    Data Chunk 8
    ==================================
    |       |所占字节数|  具体内容   |
    ==================================
    | ID    |  4 Bytes |   'data'    |
    ----------------------------------
    | Size  |  4 Bytes |             |
    ----------------------------------
    | data  |          |             |
    ----------------------------------
*/

#include "global.h"
#include <QAudioOutput>
#include <QObject>

class AUDIO_INFO
{
public:
    AUDIO_INFO()
    {
        mRepeat = false;
    }
    ~AUDIO_INFO()
    {
        if(mFile.isOpen())
            mFile.close();
    }
public:
    QString         mPath;
    QAudioFormat    mFormat;
    QFile           mFile;
    bool            mRepeat;
};

class qaduio : public QObject
{
    Q_OBJECT
public:
    qaduio(QObject *parent = 0);
    ~qaduio();

    void ReleaseAuidoBuffer();
    void SetVolume(int pValue);

signals:
    void sFinished();

public slots:
    void slot_Play(QString pPath, QString pPara=QString(), bool pRepeat=false, bool pPrior = false);
    void slot_PlayNext();
    void slot_SetVoiceEnable(bool pEnale);
    void slot_Repeat(bool pRepeat=false, quint32 pInterval=0);
    void slot_Pause();
    void slot_Resume();
    void slot_Stop();
    void slot_StateChanged(QAudio::State pState);

public:
    QList<AUDIO_INFO*>     m_AudioBuffer;
    QAudioOutput          *m_AudioOutput;
    AUDIO_INFO            *m_CurAudio;
    QMutex                *m_Mutex;
};

#endif // QADUIO_H
