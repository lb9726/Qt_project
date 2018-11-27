#ifndef EXTERNC_H
#define EXTERNC_H

#ifdef LINUX
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
    #include <mad.h>
    #include <sys/mman.h>
//    #include "wav_parser.h"
//    #include "sndwav_common.h"
//    #include "lmad.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void SetDevVolume(int pVaule);

#ifdef __cplusplus
}
#endif

#endif //EXTERNC_H
