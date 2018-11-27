#include "xmlparser.h"

const QString XmlParser::XmlHead = "version=\"1.0\" encoding=\"UTF-8\"";
const QString XmlParser::XmlSuffix = ".xml";

XmlParser::XmlParser() :
    QObject(), DomDocument()
{
    m_ForceSave = true;
    m_Valid = false;
    m_IsChanged = true;
}

XmlParser::XmlParser(QString fileName) :
    QObject(), DomDocument()
{
    m_ForceSave = true;
    m_Valid = false;
    m_IsChanged = true;

    QFileInfo tmpFileInfo(fileName);
    m_FileName = tmpFileInfo.fileName();
    m_FileDir = tmpFileInfo.dir().path();
    if(m_FileDir.contains('\\'))
    {
        if(!m_FileDir.endsWith('\\'))
            m_FileDir.append('\\');
    }
    else
    {
        if(!m_FileDir.endsWith('/'))
            m_FileDir.append('/');
    }
}

XmlParser::~XmlParser()
{
    if(m_IsChanged && m_ForceSave)
    {
        IDE_TRACE();
        SaveAs(m_FileDir + m_FileName);
    }
    clear();
}

void XmlParser::RemoveAttribute(QDomElement pElement, QString pName)
{
    if(pElement.hasAttribute(pName))
    {
        pElement.removeAttribute(pName);
        m_IsChanged = true;
    }
}

void XmlParser::SetAttribute(QDomElement pElement, QString pName, QString pValue)
{
    QString tmpValue = pElement.attribute(pName);
    if(tmpValue.compare(pValue, Qt::CaseInsensitive))
    {
        pElement.setAttribute(pName, pValue);
        if(!m_IsChanged)
            m_IsChanged = true;
    }
}

bool XmlParser::GetItemElement(QString itemPath)
{
    bool nodeFlag = true;          //>@是否为/test/subtest/类型的路径
    QString tmpString;
    QDomElement tmpElement, newElement;
    if (itemPath.at(0) != '/')
    {
        //IDE_TRACE();
        return false;
    }
    tmpElement = documentElement();
    int index = 0, indexbk = 0;
    while(index < itemPath.length()-1)
    {
        index = itemPath.indexOf('/', index+1, Qt::CaseInsensitive);
        if(index<0)
        {
            nodeFlag = false;
            break;
        }
        tmpString = itemPath.mid(indexbk+1, index-indexbk-1);
        newElement = tmpElement.firstChildElement(tmpString);
        if(newElement.isNull())
        {
            //IDE_TRACE();
            return false;
        }
        tmpElement = newElement;
        indexbk = index;
    }
    if(false == nodeFlag)
    {
        tmpString = itemPath.mid(indexbk+1);
        newElement = tmpElement.firstChildElement(tmpString);
        if(newElement.isNull())
        {
            //IDE_TRACE();
            return false;
        }
        tmpElement = newElement;
    }
    itemElement = tmpElement;
    return true;
}

bool XmlParser::GetItemElement(QDomElement itemDomElement, QString itemPath)
{
    bool nodeFlag = true;          //>@是否为/test/subtest/类型的路径
    QString tmpString;
    QDomElement tmpElement, newElement;
    if (itemPath.at(0) != '/')
    {
        //IDE_TRACE();
        return false;
    }
    tmpElement = itemDomElement;
    int index = 0, indexbk = 0;
    while(index < itemPath.length()-1)
    {
        index = itemPath.indexOf('/', index+1, Qt::CaseInsensitive);
        if(index<0)
        {
            nodeFlag = false;
            break;
        }
        tmpString = itemPath.mid(indexbk+1, index-indexbk-1);
        newElement = tmpElement.firstChildElement(tmpString);
        if(newElement.isNull())
        {
            //IDE_TRACE();
            return false;
        }
        tmpElement = newElement;
        indexbk = index;
    }
    if(false == nodeFlag)
    {
        tmpString = itemPath.mid(indexbk+1);
        newElement = tmpElement.firstChildElement(tmpString);
        if(newElement.isNull())
        {
            //IDE_TRACE();
            return false;
        }
        tmpElement = newElement;
    }
    itemElement = tmpElement;
    return true;
}




bool XmlParser::CreateElementPath(QDomElement itemDomElement, QString itemPath)
{
    QString tmpString;
    QDomElement tmpElement, newElement;

    if (itemPath.at(0) != '/'){
        return false;
    }

    if(!itemPath.endsWith("/")){
        itemPath.append("/");
    }

    tmpElement = itemDomElement;
    int index = 0, indexbk = 0;

    while(index < itemPath.length()-1)
    {
        index = itemPath.indexOf('/', index+1, Qt::CaseInsensitive);
        if(index < 0){
            break;
        }
        tmpString = itemPath.mid(indexbk+1, index-indexbk-1);
        newElement = tmpElement.firstChildElement(tmpString);

        if(newElement.isNull()){
            bool flag = CreateItemElement(tmpElement,tmpString);

            if(flag){
                if(GetItemElement(tmpElement,"/"+tmpString)){
                    newElement = itemElement;
                }else{
                    return false;
                }
            }else{
                return false;
            }
        }
        tmpElement = newElement;
        indexbk = index;
    }

    itemElement = tmpElement;
    return true;
}

QString XmlParser::GetItemElementValue()
{
    QDomNode tmpNode = itemElement.firstChild();
    if(tmpNode.isNull() == false)
        return itemElement.text();
    return QString("");
}

QString XmlParser::GetItemElementValue(QString itemPath)
{
    if(GetItemElement(itemPath))
    {
        return GetItemElementValue();
    }
    return QString("");
}

QList<QDomAttr> XmlParser::GetItemElementAttrs(QDomElement pItemDomElement)
{
    QList<QDomAttr> tmpAttrs;
    QDomNamedNodeMap tmpNodesMap = pItemDomElement.attributes();
    for(int i=0;i<tmpNodesMap.count();i++)
        tmpAttrs.append(tmpNodesMap.item(i).toAttr());
    return tmpAttrs;
}

QList<QDomAttr> XmlParser::GetItemElementAttrs()
{
    return GetItemElementAttrs(itemElement);
}

QList<QDomAttr> XmlParser::GetItemElementAttrs(QString itemPath)
{
    QList<QDomAttr> tmpAttrs;
    if(GetItemElement(itemPath))
        return GetItemElementAttrs();
    return tmpAttrs;
}

bool XmlParser::SearchItemElement(QString itemName)
{
    QDomElement docElem = documentElement();
    QDomNodeList nodes=docElem.elementsByTagName(itemName);
    if(nodes.size()>0)
    {
        QDomElement tmpElement = nodes.at(0).toElement();
        if(tmpElement.isNull())
        {
            IDE_TRACE();
            return false;
        }
        itemElement = tmpElement;
        return true;
    }
    return false;
}

bool XmlParser::CreateItemElement(QString itemPath)
{
    //>@提取待创建的元素路径，并检验路径是否存在，如果不存在则创建
    bool nodeFlag = true;
    QString tmpString;
    QDomElement tmpElement, tmpElement_bk, newElement;
    QDomNode newNode;

    if (itemPath.at(0) != '/')
    {
        IDE_TRACE();
        return false;
    }
    tmpElement = documentElement();
    int index = 0, indexbk = 0;
    while(index < itemPath.length()-1)
    {
        index = itemPath.indexOf('/', index+1, Qt::CaseInsensitive);
        if(index<0)
        {
            nodeFlag = false;
            break;
        }
        tmpString = itemPath.mid(indexbk+1, index-indexbk-1);
        tmpElement = tmpElement.firstChildElement(tmpString);
        if(tmpElement.isNull())
        {
            newElement = createElement(tmpString);
            newNode = tmpElement_bk.appendChild(newElement);
            m_IsChanged = true;
            tmpElement = newNode.toElement();
            if(tmpElement.isNull())
            {
                IDE_TRACE();
                return false;
            }
        }
        indexbk = index;
        tmpElement_bk = tmpElement;
    }
    if(!nodeFlag)
    {
        tmpString = itemPath.mid(indexbk+1);
        tmpElement = tmpElement.firstChildElement(tmpString);
        if(tmpElement.isNull())
        {
            newElement = createElement(tmpString);
            newNode = tmpElement_bk.appendChild(newElement);
            m_IsChanged = true;
            tmpElement = newNode.toElement();
            if(tmpElement.isNull())
            {
                IDE_TRACE();
                return false;
            }
        }
    }
    itemElement = tmpElement;
    return true;
}

bool XmlParser::CreateItemElement(QDomElement pParElement, QString pName, QString pValue)
{
    if(pParElement.isNull())
        return false;
    if(GetItemElement(pParElement,pName))
        return true;
    QDomElement newElement = createElement(pName);
    QDomText newTitleText = createTextNode(pValue);
    newElement.appendChild(newTitleText);
    pParElement.appendChild(newElement);
    m_IsChanged = true;
    return true;
}

bool XmlParser::SaveItemElement(QDomElement itemDomElement, QString& pContent)
{
    if(itemDomElement.isNull())
        return false;
    QTextStream out(&pContent);
    itemDomElement.save(out, 0);
    return true;
}

bool XmlParser::SaveItemElement(QString& pContent)
{
    return SaveItemElement(itemElement, pContent);
}

bool XmlParser::SaveItemElement(QDomElement itemDomElement, QIODevice *pDevice)
{
    if(itemDomElement.isNull() || !pDevice)
        return false;
    if(!pDevice->isOpen())
    {
        IDE_TRACE();
        return false;
    }
    QTextStream out(pDevice);
    itemDomElement.save(out, 0);
    return true;
}

bool XmlParser::SaveItemElement(QIODevice *pDevice)
{
    return SaveItemElement(itemElement, pDevice);
}

bool XmlParser::ModifyItemElement(QString itemPath, QString value)
{
    bool flag = GetItemElement(itemPath);
    if(flag == false)
    {
        flag = CreateItemElement(itemPath);
        if(flag == false)
        {
            IDE_TRACE();
            return false;
        }
    }
    return ModifyItemElement(itemElement, value);
}

bool XmlParser::ModifyItemElement(QDomElement pItemDomElement, QString value)
{
    if(!pItemDomElement.isElement())
    {
        IDE_TRACE();
        return false;
    }
    //>@如果值为空，则表示清空当前text
    //>@如果已经有文字节点了，则替换，否则添加即可
    QDomNodeList tmpList = pItemDomElement.childNodes();
    if(!tmpList.isEmpty())
    {
        for(int i=tmpList.count()-1;i>=0;i--)
        {
            QDomNode tmpNode = tmpList.at(i);
            if(tmpNode.isNull())
                continue;
            if(tmpNode.nodeType() == QDomNode::TextNode)
            {
                m_IsChanged = true;
                pItemDomElement.removeChild(tmpNode);
                break;
            }
        }
    }
    if(!value.isEmpty())
    {
        QDomText newTitleText = createTextNode(value);
        pItemDomElement.appendChild(newTitleText);
        m_IsChanged = true;
    }
    return true;
}

bool XmlParser::SetText(QDomElement itemDomElement, QString value)
{
    return ModifyItemElement(itemDomElement, value);
}

bool XmlParser::DeleteItemElement(QDomElement itemDomElement)
{
    if(itemDomElement.isNull())
        return true;
    QDomNode tmpParentNode = itemDomElement.parentNode();
    if(tmpParentNode.isNull())
    {
        IDE_TRACE();
        return false;
    }
    QDomNode newNode = tmpParentNode.removeChild(itemDomElement);
    if(newNode.isNull())
    {
        IDE_TRACE();
        return false;
    }
    newNode.clear();
    m_IsChanged = true;
    return true;
}

bool XmlParser::ClearItemElement(QDomElement itemDomElement)
{
    if(itemDomElement.isNull())
        return false;
    QDomNodeList tmpList = itemDomElement.childNodes();
    if(tmpList.count())
    {
        for(int i=tmpList.count()-1;i>=0;i--)
        {
            QDomNode newNode = itemDomElement.removeChild(tmpList.at(i));
            if(newNode.isNull())
                continue;
            m_IsChanged = true;
            newNode.clear();
        }
    }
    return true;
}

bool XmlParser::write(QIODevice *device)
{
    const int IndentSize = 4;
    if(device->isOpen() == false)
    {
        IDE_TRACE();
        return false;
    }
    QTextStream out(device);
    save(out, IndentSize);
    return true;
}

bool XmlParser::read(QIODevice *device)
{
    if(device->isOpen() == false)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = setContent(device, false, &errorStr, &errorLine, &errorColumn);
    if (flag == false)
    {
        IDE_TRACE();
        return false;
    }
    return true;
}

bool XmlParser::Open()
{
    if(m_FileDir.isEmpty() || m_FileName.isEmpty())
        return false;

    if(!m_FileDir.endsWith("/"))
        m_FileDir.append("/");
    QFile file(m_FileDir + m_FileName);
    bool flag = file.exists();
    if(flag == true)
    {
        flag = file.open(QFile::ReadOnly | QFile::Text);
        if (flag == false)
        {
            IDE_TRACE();
            return false;
        }
        flag = read(&file);
        if(flag == false)
        {
            file.close();
            IDE_TRACE();
            return false;
        }
    }
    else
    {
        IDE_TRACE();
        return false;
    }
    file.close();
    return true;
}

bool XmlParser::New()
{
    if(m_FileDir.isEmpty() || m_FileName.isEmpty())
        return false;

    QDir dir(m_FileDir);
    dir.mkpath(m_FileDir);

    QFile file(m_FileDir + m_FileName);       //<@此处只要新建一个空文件即可。
    bool flag = file.exists();
    if(flag == true)
    {
        IDE_TRACE();
        return false;
    }
    flag = file.open(QFile::WriteOnly | QFile::Text);
    if (flag == false)
    {
        IDE_TRACE();
        return false;
    }
    file.close();
    return true;
}

bool XmlParser::Save()
{
    if(m_FileDir.isEmpty() || m_FileName.isEmpty())
        return false;
    if(m_IsChanged)
    {
        //将当前文件备份
        QString tmpString = m_FileDir +m_FileName + QString("_bk");
        bool flag = SaveAs(tmpString);
        if(flag == false)
        {
            IDE_TRACE();
            return false;
        }
        if(!CutFile(tmpString, m_FileDir+m_FileName))
        {
            IDE_TRACE();
            return false;
        }
        //IDE_TRACE_STR(m_FileDir+m_FileName);
        m_IsChanged = false;
#if (defined(UBUNTU) || defined(LINUX))
        system("sync");
#endif
        QtSleep(10);
    }
    return true;
}

bool XmlParser::Rename(QString fileName)
{
    if(m_FileDir.isEmpty() || m_FileName.isEmpty() || fileName.isEmpty())
        return false;
#if 1
    QFile file(m_FileDir + m_FileName);
    file.setPermissions(QFile::ReadOther|QFile::WriteOther|QFile::ExeOther);
    bool flag = file.exists();
    if(flag == false)  //>@如果文件不存在，则重命名看作成功。
    {
        return true;
    }
    flag = file.rename(m_FileDir + fileName);
    return flag;
#else
    QFile file(m_FileDir + m_FileName);
    file.copy(m_FileDir + fileName);
    file.remove();
#endif
}

bool XmlParser::SaveAs(QString fileName)
{
    if(m_FileDir.isEmpty() || m_FileName.isEmpty() || fileName.isEmpty())
        return false;
    if(isNull())
    {
        IDE_TRACE();
        return false;
    }
    QFile f(fileName);
    bool flag = f.open(QIODevice::WriteOnly | QIODevice::Text);
    if (flag == false)
    {
        IDE_TRACE();
        return false;
    }
    flag = write(&f);
    if(flag == false)
    {
        IDE_TRACE();
        f.close();
        return false;
    }
    f.close();
    m_IsChanged = false;
    return true;
}

