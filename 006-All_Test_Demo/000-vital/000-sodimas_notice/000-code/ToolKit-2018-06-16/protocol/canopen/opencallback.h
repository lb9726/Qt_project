#ifndef OPENCALLBACK_H
#define OPENCALLBACK_H

#include "../include/canfestival/canfestival.h"
#include "openclient.h"
#include "define.h"

#define eprintf(...) printf (__VA_ARGS__)

extern int initCANDevice();
extern void releaseCANDevice();
extern void initNodes(CO_Data *d, u_int32_t id);
extern void exitClient(CO_Data* d, UNS32 id);
extern void registerCallbacks();

extern CO_Data *gCoData;

#endif // OPENCALLBACK_H
