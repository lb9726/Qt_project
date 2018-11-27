//File   : wav_parser.c
//Author : Loon <sepnic@gmail.com>   
  
#include <assert.h>   
#include <stdio.h>   
#include <stdlib.h>   
//#include <unistd.h>
#include <fcntl.h>   
#include "wav_parser.h"   

#define WAV_PRINT_MSG   
  
char *WAV_P_FmtString(uint16_t fmt)  
{  
    switch (fmt) {  
    case WAV_FMT_PCM:  
        return "PCM";  
        break;  
    case WAV_FMT_IEEE_FLOAT:  
        return "IEEE FLOAT";  
        break;  
    case WAV_FMT_DOLBY_AC3_SPDIF:  
        return "DOLBY AC3 SPDIF";  
        break;  
    case WAV_FMT_EXTENSIBLE:  
        return "EXTENSIBLE";  
        break;  
    default:  
        break;  
    }  
    return "NON Support Fmt";  
}  
  
void WAV_P_PrintHeader(WAVContainer_t *container)  
{  
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");  
    printf("\n");  
      
    printf("File Magic:         [%c%c%c%c]\n",   
            (char)(container->header.magic),
            (char)(container->header.magic>>8),
            (char)(container->header.magic>>16),
            (char)(container->header.magic>>24));
    printf("File Length:        [%d]\n", container->header.length);  
    printf("File Type:          [%c%c%c%c]\n",  
            (char)(container->header.type),
            (char)(container->header.type>>8),
            (char)(container->header.type>>16),
            (char)(container->header.type>>24));
    printf("\n");  
  
    printf("Fmt Magic:          [%c%c%c%c]\n",  
            (char)(container->format.magic),
            (char)(container->format.magic>>8),
            (char)(container->format.magic>>16),
            (char)(container->format.magic>>24));
    printf("Fmt Size:           [%d]\n", container->format.fmt_size);  
    printf("Fmt Format:         [%s]\n", WAV_P_FmtString(container->format.format));  
    printf("Fmt Channels:       [%d]\n", container->format.channels);  
    printf("Fmt Sample_rate:    [%d](HZ)\n", container->format.sample_rate);  
    printf("Fmt Bytes_p_second: [%d]\n", container->format.bytes_p_second);  
    printf("Fmt Blocks_align:   [%d]\n", container->format.blocks_align);  
    printf("Fmt Sample_length:  [%d]\n", container->format.sample_length);    
    printf("\n");  
    printf("Chunk Type:         [%c%c%c%c]\n",  
            (char)(container->chunk.type),
            (char)(container->chunk.type>>8),
            (char)(container->chunk.type>>16),
            (char)(container->chunk.type>>24));
    printf("Chunk Length:       [%d]\n", container->chunk.length);  
      
    printf("\n");  
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");  
}  
  
int WAV_P_CheckValid(WAVContainer_t *container)  
{  
    if (container->header.magic != WAV_RIFF ||  
        container->header.type != WAV_WAVE ||  
        container->format.magic != WAV_FMT ||  
        container->format.fmt_size != LE_INT(16) ||  
        (container->format.channels != LE_SHORT(1) && container->format.channels != LE_SHORT(2)) ||  
        container->chunk.type != WAV_DATA)
    {
        fprintf(stderr, "non standard wav file.\n");  
        return -1;  
    }  
    return 0;  
}  
  
int WAV_ReadHeaders(int fd, WAVContainer_t *container, WAVContainer_t *container_back)  
{  
    //assert((fd >=0) && container);
#ifdef GCC
    if (read(fd, &container->header, sizeof(container->header)) != sizeof(container->header) ||   
        read(fd, &container->format, sizeof(container->format)) != sizeof(container->format) ||  
        read(fd, &container->chunk, sizeof(container->chunk)) != sizeof(container->chunk))
#endif
    {
        fprintf(stderr, "Error WAV_ReadHeader\n");  
        return -1;  
    }  
    if (WAV_P_CheckValid(container) < 0)  
        return -1;
  
#ifdef WAV_PRINT_MSG   
    WAV_P_PrintHeader(container);  
#endif   

	if(container->format.sample_rate != container_back->format.sample_rate || 
		container->format.sample_length != container_back->format.sample_length ||
		container->format.channels != container_back->format.channels)
	{
		memcpy(container_back, container, sizeof(WAVContainer_t));
		return 1;  
	}  

    return 0;  
}

int WAV_ReadHeader(int fd, WAVContainer_t *container)  
{  
    if(!fd)
    {
        fprintf(stderr, "Error File\n");
        return -1;
    }
#ifdef GCC
    if (read(fd, &container->header, sizeof(container->header)) != sizeof(container->header) ||   
        read(fd, &container->format, sizeof(container->format)) != sizeof(container->format) ||  
        read(fd, &container->chunk, sizeof(container->chunk)) != sizeof(container->chunk))
#endif
    {
        fprintf(stderr, "Error WAV_ReadHeader\n");  
        return -1;  
    }  
//    if (WAV_P_CheckValid(container) < 0)
//        return -1;

    return 0;  
}  

int WAV_ReadHeaderF(FILE *fd, WAVContainer_t *container)
{
    if(!fd)
    {
        return -1;
    }
#ifdef GCC
    if (fread(&container->header, 1, sizeof(container->header), fd) != sizeof(container->header) ||
        fread(&container->format, 1, sizeof(container->format), fd) != sizeof(container->format) ||
        fread(&container->chunk, 1, sizeof(container->chunk), fd) != sizeof(container->chunk))
#endif
    {
        fprintf(stderr, "Error WAV_ReadHeader\n");
        return -1;
    }
//    if (WAV_P_CheckValid(container) < 0)
//        return -1;

    return 0;
}

qint32 GetWavTime(QString tmpAudio)
{
#ifdef GCC
    QFile tmpFile(tmpAudio);
    if(tmpFile.open(QIODevice::ReadOnly))
    {
        if(!tmpFile.seek(sizeof(WAVHeader_t)))
        {
            IDE_DEBUG("Error Seek");
            return -1;
        }
        int size = sizeof(WAVFmt_t);
        WAVFmt_t *tmpInfo = (WAVFmt_t*)malloc(size);
        if (read(tmpFile.handle(), tmpInfo, size) != size)
        {
            IDE_DEBUG("Error Format");
            return -1;
        }
        qint32 msInBuffer = (qint32)(1000) * tmpFile.size() / (tmpInfo->channels * tmpInfo->sample_length / 8) / tmpInfo->sample_rate;
        //IDE_TRACE_INT(msInBuffer);
        free(tmpInfo);
        tmpFile.close();
        return msInBuffer;
    }
#endif
    return -1;
}
  
int WAV_WriteHeader(int fd, WAVContainer_t *container)  
{  
#ifdef GCC
    assert((fd >=0) && container);  
    if (WAV_P_CheckValid(container) < 0)  
        return -1;  
    if (write(fd, &container->header, sizeof(container->header)) != sizeof(container->header) ||   
        write(fd, &container->format, sizeof(container->format)) != sizeof(container->format) ||  
        write(fd, &container->chunk, sizeof(container->chunk)) != sizeof(container->chunk))
    {
        fprintf(stderr, "Error WAV_WriteHeader\n");  
        return -1;  
    }  
#ifdef WAV_PRINT_MSG   
    WAV_P_PrintHeader(container);  
#endif   
#endif
    return 0;  
}

QString PCM_TO_WAV(QString pFile)  ///xx.raw  ->  xx.wav
{
    QFileInfo tmpFileInfo(pFile);
    if(!tmpFileInfo.exists())
        return QString();
    QString tmpPath = tmpFileInfo.path();
    if(!tmpPath.endsWith('/'))
        tmpPath.append('/');
    QString tmpName = tmpFileInfo.baseName();
    QString tmpFile = tmpPath + tmpName + ".wav";

    //转换文件为wav
    HEADER pcmHEADER;
    FMT pcmFMT;
    DATA pcmDATA;
    unsigned long m_pcmData;
    FILE *fp, *fpCopy;
    fp = fopen(pFile.toLatin1().data(), "rb");
    fpCopy = fopen(tmpFile.toLatin1().data(), "wb+");
    if(!fp)
    {
        //qDebug() << "open pcm file error";
        return QString();
    }
    if(!fpCopy)
    {
        //qDebug() << "create wave file error";
        return QString();
    }
    qstrcpy(pcmHEADER.fccID, "RIFF");
    qstrcpy(pcmHEADER.fccType, "WAVE");
    fseek(fpCopy, sizeof(HEADER), 1);
    pcmFMT.dwSamplesPerSec = 8000;
    pcmFMT.dwAvgBytesPerSec = pcmFMT.dwSamplesPerSec*sizeof(m_pcmData);
    pcmFMT.uiBitsPerSample = 16;
    qstrcpy(pcmFMT.fccID, "fmt ");
    pcmFMT.dwSize = 16;
    pcmFMT.wBlockAlign = 4;
    pcmFMT.wChannels = 2;
    pcmFMT.wFormatTag = 1;
    fwrite(&pcmFMT, sizeof(FMT), 1, fpCopy);
    qstrcpy(pcmDATA.fccID, "data");
    pcmDATA.dwSize = 0;
    fseek(fpCopy, sizeof(DATA), 1);
    fread(&m_pcmData, sizeof(unsigned long), 1, fp);
    while(!feof(fp))
    {
        pcmDATA.dwSize += 4; //计算数据的长度；每读入一个数据，长度就加一；
        fwrite(&m_pcmData, sizeof(unsigned long), 1, fpCopy); //将数据写入.wav文件;
        fread(&m_pcmData, sizeof(unsigned long), 1, fp); //从.pcm中读入数据
    }
    fclose(fp);
    pcmHEADER.dwSize = 44+pcmDATA.dwSize-8; //根据pcmDATA.dwsize得出pcmHEADER.dwsize的值
    rewind(fpCopy); //将fpCpy变为.wav的头，以便于写入HEADER和DATA;
    fwrite(&pcmHEADER, sizeof(HEADER), 1, fpCopy); //写入HEADER
    fseek(fpCopy, sizeof(FMT), 1);  //跳过FMT,因为FMT已经写入
    fwrite(&pcmDATA, sizeof(DATA), 1, fpCopy);   //写入DATA;
    fclose(fpCopy);
    return tmpFile;
}


