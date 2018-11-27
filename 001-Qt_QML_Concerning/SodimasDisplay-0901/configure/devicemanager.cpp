#include "devicemanager.h"

const QString DeviceManager::MdpSuffix = ".mdp";
const QString DeviceManager::MdpFileType = "BST MDP File";


DeviceManager::DeviceManager(QString fileName) : XmlParse(fileName)
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
}

bool DeviceManager::createMdp()
{
    bool flag = New();
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

    QDomElement tmpElement = createElement("FileType");
        tmpElement.appendChild(createTextNode("BST MDP File"));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("CreateTime");
        QDateTime time = QDateTime::currentDateTime();
        tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("LastModiTime");
        tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("Monitor");
        tmpElement.appendChild(createTextNode("5000"));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("Passwd");
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("Pattern");
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("Showlog");
        tmpElement.appendChild(createTextNode("false"));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("CfgPara");
        tmpElement.appendChild(createElement("Middleware"));
        tmpElement.appendChild(createElement("Physical"));
        tmpElement.appendChild(createElement("Logic"));
    rootElement.appendChild(tmpElement);

    m_IsChanged = true;
    Save();
    return true;
}

bool DeviceManager::openMdp()
{
    bool flag = Open();
    if(flag == false)
    {
       m_Valid = false;
       return false;
    }
    m_Valid = true;
    //>@Check FileType
    flag = GetItemElement("/FileType");
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
    return GetCfgparaGroup();
}

bool DeviceManager::closeMdp()
{
    saveMdp();
    return true;
}

bool DeviceManager::delMdp()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool DeviceManager::saveMdp()
{
    QDateTime time = QDateTime::currentDateTime();
    ModifyItemElement("/LastModiTime", time.toString("yyyy-MM-dd HH:mm:ss"));
    Save();
    return true;
}

bool DeviceManager::renameMdp(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(MdpSuffix))
        tmpString.append(MdpSuffix);
    bool flag = Rename(tmpString);
    if(flag)
        m_FileName = tmpString;     //>@如果重命名成功，则修改m_FileName为重命名后的值。
    return flag;
}

bool DeviceManager::GetCfgparaGroup()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = GetItemElement("/CfgPara");
    if(flag)
    {
        m_CfgparaElementGroup.clear();
        QDomElement tmpElement;
        QDomNodeList CfgItemList = itemElement.childNodes();  //>@查找BGIMG、Physical、Logic
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
            QString tmpSubCfgString = QString("/CfgPara/%1").arg(tmpString);
            flag = GetItemElement(tmpSubCfgString);
            if(!flag)
                continue;
            QDomNodeList SubCfgItemNodeList = itemElement.childNodes();   //>@查找CPU或者NET等
            for(int j=0;j<SubCfgItemNodeList.count();j++)              //>@插入配置页的子项元素
            {
                tmpElement = SubCfgItemNodeList.at(j).toElement();
                if(tmpElement.isNull())
                    continue;
                QString tmpSubString = tmpElement.tagName();
                if(tmpSubString.isEmpty())
                    continue;
                QString tmpSubPath = QString("/%1/%2").arg(tmpString).arg(tmpSubString);
                m_CfgparaElementGroup.insert(tmpSubPath, tmpElement);
                IDE_TRACE_STR(tmpSubPath);
                if(tmpString.compare("Physical")==0)
                {
                    m_PHElementGroup.insert(GetPHType(tmpSubString), tmpElement);
                }
                else if(tmpString.compare("Logic")==0)
                {
                    m_LGElementGroup.insert(GetLGType(tmpSubString), tmpElement);
                }
                else if(tmpString.compare("Middleware")==0)
                {
                    m_MWElementGroup.insert(GetMWType(tmpSubString), tmpElement);
                }
            }
        }
    }
    return true;
}

bool DeviceManager::clrMdp()
{
    if(GetItemElement("/CfgPara"))
    {
        clrDomCldNodes(itemElement);
        return true;
    }
    return false;
}

bool DeviceManager::setMonitor(quint32 pFreq)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Monitor", QString::number(pFreq));
}

quint32 DeviceManager::getMonitor()
{
    if(!m_Valid)
        return 0;
    bool flag = GetItemElement("/Monitor");
    if(flag == false)
        return 0;
    return itemElement.text().toUInt();
}

bool DeviceManager::setLocation(QString pLocation)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Location", pLocation);
}

QString DeviceManager::getLocation()
{
    if(!m_Valid)
        return QString();
    bool flag = GetItemElement("/Location");
    if(flag == false)
        return QString();
    return itemElement.text();
}

bool DeviceManager::setPassword(QString pPasswd)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Passwd", pPasswd);
}

QString DeviceManager::getPassword()
{
    if(!m_Valid)
        return QString();
    bool flag = GetItemElement("/Passwd");
    if(flag == false)
        return QString();
    return itemElement.text();
}

bool DeviceManager::setPattern(QString pPattern)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Pattern", pPattern);
}

QString DeviceManager::getPattern()
{
    if(!m_Valid)
        return QString();
    bool flag = GetItemElement("/Pattern");
    if(flag == false)
        return QString();
    return itemElement.text();
}

QString DeviceManager::getVersion()
{
    if(!m_Valid)
        return QString();
    return GetItemElementValue("/CfgPara/Logic/ContentManager/Application");
}

bool DeviceManager::showLog(bool pEnable)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Pattern", pEnable?QString("true"):QString("false"));
}

bool DeviceManager::canShowLog()
{
    if(!m_Valid)
        return false;
    bool flag = GetItemElement("/Showlog");
    if(flag)
        return (itemElement.text().compare("false", Qt::CaseInsensitive)==0)?(false):(true);
    return false;
}

void DeviceManager::delDeviceDom(QString pPath)
{
    DEV_TYPE tmpDevType = GetDevType(pPath);
    if(tmpDevType == DEV_MIDDLEWARE)
    {
        MW_Type tmpType = GetMWType(getFileName(pPath));
        delDeviceDom(tmpType, pPath);
    }
    else if(tmpDevType == DEV_PHYSICAL)
    {
        PH_Type tmpType = GetPHType(getFileName(pPath));
        delDeviceDom(tmpType, pPath);
    }
    else if(tmpDevType == DEV_LOGIC)
    {
        LG_Type tmpType = GetLGType(getFileName(pPath));
        delDeviceDom(tmpType, pPath);
    }
}

void DeviceManager::delDeviceDom(MW_Type pType, QString pPath)
{
    QDomElement tmpElement = m_CfgparaElementGroup.take(pPath);
    if(tmpElement.isNull())
        return;
    if(pType != M_INVALID)
        m_MWElementGroup.take(pType);
    delDom(tmpElement);
    m_IsChanged = true;
}

void DeviceManager::delDeviceDom(PH_Type pType, QString pPath)
{
    QDomElement tmpElement = m_CfgparaElementGroup.take(pPath);
    if(tmpElement.isNull())
        return;
    if(pType != P_INVALID)
        m_PHElementGroup.take(pType);
    delDom(tmpElement);
    m_IsChanged = true;
}

void DeviceManager::delDeviceDom(LG_Type pType, QString pPath)
{
    QDomElement tmpElement = m_CfgparaElementGroup.take(pPath);
    if(tmpElement.isNull())
        return;
    if(pType != L_INVALID)
        m_LGElementGroup.take(pType);
    delDom(tmpElement);
    m_IsChanged = true;
}

QDomElement DeviceManager::addDeviceDom(QString pPath, QDomElement pInitElement)
{
    if(pPath.isEmpty() || pInitElement.isNull())
        return QDomElement();
    //>@获取父节点
    QString tmpParPath = getParentDirectory(pPath);
    QString tmpParName = getDirectoryName(tmpParPath);
    tmpParPath = QString("/%1").arg(tmpParName);
    QDomElement tmpParElement = m_CfgparaElementGroup.value(tmpParPath);
    if(tmpParElement.isNull())
    {
        tmpParElement = createElement(tmpParName);
        if(!GetItemElement("/CfgPara"))
            return QDomElement();
        itemElement.appendChild(tmpParElement);
        m_CfgparaElementGroup.insert(tmpParPath, tmpParElement);
    }

    QDomElement deviceElement = createElement(pInitElement.tagName());
    //>@创建属性
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
    DEV_TYPE tmpDevType = GetDevType(pPath);
    if(tmpDevType == DEV_MIDDLEWARE)
    {
        MW_Type tmpType = GetMWType(getFileName(pPath));
        if(tmpType != M_INVALID)
            m_MWElementGroup.insert(tmpType, deviceElement);
    }
    else if(tmpDevType == DEV_PHYSICAL)
    {
        PH_Type tmpType = GetPHType(getFileName(pPath));
        if(tmpType != P_INVALID)
            m_PHElementGroup.insert(tmpType, deviceElement);
    }
    else if(tmpDevType == DEV_LOGIC)
    {
        LG_Type tmpType = GetLGType(getFileName(pPath));
        if(tmpType != L_INVALID)
            m_LGElementGroup.insert(tmpType, deviceElement);
    }
    return deviceElement;
}

QDomElement DeviceManager::createParaDom(QDomElement pInitElement)
{
    if(pInitElement.isNull())
        return QDomElement();
    QDomElement tmpElement;
    tmpElement = createElement(pInitElement.tagName());
    QString tmpParaString = pInitElement.attribute("Para");
    if(!tmpParaString.isEmpty())
        tmpElement.setAttribute("Para", tmpParaString);
    QString tmpListString = pInitElement.attribute("List");
    if(!tmpListString.isEmpty())
        tmpElement.setAttribute("List", tmpListString);
    tmpElement.appendChild(createTextNode(pInitElement.text()));
    return tmpElement;
}

bool DeviceManager::clrDomCldNodes(QDomElement pElement)
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

bool DeviceManager::delDom(QDomElement pElement)
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

devManager::devManager(QString fileName) : XmlParse(fileName)
{
    int tmpProNum, tmpSuffixNum;
    tmpProNum = m_FileName.length();
    tmpSuffixNum = DevSuffix.length();
    if(tmpProNum <= tmpSuffixNum)
    {
        IDE_TRACE_INT(tmpProNum);
        return;
    }
    QString suffix = m_FileName.mid(tmpProNum-tmpSuffixNum, tmpSuffixNum);
    if(suffix != DevSuffix)
        m_FileName = QString("%1%2").arg(m_FileName).arg(DevSuffix);
}

bool devManager::NewDEV()
{
    bool flag = New();
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

    QDomElement tmpElement = createElement("FileType");
    tmpElement.appendChild(createTextNode("BST DEV File"));
    rootElement.appendChild(tmpElement);

    m_IsChanged = true;
    Save();
    return true;
}

bool devManager::OpenDEV()
{
    bool flag = Open();
    if(flag == false)
    {
       m_Valid = false;
       return false;
    }
    m_Valid = true;
    //>@Check FileType
    flag = GetItemElement("/FileType");
    if(flag == false)
    {
        m_Valid = false;
        return false;
    }
    QString tmpString = itemElement.text();
    if(tmpString.compare(DevFileType, Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    return GetDevGroup();
}

bool devManager::CloseDEV()
{
    SaveDEV();
    return true;
}

bool devManager::DelDEV()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool devManager::SaveDEV()
{
    QDateTime time = QDateTime::currentDateTime();
    ModifyItemElement("/LastModiTime", time.toString("yyyy-MM-dd HH:mm:ss"));
    Save();
    return true;
}

bool devManager::RenameDEV(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(DevSuffix))
        tmpString.append(DevSuffix);
    bool flag = Rename(tmpString);
    if(flag)
        m_FileName = tmpString;     //>@如果重命名成功，则修改m_FileName为重命名后的值。
    return flag;
}

bool devManager::SetCode(QString pCode)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Code", pCode);
}

QString devManager::GetCode()
{
    if(!m_Valid)
        return QString();
    bool flag = GetItemElement("/Code");
    if(flag == false)
        return QString();
    return itemElement.text();
}
