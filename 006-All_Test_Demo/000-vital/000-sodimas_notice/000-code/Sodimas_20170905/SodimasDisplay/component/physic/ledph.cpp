#include "ledph.h"

LedPh::LedPh(GlobalSetting *globalSetting) :
    PhysicBase(globalSetting)
{
}

bool LedPh::initDevice(PH_Type pPHType, QDomElement pElement)
{
    QString tmp;

    QDomElement  tmpDomElement = pElement.firstChildElement("normal");
    if(!tmpDomElement.isNull()){
        tmp = tmpDomElement.text();
        m_Normal = getSpeedFromSting(tmp);
    }else{
        m_Normal = LED_SLOW;
    }

    tmpDomElement = pElement.firstChildElement("busy");
    if(!tmpDomElement.isNull()){
        tmp = tmpDomElement.text();
        m_Busy = getSpeedFromSting(tmp);
    }else{
        m_Busy = LED_SLOW;
    }

    tmpDomElement = pElement.firstChildElement("error");
    if(!tmpDomElement.isNull()){
        tmp = tmpDomElement.text();
        m_Error = getSpeedFromSting(tmp);
    }else{
        m_Error = LED_SLOW;
    }
}


LED_SPEED LedPh::getSpeedFromSting(QString speed){

    if(speed.isEmpty())
        return LED_SLOW;
    if(!speed.compare(D_SLOWTEXT)){
        return LED_SLOW;
    }else if(!speed.compare(D_FASTTEXT)){
        return LED_FAST;
    }else if(!speed.compare(D_HIGHTEXT)){
        return LED_HIGH;
    }else{
        return LED_SLOW;
    }
}
