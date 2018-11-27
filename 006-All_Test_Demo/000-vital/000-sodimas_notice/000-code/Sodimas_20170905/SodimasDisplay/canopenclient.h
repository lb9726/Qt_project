#ifndef CANOPENCLIENT_H
#define CANOPENCLIENT_H

#include <QObject>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "./include/canfestival/canfestival.h"

#define eprintf(...) printf (__VA_ARGS__)

class CanOpenClient : public QObject
{
    Q_OBJECT
public:
    explicit CanOpenClient(QObject *parent = 0);
    ~CanOpenClient();

    //int initDevice();
    //void releaseDevice();
    int initCANDevice();
    void releaseCANDevice();

    static void initNodes(CO_Data *d, u_int32_t id);
    static void exitClient(CO_Data* d, UNS32 id);

    static void TestSlave_initialisation(CO_Data* d);
    static void TestSlave_preOperational(CO_Data* d);
    static void TestSlave_operational(CO_Data* d);
    static void TestSlave_stopped(CO_Data* d);
    static void TestSlave_post_sync(CO_Data* d);
    static void TestSlave_post_TPDO(CO_Data* d);
    static void TestSlave_storeODSubIndex(CO_Data* d, UNS16 wIndex, UNS8 bSubindex);
    static void TestSlave_post_emcy(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg);
    static void TestSlave_heartbeatError(CO_Data* d, UNS8);
    static void catch_signal(int sig);
    UNS32 Panel_output_Level_callbacks1(CO_Data* d, const indextable *, UNS8 bSubindex);
signals:

public slots:

};

#endif // CANOPENCLIENT_H
