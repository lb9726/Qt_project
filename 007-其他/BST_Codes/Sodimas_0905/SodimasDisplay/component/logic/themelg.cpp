#include "themelg.h"

ThemeLg::ThemeLg(ThemeParser *themeParser) :
    LogicBase(themeParser)
{

}

bool ThemeLg::initDevice(LG_Type pPHType, QDomElement pElement)
{
    bool flag = false;
    QString tmp;

    QDomElement  tmpDomElement = pElement.firstChildElement("layout");
    if(!tmpDomElement.isNull()){
        tmp = tmpDomElement.text();
        m_Layout = tmp.toInt(&flag);
        if(!flag){
           m_Layout = 0;
        }
    }else{
        m_Layout = 0;
    }

    tmpDomElement = pElement.firstChildElement("activetheme");
    if(!tmpDomElement.isNull()){
        m_ThemeName = tmpDomElement.text();
    }else{
        m_ThemeName = "sodimas";
    }
}
