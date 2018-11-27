#include "mdpparser.h"

const QString MdpParser::MdpSuffix = ".xml";
const QString MdpParser::MdpFileType = "device_define_file";

PH_Type getPHType(QString pPara)
{
    PH_Type PHType = P_INVALID;
    if(pPara.isEmpty())
        return PHType;
    if(pPara.compare("player", Qt::CaseInsensitive) == 0)
        PHType = P_Player;
    else if(pPara.compare("lanc", Qt::CaseInsensitive) == 0)
        PHType = P_LANC;
    else if(pPara.compare("lanr", Qt::CaseInsensitive) == 0)
        PHType = P_LANR;
    else if(pPara.compare("lcd", Qt::CaseInsensitive) == 0)
        PHType = P_LCD;
    else if(pPara.compare("led", Qt::CaseInsensitive) == 0)
        PHType = P_LED;
    else if(pPara.compare("com", Qt::CaseInsensitive) == 0)
        PHType = P_COM;
    return PHType;
}

PH_Type getElementPHType(QDomElement pDevElement)
{
    if(pDevElement.isNull())
        return P_INVALID;
    return getPHType(pDevElement.tagName());
}

LG_Type getLGType(QString pPara)
{
    LG_Type LGType = L_INVALID;
    if(pPara.isEmpty())
        return LGType;

    if(pPara.compare("thememanager", Qt::CaseInsensitive) == 0)
        LGType = L_ThemeManager;
    else if(pPara.compare("contentmanager", Qt::CaseInsensitive) == 0)
        LGType = L_ContentManager;
    return LGType;
}

DEV_TYPE getDevType(QString pDevName)
{
    if(pDevName.contains("physical"))
        return DEV_PHYSICAL;
    else if(pDevName.contains("logic"))
        return DEV_LOGIC;
    return DEV_NONE;
}

QString getParentDirectory(QString pDirPath)
{
    if (pDirPath.isEmpty())
        return QString();
    pDirPath.replace("\\", "/");
    if(!pDirPath.endsWith('/'))
        pDirPath.append('/');
    int index1 = pDirPath.lastIndexOf('/');
    if(index1 < 0)
        return QString();
    int index2 = pDirPath.lastIndexOf('/', index1-1);
    if(index2 < 0)
        return QString();
    return pDirPath.mid(0, index2 + 1);

}
QString getDirectoryName(QString pDirPath)
{
    if (pDirPath.isEmpty())
        return QString();
    pDirPath.replace("\\", "/");
    if(!pDirPath.endsWith('/'))
        pDirPath.append('/');
    int index1 = pDirPath.lastIndexOf('/');
    if(index1 < 0)
        return QString();
    int index2 = pDirPath.lastIndexOf('/', index1-1);
    if(index2 < 0)
        return QString();
    return pDirPath.mid(index2 + 1, index1 - index2 - 1);
}

QString getFileName(QString pFilePath)
{
    QFileInfo tmpFileInfo(pFilePath);
    return tmpFileInfo.fileName();
}


MdpParser::MdpParser(QString fileName) : XmlParse(fileName)
{
    int tmpProNum, tmpSuffixNum;
    tmpProNum = m_FileName.length();
    tmpSuffixNum = MdpSuffix.length();
    if(tmpProNum <= tmpSuffixNum)
    {
        IDE_TRACE_INT(tmpProNum);
        return;
    }
    QString suffix = m_FileName.mid(tmpProNum-tmpSuffixNum, tmpSuffixNum);
    if(suffix != MdpSuffix)
        m_FileName = QString("%1%2").arg(m_FileName).arg(MdpSuffix);

    m_password = "";
}

bool MdpParser::newMdp()
{
    bool flag = createXml();
    if(flag == false)
    {
        IDE_TRACE();
        m_Valid = false;
        return false;
    }
    m_Valid = true;

    appendChild(createProcessingInstruction("xml", XmlHead));

    QDomElement rootElement;
    rootElement = createElement("root");
    appendChild(rootElement);

    QDomElement tmpElement = createElement("filetype");
    tmpElement.appendChild(createTextNode("device_define_file"));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("createtime");
    QDateTime time = QDateTime::currentDateTime();
    tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("lastmoditime");
    tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("password");
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("configure");
    tmpElement.appendChild(createElement("physical"));
    tmpElement.appendChild(createElement("logic"));
    rootElement.appendChild(tmpElement);

    m_IsChanged = true;
    saveXml();
    return true;
}

bool MdpParser::openMdp()
{
    bool flag = openXml();
    if(flag == false)
    {
        m_Valid = false;
        return false;
    }
    m_Valid = true;
    flag = getItemElement("/filetype");
    if(flag == false)
    {
        m_Valid = false;
        return false;
    }
    QString tmpString = itemElement.text();
    if(tmpString.compare(MdpFileType, Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    return getCfgparaGroup();
}

bool MdpParser::closeMdp()
{
    saveMdp();
    return true;
}

bool MdpParser::delMdp()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool MdpParser::saveMdp()
{
    QDateTime time = QDateTime::currentDateTime();
    modifyItemElement("/lastmoditime", time.toString("yyyy-MM-dd HH:mm:ss"));
    saveXml();
    return true;
}

bool MdpParser::renameMdp(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(MdpSuffix))
        tmpString.append(MdpSuffix);
    bool flag = renameXml(tmpString);
    if(flag)
        m_FileName = tmpString;
    return flag;
}

bool MdpParser::getCfgparaGroup()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = getItemElement("/password");
    if(flag){
        m_password = itemElement.text();
    }

    flag = getItemElement("/configure");
    if(flag)
    {
        m_CfgparaElementGroup.clear();
        QDomElement tmpElement;
        QDomNodeList CfgItemList = itemElement.childNodes();  ///查找BGIMG、Physical、Logic
        for(int i=0;i<CfgItemList.count();i++)
        {
            tmpElement = CfgItemList.at(i).toElement();
            if(tmpElement.isNull())
                continue;
            QString tmpString = tmpElement.tagName();
            if(tmpString.isEmpty())
                continue;
            QString tmpPath = QString("/%1").arg(tmpString);
            m_CfgparaElementGroup.insert(tmpPath, tmpElement);
            IDE_TRACE_STR(tmpPath);
            QString tmpSubCfgString = QString("/configure/%1").arg(tmpString);
            flag = getItemElement(tmpSubCfgString);
            if(!flag)
                continue;
            QDomNodeList SubCfgItemNodeList = itemElement.childNodes();
            for(int j=0;j<SubCfgItemNodeList.count();j++)
            {
                tmpElement = SubCfgItemNodeList.at(j).toElement();
                if(tmpElement.isNull())
                    continue;
                QString tmpSubString = tmpElement.tagName();
                IDE_TRACE_STR(tmpSubString);
                if(tmpSubString.isEmpty())
                    continue;
                QString tmpSubPath = QString("/%1/%2").arg(tmpString).arg(tmpSubString);
                IDE_TRACE_STR(tmpSubCfgString);
                m_CfgparaElementGroup.insert(tmpSubPath, tmpElement);
                IDE_TRACE_STR(tmpSubPath);
                if(tmpString.compare("physical")==0)
                {
                    m_PHElementGroup.insert(getPHType(tmpSubString), tmpElement);
                }
                else if(tmpString.compare("logic")==0)
                {
                    m_LGElementGroup.insert(getLGType(tmpSubString), tmpElement);
                    IDE_TRACE_STR(tmpSubString);
                }else{

                }
            }
        }
    }
    return true;
}

bool MdpParser::setPassword(QString pPasswd)
{
    if(!m_Valid)
        return false;
    return modifyItemElement("/password", pPasswd);
}

QString MdpParser::getPassword()
{
    if(!m_Valid)
        return QString();
    bool flag = getItemElement("/password");
    if(flag == false)
        return QString();
    return itemElement.text();
}

QDomElement MdpParser::addDeviceDom(QString pPath, QDomElement pInitElement)
{
    if(pPath.isEmpty() || pInitElement.isNull())
        return QDomElement();
    ///获取父节点
    QString tmpParPath = getParentDirectory(pPath);
    QString tmpParName = getDirectoryName(tmpParPath);
    tmpParPath = QString("/%1").arg(tmpParName);
    QDomElement tmpParElement = m_CfgparaElementGroup.value(tmpParPath);
    if(tmpParElement.isNull())
    {
        tmpParElement = createElement(tmpParName);
        if(!getItemElement("/CfgPara"))
            return QDomElement();
        itemElement.appendChild(tmpParElement);
        m_CfgparaElementGroup.insert(tmpParPath, tmpParElement);
    }

    QDomElement deviceElement = createElement(pInitElement.tagName());
    ///创建属性
    QDomNodeList CfgItemList = pInitElement.childNodes();
    for(int i=0;i<CfgItemList.count();i++)
    {
        QDomElement tmpSubInitElement = CfgItemList.at(i).toElement();
        if(tmpSubInitElement.isNull())
            continue;
        QDomElement tmpSubElement = createParaDom(tmpSubInitElement);
        if(tmpSubElement.isNull())
            continue;
        deviceElement.appendChild(tmpSubElement);
    }
    tmpParElement.appendChild(deviceElement);
    m_IsChanged = true;
    m_CfgparaElementGroup.insert(pPath, deviceElement);
    DEV_TYPE tmpDevType = getDevType(pPath);
    if(tmpDevType == DEV_PHYSICAL)
    {
        PH_Type tmpType = getPHType(getFileName(pPath));
        if(tmpType != P_INVALID)
            m_PHElementGroup.insert(tmpType, deviceElement);
    }
    else if(tmpDevType == DEV_LOGIC)
    {
        LG_Type tmpType = getLGType(getFileName(pPath));
        if(tmpType != L_INVALID)
            m_LGElementGroup.insert(tmpType, deviceElement);
    }
    return deviceElement;
}

QDomElement MdpParser::createParaDom(QDomElement pInitElement)
{
    if(pInitElement.isNull())
        return QDomElement();
    QDomElement tmpElement;
    tmpElement = createElement(pInitElement.tagName());
    QString tmpParaString = pInitElement.attribute("para");
    if(!tmpParaString.isEmpty())
        tmpElement.setAttribute("para", tmpParaString);
    QString tmpListString = pInitElement.attribute("list");
    if(!tmpListString.isEmpty())
        tmpElement.setAttribute("list", tmpListString);
    tmpElement.appendChild(createTextNode(pInitElement.text()));
    return tmpElement;
}

bool MdpParser::clrDomCldNodes(QDomElement pElement)
{
    if(pElement.isNull())
        return true;
    QDomNodeList cldNodeList = pElement.childNodes();
    for(int i=cldNodeList.count()-1;i>=0;i--)
    {
        pElement.removeChild(cldNodeList.at(i));
    }
    return true;
}

bool MdpParser::delDom(QDomElement pElement)
{
    if(pElement.isNull())
        return true;
    if(!clrDomCldNodes(pElement))
        return false;
    QString key = m_CfgparaElementGroup.key(pElement);
    QDomNode parNode = pElement.parentNode();
    parNode.removeChild(pElement);
    m_CfgparaElementGroup.remove(key);
    return true;
}
