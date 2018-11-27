#include "mp3_parser.h"

int GetMp3BitRate(int bRateIndex, int LayerDescript, int Version)
{
    int BitrateTable[6][15]= {
        {-1,32,64,96,128,160,192,224,256,288,320,352,384,416,448},
        {-1,32,48,56,64,80,96,112,128,160,192,224,256,320,384},
        {-1,32,40,48,56,64,80,96,112,128,160,192,224,256,320},
        {-1,32,64,96,128,160,192,2324,256,288,320,352,384,416,448},
        {-1,32,48,56,64,80,96,112,128,160,192,224,256,320,384},
        {-1,8,16,24,32,64,80,56,64,128,160,112,128,256,320}
    };         //kbps       (-1)   means   :free
    int   i,j;
    if(Version==3 && LayerDescript==3)
        j=0;
    else if(Version==3 && LayerDescript==2)
        j=1;
    else if(Version==3 && LayerDescript==1)
        j=2;
    else if(Version==2 && LayerDescript==1)
        j=3;
    else if(Version==2 && LayerDescript==2)
        j=4;
    else if(Version==2 && LayerDescript==3)
        j=5;

    i=BitrateTable[j][bRateIndex];
    return   i;
}


//>@mp3文件信息： http://blog.csdn.net/sunshine1314/article/details/2514322
//>@返回毫秒
int GetMp3Time(QString tmpAudio)
{
    FILE *fp;
    int   bHasID3V2;
    char  cVer;
    char  cRevision;
    int   bHasExtHeader=0;
    int   ID3V2_size;

    char   cID3V2_head[10];
    char   cID3v2Frame_head[10];
    //char  *cID3v2Frame;
    qint64   iID3v2Frame_length;

    /*##########################Symbol   In   Frame###################################*/

    qint64   iFrameStart;
    char   cFrameHead_bin[33];

    /*##########################Symbol   In   ID3V1###################################*/

    //char   cID3V1[128];

    /*##########################Symbol   Above######################################*/
    unsigned char*   cBuffer;
    int   SamplingrateTable[3][3]={{44100,22050,11025},{48000,24000,120000},{32000,16000,8000}};
    //char* ChannelDescrip[4]={"stereo","joint stereo","dual mono","mono"};

    int    cBuffer_size=1024, Bitrate, FrameSize, FrameCount, mp3Duration = -1;
    char   LayerDescript   ,bRateIndex   ,bSampleRate,Version,bPadding,bChannelMode;
    qint64   flength;

    int   i=0,j=0;

    /*##########################Symbol   Above######################################*/

    if((fp=fopen(tmpAudio.toLatin1().data(),"rb"))==NULL)
    {
        IDE_TRACE();
        return -1;
    }

    /*##########################ID3V2   Reading#####################################*/

    fread(cID3V2_head,10,1,fp);
    if((cID3V2_head[0]=='I'||cID3V2_head[1]=='i') && (cID3V2_head[1]=='D'||cID3V2_head[2]=='d') && cID3V2_head[2]=='3')
    {
        bHasID3V2=1;
        cVer=cID3V2_head[3];
        cRevision=cID3V2_head[4];

        if(cID3V2_head[5]&0x64==0x64)
            bHasExtHeader   =   1;
        if(cID3V2_head[5]&0x10==0x10)
            bHasExtHeader   =   1;

        //ID3V2_size   =   (cID3V2_head[6]&0x7F)*0x200000
        //+   (cID3V2_head[7]&0x7F)*0x400
        //+   (cID3V2_head[8]&0x7F)*0x80
        //+   (cID3V2_head[9]&0x7F);
        ID3V2_size= (cID3V2_head[6]&0x7F) << 21 | (cID3V2_head[7]&0x7F) << 14 | (cID3V2_head[8]&0x7F) << 7 | (cID3V2_head[9]&0x7F) + 10 ;
        //IDE_TRACE_INT(ID3V2_size); //>@bytes
        iFrameStart=--ID3V2_size;
    }
    else
    {
        bHasID3V2=0;
        //IDE_TRACE();  //>@There   is   no   ID3V2*
    }

    rewind(fp); /*改文件指针指向文件头*/
    fseek(fp,10L,1);

    while((ftell(fp)+10)<=ID3V2_size)
    {
        memset(cID3v2Frame_head,0,10); /*内存清空*/
        fread(cID3v2Frame_head,10,1,fp); /*ID3V2帧信息头读取*/

        iID3v2Frame_length = (quint64)((quint64)(cID3v2Frame_head[4])*(quint64)0x100000000LL + (quint64)(cID3v2Frame_head[5])*0x10000 + (quint64)(cID3v2Frame_head[6])*0x100 + cID3v2Frame_head[7]); /*计算帧的大小.*/

        if(cID3v2Frame_head[0]=='/0' && cID3v2Frame_head[1]=='/0' && cID3v2Frame_head[2]=='/0' && cID3v2Frame_head[3]=='/0')
            break;

//        for(i=0;i<4;i++)
//            putchar(cID3v2Frame_head[i]);     /*FrameSign(4)*/

//        cID3v2Frame = (char*)malloc(iID3v2Frame_length);  //为cID3v2Frame指针分配内存
//        if(cID3v2Frame!=NULL) //检测动态内存分配是否已经成功
//        {
//            fread(cID3v2Frame,iID3v2Frame_length,1,fp);
//            i=0;
//            while(i<iID3v2Frame_length)
//            {
//                putchar(cID3v2Frame[i]);
//                i++;
//            }
//            free(cID3v2Frame);
//        }
//        else
//        {
//            if(fp!=NULL)
//                fclose(fp);
//            IDE_TRACE(); //>@ERROR:Memory   Exhausted!
//            return -1;
//        }
    }

    /*(mp3_FrameHeader)   Reading*/
    fseek(fp, 0L, SEEK_END);   //移文件指针到文件尾
    flength = ftell(fp);   //文件长

    rewind(fp);
    fseek(fp,10+ID3V2_size,1);
    while((ftell(fp)+4)<flength)
    {
        if(ftell(fp)+1024<=flength)
            cBuffer_size=1024;
        else
            cBuffer_size=flength-ftell(fp);

        cBuffer = (unsigned char*)malloc(cBuffer_size);
        if(cBuffer==NULL) //   检测动态内存分配是否已经成功
        {
            if(fp!=NULL)
                fclose(fp);
            IDE_TRACE(); //>@ERROR:Memory   Exhausted!
            return -1;
        }
        memset(cBuffer,0,cBuffer_size);
        fread(cBuffer,cBuffer_size,1,fp);
        for(i=0; i<(cBuffer_size-4); i++)
        {
            //Mp3帧头(FRAMEHEADER)格式如下：AAAAAAAA   AAABBCCD   EEEEFFGH   IIJJKLMM
            LayerDescript = (cBuffer[i+1]&0x6)>>1; //Part CC IN 'AAABBCCD',Layer description '00' -  reserved

            //Part EEEE in 'EEEEFFGH', But '1111' is Bad Bitrate index, "bad" means that this is not an allowed value
            bRateIndex = cBuffer[i+2] / 0x10; //   EEEEXXXX!='1111'
            bSampleRate = (cBuffer[i+2] & 0xA) >> 2   ; //Part FF IN 'EEEEFFGH','11' reserved

            if(cBuffer[i] == 0xFF && cBuffer[i+1] > 0xE0 && bRateIndex != 0xF && LayerDescript != 0x0 && bSampleRate < 0x3)
            {
                //Mp3_FrameHeader
                memset(cFrameHead_bin,0,32);
                j=cBuffer[i];
                strcat(cFrameHead_bin,QString::number(j, 2).toLatin1().data());

                j=cBuffer[i+1];
                strcat(cFrameHead_bin,QString::number(j, 2).toLatin1().data());

                j=cBuffer[i+2];
                strcat(cFrameHead_bin,QString::number(j, 2).toLatin1().data());

                j=cBuffer[i+3];
                strcat(cFrameHead_bin,QString::number(j, 2).toLatin1().data());

                Version   =   (cBuffer[i+1]   &   0x18)>>3;
                bPadding=(cBuffer[i+1]   &   0x2)>>1;
                Bitrate   =   GetMp3BitRate(bRateIndex,   LayerDescript,   Version);
                bChannelMode=(cBuffer[i+3]   &   0xC0)>>6;

                if(bRateIndex != 0) //This  is not  a   Free   Rate   MP3   File
                {
                    switch(Version)
                    {
                        case 0:
                            //>@printf("MPEG 2.5 Layout %d, %dHz %s\n", 4-LayerDescript, SamplingrateTable[bSampleRate][2], ChannelDescrip[bChannelMode]);

                            FrameSize = ((72 * Bitrate*1000) / SamplingrateTable[bSampleRate][2]) + bPadding;
                            FrameCount=(int)((flength - (ftell(fp) - cBuffer_size+i)) / FrameSize);
                            mp3Duration=(int)(FrameCount*0.026);        //每帧的播放时间：无论帧长是多少，每帧的播放时间都是26ms；
                            //>@printf("CBR:%dKbit %d frames\n",Bitrate,FrameCount);
                            //>@printf("length:%d seconds\n",mp3Duration);
                            break;
                        case 2:
                            //>@printf("MPEG 2.0 Layout %d, %dHz %s\n",4-LayerDescript, SamplingrateTable[bSampleRate][1],ChannelDescrip[bChannelMode]);

                            FrameSize   =   ((72   *   Bitrate*1000)   /   SamplingrateTable[bSampleRate][1])   +

                            bPadding;
                            FrameCount=(int)((flength-(ftell(fp)-cBuffer_size+i))   /FrameSize);
                            mp3Duration=(int)(FrameCount*0.026);             //每帧的播放时间：无论帧长是多少，每帧的播放时间都是26ms；
                            //>@printf("CBR:%dKbit %d frames\n",Bitrate,FrameCount);
                            //>@printf("length:%d seconds",mp3Duration);
                            break;
                        case   3:
                            //>@printf("MPEG 1.0 Layout %d, %dHz %s\n",4-LayerDescript, SamplingrateTable[bSampleRate][0],ChannelDescrip[bChannelMode]);

                            FrameSize   =   ((144   *   Bitrate*1000)   /   SamplingrateTable[bSampleRate][0])   +

                            bPadding;
                            FrameCount=(int)((flength-(ftell(fp)-cBuffer_size+i))   /FrameSize);
                            mp3Duration=(int)(FrameCount*0.026);        //每帧的播放时间：无论帧长是多少，每帧的播放时间都是26ms；

                            //>@printf("CBR:%dKbit %d frames\n",Bitrate,FrameCount);
                            //>@printf("length:%d seconds",mp3Duration);
                            break;
                        default:
                            break;
                    }
                }

                cBuffer_size=-1;
                break;
            }
        }
        free(cBuffer);
        if(cBuffer_size == (-1))
            break;
    }

    /*ID3V1   Reading*/

//    fseek(fp,-128L,SEEK_END);
//    fread(cID3V1,ID3V1_size   ,1,fp);

//    if((cID3V1[0]=='T'||cID3V1[0]=='t') && (cID3V1[1]=='A'||cID3V1[1]=='a') && (cID3V1[2]=='G'||cID3V1[2]=='g'))
//    {
//        printf("\n*\nID3V1_size:%d bytes\n",ID3V1_size   );
//        for(i=0;i<128;i++)
//            putchar(cID3V1[i]);
//        putchar('|');
//        putchar('\n');
//    }
//    else
//    {
//        IDE_TRACE(); //There   is   no   ID3V1
//    }

    fclose(fp);

    mp3Duration *= 1000;
    IDE_TRACE_INT(mp3Duration);
    return mp3Duration;
}

