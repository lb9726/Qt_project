#include "lcdph.h"
#include  "button/backlightctrl.h"
#include <QtCore/qmath.h>

LcdPh::LcdPh(GlobalSetting *globalSetting) :
    PhysicBase(globalSetting)
{
}

bool LcdPh::initDevice(PH_Type pPHType, QDomElement pElement)
{
    bool flag = false;
    QString tmp;
    QDomElement  tmpDomElement = pElement.firstChildElement("rotation");

    if(!tmpDomElement.isNull()){
        tmp = tmpDomElement.text();
        m_Rotation = tmp.toInt(&flag);
        if(!flag){
            m_Rotation = 0;
        }
    }else{
        m_Rotation = 0;
    }

    tmpDomElement = pElement.firstChildElement("luminance");
    if(!tmpDomElement.isNull()){
        tmp = tmpDomElement.text();
        m_Luminance = tmp.toInt(&flag);
        if(!flag){
            m_Luminance = 100;
        }
    }else{
        m_Luminance = 100;
    }

    tmpDomElement = pElement.firstChildElement("standby");
    if(!tmpDomElement.isNull()){
        tmp = tmpDomElement.text();
        m_Standby = tmp.toInt(&flag);
        if(!flag){
            m_Standby = 300000;
        }
    }else{
        m_Standby = 300000;
    }

    tmpDomElement = pElement.firstChildElement("standbyluminance");
    if(!tmpDomElement.isNull()){
        tmp = tmpDomElement.text();
        m_Standbyluminance = tmp.toInt(&flag);
        if(!flag){
            m_Standbyluminance = 30;
        }
    }else{
        m_Standbyluminance = 30;
    }

    tmpDomElement = pElement.firstChildElement("screensave");
    if(!tmpDomElement.isNull()){
        tmp = tmpDomElement.text();
        m_Screensave = tmp.toInt(&flag);
        if(!flag){
            m_Screensave = 1800000;
        }
    }else{
        m_Screensave = 1800000;
    }

    tmpDomElement = pElement.firstChildElement("screensavehold");
    if(!tmpDomElement.isNull()){
        tmp = tmpDomElement.text();
        m_Screensavehold = tmp.toInt(&flag);
        if(!flag){
            m_Screensavehold = 5000;
        }
    }else{
        m_Screensavehold = 5000;
    }
}

void  LcdPh::setbacklight(int level){
    if(level<=0)
        level = 0;
    if(level>=100)
        level = 100;
    IDE_TRACE();
    BacklightCtrl::setBacklightValue(qFloor(level));
}

void  LcdPh::standbyFilter(bool withArrow){

}
