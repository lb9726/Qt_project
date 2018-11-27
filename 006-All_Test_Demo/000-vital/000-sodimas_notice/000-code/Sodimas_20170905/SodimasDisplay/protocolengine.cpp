#include "protocolengine.h"

ProtocolEngine::ProtocolEngine(QObject *parent) :
    QObject(parent)
{
}

void ProtocolEngine::initCanopen(){
#ifdef CANOPEN
    m_canopenClient = new OpenClient();
    connect(m_canopenClient,SIGNAL(sigNodeState(int)),this,SLOT(nodeStateChanged(int)));
    connect(m_canopenClient,SIGNAL(sigFCT_AFF(bool)),this,SLOT(fct_affChanged(bool)));
    connect(m_canopenClient,SIGNAL(sigArrowAction(bool,bool,bool)),this,SLOT(arrowAction(bool,bool,bool)));
    connect(m_canopenClient,SIGNAL(sigMainVolumeChange(int,int)),this,SLOT(mainVolumeChange(int,int)));
    connect(m_canopenClient,SIGNAL(sigMusicVolumeChange(int)),this,SLOT(musicVolumeChange(int)));
    connect(m_canopenClient,SIGNAL(sigButtonLightOn(int,bool)),this,SLOT(buttonLightOn(int,bool)));
    connect(m_canopenClient,SIGNAL(sigButtonLightOff(int)),this,SLOT(buttonLightOff(int)));
    connect(m_canopenClient,SIGNAL(sigFloorArrive(int,int,int,int)),this,SLOT(floorArrive(int,int,int,int)));
    connect(m_canopenClient,SIGNAL(sigShowFuncLogo(int,int)),this,SLOT(showFuncLogo(int,int)));
    connect(m_canopenClient,SIGNAL(sigMessage(QString,QString,QString,QString)),this,SLOT(textMessage(QString,QString,QString,QString)));
    connect(m_canopenClient,SIGNAL(sigSDO1(int,int,int,int)),this,SLOT(sDO1(int,int,int,int)));
    connect(m_canopenClient,SIGNAL(sigSDO2(int,int,int)),this,SLOT(sDO2(int,int,int)));

    //m_canopenClient->initCanopenClient(0x3c,150,"0");

#endif
}

///NMT
void ProtocolEngine::nodeStateChanged(int state){

}

///RPDO1
void ProtocolEngine::fct_affChanged(bool isFCT){

}

void ProtocolEngine::arrowAction(bool arrows_FLM,bool arrows_FLD,bool arrows_SCROL){

}

void ProtocolEngine::mainVolumeChange(int divGong,int divBip){

}

void ProtocolEngine::musicVolumeChange(int div){

}

void ProtocolEngine::buttonLightOn(int btIndex,bool isBuze){

}

void ProtocolEngine::buttonLightOff(int btIndex){

}

void ProtocolEngine::sigButtonLightOff(int btIndex){

}

///RPDO2
void ProtocolEngine::floorArrive(int ge,int shi,int bai,int soundNumber){

}

void ProtocolEngine::showFuncLogo(int funcLogoIndex,int soundNumber){

}

///RPDO3&RPDO4
void ProtocolEngine::textMessage(QString m1,QString m2,QString m3,QString m4){

}

///SDO1
void ProtocolEngine::sDO1(int orientation,int contraste,int backlight,int vit_scrol){

}

///SDO2
void ProtocolEngine::sDO2(int logo,int periode,int declenchement){

}
