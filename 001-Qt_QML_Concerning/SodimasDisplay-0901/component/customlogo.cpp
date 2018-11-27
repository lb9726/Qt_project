#include "customlogo.h"
#include "define.h"
CustomLogo::CustomLogo(QObject *parent) :
    QObject(parent)
{
    m_logo = 0;
    m_periode = 5;
    m_declenchement = 5;
    m_logoTriggerTimer = new QTimer;
    m_logoTriggerTimer->setInterval(m_declenchement*1000);
    connect(m_logoTriggerTimer,SIGNAL(timeout()),this,SLOT(logoTimeOut()));

    m_logoSwitchTimer = new QTimer;
    m_logoSwitchTimer->setInterval(m_periode*1000);
    connect(m_logoTriggerTimer,SIGNAL(timeout()),this,SLOT(logoSwitchTimeOut()));
    isShowingLogo= false;
}


void CustomLogo::startLogo(){
    m_logoTriggerTimer->start();
}

void CustomLogo::stop(){
    m_logoTriggerTimer->stop();
    m_logoSwitchTimer->stop();
}

void CustomLogo::logoTrigger(int logo,int periode,int declenchement){
    m_logo = logo;
    m_periode = periode;
    m_declenchement = declenchement;
    m_logoTriggerTimer->setInterval(m_declenchement*1000);
    m_logoTriggerTimer->start();
}


void CustomLogo::logoSwitchTimeOut(){
    if(m_logo == 2){
        if(isShowingLogo){
            emit sigShowNormal();
        }else{
            emit sigShowCustomLogo();
        }
    }else if(m_logo == 3){
        if(isShowingLogo){
            emit sigShowMintenanceLogo();
        }else{
            emit sigShowCustomLogo();
        }
    }else{
        m_logoSwitchTimer->stop();
    }

    isShowingLogo = !isShowingLogo;
}

void CustomLogo::logoTimeOut(){
    if(m_logo == 0 ){
        IDE_TRACE();
    }else if(m_logo == 1){
        emit sigShowCustomLogo();
    }else if(m_logo == 2){
        m_logoSwitchTimer->setInterval(m_periode*1000);
        m_logoSwitchTimer->start();

        isShowingLogo = false;
    }else if(m_logo == 3){

    }
}
