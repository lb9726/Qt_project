#include "opencallback.h"
#include "display.h"
#include "include/canfestival/pdo.h"
#include <QDebug>

extern OpenClient* canOpenCallbackClient;
extern s_BOARD CopSlaveBoard;
extern UNS8 CopNodeID;
void catch_signal(int sig)
{
    Q_UNUSED(sig);
    //signal(SIGTERM, &catch_signal);
    //signal(SIGINT, &catch_signal);
    //eprintf("Got Signal %d\n",sig);
}

void Display_heartbeatError(CO_Data* d, UNS8){
    eprintf("Display_heartbeatError");
}

void Display_initialisation(CO_Data* d){
    //eprintf("Display_initialisation\n");
    canOpenCallbackClient->nodeStateChanged(1);
}

void Display_preOperational(CO_Data* d){
    //eprintf("Display_preOperational\n");
    canOpenCallbackClient->nodeStateChanged(2);
}

void Display_operational(CO_Data* d){
    //eprintf("Display_operational\n");
    canOpenCallbackClient->nodeStateChanged(3);
}

void Display_stopped(CO_Data* d){
    //eprintf("Display_stopped\n");
    canOpenCallbackClient->nodeStateChanged(4);
}

void Display_post_sync(CO_Data* d){
    //eprintf("Display_post_sync");
}

void Display_post_TPDO(CO_Data* d){
    //eprintf("Display_post_TPDO");
}

void Display_storeODSubIndex(CO_Data* d, UNS16 wIndex, UNS8 bSubindex){
    //eprintf("Display_storeODSubIndex");
}

void Display_post_emcy(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg){

}

extern ODCallback_t Sound_callbacks[6];             //RPDO1's Callback
extern ODCallback_t Level_text_Logo_callbacks[5];   //RPDO2's Callback
extern ODCallback_t text_segmented_1_callbacks[9];  //RPDO3's Callback
extern ODCallback_t texte_segmented_2_callbacks[9]; //RPDO4's Callback
extern ODCallback_t Display_parameters_callbacks[5];//SDO:2000 Callback
extern ODCallback_t Customer_Logo_callbacks[4];     //SDO:2001 Callback

UNS32 RPDO1_Callback(CO_Data* d, const indextable *, UNS8 bSubindex)
{
    if(canOpenCallbackClient)
        canOpenCallbackClient->rPDOArrived(1);
    return 0;
}

UNS32 RPDO2_Callback(CO_Data* d, const indextable *, UNS8 bSubindex)
{
    if(canOpenCallbackClient)
        canOpenCallbackClient->rPDOArrived(2);
    return 0;
}

UNS32 RPDO3_Callback(CO_Data* d, const indextable *, UNS8 bSubindex)
{
    if(canOpenCallbackClient)
        canOpenCallbackClient->rPDOArrived(3);
    return 0;
}

UNS32 RPDO4_Callback(CO_Data* d, const indextable *, UNS8 bSubindex)
{
    if(canOpenCallbackClient)
        canOpenCallbackClient->rPDOArrived(4);
    return 0;
}

UNS32 SDO_2000_Callback(CO_Data* d, const indextable *, UNS8 bSubindex)
{
    if(canOpenCallbackClient)
        canOpenCallbackClient->sSDOArrived(1);
    return 0;
}

UNS32 SDO_2001_Callback(CO_Data* d, const indextable *, UNS8 bSubindex)
{
    if(canOpenCallbackClient)
        canOpenCallbackClient->sSDOArrived(2);
    return 0;
}


int initCANDevice()
{
    char* LibraryPath="libcanfestival_can_socket.so";
    TimerInit();

    if (LoadCanDriver(LibraryPath) == NULL){
        printf("Unable to load library: %s\n",LibraryPath);
    }

    display_Data.heartbeatError = Display_heartbeatError;
    display_Data.initialisation = Display_initialisation;
    display_Data.preOperational = Display_preOperational;
    display_Data.operational = Display_operational;
    display_Data.stopped = Display_stopped;
    //display_Data.post_sync = Display_post_sync;
    //display_Data.post_TPDO = Display_post_TPDO;
    display_Data.storeODSubIndex = Display_storeODSubIndex;
    display_Data.post_emcy = Display_post_emcy;

    registerCallbacks();

    if(!canOpen(&CopSlaveBoard,&display_Data)){
        eprintf("Cannot open Slave Board (%s,%s)\n",CopSlaveBoard.busname, CopSlaveBoard.baudrate);
        return -1;
    }

    StartTimerLoop(&initNodes);
    //eprintf("StartTimerLoop");
    return 0;
}

void releaseCANDevice(){
    StopTimerLoop(exitClient);
    TimerCleanup();
}

void initNodes(CO_Data* d, UNS32 id)
{
    setNodeId(&display_Data, CopNodeID);
    setState(&display_Data, Initialisation);
}

void exitClient(CO_Data* d, UNS32 id)
{

}

void registerCallbacks()
{
    Sound_callbacks[5] = RPDO1_Callback;
    Level_text_Logo_callbacks[4] = RPDO2_Callback;
    text_segmented_1_callbacks[8] = RPDO3_Callback;
    texte_segmented_2_callbacks[8] = RPDO4_Callback;
    Display_parameters_callbacks[4] = SDO_2000_Callback;
    Customer_Logo_callbacks[3] = SDO_2001_Callback;
}
