#include "xmlparse.h"
#include "define.h"
const QString XmlParse::XmlHead = "version=\"1.0\" encoding=\"UTF-8\"";
const QString XmlParse::XmlSuffix = ".xml";

XmlParse::XmlParse() :QObject(), QDomDocument()
{
    m_ForceSave = true;
    m_Valid = false;
    m_IsChanged = true;
}

XmlParse::XmlParse(QString fileName) :QObject(), QDomDocument()
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
        {
            m_FileDir.append('\\');
        }
    }
    else
    {
        if(!m_FileDir.endsWith('/'))
        {
            m_FileDir.append('/');
        }
    }
    IDE_DEBUG(QString("%1, %2").arg(m_FileName).arg(m_FileDir));
}

XmlParse::~XmlParse()
{
    if(m_IsChanged && m_ForceSave)
    {
        //IDE_TRACE();
        saveXmlAs(m_FileDir + m_FileName);
    }
    //clear();
}

bool XmlParse::getItemElement(QString itemPath)
{
    //IDE_TRACE_STR(itemPath);
    bool nodeFlag = true;          //是否为/test/subtest/类型的路径
    QString tmpString;
    QDomElement tmpElement, newElement;
    if (itemPath.at(0) != '/')
    {
        IDE_TRACE();
        return false;
    }
    tmpElement = documentElement(); // 读取的入口根据 QDomDocument 调用 documentElement() 返回的根元素。
    int index = 0, indexbk = 0;
    while(index < itemPath.length() - 1)
    {
        index = itemPath.indexOf('/', index+1, Qt::CaseInsensitive);
        if(index < 0)
        {
            nodeFlag = false;
            break;
        }
        tmpString = itemPath.mid(indexbk+1, index-indexbk-1);
        newElement = tmpElement.firstChildElement(tmpString);
        if(newElement.isNull())
        {
            IDE_TRACE_STR(tmpString);
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
            IDE_TRACE_STR(tmpString);
            return false;
        }
        tmpElement = newElement;
    }
    itemElement = tmpElement;
    return true;
}

bool XmlParse::getItemElement(QDomElement itemDomElement, QString itemPath)
{
    bool nodeFlag = true;          //是否为/test/subtest/类型的路径
    QString tmpString;
    QDomElement tmpElement, newElement;
    if (itemPath.at(0) != '/')
    {
        IDE_TRACE();
        return false;
    }
    tmpElement = itemDomElement;
    int index = 0, indexbk = 0;
    while(index < itemPath.length() - 1)
    {
        index = itemPath.indexOf('/', index+1, Qt::CaseInsensitive);
        if(index < 0)
        {
            nodeFlag = false;
            break;
        }
        tmpString = itemPath.mid(indexbk+1, index-indexbk-1);
        newElement = tmpElement.firstChildElement(tmpString);
        if(newElement.isNull())
        {
            IDE_TRACE();
            return false;
        }
        tmpElement = newElement;
        indexbk = index;
    }
    if(false == nodeFlag)
    {
        tmpString = itemPath.mid(indexbk + 1);
        newElement = tmpElement.firstChildElement(tmpString);
        if(newElement.isNull())
        {
            IDE_TRACE();
            return false;
        }
        tmpElement = newElement;
    }
    itemElement = tmpElement;
    return true;
}

QString XmlParse::getItemElementValue()
{
    QDomNode tmpNode = itemElement.firstChild();
    if(tmpNode.isNull() == false)
    {
        return itemElement.text();
    }
    return QString("");
}

QString XmlParse::getItemElementValue(QString itemPath)
{
    if(getItemElement(itemPath))
    {
        return getItemElementValue();
    }
    return QString("");
}

QList<QDomAttr> XmlParse::getItemElementAttrs(QDomElement itemDomElement)
{
    QList<QDomAttr> tmpAttrs;
    QDomNamedNodeMap tmpNodesMap = itemDomElement.attributes();
    for(int i = 0; i<tmpNodesMap.count(); ++i)
    {
        tmpAttrs.append(tmpNodesMap.item(i).toAttr());
    }
    return tmpAttrs;
}

QList<QDomAttr> XmlParse::getItemElementAttrs()
{
    return getItemElementAttrs(itemElement);
}

QList<QDomAttr> XmlParse::getItemElementAttrs(QString itemPath)
{
    QList<QDomAttr> tmpAttrs;
    if(getItemElement(itemPath))
    {
        return getItemElementAttrs();
    }
    return tmpAttrs;
}

bool XmlParse::searchItemElement(QString itemName)
{
    QDomElement docElem = documentElement(); // 读取的入口根据 QDomDocument 调用 documentElement() 返回的根元素。
    QDomNodeList nodes = docElem.elementsByTagName(itemName);
    if(nodes.size() > 0)
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

bool XmlParse::createItemElement(QString itemPath)
{
    //提取待创建的元素路径，并检验路径是否存在，如果不存在则创建
    bool nodeFlag = true;
    QString tmpString;
    QDomElement tmpElement, tmpElement_bk, newElement;
    QDomNode newNode;

    if (itemPath.at(0) != '/')
    {
        IDE_TRACE();
        return false;
    }
    tmpElement = documentElement(); // 读取的入口根据 QDomDocument 调用 documentElement() 返回的根元素。
    int index = 0, indexbk = 0;
    while(index < itemPath.length() - 1)
    {
        index = itemPath.indexOf('/', index+1, Qt::CaseInsensitive);
        if(index < 0)
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
    m_IsChanged = true;
    return true;
}

bool XmlParse::createItemElement(QDomElement pParElement, QString pName, QString pValue)
{
    if(pParElement.isNull())
    {
        return false;
    }
    QDomElement newElement = createElement(pName);
    QDomText newTitleText = createTextNode(pValue);
    newElement.appendChild(newTitleText);
    pParElement.appendChild(newElement);
    return true;
}

bool XmlParse::saveItemElement(QDomElement itemDomElement, QString& pContent)
{
    if(itemDomElement.isNull())
    {
        return false;
    }
    QTextStream out(&pContent);
    itemDomElement.save(out, 0);
    return true;
}

bool XmlParse::saveItemElement(QString& pContent)
{
    return saveItemElement(itemElement, pContent);
}

bool XmlParse::saveItemElement(QDomElement itemDomElement, QIODevice *pDevice)
{
    if(itemDomElement.isNull() || !pDevice)
    {
        return false;
    }
    if(!pDevice->isOpen())
    {
        IDE_TRACE();
        return false;
    }
    QTextStream out(pDevice);
    itemDomElement.save(out, 0);
    return true;
}

bool XmlParse::saveItemElement(QIODevice *pDevice)
{
    return saveItemElement(itemElement, pDevice);
}

bool XmlParse::modifyItemElement(QString itemPath, QString value)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"itemPath = "<<itemPath<<"value = "<<value;
    bool flag = getItemElement(itemPath);
    if(flag == false)
    {
        flag = createItemElement(itemPath);
        if(flag == false)
        {
            IDE_TRACE();
            return false;
        }
    }
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"will call modifyItemElement";
    return modifyItemElement(itemElement, value);
}

bool XmlParse::modifyItemElement(QDomElement itemDomElement, QString value)
{
    IDE_TRACE_STR(itemDomElement.text());
    IDE_TRACE_STR(itemDomElement.tagName());
    if(itemDomElement.isElement() == false)
    {
        IDE_TRACE();
        return false;
    }
    //如果值为空，则表示清空当前text
    //如果已经有文字节点了，则替换，否则添加即可
    QDomNodeList tmpList = itemDomElement.childNodes();
    if(!tmpList.isEmpty())
    {
        for(int i = tmpList.count() - 1; i >= 0; --i)
        {
            QDomNode tmpNode = tmpList.at(i);
            if(tmpNode.isNull())
            {
                continue;
            }
            if(tmpNode.nodeType() == QDomNode::TextNode)
            {
                m_IsChanged = true;
                itemDomElement.removeChild(tmpNode);
                break;
            }
        }
    }
    if(!value.isEmpty())
    {
        QDomText newTitleText = createTextNode(value);
        QDomNode tmpNewNode = itemDomElement.appendChild(newTitleText);
        m_IsChanged = true;
        if(tmpNewNode.isNull())
        {
            IDE_TRACE();
            //return false;
        }
    }
    return true;
}

bool XmlParse::setText(QDomElement itemDomElement, QString value)
{
    return modifyItemElement(itemDomElement, value);
}

bool XmlParse::deleteItemElement(QDomElement itemDomElement)
{
    if(itemDomElement.isNull())
    {
        return true;
    }
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

bool XmlParse::clearItemElement(QDomElement itemDomElement)
{
    if(itemDomElement.isNull())
    {
        return false;
    }
    QDomNodeList tmpList = itemDomElement.childNodes();
    if(tmpList.count())
    {
        for(int i = tmpList.count() - 1; i >= 0; --i)
        {
            QDomNode newNode = itemDomElement.removeChild(tmpList.at(i));
            if(newNode.isNull())
            {
                continue;
            }
            newNode.clear();
        }
    }
    m_IsChanged = true;
    return true;
}

bool XmlParse::writeXml(QIODevice *device)
{
    qDebug()<<__PRETTY_FUNCTION__<<"filename will writefile";
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

bool XmlParse::readXml(QIODevice *device)
{
    if(device->isOpen() == false)
    {
        IDE_TRACE();
        return false;
    }
    /*
     * This function reads the XML document from the IO device dev,
     *  returning true if the content was successfully parsed;
     *  otherwise returns false.
    */
    bool flag = setContent(device, false, &errorStr, &errorLine, &errorColumn);
    if (flag == false)
    {
        IDE_TRACE();
        return false;
    }
    return true;
}

bool XmlParse::openXml()
{
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
    if(m_FileDir.isEmpty() || m_FileName.isEmpty())
    {
        return false;
    }

    if(!m_FileDir.endsWith("/"))
    {
        m_FileDir.append("/");
    }
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
        flag = readXml(&file);
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
        IDE_TRACE_STR(m_FileDir + m_FileName);
        return false;
    }
    return true;
}

bool XmlParse::createXml()
{
    if(m_FileDir.isEmpty() || m_FileName.isEmpty())
    {
        return false;
    }

    QDir dir(m_FileDir);
    dir.mkpath(m_FileDir);

    QFile file(m_FileDir + m_FileName);       //此处只要新建一个空文件即可。
    bool flag = file.exists();
    if(flag == true)
    {
        IDE_TRACE();
        return false;
    }
    flag = file.open(QFile::WriteOnly| QFile::Text);  ///QFile::WriteOnly | QFile::Text);
    if (flag == false)
    {
        IDE_TRACE();
        return false;
    }
    file.close();
    return true;
}

bool cutFile(QString srcFile, QString dstFile)
{
    if(!QFile::copy(srcFile, dstFile))
    {
        return false;
    }
    return true;
}

bool XmlParse::saveXml()
{
    qDebug()<<__PRETTY_FUNCTION__<<"is call";
    if(m_FileDir.isEmpty() || m_FileName.isEmpty())
    {
        return false;
    }
    if(m_IsChanged)
    {
        QString tmpString = m_FileDir + m_FileName + QString("_bk");
        if (QFile::exists(tmpString))
        {
            QFile::remove(tmpString);
        }
        //将当前文件备份
        if(!cutFile(m_FileDir + m_FileName, tmpString))
        {
            IDE_TRACE();
            return false;
        }

        bool flag = saveXmlAs(m_FileDir + m_FileName);
        IDE_TRACE_STR(m_FileDir + m_FileName);
        if(flag == false)
        {
            IDE_TRACE();
            return false;
        }

        m_IsChanged = false;
#if (defined(UBUNTU) || defined(LINUX))
        system("sync");
#endif
        QtSleep(10);
    }
    return true;
}

bool XmlParse::renameXml(QString fileName)
{
    if(m_FileDir.isEmpty() || m_FileName.isEmpty() || fileName.isEmpty())
    {
        return false;
    }
#if 1
    QFile file(m_FileDir + m_FileName);
    file.setPermissions(QFile::ReadOther|QFile::WriteOther|QFile::ExeOther);
    bool flag = file.exists();
    if(flag == false)  //如果文件不存在，则重命名看作成功。
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

bool XmlParse::saveXmlAs(QString fileName)
{
    qDebug()<<__PRETTY_FUNCTION__<<"filename = "<<fileName;
    if(m_FileDir.isEmpty() || m_FileName.isEmpty() || fileName.isEmpty())
    {
        return false;
    }
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
    flag = writeXml(&f);
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
