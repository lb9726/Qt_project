#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <locale.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <alsa/asoundlib.h>
#include <assert.h>
#include <sys/mman.h>
#include "wav_parser.h"
#include "sndwav_common.h"

static WAVStatus_t Playstatus;


ssize_t SNDWAV_P_SaveRead(int fd, void *buf, size_t count)
{
    ssize_t result = 0, res;
    while (count > 0)
    {
        if ((res = read(fd, buf, count)) == 0)
            break;
        if (res < 0)
            return result > 0 ? result : res;
        count -= res;
        result += res;
        buf = (char *)buf + res;
    }
    return result;
}

void SNDWAV_Play(SNDPCMContainer_t *sndpcm, WAVContainer_t *wav, int fd)  
{  
    int load, ret, fp;
	WAVContainer_t wav_check;
    off64_t written = 0;  
    off64_t c;  
    off64_t count = LE_INT(wav->chunk.length);  
    load = 0;  
    while (written < count)
    {
        /* Must read [chunk_bytes] bytes data enough. */  
        do {  
            c = count - written;  
            if (c > sndpcm->chunk_bytes)  
                c = sndpcm->chunk_bytes;  
            c -= load;  
  
            if (c == 0)  
                break;  
            ret = SNDWAV_P_SaveRead(fd, sndpcm->data_buf + load, c);  
            if (ret < 0) {  
                fprintf(stderr, "Error safe_read/n");  
                exit(-1);  
            }  
            if (ret == 0)  
                break;  
            load += ret;  
        } while ((size_t)load < sndpcm->chunk_bytes);  
        if(!Playstatus.Play_voiceEnable)
			memset(sndpcm->data_buf, 0x0, sndpcm->chunk_bytes);

        if(Playstatus.Play_stop || Playstatus.Play_quit)
			break;

		if(Playstatus.Play_path!=NULL)
		{
			fp = open(Playstatus.Play_path, O_RDONLY);

			if(WAV_ReadHeader(fp, &wav_check) == 0)
			{
				close(fp);  
				break;
			}
			else
			{
				Playstatus.Play_path = NULL;
				close(fp);  
			}

		}

		if(Playstatus.Play_pause)
		{
			snd_pcm_drop(sndpcm->handle);
			snd_pcm_prepare(sndpcm->handle);

			while(1) 
			{
				if(!Playstatus.Play_pause || (Playstatus.Play_path != NULL) || Playstatus.Play_stop || Playstatus.Play_quit)
				{
					Playstatus.Play_pause = false;
					break;
				}

				usleep(10000);
			}
		}
        /* Transfer to size frame */  
        load = load * 8 / sndpcm->bits_per_frame; 
        ret = SNDWAV_WritePcm(sndpcm, load);  
        if (ret != load)  
            break;  
        ret = ret * sndpcm->bits_per_frame / 8;  
        written += ret;  
        load = 0;
    } 
 	if(Playstatus.Play_path==NULL)
        Playstatus.Play_finished = true; 
}

int Alsa_init(void)
{
	int fd,config_flag;
	WAVContainer_t wav,wav_back;  
    SNDPCMContainer_t playback; 

	Playstatus.Play_voiceEnable = true;
	Playstatus.Play_stop = false;
	Playstatus.Play_finished = false;
	Playstatus.Play_pause = false;
	Playstatus.Play_quit = false;
	Playstatus.Play_path = NULL;
	
    memset(&playback, 0x0, sizeof(playback));
#if defined(PLAYBACK_LOG)
    if (snd_output_stdio_attach(&playback.log, stderr, 0) < 0)
    {
		fprintf(stderr, "Error snd_output_stdio_attach/n");  
		goto Errs;  
	}  
#endif
    if (snd_pcm_open(&playback.handle, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0)
    {
		fprintf(stderr, "Error snd_pcm_open [ %s]/n", "default");  
		goto Errs;  
	}   

#if defined(PLAYBACK_LOG)
	snd_pcm_dump(playback.handle, playback.log);
#endif

	while(1)
	{

		if(Playstatus.Play_stop)
		{
		    Playstatus.Play_stop = false;
			snd_pcm_drop(playback.handle);
			snd_pcm_prepare(playback.handle);
			close(fd);
		}

		if(Playstatus.Play_path!=NULL)
		{
			fd = open(Playstatus.Play_path, O_RDONLY);

			Playstatus.Play_path = NULL;
			Playstatus.Play_finished = false;

		   	snd_pcm_drop(playback.handle);
			snd_pcm_prepare(playback.handle);

			if((config_flag = WAV_ReadHeaders(fd, &wav, &wav_back)) >= 0)
			{
				if(config_flag > 0)
				{
        			free(playback.data_buf);
					SNDWAV_SetParams(&playback, &wav);
				}

				SNDWAV_Play(&playback, &wav, fd);
			}

			close(fd);
		}

		if(Playstatus.Play_quit)
		{
		   	snd_pcm_drop(playback.handle);
			snd_pcm_prepare(playback.handle);
			close(fd);
			break;
		}

		usleep(10000);
	}

        free(playback.data_buf);

#ifdef PLAYBACK_LOG
        snd_output_close(playback.log);
#endif
        snd_pcm_close(playback.handle);

		printf("alsa_exit!!\n");
		return 1;

	Errs:
        if (playback.data_buf)
            free(playback.data_buf);
        if (playback.log)
            snd_output_close(playback.log);
        if (playback.handle)
            snd_pcm_close(playback.handle);
		return -1;
}

void Alsa_pause(void)
{
	Playstatus.Play_pause = true;
}

void Alsa_resume(void)
{
	Playstatus.Play_pause = false;
}

void Alsa_voiceEnable(bool en_voice)
{
	Playstatus.Play_voiceEnable = en_voice;
}

void Alsa_stop(void)
{
	Playstatus.Play_stop = true;
}

void Alsa_quit(void)
{
	Playstatus.Play_quit = true;
}

void Alsa_play(char  *path)
{
	strcpy(Playstatus.Play_path_backup,path); 
	Playstatus.Play_path = Playstatus.Play_path_backup;
}

bool Alsa_get_finished(void)
{
	return Playstatus.Play_finished;
}
