#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mad.h>
#include <alsa/asoundlib.h>
#include <stdbool.h>
#include "sndwav_common.h"   
#include "lmad.h"

SNDPCMContainer_t       playback;
WAVContainer_t          wav;
int                     first = 0;
bool                    m_VoiceEnable = true;
bool                    m_Stop = false;
bool                    m_Finished = false;
const char              *m_MusicPath = NULL;

struct buffer {
    unsigned char const *start;
    unsigned long length;
};

enum mad_flow gain_filter(void *data, struct mad_frame *frame)
{
    register mad_fixed_t gain = *(mad_fixed_t *) data;
    if (gain != MAD_F_ONE)
    {
        unsigned int nch, ch, ns, s, sb;
        nch = MAD_NCHANNELS(&frame->header);
        ns  = MAD_NSBSAMPLES(&frame->header);
        for (ch = 0; ch < nch; ++ch)
        {
            for (s = 0; s < ns; ++s)
            {
                for (sb = 0; sb < 32; ++sb)
                {
                    frame->sbsample[ch][s][sb] = mad_f_mul(frame->sbsample[ch][s][sb], gain);
                }
            }
        }
    }
    return MAD_FLOW_CONTINUE;
}

static inline signed int scale(mad_fixed_t sample)
{
    /* round */
    sample += (1L << (MAD_F_FRACBITS - 16));
    /* clip */
    if (sample >= MAD_F_ONE)
    sample = MAD_F_ONE - 1;
    else if (sample < -MAD_F_ONE)
    sample = -MAD_F_ONE;
    /* quantize */
    return sample >> (MAD_F_FRACBITS + 1 - 16);
}

static void audio_pcm  (unsigned int nsamples,
                        mad_fixed_t const *left_ch,
                        mad_fixed_t const *right_ch,
                        short *n_ptr, unsigned int channels)
{
    int sample,i=0;
    short bufer;
    while (nsamples--)
    {
		/* output sample(s) in 16-bit signed little-endian PCM */
		sample = scale(*left_ch++);
		bufer = sample & 0xffff;
		n_ptr[i++] = bufer;
        if (channels == 2)
        {
            sample = scale(*right_ch++);
            bufer=sample & 0xffff;
            n_ptr[i++]=bufer;
		}
	}
}

static void play(unsigned int len,char *ptr)
{
    unsigned int err;
    while (len > 0)
    {
        err = snd_pcm_writei(playback.handle, ptr, len);
		if (err == -EAGAIN)
			continue;
        if (err < 0)
        {
            if (xrun_recovery(playback.handle, err) < 0)
            {
				return -1;
			}
			break;
		}
		len -= err;
		ptr += err * 4;
    }
}

static enum mad_flow decode_filter(void *data, struct mad_stream const *stream,struct mad_frame *frame)
{
	mad_fixed_t gain;
    if(m_VoiceEnable)
		 return MAD_FLOW_CONTINUE;
	else
	{
		gain = mad_f_tofixed(pow(10, -175 / 20));
		return gain_filter(&gain, frame);
	}
}

static enum mad_flow input(void *data,struct mad_stream *stream)
{
    struct buffer *buffer = data;
    if (!buffer->length)
        return MAD_FLOW_STOP;
    mad_stream_buffer(stream, buffer->start, buffer->length);
    buffer->length = 0;
    return MAD_FLOW_CONTINUE;
}


static enum mad_flow output(void *data,struct mad_header const *header,struct mad_pcm *pcm)
{
    if(first==0)
	{
        first = 1;
        wav.format.format = WAV_FMT_PCM;
        wav.format.sample_length = 16;
        wav.format.channels = pcm->channels;
        wav.format.sample_rate = pcm->samplerate;
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
        if (SNDWAV_SetParams(&playback, &wav) < 0)
        {
			fprintf(stderr, "Error set_snd_pcm_params/n");  
			goto Errs;  
		} 

#if defined(PLAYBACK_LOG)
		snd_pcm_dump(playback.handle, playback.log);
#endif
	}
	audio_pcm(pcm->length, pcm->samples[0], pcm->samples[1], playback.data_buf, pcm->channels); 
	play(pcm->length, playback.data_buf);
	return MAD_FLOW_CONTINUE;
	Errs:
        if (playback.data_buf)
            free(playback.data_buf);
        if (playback.log)
            snd_output_close(playback.log);
        if (playback.handle)
            snd_pcm_close(playback.handle);
		exit(-1);
}

static enum mad_flow error(void *data,struct mad_stream *stream,struct mad_frame *frame)
{
    struct buffer *buffer = data;
    fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
            stream->error, mad_stream_errorstr(stream),
            stream->this_frame - buffer->start);
    /* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */
    return MAD_FLOW_CONTINUE;
}

int decode(unsigned char const *start, unsigned long length)
{
    struct buffer buffer;
    struct mad_decoder decoder;
    int result;
    /* initialize our private message structure */
    buffer.start  = start;
    buffer.length = length;
    /* configure input, output, and error functions */
    mad_decoder_init(&decoder, &buffer,
                    input, 0 /* header */, decode_filter /* filter */, output,
                    error, 0 /* message */);
    /* start decoding */
    result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);
    mad_decoder_finish(&decoder);
    snd_pcm_drain(playback.handle);
    free(playback.data_buf);
    #if defined(PLAYBACK_LOG)
    snd_output_close(playback.log);
    #endif
    snd_pcm_close(playback.handle);
    return result;
}
