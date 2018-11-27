#include "themeparser.h"

const QString ThemeParser::UiSuffix = ".xml";
const QString ThemeParser::UiFileType = "theme_define_file";

ThemeParser::ThemeParser(QString fileName):XmlParse(fileName)
{
    int tmpProNum, tmpSuffixNum;
    tmpProNum = m_FileName.length();
    tmpSuffixNum = UiSuffix.length();
    if (tmpProNum <= tmpSuffixNum)
    {
        IDE_TRACE_INT(tmpProNum);
        return;
    }
    QString suffix = m_FileName.mid(tmpProNum-tmpSuffixNum, tmpSuffixNum);
    if (suffix != UiSuffix)
        m_FileName = QString("%1%2").arg(m_FileName).arg(UiSuffix);
}

bool ThemeParser::createUi()
{
    bool flag = createXml();
    if (flag == false)
    {
        IDE_TRACE();
        m_Valid = false;
        return false;
    }
    m_Valid = true;
    m_ComElementGroup.clear();

    appendChild(createProcessingInstruction("xml", XmlHead));

    QDomElement rootElement;
    rootElement = createElement("root");
    appendChild(rootElement);

    QDomElement tmpElement = createElement("filetype");
    tmpElement.appendChild(createTextNode(UiFileType));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("createtime");
    QDateTime time = QDateTime::currentDateTime();
    tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("lastmodiTime");
    tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("solution");
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("direction");
    tmpElement.appendChild(createTextNode("hv"));
    m_DirectionPermisson = HVBoth;
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("ui");
    QDomElement tmpChildElement = createElement("component");
    tmpElement.appendChild(tmpChildElement);
    rootElement.appendChild(tmpElement);

    m_IsChanged = true;
    saveXml();

    getComponentGroup();
    return true;
}

bool ThemeParser::openUi()
{
    bool flag = openXml();
    if (flag == false)
    {
        m_Valid = false;
        return false;
    }
    m_Valid = true;
    //Check FileType
    flag = getItemElement("/filetype");
    if (flag == false)
    {
        m_Valid = false;
        return false;
    }
    QString tmpString = itemElement.text();
    if (tmpString.compare(UiFileType, Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    m_DirectionPermisson = getDirectionPermisson();
    getComponentGroup();
    return true;
}

bool ThemeParser::closeUi()
{
    saveUi();
    return true;
}

bool ThemeParser::delUi()
{
    QFile file(m_FileDir + m_FileName);
    if (file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool ThemeParser::saveUi()
{
    QDateTime time = QDateTime::currentDateTime();
    modifyItemElement("/lastmoditime", time.toString("yyyy-MM-dd HH:mm:ss"));
    return saveXml();
}

bool ThemeParser::renameUi(QString FileName)
{
    QString tmpString = FileName;
    if (!tmpString.endsWith(UiSuffix))
    {
        tmpString.append(UiSuffix);
    }
    bool flag = renameXml(tmpString);
    if (flag)
    {
        m_FileName = tmpString;     ///如果重命名成功，则修改m_FileName为重命名后的值。
    }
    return flag;
}

bool ThemeParser::isValid()
{
    if (!m_Valid)
    {
        return false;
    }
    if (m_FileDir.endsWith('/') == false)
    {
        m_FileDir.append('/');
    }
    QDir tmpDir(m_FileDir);
    if (tmpDir.exists() == false)
    {
        return tmpDir.mkpath(m_FileDir);
    }
    return true;
}

QString ThemeParser::getSolution()
{
    return getItemElementValue("/solution");
}

bool ThemeParser::setSolution(QString pSolution)
{
    return modifyItemElement("/solution", pSolution);
}

DIRECTION ThemeParser::getDirectionPermisson()
{
    return getDirection(getItemElementValue("/direction"));
}

bool ThemeParser::setDirectionPermisson(DIRECTION pDirection)
{
    if (!m_Valid)
    {
        return false;
    }
    QString tmpDirString = getDirectionString(pDirection);
    if (tmpDirString.isEmpty())
    {
        return false;
    }
    m_DirectionPermisson = pDirection;
    return modifyItemElement("/direction", tmpDirString);
}

void ThemeParser::getComponentGroup()
{
    if (!m_Valid)
    {
        IDE_TRACE();
        return;
    }
    bool flag = getItemElement("/ui/desktop");
    if (flag)
    {
        m_ComElementGroup.clear();
        m_ComElementGroup.insert(COM_DESKTOP, itemElement);
    }
    else
    {
        IDE_TRACE();
        return;
    }

    flag = getItemElement("/ui/component");
    if (flag)
    {
        QDomElement tmpElement;
        QDomNodeList SubuiNodeList = itemElement.childNodes();
        for(int i = 0; i < SubuiNodeList.count(); ++i)
        {
            tmpElement = SubuiNodeList.at(i).toElement();
            QString tmpString = tmpElement.tagName();
            COM_TYPE tmpComType = getComponetType(tmpString);
            if (tmpComType < COM_DESKTOP)
            {
                continue;
            }
            m_ComElementGroup.insert(tmpComType, tmpElement);
//            QString tmpControlString = QString("/ui/component/%1/control").arg(tmpString);
//            flag = getItemElement(tmpControlString);
//            if (flag)
//            {
//                QDomNodeList ControlNodeList = itemElement.childNodes();
//                for(int i=0;i<ControlNodeList.count();++i)//插入subui的子control元素
//                {
//                    tmpElement = ControlNodeList.at(i).toElement();
//                    QString tmpSubString = tmpElement.tagName();
//                    COM_TYPE tmpSubComType = getComponetType(tmpSubString);
//                    if (tmpSubComType < COM_DESKTOP){
//                        continue;
//                    }
//                    m_ComElementGroup.insert(tmpSubComType, tmpElement);
//                }
//            }
        }
    }
}

bool ThemeParser::getComRect(int pLayout, QDomElement pComElement, QRect &pHRect, QRect &pVRect)
{
    if (pComElement.isNull())
    {
        return false;
    }
    QString tmpRectName;
    if (pLayout <= 0)
    {
        tmpRectName = QString("rect");
    }
    else
    {
        tmpRectName = QString("rect%1").arg(pLayout);
    }
    QDomElement tmpRectElement = pComElement.firstChildElement(tmpRectName);
    if (tmpRectElement.isNull())
    {
        tmpRectElement = pComElement.firstChildElement(QString("rect"));
        if (tmpRectElement.isNull())
        {
            return false;
        }
    }
    QDomElement tmpElement = tmpRectElement.firstChildElement("horizontal");
    if (tmpElement.isNull())
    {
        return false;
    }
    pHRect = getRectFromString(tmpElement.text());
    tmpElement = tmpRectElement.firstChildElement("vertical");
    if (tmpElement.isNull())
    {
        return false;
    }
    pVRect = getRectFromString(tmpElement.text());
    return true;
}

QRect ThemeParser::getComRect(int pLayout, QDomElement pComElement, DIRECTION pDirection)
{
    if (pComElement.isNull())
    {
        return QRect();
    }
    QString tmpRectName;
    if (pLayout <= 0)
    {
        tmpRectName = QString("rect");
    }
    else
    {
        tmpRectName = QString("rect%1").arg(pLayout);
    }
    QDomElement tmpRectElement = pComElement.firstChildElement(tmpRectName);
    if (tmpRectElement.isNull())
    {
        tmpRectElement = pComElement.firstChildElement(QString("rect"));
        if (tmpRectElement.isNull())
        {
            return QRect();
        }
    }
    QDomElement tmpElement;
    if (pDirection == Horizontal)
    {
        tmpElement = tmpRectElement.firstChildElement("horizontal");
    }
    else
    {
        tmpElement = tmpRectElement.firstChildElement("vertical");
    }
    if (tmpElement.isNull())
    {
        return QRect();
    }
    return getRectFromString(tmpElement.text());
}

bool ThemeParser::modifyComRect(int pLayout, QDomElement pComElement, QRect pRect, DIRECTION pDirection, bool pForceNew)
{
    if (pComElement.isNull() || pRect.isNull())
    {
        return false;
    }
    COM_TYPE tmpComType = getComponetType(pComElement.tagName());
    if (tmpComType < COM_DESKTOP)
    {
        return false;
    }
    QString tmpRectName;
    if (pLayout <= 0)
    {
        tmpRectName = QString("rect");
    }
    else
    {
        tmpRectName = QString("rect%1").arg(pLayout);
    }
    QDomElement tmpRectElement = pComElement.firstChildElement(tmpRectName);
    if (tmpRectElement.isNull())
    {
        if (pForceNew)
        {
            tmpRectElement = createElement("rect");
            if (tmpRectElement.isNull())
            {
                return false;
            }
            pComElement.appendChild(tmpRectElement);
        }
        else
        {
            tmpRectElement = pComElement.firstChildElement(QString("rect"));
            if (tmpRectElement.isNull())
            {
                return false;
            }
        }
    }
    ///
    if (pRect.left() < 0 || pRect.top() < 0
        || pRect.width() <= 0 || pRect.height() <= 0)
    {
        IDE_TRACE();
        return false;
    }
    ///查看横竖坐标是否存在，不存在则创建
    ///在同步修改另一种类型坐标时，只修改宽高度，不修改左上角坐标；
    ///并且如果是新插入坐标，则把左上角坐标设为0；如果是修改坐标，则保持左上角坐标不变
    QDomElement tmpHElement = tmpRectElement.firstChildElement("horizontal");
    QDomElement tmpVElement = tmpRectElement.firstChildElement("vertical");
    if (tmpHElement.isNull())
    {
        tmpHElement = createElement("horizontal");
        tmpRectElement.appendChild(tmpHElement);
    }
    if (tmpVElement.isNull())
    {
        tmpVElement = createElement("vertical");
        tmpRectElement.appendChild(tmpVElement);
    }
    QString tmpHOldString = tmpHElement.text();
    QString tmpVOldString = tmpVElement.text();
    QString tmpHString, tmpVString;
    if (pDirection == Horizontal)
    {
        tmpHString = QString("%1,%2,%3,%4").arg(pRect.left()).arg(pRect.top()).arg(pRect.width()).arg(pRect.height());
        if (!modifyItemElement(tmpHElement, tmpHString))
        {
            IDE_TRACE_STR(tmpHString);
            return false;
        }
        if (canSwitch())
        {
            QRect tmpRect = getRectFromString(tmpVOldString);
            if (tmpComType == COM_DESKTOP)
            {
                if (tmpRect.isNull())
                {
                    tmpVString = QString("0,0,%1,%2").arg(pRect.height()).arg(pRect.width());
                }
                else
                {
                    tmpVString = QString("%1,%2,%3,%4").arg(tmpRect.left()).arg(tmpRect.top())
                            .arg(pRect.height()).arg(pRect.width());
                }
            }
            else
            {
                if (tmpRect.isNull())
                {
                    tmpVString = QString("0,0,%1,%2").arg(pRect.width()).arg(pRect.height());
                }
                else
                {
                    tmpVString = QString("%1,%2,%3,%4").arg(tmpRect.left()).arg(tmpRect.top())
                            .arg(pRect.width()).arg(pRect.height());
                }
            }
            if (!modifyItemElement(tmpVElement, tmpVString))
            {
                IDE_TRACE_STR(tmpVString);
                modifyItemElement(tmpHElement, tmpHOldString);
                return false;
            }
        }
    }
    else if (pDirection == Vertical)
    {
        tmpVString = QString("%1,%2,%3,%4").arg(pRect.left()).arg(pRect.top()).arg(pRect.width()).arg(pRect.height());
        if (!modifyItemElement(tmpVElement, tmpVString))
        {
            IDE_TRACE_STR(tmpVString);
            return false;
        }
        if (canSwitch())
        {
            QRect tmpRect = getRectFromString(tmpHOldString);
            if (tmpComType == COM_DESKTOP)
            {
                if (tmpRect.isNull())
                {
                    tmpHString = QString("0,0,%1,%2").arg(pRect.height()).arg(pRect.width());
                }
                else
                {
                    tmpHString = QString("%1,%2,%3,%4").arg(tmpRect.left()).arg(tmpRect.top())
                            .arg(pRect.height()).arg(pRect.width());
                }
            }
            else
            {
                if (tmpRect.isNull())
                {
                    tmpHString = QString("0,0,%1,%2").arg(pRect.width()).arg(pRect.height());
                }
                else
                {
                    tmpHString = QString("%1,%2,%3,%4").arg(tmpRect.left()).arg(tmpRect.top())
                            .arg(pRect.width()).arg(pRect.height());
                }
            }
            if (!modifyItemElement(tmpHElement, tmpHString))
            {
                IDE_TRACE_STR(tmpHString);
                modifyItemElement(tmpVElement, tmpVOldString);
                return false;
            }
        }
    }
    return true;
}


QDomNodeList ThemeParser::getComStateList(QDomElement pComElement)
{
    if (pComElement.isNull())
    {
        return QDomNodeList();
    }
    QDomElement tmpSMElement = pComElement.firstChildElement("statemachine");
    if (tmpSMElement.isNull())
    {
        return QDomNodeList();
    }
    return tmpSMElement.childNodes();
}

QDomNodeList ThemeParser::getComRcList(QDomElement pComElement)
{
    if (pComElement.isNull())
    {
        return QDomNodeList();
    }
    QDomElement tmpRcElement = pComElement.firstChildElement("resource");
    if (tmpRcElement.isNull())
    {
        return QDomNodeList();
    }
    return tmpRcElement.childNodes();
}

QDomElement ThemeParser::getComRcElement(QDomElement pComElement)
{
    if (pComElement.isNull())
    {
        return QDomElement();
    }
    QDomElement tmpRcElement = pComElement.firstChildElement("resource");
    if (tmpRcElement.isNull())
    {
        return QDomElement();
    }
    return tmpRcElement;
}

QDomElement ThemeParser::getComState(QDomElement pComElement, QString pStateName, bool pCreateFlag)
{
    if (pComElement.isNull() || pStateName.isEmpty())
    {
        return QDomElement();
    }
    QDomElement tmpSMElement = pComElement.firstChildElement("statemachine");
    if (tmpSMElement.isNull())
    {
        return QDomElement();
    }
    QDomElement tmpStateElement = tmpSMElement.firstChildElement(pStateName);
    if (pCreateFlag && tmpStateElement.isNull())
    {
        tmpStateElement = createElement(pStateName);
        //tmpStateElement.setAttribute(D_EFFECTTYPE, "signalmap");
        tmpSMElement.appendChild(tmpStateElement);
    }
    return tmpStateElement;
}

bool ThemeParser::modifyEffectType(QDomElement pStateElement, QString pEffectType)
{
    if (pStateElement.isNull())
    {
        return false;
    }
    if (!pEffectType.isEmpty())
    {
        QString tmpEffectType = pStateElement.attribute(D_EFFECTTYPE);
        if (pEffectType.compare(tmpEffectType))
        {
            clearItemElement(pStateElement);
            pStateElement.setAttribute(D_EFFECTTYPE, pEffectType);
            pStateElement.removeAttribute(D_EFFECTPARA);
            return true;
        }
    }
    return true;
}

QDomElement ThemeParser::modifyComState(QDomElement pComElement, QString pStateName, QString pEffectType, QString pEffectPara)
{
    if (pComElement.isNull() || pStateName.isEmpty())
    {
        return QDomElement();
    }
    QDomElement tmpSMElement = pComElement.firstChildElement("statemachine");
    if (tmpSMElement.isNull())
    {
        ///不存在，则创建
        tmpSMElement = createElement("statemachine");
        if (tmpSMElement.isNull())
        {
            return QDomElement();
        }
        pComElement.appendChild(tmpSMElement);
    }
    QDomElement tmpStateElement = tmpSMElement.firstChildElement(pStateName);
    if (tmpStateElement.isNull())
    {
        ///不存在，则创建
        tmpStateElement = createElement(pStateName);
        if (tmpStateElement.isNull())
        {
            return QDomElement();
        }
        tmpSMElement.appendChild(tmpStateElement);
    }
    ///修改特效名，如果修改成功，则更新参数。
    ///注意，如果待修改的特效名相同，则认为修改不成功
    if (modifyEffectType(tmpStateElement, pEffectType))
    {
        modifyEffectPara(tmpStateElement, pEffectPara);
    }
    return tmpStateElement;
}

bool ThemeParser::modifyEffectPara(QDomElement pStateElement, QString pEffectPara)
{
    if (pStateElement.isNull())
    {
        return false;
    }
    if (!pEffectPara.isEmpty())
    {
        QString tmpOldEffectPara = pStateElement.attribute(D_EFFECTPARA);
        if (tmpOldEffectPara.compare(pEffectPara))
        {
            QStringList tmpParaList = pEffectPara.split(';');
            QStringList tmpOldParaList = tmpOldEffectPara.split(';');
            for(int m=0;m<tmpParaList.count();m++)
            {
                QString tmpPara = tmpParaList.at(m);
                QStringList tmpValueList = tmpPara.split(':');
                if (tmpValueList.count()!=2)
                {
                    continue;
                }
                QString tmpName = tmpValueList.at(0);
                if (tmpName.isEmpty())
                {
                    continue;
                }
                for(int n=0;n<tmpOldParaList.count();n++)
                {
                    QString tmpOldPara = tmpOldParaList.at(n);
                    if (tmpOldPara.startsWith(tmpName))
                    {
                        if (tmpPara.contains('@'))
                        {
                            tmpOldParaList.replace(n, tmpPara);
                        }
                        else if (tmpOldPara.contains('@'))
                        {
                            tmpOldParaList.replace(n, tmpPara + QString('@') + tmpOldPara.split('@').last());
                        }
                        else
                        {
                            tmpOldParaList.replace(n, tmpPara);
                        }
                        break;
                    }
                }
            }
            QString tmpNewString = tmpOldParaList.join(QString(";"));
            pStateElement.setAttribute(D_EFFECTPARA, tmpNewString);
            return true;
        }
    }
    return true;  ///如果不需要修改，则正确
}

QDomElement ThemeParser::getParComDom(QString pComPath)
{
    COM_TYPE tmpType = getParentComType(getCurComType(pComPath));
    return m_ComElementGroup.value(tmpType);
}

QDomElement ThemeParser::getParComDom(QDomElement pComElement)
{
    QDomElement tmpElement = pComElement.parentNode().toElement();
    if (tmpElement.isNull())
    {
        return QDomElement();
    }
    QString tmpName = tmpElement.tagName();
    if (tmpName.compare("ui") == 0)
    {
        return QDomElement();
    }
    else if (tmpName.compare("control") == 0)
    {
        return tmpElement.parentNode().toElement();
    }
    else if (tmpName.compare("component") == 0)
    {
        return tmpElement.parentNode().firstChildElement("desktop");
    }
    return QDomElement();
}

bool ThemeParser::getParentElement(QDomElement &pParentElement, QString pComPath)
{
    ///规整父节点路径
    COM_TYPE tmpType = getCurComType(pComPath);
    if (tmpType == COM_DESKTOP)
    {
        if (!getItemElement(QString("/ui")))
        {
            if (!createItemElement(QString("/ui")))
            {
                return false;
            }
        }
    }
    else if (tmpType > COM_STARTDEF && tmpType < COM_ENDDEF)
    {
        ///如果没有桌面，则认为没有父
        QDomElement tmpElement = m_ComElementGroup.value(COM_DESKTOP);
        if (tmpElement.isNull())
        {
            return false;
        }
        if (!getItemElement(QString("/ui/component")))
        {
            if (!createItemElement(QString("/ui/component")))
            {
                return false;
            }
        }
    }
    else if (tmpType > COM_ENDDEF)
    {
        COM_TYPE tmpParType = getParentComType(tmpType);
        if (tmpParType < COM_STARTDEF)
        {
            return false;
        }
        ///如果没有父控件，则返回假
        QDomElement parentElement = m_ComElementGroup.value(tmpParType);
        if (parentElement.isNull())
        {
            return false;
        }
        QString tmpComString = QString("/ui/component/%1/control").arg(parentElement.tagName());
        if (getItemElement(tmpComString) == false)
        {
            if (!createItemElement(tmpComString))
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }
    pParentElement = itemElement;
    return true;
}

QDomElement ThemeParser::addComDom(COM_TYPE pComType, QRect pRect, DIRECTION pDirection)
{
    if (pComType < COM_DESKTOP)
    {
        return QDomElement();
    }
    ///如果已经有此控件，返回真，不再重复创建
    QDomElement tmpComElement = m_ComElementGroup.value(pComType);
    if (!tmpComElement.isNull())
    {
        return tmpComElement;
    }
    ///
    if (pRect.isNull())
    {
        return QDomElement();
    }
    ///如果没有父控件，则退出，但是对于Desktop需要单独处理
    if (pComType > COM_DESKTOP)
    {
        QDomElement tmpParElement = m_ComElementGroup.value(getParentComType(pComType));
        if (tmpParElement.isNull())
        {
            return QDomElement();
        }
    }
    ///
    QDomElement tmpParent;
    if (!getParentElement(tmpParent, getComponetPath(pComType)))
    {
        return QDomElement();
    }
    QString tmpComName = getComponetName(pComType);
    tmpComElement = createElement(tmpComName);
    if (tmpComElement.isNull())
    {
        return QDomElement();
    }
    ///添加Rect
    if (!modifyComRect(0, tmpComElement, pRect, pDirection, true))
    {
        return QDomElement();
    }
    tmpParent.appendChild(tmpComElement);
    m_IsChanged = true;
    m_ComElementGroup.insert(pComType, tmpComElement);
    return tmpComElement;
}

bool ThemeParser::deleteComDom(COM_TYPE pComType)
{
    QDomElement tmpElement = m_ComElementGroup.take(pComType);
    if (tmpElement.isNull())
    {
        return true;
    }
    return deleteItemElement(tmpElement);
}

bool ThemeParser::saveRelativeComDom(QDomElement pComElement)
{
    if (!isValid())
    {
        return false;
    }
    QString tmpRelativePath = m_FileDir;
    COM_TYPE tmpType = getComponetType(pComElement.tagName());
    QString tmpComPath = getComponetPath(tmpType);
    QDomNodeList tmpStateList = getComStateList(pComElement);
    for(int i = 0; i < tmpStateList.count(); ++i)
    {
        QDomElement tmpStateElement = tmpStateList.at(i).toElement();
        if (tmpStateElement.isNull())
        {
            continue;
        }
        QString tmpStateName = tmpStateElement.tagName();
        QString tmpEffectName = tmpStateElement.attribute(D_EFFECTTYPE);
        QDomNodeList tmpRcList = tmpStateElement.childNodes();
        for(int j = 0; j < tmpRcList.count(); ++j)
        {
            QDomElement tmpRcElement = tmpRcList.at(j).toElement();
            if (tmpRcElement.isNull())
            {
                continue;
            }
            QString tmpRcName = tmpRcElement.tagName();
            QString tmpRcFile = tmpRcElement.text();
            if (tmpRcName.compare(QString(D_RCINVALID)) && !isRelativePath(tmpRcFile)) //针对于不需要资源的特效以及已经变换为相对路径的资源
            {
                QString tmpString;
                bool needCopy = true;
                if (tmpType == COM_DESKTOP)
                {
                    tmpString = QString("desktop") + QString("/") +
                            tmpStateName + QString("/") +
                            tmpEffectName + QString("/") +
                            QFileInfo(tmpRcFile).fileName();
                }
                else
                {
                    FILEPATH_TYPE tmpType = getRcFileType(tmpRcFile);
                    if (tmpType == PATH_TYPENONE)
                    {
                        continue;
                    }
                    else if (tmpType == PATH_TYPENETADDR)
                    {
                        needCopy = false;
                        tmpString = tmpRcFile;
                    }
                    else
                    {
                        tmpString = tmpComPath.mid(1) + QString("/") +
                                tmpStateName + QString("/") +
                                tmpEffectName + QString("/") +
                                QFileInfo(tmpRcFile).fileName();
                    }
                }
                if (needCopy)
                {
                    QString tmpDst = tmpRelativePath + tmpString;
                    if (!QFile::copy(tmpRcFile, tmpDst))
                    {
                        IDE_TRACE_STR(tmpRcFile);
                        continue;
                    }
                }
                modifyItemElement(tmpRcElement, tmpString);
            }
        }
    }
    return true;
}

bool ThemeParser::isRelativePath(QString pFileName)
{
    if (pFileName.isEmpty())
    {
        return false;
    }
    QString tmpString = m_FileDir + pFileName;
    if (QFile::exists(tmpString))
    {
        return true;
    }
    return false;
}

FILEPATH_TYPE ThemeParser::getRcFileType(QString pFileName)
{
    FILEPATH_TYPE tmpType = getRcFileType(pFileName);
    if (PATH_TYPENONE != tmpType)
    {
        return tmpType;
    }
    if (pFileName.isEmpty())
    {
        return PATH_TYPENONE;
    }
    //查看是否为当前UI下的文件
    QString tmpString = m_FileDir + pFileName;
    if (QFile::exists(tmpString))
    {
        return PATH_TYPERC;
    }
    return PATH_TYPENONE;
}

FILEPATH_TYPE ThemeParser::regulatRcFile(QString& pFileName)
{
    FILEPATH_TYPE tmpType = getRcFileType(pFileName);
    if (tmpType == PATH_TYPERC)
    {
        pFileName.insert(0, m_FileDir);
    }
    return tmpType;
}

bool ThemeParser::regularFile(COM_TYPE pComType, QString pRcName, QString &pRcFile, QString pDstPath, QSize pSize)
{
    if (pRcName == QString(D_RCINVALID))
    {
        return true;
    }
    else if (pComType >= COM_DESKTOP)
    {
        if (pDstPath.endsWith('/') == false)
        {
            pDstPath.append('/');
        }
        QString tmpSrc = m_FileDir + pRcFile;

        RC_TYPE tmpRcType = getRcType(pRcFile);
        switch(tmpRcType)
        {
        case RC_BMP:
        case RC_JPG:
        case RC_PNG:
        {
            ///修改目标文件名为RcName，后缀为png
            int index = pRcFile.lastIndexOf(".");
            if (index < 0)
            {
                pRcFile = pRcName + QString(".png");
            }
            else
            {
                QString tmpString = pRcFile.mid(0, index+1);
                pRcFile = tmpString + QString("png");
            }
            QString tmpDst = pDstPath + pRcFile;
            if (convertImage(tmpSrc, tmpDst, pSize) == false)
            {
                IDE_TRACE_STR(tmpSrc);
                return false;
            }
            break;
        }
        case RC_MP3:
        case RC_WAV:
        {
            //只转换相对和绝对路径下的文件，QRC以及网络文件都不可以转换
            FILEPATH_TYPE tmpType = getRcFileType(pRcFile);
            if (tmpType == PATH_TYPERC || tmpType == PATH_TYPEABS)
            {
                //修改目标文件名为RcName，后缀为wav
                int index = pRcFile.lastIndexOf("/");
                if (index < 0)
                {
                    pRcFile = pRcName + QString(".wav");
                }
                else
                {
                    QString tmpString = pRcFile.mid(0, index+1);
                    pRcFile = tmpString + pRcName + QString(".wav");
                }
                QString tmpDst = pDstPath + pRcFile;
                if (convertAudio(tmpSrc, tmpDst) == false)
                {
                    IDE_TRACE_STR(tmpSrc);
                    return false;
                }
            }
            break;
        }
        default:
            break;
        }
        return true;
    }
    return false;
}
