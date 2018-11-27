#ifndef BASEAUDIOCODEC_H
#define BASEAUDIOCODEC_H

#include "global.h"
#include "fft.h"
#include <math.h>
#include <QtCore/qmath.h>
#include <QtCore/qendian.h>
#include <QCoreApplication>
#include <QtCore/qendian.h>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioOutput>
#include <QByteArray>
#include <QBuffer>
#include <QVector>
#include <QMetaObject>
#include <QSet>
#include <QThread>
#include <QFile>
#include <QVector>
#include <QTimer>
#include <QMutex>
#include <QObject>

#define D_UnitFormatTime            quint32(50000)//>@2000 is the least vaule
#define D_AudioNotifyInterval       quint32(50)
//>@
typedef enum{
    Object_File,
    Object_Mem
}Object_Type;

typedef enum{
    HD_In,
    HD_Out
}HD_Type;
//>@ÒôÆµÍ·ÐÅÏ¢
typedef struct
{
    char        id[4];
    quint32     size;
}chunk;
typedef struct
{
    chunk       descriptor;     // "RIFF"
    char        type[4];        // "WAVE"
}RIFFhead;
typedef struct
{
    chunk       descriptor;
    quint16     audioFormat;
    quint16     numChannels;
    quint32     sampleRate;
    quint32     byteRate;
    quint16     blockAlign;
    quint16     bitsPerSample;
}WAVEhead;
typedef struct
{
    chunk       descriptor;
}DATAhead;
typedef struct
{
    RIFFhead  riff;
    WAVEhead  wave;
}FileHead;
//>@
class HDInfo : public QObject
{
    Q_OBJECT
public:
    HDInfo(QObject *parent = 0)
    {
        m_AudioInput = 0;
        m_AudioOutput = 0;
    }
    ~HDInfo()
    {
        ReleaseHD(HD_In);
        ReleaseHD(HD_Out);
    }
    bool IsHDSupportFormat(QAudioFormat pFormat, HD_Type pType)
    {
        QAudioDeviceInfo tmpHDInfo;
        switch(pType)
        {
            case HD_In:tmpHDInfo = QAudioDeviceInfo::defaultInputDevice();break;
            case HD_Out:tmpHDInfo = QAudioDeviceInfo::defaultOutputDevice();break;
            default:return false;
        }
        if(pFormat == QAudioFormat())
            return false;
        if (tmpHDInfo.isFormatSupported(pFormat))
            return true;
        else
            return false;
    }
    bool CreatHDSupportFormat(QAudioFormat &pFormat, HD_Type pType)
    {
        QAudioDeviceInfo tmpHDInfo;
        switch(pType)
        {
            case HD_In:tmpHDInfo = QAudioDeviceInfo::defaultInputDevice();break;
            case HD_Out:tmpHDInfo = QAudioDeviceInfo::defaultOutputDevice();break;
            default:return false;
        }
        QList<int> frqList;
        QList<int> channelList;
        frqList += tmpHDInfo.supportedFrequencies();
        qSort(frqList);
        channelList += tmpHDInfo.supportedChannels();
        qSort(channelList);
        pFormat.setByteOrder(QAudioFormat::LittleEndian);
        pFormat.setCodec("audio/pcm");
        pFormat.setSampleSize(16);
        pFormat.setSampleType(QAudioFormat::SignedInt);
        int frq, channel;
        foreach (frq, frqList)
        {
            pFormat.setFrequency(frq);
            foreach (channel, channelList)
            {
                pFormat.setChannels(channel);
                if(tmpHDInfo.isFormatSupported(pFormat))
                    return true;
            }
        }
        return false;
    }
    quint64 FormatToLenth(QAudioFormat pFormat, quint64 pUs)
    {
        quint64 tmpSize =
                (pFormat.frequency() * pFormat.channels() * (pFormat.sampleSize() / 8))
                * pUs / 1000000;
        tmpSize -= tmpSize % (pFormat.channels() * pFormat.sampleSize());
        return tmpSize;
    }
    bool CreatHD(QAudioFormat pFormat, HD_Type pType)
    {
        if(!IsHDSupportFormat(pFormat, pType))
            return false;
        ReleaseHD(pType);
        switch(pType)
        {
            case HD_In:
                m_AudioInput = new QAudioInput(pFormat, this);
                m_AudioInput->setNotifyInterval(D_AudioNotifyInterval);
                break;
            case HD_Out:
                m_AudioOutput = new QAudioOutput(pFormat, this);
                m_AudioOutput->setNotifyInterval(D_AudioNotifyInterval);
                break;
            default:return false;
        }
        return true;
    }
    bool ReleaseHD(HD_Type pType)
    {
        switch(pType)
        {
            case HD_In:
                if(m_AudioInput != 0)
                {
                    disconnect(m_AudioInput, SIGNAL(notify()), 0, 0);
                    m_AudioInput->deleteLater();
                    m_AudioInput = 0;
                }
                break;
            case HD_Out:
                if(m_AudioOutput != 0)
                {
                    disconnect(m_AudioOutput, SIGNAL(notify()), 0, 0);
                    m_AudioOutput->deleteLater();
                    m_AudioOutput = 0;
                }
                break;
            default:return false;
        }
    }
public:
    QAudioInput                             *m_AudioInput;
    QAudioOutput                            *m_AudioOutput;
};

class FileInfo
{
public:
    FileInfo()
    {
        m_HeadLenth = 0;
    }
    ~FileInfo()
    {
        Release();
    }
    void Release()
    {
        if(m_Src.isOpen())
            m_Src.close();
        if(m_SrcCopy.isOpen())
            m_SrcCopy.close();
        if(m_Dst.isOpen())
            m_Dst.close();
        m_HeadLenth = 0;
    }
    bool GetFileFormat(QString pSrc, QAudioFormat &pFormat)
    {
        Release();
        m_Src.setFileName(pSrc);
        if (!m_Src.open(QIODevice::ReadOnly))
            return false;
        m_SrcCopy.setFileName(pSrc);
        if (!m_SrcCopy.open(QIODevice::ReadOnly))
            return false;
        m_Src.seek(0);
        m_SrcCopy.seek(0);
        FileHead head;
        bool result = m_Src.read(reinterpret_cast<char *>(&head), sizeof(FileHead)) == sizeof(FileHead);
    //        bool result = m_Src.read(static_cast<char *>(&head), sizeof(FileHead)) == sizeof(FileHead);
        if (result)
        {
    //>@riff
            if ((memcmp(&head.riff.descriptor.id, "RIFF", 4) == 0 ||
                 memcmp(&head.riff.descriptor.id, "RIFX", 4) == 0) &&
                 memcmp(&head.riff.type, "WAVE", 4) == 0 &&
                 memcmp(&head.wave.descriptor.id, "fmt ", 4) == 0 &&
                 (head.wave.audioFormat == 1 || head.wave.audioFormat == 0))
            {
    //>@data
                DATAhead datahead;
                if (qFromLittleEndian<quint32>(head.wave.descriptor.size) > sizeof(WAVEhead))
                {
                    quint16 extraFormatBytes;
                    if (m_Src.peek((char*)&extraFormatBytes, sizeof(quint16)) != sizeof(quint16))
                        return false;
                    const qint64 throwAwayBytes = sizeof(quint16) + qFromLittleEndian<quint16>(extraFormatBytes);
                    if (m_Src.read(throwAwayBytes).size() != throwAwayBytes)
                        return false;
                }
                if (m_Src.read((char*)&datahead, sizeof(DATAhead)) != sizeof(DATAhead))
                    return false;
    //>@format
                if (memcmp(&head.riff.descriptor.id, "RIFF", 4) == 0)
                    pFormat.setByteOrder(QAudioFormat::LittleEndian);
                else
                    pFormat.setByteOrder(QAudioFormat::BigEndian);
                int bps = qFromLittleEndian<quint16>(head.wave.bitsPerSample);
                pFormat.setChannels(qFromLittleEndian<quint16>(head.wave.numChannels));
                pFormat.setCodec("audio/pcm");
                pFormat.setFrequency(qFromLittleEndian<quint32>(head.wave.sampleRate));
                pFormat.setSampleSize(qFromLittleEndian<quint16>(head.wave.bitsPerSample));
                pFormat.setSampleType(bps == 8 ? QAudioFormat::UnSignedInt : QAudioFormat::SignedInt);
            }
            else
            {
                result = false;
            }
        }
        m_HeadLenth = m_Src.pos();
        return result;
    }
    bool AddHead(QAudioFormat pFormat, QByteArray &pByteArray)
    {
        if(pFormat.codec().compare("audio/pcm",Qt::CaseSensitive) != 0)
            return false;
        quint32 dataSize = pByteArray.size();
        FileHead head;
    //>@riff
        if(pFormat.byteOrder() == QAudioFormat::LittleEndian)
            memcpy(&head.riff.descriptor.id, "RIFF", 4);
        else
            memcpy(&head.riff.descriptor.id, "RIFX", 4);
        head.riff.descriptor.size = sizeof(FileHead) + dataSize;
        memcpy(&head.riff.type, "WAVE", 4);
    //>@format
        head.wave.audioFormat = 1;
        head.wave.bitsPerSample = pFormat.sampleSize();
        head.wave.blockAlign = 4;
        head.wave.byteRate = 176400;
        memcpy(&head.wave.descriptor.id, "fmt ", 4);
        head.wave.descriptor.size = 16;
        head.wave.numChannels = pFormat.channels();
        head.wave.sampleRate = pFormat.sampleRate();
        QByteArray byteArray;
        byteArray.append(reinterpret_cast<char *>(&head),sizeof(FileHead));
    //>@data
        DATAhead datahead;
        memcpy(&datahead.descriptor.id, "data", 4);
        datahead.descriptor.size = dataSize;
        byteArray.append(reinterpret_cast<char *>(&datahead),sizeof(DATAhead));

        pByteArray.insert(0,byteArray);
        byteArray.clear();
        return true;
    }
public:
    QFile                                   m_Src, m_SrcCopy, m_Dst;
    quint32                                 m_HeadLenth;
};

class baseAudiocodec : public QObject
{
    Q_OBJECT
public:
    baseAudiocodec(QObject *parent = 0);
    ~baseAudiocodec();
    void InitBaseClass();
    void ReleaseBaseClass();
    void Reset();

    void PlayFile(QString pSrc);
    void RecordToMem(quint32 pTimeMs);
    void CreatToMem(quint32 pCnt, quint32 pTimeMs);
    void MemSave(QString pDst);
    void Play();

    void CalculateTFD(qint64 position, qint64 length);
public:
    HDInfo                                  *m_HDInfo;
    FileInfo                                *m_FileInfo;
    FFT                                     *m_FFT;
    QThread                                 *m_Thread;
    QIODevice                               *m_IODevice;

    QAudioFormat                            m_AudioFormat;
    quint64                                 m_UnitFormatLength;
    QByteArray                              m_ByteArray;
    QBuffer                                 m_Buffer;

    Object_Type                             m_ObjectType;
    bool                                    m_Playing;
signals:
    void sFFT1D(Complex_Array* pFD, Complex_Array* pTD, quint32 pGrade);
    void sIFFT1D(Complex_Array* pTD, Complex_Array* pFD, quint32 pGrade);

    void sLevel(qreal pRms, qreal pPeak, qint64 pTimeline);
    void sFD(double *pFDRms, quint32 pGrade, quint32 pFrq);
public slots:
    void slot_readyRead();
    void slot_notify();

    void slot_FDComplexArray(Complex_Array* pFD, quint32 pNum);
    void slot_TDComplexArray(Complex_Array* pTD, quint32 pNum);
};

#endif // BASEAUDIOCODEC_H
