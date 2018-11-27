#ifndef MP3_PARSER_H
#define MP3_PARSER_H

#include "define.h"
#include <stdio.h>
//#include <io.h>
#include <string.h>
#include <stdlib.h>


#define   SEEK_END       2
#define   ID3V1_size     128

int GetMp3BitRate(int bRateIndex, int LayerDescript, int Version);
int GetMp3Time(QString tmpAudio);

#endif // MP3_PARSER_H
