#include "iddailer.h"

#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include <fcntl.h>
#include "define.h"

#define DAIL_STATU_ON 1
#define DAIL_STATU_OFF 0

IdDailer::IdDailer(QObject *parent) : QObject(parent)
{    
    for(int j = 0; j < 8; j++){
        m_dailStatu[j] = DAIL_STATU_OFF;
    }
    getDailStatus();
}

void IdDailer::getDailStatus(){
    int ret;
    char bcValue[2];
    int fd;
    m_dialGpio.append("/sys/class/gpio/gpio25/value");
    m_dialGpio.append("/sys/class/gpio/gpio24/value");
    m_dialGpio.append("/sys/class/gpio/gpio23/value");
    m_dialGpio.append("/sys/class/gpio/gpio22/value");
    m_dialGpio.append("/sys/class/gpio/gpio21/value");
    m_dialGpio.append("/sys/class/gpio/gpio20/value");
    m_dialGpio.append("/sys/class/gpio/gpio19/value");
    m_dialGpio.append("/sys/class/gpio/gpio18/value");

    for(int i = 0; i < 8; i++){
        QByteArray ba;
        ba.append(m_dialGpio.at(i));
        fd = open(ba.data(),O_RDONLY);
        if(fd == -1)
        {
            IDE_TRACE();
            continue;
        }
        else
        {
            ret = read(fd,bcValue,2);
            if(ret>0){
                if(bcValue[0] == '0'){
                    m_dailStatu[i] = DAIL_STATU_ON;
                }else if(bcValue[0] == '1'){
                    //IDE_TRACE();
                    m_dailStatu[i] = DAIL_STATU_OFF;
                }else{
                    m_dailStatu[i] = DAIL_STATU_OFF;
                }
            }
            close(fd);
        }
        ///IDE_TRACE();
    }

//        for(int j = 0; j < 8; j++){
//            if(m_dailStatu[j])
//                IDE_TRACE_STR(" ON");
//            else
//                IDE_TRACE_STR(" OFF");
//        }
}

QString IdDailer::getDailerStateString(){
    QString str;
    for(int j = 0; j < 8; j++){
        if(m_dailStatu[j])
            str +=" ON";
        else
            str +=" OFF";
    }

    return str;
}

quint8 IdDailer::getNodeId(){
    quint8 id = 0;
    if(m_dailStatu[7]==1){
        return id;
    }

    if(m_dailStatu[5] == 1){
        id  = m_dailStatu[0]+2*m_dailStatu[1]+4*m_dailStatu[2]+8*m_dailStatu[3]+16*m_dailStatu[4];
        id++;
    }else{
        id  = m_dailStatu[0]+2*m_dailStatu[1]+4*m_dailStatu[2]+8*m_dailStatu[3]+16*m_dailStatu[4];
        IDE_TRACE_INT(id);
        if(id == 0)
        {
            id= 60;
            IDE_TRACE();
        }else if(id == 1){
            id= 61;
            IDE_TRACE();
        }else{
            id -= 1;
            IDE_TRACE();
        }
    }
    return id;
}

quint8 IdDailer::getBoutrate(){
    if(m_dailStatu[5]==1){
        return 50;//50k
    }else{
        return 250;
    }
}
