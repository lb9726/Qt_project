#include "audio.h"

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


Audio::Audio(QObject *parent)
    :QIODevice(parent)
{
    AudioStatus = e_IdleState;
}

qint64 Audio::readData(char *data, qint64 len)
{
    qint64 total = 0;
    while (len - total > 0) {
        const qint64 chunk = qMin((m_buffer.size() - m_pos), len - total);
        memcpy(data + total, m_buffer.constData() + m_pos, chunk);
        m_pos = (m_pos + chunk) % m_buffer.size();
        total += chunk;
    }
    return total;
}

qint64 Audio::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);
    return 0;
}

qint64 Audio::bytesAvailable() const
{
    return m_buffer.size() + QIODevice::bytesAvailable();
}

void Audio::PraseFile(QByteArray Block,QAudioFormat &FileFormat)
{
    QByteArray tempBlock;
    tempBlock = Block.mid(24,4);
    quint32 FrequencyIntHH =  (quint16)tempBlock.at(3);
    quint32 FrequencyIntHL =  (quint16)tempBlock.at(2);
    quint32 FrequencyIntLH =  (quint16)tempBlock.at(1);
    quint32 FrequencyIntLL =  (quint16)tempBlock.at(0);
    quint32 FrequencyInt = (FrequencyIntHH<<24)+(FrequencyIntHL<<16)+(FrequencyIntLH<<8)+FrequencyIntLL;
    IDE_TRACE_INT(FrequencyInt);
    FileFormat.setFrequency(FrequencyInt);
    tempBlock.resize(0);
    tempBlock = Block.mid(22,2);
    quint16 ChannelsIntH =  (quint16)tempBlock.at(1);
    quint16 ChannelsIntL =  (quint16)tempBlock.at(0);
    quint16 ChannelsInt = (ChannelsIntH<<8)+ChannelsIntL;
    IDE_TRACE_INT(ChannelsInt);
    FileFormat.setChannels(ChannelsInt);
    tempBlock.resize(0);
    tempBlock = Block.mid(34,2);
    quint16 SampleSizeIntH =  (quint16)tempBlock.at(1);
    quint16 SampleSizeIntL =  (quint16)tempBlock.at(0);
    quint16 SampleSizeInt = (SampleSizeIntH<<8)+SampleSizeIntL;
    IDE_TRACE_INT(SampleSizeInt);
    FileFormat.setSampleSize(SampleSizeInt);
    tempBlock.resize(0);
    FileFormat.setCodec("audio/pcm");
    FileFormat.setByteOrder(QAudioFormat::LittleEndian);
    FileFormat.setSampleType(QAudioFormat::SignedInt);
}

void Audio::startPlaying(QString FileDir)
{
    inputFile.setFileName(FileDir);
    inputFile.open(QIODevice::ReadOnly);
    QAudioFormat format;
    QByteArray tempBlock = inputFile.read(38);
    PraseFile(tempBlock,format);
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format))
    {
        QString tempWarning = "Warning! The format does not support,System will try to find support format automatically";
        IDE_TRACE_STR(tempWarning);
        format = info.nearestFormat(format);
    }
//    foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
//            qDebug() << "Device name: " << deviceInfo.deviceName();
    audioOutput = new QAudioOutput(format, this);
    audioOutput->setBufferSize(51200);
    connect(audioOutput,SIGNAL(stateChanged(QAudio::State)),SLOT(slot_stateChanged(QAudio::State)));
    audioOutput->start(&inputFile);
    IDE_TRACE_INT(audioOutput->bufferSize());
    IDE_TRACE_INT(audioOutput->error());
}

void Audio::stop()
{
//    IDE_TRACE_INT(AudioStatus);
    if(AudioStatus!=e_ActiveState)
    {
        return;
    }
    audioOutput->stop();
    inputFile.close();
    delete audioOutput;
}

void Audio::slot_stateChanged(QAudio::State state)
{
    switch(state)
    {
        case QAudio::IdleState:
            audioOutput->stop();
            inputFile.close();
            delete audioOutput;
            AudioStatus = e_IdleState;
            break;
        case QAudio::ActiveState:
            AudioStatus = e_ActiveState;
            break;
        case QAudio::StoppedState:
            AudioStatus = e_StoppedState;
            break;
        case QAudio::SuspendedState:
            AudioStatus = e_SuspendedState;
            break;
    }
//    IDE_TRACE_INT(AudioStatus);
}
