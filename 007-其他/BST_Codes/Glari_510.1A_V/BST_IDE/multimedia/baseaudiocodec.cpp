#include "baseaudiocodec.h"

baseAudiocodec::baseAudiocodec(QObject *parent)
    :   QObject(parent)
    ,   m_HDInfo(0)
    ,   m_FileInfo(0)
    ,   m_FFT(0)
    ,   m_Thread(0)
    ,   m_IODevice(0)
    ,   m_ByteArray(QByteArray())
    ,   m_UnitFormatLength(0)
    ,   m_ObjectType(Object_File)
    ,   m_Playing(false)
{
    InitBaseClass();
}

baseAudiocodec::~baseAudiocodec()
{
    Reset();
    ReleaseBaseClass();
}

void baseAudiocodec::InitBaseClass()
{
    m_HDInfo = new HDInfo(this);
    m_FileInfo = new FileInfo;
    m_Thread = new QThread(this);
    m_FFT = new FFT(0);
    connect(this, SIGNAL(sFFT1D(Complex_Array*, Complex_Array*, quint32)), m_FFT, SLOT(slot_FFT1D(Complex_Array*, Complex_Array*, quint32)));
    connect(this, SIGNAL(sIFFT1D(Complex_Array*, Complex_Array*, quint32)), m_FFT, SLOT(slot_IFFT1D(Complex_Array*, Complex_Array*, quint32)));
    connect(m_FFT, SIGNAL(sFDComplexArray(Complex_Array*, quint32)), this, SLOT(slot_FDComplexArray(Complex_Array*, quint32)));
    connect(m_FFT, SIGNAL(sTDComplexArray(Complex_Array*, quint32)), this, SLOT(slot_TDComplexArray(Complex_Array*, quint32)));
    m_FFT->moveToThread(m_Thread);
    m_Thread->start(QThread::LowPriority);
    m_Thread->wait(10);
}

void baseAudiocodec::ReleaseBaseClass()
{
    if(m_HDInfo != 0)
    {
        m_HDInfo->deleteLater();
        m_HDInfo = 0;
    }
    if(m_FileInfo != 0)
    {
       delete  m_FileInfo;
       m_FileInfo = 0;
    }
    if(m_FFT !=0)
    {
        disconnect(this, SIGNAL(sFFT1D(Complex_Array*, Complex_Array*, quint32)), 0, 0);
        disconnect(this, SIGNAL(sIFFT1D(Complex_Array*, Complex_Array*, quint32)), 0, 0);
        m_FFT->deleteLater();
        m_FFT = 0;
    }
    if(m_Thread != 0)
    {
        if(m_Thread->isRunning())
        {
            m_Thread->exit();
        }
        m_Thread->wait();
        m_FFT->deleteLater();
        m_Thread = 0;
    }
}

void baseAudiocodec::Reset()
{
    m_AudioFormat = QAudioFormat();
    m_UnitFormatLength = 0;
    m_ByteArray.clear();
    m_Buffer.close();
    m_ObjectType = Object_File;
    m_Playing = false;
}

void baseAudiocodec::PlayFile(QString pSrc)
{
    Reset();
    m_ObjectType = Object_File;
    if(m_FileInfo != 0)
    {
        if(!m_FileInfo->GetFileFormat(pSrc, m_AudioFormat))
        {
            IDE_TRACE_STR("PlayFileErr");
            return;
        }
    }
    Play();
}

void baseAudiocodec::RecordToMem(quint32 pTimeMs)
{
    Reset();
    m_ObjectType = Object_Mem;
    if(m_HDInfo == 0)
    {
        IDE_TRACE_STR("RecordToMemErr");
        return;
    }
    m_HDInfo->ReleaseHD(HD_Out);
    if(!m_HDInfo->CreatHDSupportFormat(m_AudioFormat, HD_In))
    {
        IDE_TRACE_STR("RecordToMemErr");
        return;
    }
    m_UnitFormatLength = m_HDInfo->FormatToLenth(m_AudioFormat, D_UnitFormatTime);
    quint64 size = m_HDInfo->FormatToLenth(m_AudioFormat, pTimeMs*1000);
    m_ByteArray.resize(size);
    m_ByteArray.fill(0);
    if(!m_HDInfo->CreatHD(m_AudioFormat, HD_In))
    {
        IDE_TRACE_STR("RecordToMemErr");
        return;
    }
    if(m_HDInfo->m_AudioInput == 0)
    {
        IDE_TRACE_STR("PlayFileErr");
        return;
    }
    connect(m_HDInfo->m_AudioInput, SIGNAL(notify()),this, SLOT(slot_notify()));
    m_IODevice = m_HDInfo->m_AudioInput->start();
    connect(m_IODevice, SIGNAL(readyRead()),this, SLOT(slot_readyRead()));
}

void baseAudiocodec::CreatToMem(quint32 pCnt, quint32 pTimeMs)
{
    Reset();
    m_ObjectType = Object_Mem;
    if(m_HDInfo == 0)
    {
        IDE_TRACE_STR("CreatToMemErr");
        return;
    }
    m_HDInfo->ReleaseHD(HD_Out);
    m_HDInfo->ReleaseHD(HD_In);
    if(!m_HDInfo->CreatHDSupportFormat(m_AudioFormat, HD_In))
    {
        IDE_TRACE_STR("CreatToMemErr");
        return;
    }
    const quint64 size = m_HDInfo->FormatToLenth(m_AudioFormat, pTimeMs*1000);
    const quint64 pulseWidth = m_HDInfo->FormatToLenth(m_AudioFormat, D_UnitFormatTime);
    const quint64 arrayCnt = size / 2;
    const quint64 sameArrayCnt = pulseWidth / 2;
    const quint64 totalChangePoint = arrayCnt / sameArrayCnt;
    IDE_TRACE_INT(arrayCnt);
    IDE_TRACE_INT(sameArrayCnt);
    IDE_TRACE_INT(totalChangePoint);
    m_ByteArray.resize(size);
    m_ByteArray.fill(0);
    QDataStream dataStreamIn(&m_ByteArray,QIODevice::WriteOnly);
    dataStreamIn.setVersion(QDataStream::Qt_4_8);
    dataStreamIn.setByteOrder(QDataStream::LittleEndian);
    for(int i=0;i<totalChangePoint;i++)
    {
        qreal amplitude = qreal(16383 + 16383 * qSin(2 * M_PI * i * 360.0 * pCnt / totalChangePoint / 360.0));
        for(int j=0;j<sameArrayCnt;j++)
        {
            dataStreamIn<<quint16(amplitude);
        }
    }
}

void baseAudiocodec::MemSave(QString pDst)
{
    if(m_FileInfo == 0)
    {
        IDE_TRACE_STR("MemSaveErr");
        return;
    }
    if(m_HDInfo == 0)
    {
        IDE_TRACE_STR("MemSaveErr");
        return;
    }
    IDE_TRACE_INT(m_ByteArray.size());
    if(!m_FileInfo->AddHead(m_AudioFormat, m_ByteArray))
    {
        IDE_TRACE_STR("MemSaveErr");
        return;
    }
    m_FileInfo->Release();
    m_FileInfo->m_Dst.setFileName(pDst);
    if(!m_FileInfo->m_Dst.open(QIODevice::WriteOnly))
    {
        IDE_TRACE_STR("MemSaveErr");
        return;
    }
    m_FileInfo->m_Dst.write(m_ByteArray);
    m_FileInfo->m_Dst.close();
}

void baseAudiocodec::Play()
{
    if(m_HDInfo == 0)
    {
        IDE_TRACE_STR("PlayFileErr");
        return;
    }
    m_HDInfo->ReleaseHD(HD_In);
    m_UnitFormatLength = m_HDInfo->FormatToLenth(m_AudioFormat, D_UnitFormatTime);
    if(!m_HDInfo->CreatHD(m_AudioFormat, HD_Out))
    {
        IDE_TRACE_STR("PlayFileErr");
        return;
    }
    if(m_HDInfo->m_AudioOutput == 0)
    {
        IDE_TRACE_STR("PlayFileErr");
        return;
    }
    connect(m_HDInfo->m_AudioOutput, SIGNAL(notify()),this, SLOT(slot_notify()));
    switch(m_ObjectType)
    {
        case Object_File:
            if(!m_FileInfo->m_Src.isOpen())
            {
                IDE_TRACE_STR("PlayFileErr");
                return;
            }
            m_Playing = true;
            m_HDInfo->m_AudioOutput->start(&m_FileInfo->m_Src);
            break;
        case Object_Mem:
            m_Playing = true;
            m_Buffer.close();
            m_Buffer.setBuffer(&m_ByteArray);
            m_Buffer.open(QIODevice::ReadOnly);
            m_HDInfo->m_AudioOutput->start(&m_Buffer);
            break;
    }
}

void baseAudiocodec::slot_readyRead()
{
    static quint64 dataLenth = 0;
    qint64 bytesReady = 0;
    qint64 bytesSpace = 0;
    qint64 bytesToRead = 0;
    qint64 bytesRead = 0;
    if(m_IODevice == 0)
        return;
    if(!m_IODevice->isOpen())
        return;
    if(m_HDInfo->m_AudioInput == 0)
        return;
    bytesReady = m_HDInfo->m_AudioInput->bytesReady();
    bytesSpace = m_ByteArray.size() - dataLenth;
    bytesToRead = qMin(bytesReady, bytesSpace);
    bytesRead = m_IODevice->read(m_ByteArray.data() + dataLenth, bytesToRead);
    if (bytesRead)
        dataLenth += bytesRead;
    if(m_ByteArray.size() == dataLenth)
    {
        m_HDInfo->m_AudioInput->stop();
        dataLenth = 0;
    }
}

void baseAudiocodec::slot_notify()
{
    qint64 curFormatLenth = 0;
    qint64 sampleStartPoint = 0;
    switch(m_ObjectType)
    {
        case Object_File:
        {
            curFormatLenth = m_HDInfo->FormatToLenth(m_AudioFormat, m_HDInfo->m_AudioOutput->processedUSecs());
            sampleStartPoint = curFormatLenth - m_UnitFormatLength;
            if((sampleStartPoint >= 0)&&(curFormatLenth <= m_FileInfo->m_SrcCopy.size()))
            {
                if (m_FileInfo->m_SrcCopy.seek(sampleStartPoint + m_FileInfo->m_HeadLenth))
                {
                    m_ByteArray.resize(m_UnitFormatLength);
                    m_FileInfo->m_SrcCopy.read(m_ByteArray.data(), m_UnitFormatLength);
                    CalculateTFD(sampleStartPoint, m_UnitFormatLength);
                }
            }
            break;
        }
        case Object_Mem:
        {
            if(m_Playing)
                curFormatLenth = m_HDInfo->FormatToLenth(m_AudioFormat, m_HDInfo->m_AudioOutput->processedUSecs());
            else
                curFormatLenth = m_HDInfo->FormatToLenth(m_AudioFormat, m_HDInfo->m_AudioInput->processedUSecs());
            sampleStartPoint = curFormatLenth - m_UnitFormatLength;
            if(sampleStartPoint<=0)
                return;
            CalculateTFD(sampleStartPoint, m_UnitFormatLength);
            break;
        }
    }
}

void baseAudiocodec::CalculateTFD(qint64 position, qint64 length)
{
    qreal curValue = 0.0;
    qreal peakLevel = 0.0;
    qreal sum = 0.0;
    qint64 bufIndex = 0;
    switch(m_ObjectType)
    {
        case Object_File:bufIndex = 0;break;
        case Object_Mem:bufIndex = position;break;
    }
    const char *ptr = m_ByteArray.constData() + bufIndex;
    const char *const end = ptr + length;
    Complex_Array *tmpData = new Complex_Array[16];
    Complex_Array *tmpResult = new Complex_Array[16];
    int arrayID = 0,cnt = 0;
    int sampleInterval = length/2/16;
    for(;ptr<end;ptr+=2)
    {
        const qint16 value = *reinterpret_cast<const qint16*>(ptr);
//>@m_AudioFormat.sampleSize() = 16;Ë«×Ö½Ú±íÊ¾Õñ·ù(32767)
        curValue = (qreal)value / 32767;
        peakLevel = qMax(peakLevel, curValue);
        sum += curValue * curValue;
        cnt += 2;
        if((cnt%sampleInterval) == 0)
        {
            tmpData[arrayID].Real = curValue;
            tmpData[arrayID].Imag = 0;
            arrayID++;
            if(arrayID >= 15)
                arrayID = 15;
        }
    }
    emit sFFT1D(tmpData,tmpResult,4);
    const int numSamples = length / 2;
    qreal rmsLevel = sqrt(sum / numSamples);
    rmsLevel = qMax(qreal(0.0), rmsLevel);
    rmsLevel = qMin(qreal(1.0), rmsLevel);
    emit sLevel(rmsLevel,peakLevel,position);
}

void baseAudiocodec::slot_FDComplexArray(Complex_Array* pFD, quint32 pNum)
{
    double Rms[pNum];
    for(int i=0;i<pNum;i++)
    {
        Rms[i] =sqrt(pFD[i].Real*pFD[i].Real+pFD[i].Imag*pFD[i].Imag);
    }
    emit sFD(Rms, pNum, 100);
}

void baseAudiocodec::slot_TDComplexArray(Complex_Array* pTD, quint32 pNum)
{
    qDebug()<<"ooo"<<pTD->Real<<"---"<<pTD->Imag;
}










