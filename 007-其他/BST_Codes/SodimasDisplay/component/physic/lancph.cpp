#include "lancph.h"

LancPh::LancPh(GlobalSetting *globalSetting) :
    PhysicBase(globalSetting)
{
}

bool LancPh::initDevice(PH_Type pPHType, QDomElement pElement)
{
    bool flag = false;
    QString tmp;

    QDomElement  tmpDomElement = pElement.firstChildElement("wifion");
    if(!tmpDomElement.isNull()){
        tmp = tmpDomElement.text();
        if(!tmp.compare("true")){
            m_Wifion = true;
        }else if(!tmp.compare("false")){
            m_Wifion = false;
        }else{
            m_Wifion = false;
        }
    }else{
        m_Wifion = false;
    }

    tmpDomElement = pElement.firstChildElement("wifiip");
    if(!tmpDomElement.isNull()){
        m_WifiIp = tmpDomElement.text();
    }else{
        m_WifiIp = "192.168.1.10";
    }

    tmpDomElement = pElement.firstChildElement("dhcp");
    if(!tmpDomElement.isNull()){
        tmp = tmpDomElement.text();
        if(!tmp.compare("true")){
            m_Dhcp = true;
        }else if(!tmp.compare("false")){
            m_Dhcp = false;
        }else{
            m_Dhcp = false;
        }
    }else{
        m_Dhcp = false;
    }


    tmpDomElement = pElement.firstChildElement("udp");
    if(!tmpDomElement.isNull()){
        tmp = tmpDomElement.text();
        m_UdpPort = tmp.toInt(&flag);
        if(!flag){
           m_UdpPort = 6002;
        }
    }else{
        m_UdpPort = 6002;
    }

    tmpDomElement = pElement.firstChildElement("tcp");
    if(!tmpDomElement.isNull()){
        tmp = tmpDomElement.text();
        m_TcpPort = tmp.toInt(&flag);
        if(!flag){
           m_TcpPort = 6001;
        }
    }else{
        m_TcpPort = 6001;
    }
}
