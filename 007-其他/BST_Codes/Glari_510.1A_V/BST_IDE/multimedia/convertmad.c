# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <sys/soundcard.h>
# include <sys/ioctl.h>
# include <sys/fcntl.h>
# include <sys/types.h>
# include <mad.h>
# include "wav.h"

struct buffer {
	unsigned char const *start;
	unsigned long length;
};

unsigned int length;
static int sfd;
static int decode(unsigned char const *, unsigned long);
static char *outfilename;
unsigned int dwSamplesPerSec;
unsigned short wChannels; 

struct RIFF_HEADER	riff_header;
struct FMT_BLOCK	fmt_block;
struct FACT_BLOCK	fact_block;
struct DATA_BLOCK	data_block;

void addheader(void)
{
    FILE *filefd;
    if((filefd = fopen(outfilename, "r+")) < 0)
    {
        printf("fopen error\n");
        return;
    }
    fseek(filefd, 0, SEEK_SET);

    /**** RIFF_HEADER ****/
    memcpy(riff_header.szRiffID, "RIFF", 4);
    riff_header.dwRiffSize = length - 8;
    memcpy(riff_header.szRiffFormat, "WAVE", 4);
    
    /**** FMT_BLOCK  WAVE_FORMAT ****/
    memcpy(fmt_block.szFmtID, "fmt ", 4);
    fmt_block.dwFmtSize = 16;
    fmt_block.wavFormat.wFormatTag = 1;
    fmt_block.wavFormat.wChannels = wChannels;
    fmt_block.wavFormat.dwSamplesPerSec = dwSamplesPerSec;
    fmt_block.wavFormat.dwAvgBytesPerSec = dwSamplesPerSec * 4;
    fmt_block.wavFormat.wBitsPerSample = 16;
    fmt_block.wavFormat.wBlockAlign = 
            (wChannels * fmt_block.wavFormat.wBitsPerSample) / 8;
    
    /* wBlockAlign = (wChannels * wBitsPerSample) / 8 */
    printf("samplerate = %u\nwChannels = %u\n",dwSamplesPerSec, wChannels);

    /**** DATA_BLOCK ****/
    memcpy(data_block.szDataID, "data", 4);
    data_block.dwDataSize = (((length -8) >> 0) & 0xffffff00);
    printf("dwdatasize = %u\n", data_block.dwDataSize);   
    
    fwrite(&riff_header, sizeof(struct RIFF_HEADER), 1, filefd);
    fwrite(&fmt_block, sizeof(struct FMT_BLOCK), 1, filefd);
    fwrite(&data_block, sizeof(struct DATA_BLOCK), 1, filefd);

    fclose(filefd);
    return;
}

int main(int argc, char *argv[])
{
	struct stat stat;
	void *fdm;
//	char const *file;
	int fd;
	unsigned char temp[44];
//	file = argv[1];
	fd = open(argv[1], O_RDONLY);

	outfilename = strncat(strtok(argv[1], "."), ".wav", 4);

	if ((sfd = open(outfilename, O_WRONLY | O_CREAT, 0755)) < 0)
	{
		printf("can not open device!!!\n");
		return 5;
	}
    write(sfd, temp, 44);
	
	if (fstat(fd, &stat) == -1 || stat.st_size == 0)
		return 2;
	
	fdm = mmap(0, stat.st_size, PROT_READ, MAP_SHARED, fd, 0);
	
	if (fdm == MAP_FAILED)
		return 3;
	
	decode(fdm, stat.st_size);
	
	if (munmap(fdm, stat.st_size) == -1)
		return 4;

	close(sfd);
    
    addheader();
	return 0;
}

static enum mad_flow input(void *data, struct mad_stream *stream)
{
	struct buffer *buffer = data;
	if (!buffer->length)
		return MAD_FLOW_STOP;
	
	mad_stream_buffer(stream, buffer->start, buffer->length);
	buffer->length = 0;
    
	return MAD_FLOW_CONTINUE;
}

/*这一段是处理采样后的pcm音频 */
static inline signed int scale(mad_fixed_t sample)
{
	sample += (1L << (MAD_F_FRACBITS - 16));
	if (sample >= MAD_F_ONE)
		sample = MAD_F_ONE - 1;
	else if (sample < -MAD_F_ONE)
		sample = -MAD_F_ONE;
	return sample >> (MAD_F_FRACBITS + 1 - 16);
}

static enum mad_flow output(void *data,
	struct mad_header const *header, struct mad_pcm *pcm)
{
	unsigned int nchannels, nsamples, n;
	mad_fixed_t const *left_ch, *right_ch;
	unsigned char Output[6912], *OutputPtr;
	int wrote;

	nchannels = pcm->channels;
	n = nsamples = pcm->length;
	left_ch = pcm->samples[0];
	right_ch = pcm->samples[1];

    wChannels = pcm->channels;
    dwSamplesPerSec = pcm->samplerate;
    
	OutputPtr = Output;
	while (nsamples--)
	{
		signed int sample;
		sample = scale(*left_ch++);
		*(OutputPtr++) = sample >> 0;
		*(OutputPtr++) = sample >> 8;
		if (nchannels == 2)
		{
			sample = scale(*right_ch++);
			*(OutputPtr++) = sample >> 0;
			*(OutputPtr++) = sample >> 8;
		}
	}
	n *= 4;         /* 数据长度为pcm音频采样的4倍 */
	length += n;
	OutputPtr = Output;
	while (n)
	{
		wrote = write(sfd, OutputPtr, n);
		OutputPtr += wrote;
		n -= wrote;
	}
	OutputPtr = Output;
	return MAD_FLOW_CONTINUE;
}

static enum mad_flow error(void *data,
	struct mad_stream *stream, struct mad_frame *frame)
{
    struct buffer *buffer = data;

    fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
        stream->error, mad_stream_errorstr(stream),
        stream->this_frame - buffer->start);

	return MAD_FLOW_CONTINUE;
}

static int decode(unsigned char const *start, unsigned long length)
{
	struct buffer buffer;
	struct mad_decoder decoder;
	int result;
	buffer.start = start;
	buffer.length = length;
	mad_decoder_init(&decoder, &buffer, input, 0, 0, output, error, 0);
	mad_decoder_options(&decoder, 0);
	result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);
	mad_decoder_finish(&decoder);
	return result;
}
