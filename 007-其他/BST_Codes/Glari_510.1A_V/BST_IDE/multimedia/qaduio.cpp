#include "qaduio.h"

qaduio::qaduio(QObject *parent) :
    QObject(parent)
{
    m_AudioOutput = 0;
    m_CurAudio = 0;
    m_Mutex = new QMutex( QMutex::Recursive );
}

qaduio::~qaduio()
{
    if(m_AudioOutput)
    {
        disconnect(m_AudioOutput, SIGNAL(stateChanged(QAudio::State)), 0, 0);
        m_AudioOutput->stop();
        m_AudioOutput->deleteLater();
        m_AudioOutput = 0;
    }
    ReleaseAuidoBuffer();
    if(m_Mutex)
        delete m_Mutex;
}

void qaduio::ReleaseAuidoBuffer()
{
    m_Mutex->lock();
    if(m_CurAudio)
    {
        if(m_CurAudio->mFile.isOpen())
            m_CurAudio->mFile.close();
        delete m_CurAudio;
        m_CurAudio = 0;
    }
    for(int i=m_AudioBuffer.count()-1;i>=0;i--)
    {
        AUDIO_INFO* tmpAudio = m_AudioBuffer.at(i);
        if(tmpAudio)
        {
            if(tmpAudio->mFile.isOpen())
                tmpAudio->mFile.close();
            delete tmpAudio;
        }
    }
    m_AudioBuffer.clear();
    m_Mutex->unlock();
}

void qaduio::slot_Play(QString pPath, QString pPara, bool pRepeat, bool pPrior)
{
    //>@
    if(pPrior)
    {
        ReleaseAuidoBuffer();
        if(m_AudioOutput)
        {
            disconnect(m_AudioOutput, SIGNAL(stateChanged(QAudio::State)), 0, 0);
            m_AudioOutput->stop();
        }
    }

    m_Mutex->lock();
    AUDIO_INFO *tmpAudio = new AUDIO_INFO;
    tmpAudio->mPath = pPath;
    tmpAudio->mRepeat = pRepeat;

    tmpAudio->mFile.setFileName(pPath);
    if(tmpAudio->mFile.open(QIODevice::ReadOnly | QIODevice::Truncate) == false)
    {
        delete tmpAudio;
        m_Mutex->unlock();
        return;
    }

    QByteArray tempBlock,tempInfoBlock;
    tempBlock.clear();
    tempInfoBlock.clear();

    tempBlock = tmpAudio->mFile.read(38);
    if(tempBlock.count() != 38)
    {
        delete tmpAudio;
        m_Mutex->unlock();
        return;
    }

    tempInfoBlock = tempBlock.mid(24,4);
    quint32 FrequencyIntHH =  (quint8)tempInfoBlock.at(3);
    quint32 FrequencyIntHL =  (quint8)tempInfoBlock.at(2);
    quint32 FrequencyIntLH =  (quint8)tempInfoBlock.at(1);
    quint32 FrequencyIntLL =  (quint8)tempInfoBlock.at(0);
    quint32 FrequencyInt = (FrequencyIntHH<<24)+(FrequencyIntHL<<16)+(FrequencyIntLH<<8)+FrequencyIntLL;
    IDE_TRACE_INT(FrequencyInt);
    tmpAudio->mFormat.setFrequency(FrequencyInt);
    tempInfoBlock.clear();

    tempInfoBlock = tempBlock.mid(22,2);
    quint16 ChannelsIntH =  (quint8)tempInfoBlock.at(1);
    quint16 ChannelsIntL =  (quint8)tempInfoBlock.at(0);
    quint16 ChannelsInt = (ChannelsIntH<<8)+ChannelsIntL;
    IDE_TRACE_INT(ChannelsInt);
    tmpAudio->mFormat.setChannels(ChannelsInt);
    tempInfoBlock.clear();

    tempInfoBlock = tempBlock.mid(34,2);
    quint16 SampleSizeIntH =  (quint8)tempInfoBlock.at(1);
    quint16 SampleSizeIntL =  (quint8)tempInfoBlock.at(0);
    quint16 SampleSizeInt = (SampleSizeIntH<<8)+SampleSizeIntL;
    IDE_TRACE_INT(SampleSizeInt);
    tmpAudio->mFormat.setSampleSize(SampleSizeInt);
    tempInfoBlock.clear();

    tmpAudio->mFormat.setCodec("audio/pcm");
    tmpAudio->mFormat.setByteOrder(QAudioFormat::LittleEndian);
    tmpAudio->mFormat.setSampleType(QAudioFormat::SignedInt);
    tempBlock.clear();

    m_AudioBuffer.append(tmpAudio);
    m_Mutex->unlock();

    //>@
    slot_PlayNext();
}

void qaduio::slot_PlayNext()
{
    if(m_AudioOutput && m_AudioOutput->state() == QAudio::ActiveState)
    {
        IDE_TRACE();
        return;
    }
    if(m_AudioBuffer.isEmpty() == false)
    {
        if(m_AudioOutput)
        {
            disconnect(m_AudioOutput, SIGNAL(stateChanged(QAudio::State)), 0, 0);
            m_AudioOutput->stop();
            m_AudioOutput->deleteLater();
            m_AudioOutput = 0;
        }
        m_Mutex->lock();
        m_CurAudio = m_AudioBuffer.takeFirst();
        if(m_CurAudio)
        {
            m_AudioOutput = new QAudioOutput(m_CurAudio->mFormat);
            if(m_AudioOutput)
            {
                m_AudioOutput->setBufferSize(51200);
                connect(m_AudioOutput,SIGNAL(stateChanged(QAudio::State)),this,SLOT(slot_StateChanged(QAudio::State)));
                if(m_CurAudio && m_CurAudio->mFile.isOpen())
                {
                    if(m_CurAudio->mFile.seek(44))
                    {
                        m_AudioOutput->start(&m_CurAudio->mFile);
                        IDE_TRACE_STR(m_CurAudio->mPath);
                    }
                }
            }
        }
        m_Mutex->unlock();
    }
}

void qaduio::SetVolume(int pValue)
{
    m_Mutex->lock();
    if(m_CurAudio)
    {

    }
    m_Mutex->unlock();
}

void qaduio::slot_SetVoiceEnable(bool pEnale)
{
    m_Mutex->lock();
    if(m_CurAudio)
    {

    }
    m_Mutex->unlock();
}

void qaduio::slot_Repeat(bool pRepeat, quint32 pInterval)
{
    m_Mutex->lock();
    if(m_CurAudio)
    {
        m_CurAudio->mRepeat = pRepeat;
    }
    m_Mutex->unlock();
}

void qaduio::slot_Pause()
{
    m_Mutex->lock();
    if(m_AudioOutput/* && m_AudioOutput->state() == QAudio::ActiveState*/)
        m_AudioOutput->suspend();
    m_Mutex->unlock();
}

void qaduio::slot_Resume()
{
    m_Mutex->lock();
    if(m_AudioOutput /*&& m_AudioOutput->state() == QAudio::SuspendedState*/)
        m_AudioOutput->resume();
    m_Mutex->unlock();
}

void qaduio::slot_Stop()
{
    m_Mutex->lock();
    if(m_AudioOutput /*&& m_AudioOutput->state() != QAudio::StoppedState*/)
    {
        disconnect(m_AudioOutput,SIGNAL(stateChanged(QAudio::State)),0,0);
        m_AudioOutput->stop();
    }
    if(m_CurAudio)
    {
        delete m_CurAudio;
        m_CurAudio = 0;
    }
    m_Mutex->unlock();
}

void qaduio::slot_StateChanged(QAudio::State pState)
{
    switch(pState)
    {
        case QAudio::IdleState:
            if(m_CurAudio && m_CurAudio->mRepeat)
            {
                m_Mutex->lock();
                if(m_CurAudio && m_CurAudio->mFile.seek(44))
                {
                    if(m_AudioOutput)
                    {
                        m_AudioOutput->start(&m_CurAudio->mFile);
                        IDE_TRACE_STR(m_CurAudio->mPath);
                    }
                }
                m_Mutex->unlock();
                break;
            }
            else
            {
                //emit sFinished();
                slot_Stop();
                slot_PlayNext();
            }
            break;
        case QAudio::ActiveState:
            break;
        case QAudio::StoppedState:
            if (m_AudioOutput->error() != QAudio::NoError)
            {
                // Perform error handling
                IDE_TRACE();
            }
            else
            {
                // Normal stop
                IDE_TRACE();
            }
            //emit sFinished();
            break;
        case QAudio::SuspendedState:
            break;
        default:
            break;
    }
}
