#include "canopenclient.h"
#include "open/display.h"
#include "include/canfestival/pdo.h"
#include <QDebug>

s_BOARD SlaveBoard = {"0", "100K"};

CanOpenClient::CanOpenClient(QObject *parent):QObject(parent)
{

}

CanOpenClient::~CanOpenClient()
{
    //releaseDevice();
}


int CanOpenClient::initCANDevice()
{
    char* LibraryPath="libcanfestival_can_socket.so";
    //signal(SIGTERM, catch_signal);
    //signal(SIGINT, catch_signal);
    TimerInit();

    if (LoadCanDriver(LibraryPath) == NULL){
        printf("Unable to load library: %s\n",LibraryPath);
    }

    display_Data.heartbeatError = TestSlave_heartbeatError;
    display_Data.initialisation = TestSlave_initialisation;
    display_Data.preOperational = TestSlave_preOperational;
    display_Data.operational = TestSlave_operational;
    display_Data.stopped = TestSlave_stopped;
    display_Data.post_sync = TestSlave_post_sync;
    display_Data.post_TPDO = TestSlave_post_TPDO;
    display_Data.storeODSubIndex = TestSlave_storeODSubIndex;
    display_Data.post_emcy = TestSlave_post_emcy;

    if(!canOpen(&SlaveBoard,&display_Data)){
        eprintf("Cannot open Slave Board (%s,%s)\n",SlaveBoard.busname, SlaveBoard.baudrate);
        return -1;
    }

    StartTimerLoop(&initNodes);

    eprintf("StartTimerLoop");

    pause();

    // Stop timer thread
    //StopTimerLoop(&Exit);
    return 0;
}

void CanOpenClient::releaseCANDevice(){
    StopTimerLoop(exitClient);
    TimerCleanup();
}


void CanOpenClient::initNodes(CO_Data* d, UNS32 id)
{
    setNodeId(&display_Data, 0x3c);
    setState(&display_Data, Initialisation);
}

void CanOpenClient::exitClient(CO_Data* d, UNS32 id)
{
}



void CanOpenClient::catch_signal(int sig)
{
    //signal(SIGTERM, &catch_signal);
    //signal(SIGINT, &catch_signal);
    eprintf("Got Signal %d\n",sig);
}

void CanOpenClient::TestSlave_heartbeatError(CO_Data* d, UNS8){
    eprintf("TestSlave_heartbeatError");
}

void CanOpenClient::TestSlave_initialisation(CO_Data* d){
    eprintf("TestSlave_initialisation\n");
}

void CanOpenClient::TestSlave_preOperational(CO_Data* d){
    eprintf("TestSlave_preOperational\n");
}

void CanOpenClient::TestSlave_operational(CO_Data* d){
    eprintf("TestSlave_operational\n");
}

void CanOpenClient::TestSlave_stopped(CO_Data* d){
    eprintf("TestSlave_stopped\n");
}

void CanOpenClient::TestSlave_post_sync(CO_Data* d){

}

void CanOpenClient::TestSlave_post_TPDO(CO_Data* d){

}

void CanOpenClient::TestSlave_storeODSubIndex(CO_Data* d, UNS16 wIndex, UNS8 bSubindex){

}

void CanOpenClient::TestSlave_post_emcy(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg){

}

UNS32 CanOpenClient::Panel_output_Level_callbacks1(CO_Data* d, const indextable *, UNS8 bSubindex){

}

