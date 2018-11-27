#ifndef OPENCALLBACK_H
#define OPENCALLBACK_H

#include "../include/canfestival/canfestival.h"
#include "openclient.h"
#include "define.h"
//class OpenClient;

#define eprintf(...) printf (__VA_ARGS__)

extern int initCANDevice();
extern void releaseCANDevice();
extern void initNodes(CO_Data *d, u_int32_t id);
extern void exitClient(CO_Data* d, UNS32 id);
extern void registerCallbacks();

extern CO_Data *gCoData;

//extern void Display_initialisation(CO_Data* d);
//extern void Display_preOperational(CO_Data* d);
//extern void Display_operational(CO_Data* d);
//extern void Display_stopped(CO_Data* d);
//extern void Display_post_sync(CO_Data* d);
//extern void Display_post_TPDO(CO_Data* d);
//extern void Display_storeODSubIndex(CO_Data* d, UNS16 wIndex, UNS8 bSubindex);
//extern void Display_post_emcy(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg);
//extern void Display_heartbeatError(CO_Data* d, UNS8);
//extern void catch_signal(int sig);



#endif // OPENCALLBACK_H
