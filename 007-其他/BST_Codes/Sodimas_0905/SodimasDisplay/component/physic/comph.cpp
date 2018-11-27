#include "comph.h"

ComPh::ComPh(GlobalSetting *globalSetting) :
    PhysicBase(globalSetting)
{

}

bool ComPh::initDevice(PH_Type pPHType, QDomElement pElement)
{
    bool flag = false;
    QString tmp;

    QDomElement  tmpDomElement = pElement.firstChildElement("autotest");
    if(!tmpDomElement.isNull()){
        tmp = tmpDomElement.text();
        if(!tmp.compare("true")){
            m_Autotest = true;
        }else if(!tmp.compare("false")){
            m_Autotest = false;
        }else{
            m_Autotest = false;
        }
    }else{
        m_Autotest = false;
    }

    tmpDomElement = pElement.firstChildElement("testfreq");
    if(!tmpDomElement.isNull()){
        tmp = tmpDomElement.text();
        m_TestInterval = tmp.toInt(&flag);
        if(!flag){
           m_TestInterval = 2000;
        }
    }else{
        m_TestInterval = 2000;
    }
}
