#include "mdpparser.h"

const QString MdpParser::MdpSuffix = ".mdp";
const QString MdpParser::MdpFileType = "BST MDP File";

MdpParser::MdpParser(QString fileName) : XmlParser(fileName)
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

bool MdpParser::NewMDP()
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

bool MdpParser::OpenMDP()
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

bool MdpParser::CloseMDP()
{
    SaveMDP();
    return true;
}

bool MdpParser::DelMDP()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool MdpParser::SaveMDP()
{
    QDateTime time = QDateTime::currentDateTime();
    ModifyItemElement("/LastModiTime", time.toString("yyyy-MM-dd HH:mm:ss"));
    Save();
    return true;
}

bool MdpParser::RenameMDP(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(MdpSuffix))
        tmpString.append(MdpSuffix);
    bool flag = Rename(tmpString);
    if(flag)
        m_FileName = tmpString;     //>@如果重命名成功，则修改m_FileName为重命名后的值。
    return flag;
}

bool MdpParser::GetCfgparaGroup()
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
            }
        }
    }
    return true;
}

bool MdpParser::ClrMDP()
{
    if(GetItemElement("/CfgPara"))
    {
        ClrDomCldNodes(itemElement);
        return true;
    }
    return false;
}

bool MdpParser::SetMonitor(quint32 pFreq)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Monitor", QString::number(pFreq));
}

quint32 MdpParser::GetMonitor()
{
    if(!m_Valid)
        return 0;
    bool flag = GetItemElement("/Monitor");
    if(flag == false)
        return 0;
    return itemElement.text().toUInt();
}

bool MdpParser::SetTheme(QString pLocation)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Theme", pLocation);
}

QString MdpParser::GetTheme()
{
    if(!m_Valid)
        return QString();
    bool flag = GetItemElement("/Theme");
    if(flag == false)
        return QString();
    return itemElement.text();
}

bool MdpParser::SetPattern(QString pPattern)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Pattern", pPattern);
}

QString MdpParser::GetPattern()
{
    if(!m_Valid)
        return QString();
    bool flag = GetItemElement("/Pattern");
    if(flag == false)
        return QString();
    return itemElement.text();
}

bool MdpParser::ShowLog(bool pEnable)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Pattern", pEnable?QString("true"):QString("false"));
}

bool MdpParser::CanShowLog()
{
    if(!m_Valid)
        return false;
    bool flag = GetItemElement("/Showlog");
    if(flag)
        return (itemElement.text().compare("false", Qt::CaseInsensitive)==0)?(false):(true);
    return false;
}

void MdpParser::DelDeviceDom(QString pPath)
{
    E_DEV_TYPE tmpDevType = GetDevType(pPath);
    if(tmpDevType == DEV_PHYSICAL)
    {
        PH_Type tmpType = GetPHType(GetFileName(pPath));
        DelDeviceDom(tmpType, pPath);
    }
}

void MdpParser::DelDeviceDom(MW_Type pType, QString pPath)
{
    QDomElement tmpElement = m_CfgparaElementGroup.take(pPath);
    if(tmpElement.isNull())
        return;
    if(pType != M_INVALID)
        m_MWElementGroup.take(pType);
    DelDom(tmpElement);
    m_IsChanged = true;
}

void MdpParser::DelDeviceDom(PH_Type pType, QString pPath)
{
    QDomElement tmpElement = m_CfgparaElementGroup.take(pPath);
    if(tmpElement.isNull())
        return;
    if(pType != P_INVALID)
        m_PHElementGroup.take(pType);
    DelDom(tmpElement);
    m_IsChanged = true;
}

void MdpParser::DelDeviceDom(LG_Type pType, QString pPath)
{
    QDomElement tmpElement = m_CfgparaElementGroup.take(pPath);
    if(tmpElement.isNull())
        return;
    if(pType != L_INVALID)
        m_LGElementGroup.take(pType);
    DelDom(tmpElement);
    m_IsChanged = true;
}

QDomElement MdpParser::AddDeviceDom(QString pPath, QDomElement pInitElement)
{
    if(pPath.isEmpty() || pInitElement.isNull())
        return QDomElement();
    //>@获取父节点
    QString tmpParPath = GetParentPath(pPath);
    QString tmpParName = GetDirName(tmpParPath);
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
        QDomElement tmpSubElement = CreateParaDom(tmpSubInitElement);
        if(tmpSubElement.isNull())
            continue;
        deviceElement.appendChild(tmpSubElement);
    }
    tmpParElement.appendChild(deviceElement);
    m_IsChanged = true;
    m_CfgparaElementGroup.insert(pPath, deviceElement);
    E_DEV_TYPE tmpDevType = GetDevType(pPath);
    if(tmpDevType == DEV_PHYSICAL)
    {
        PH_Type tmpType = GetPHType(GetFileName(pPath));
        if(tmpType != P_INVALID)
            m_PHElementGroup.insert(tmpType, deviceElement);
    }
    return deviceElement;
}

QDomElement MdpParser::CreateParaDom(QDomElement pInitElement)
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

bool MdpParser::ClrDomCldNodes(QDomElement pElement)
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

bool MdpParser::DelDom(QDomElement pElement)
{
    if(pElement.isNull())
        return true;
    if(!ClrDomCldNodes(pElement))
        return false;
    QString key = m_CfgparaElementGroup.key(pElement);
    QDomNode parNode = pElement.parentNode();
    parNode.removeChild(pElement);
    m_CfgparaElementGroup.remove(key);
    return true;
}
