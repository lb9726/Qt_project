#include "rccontainer.h"

void RC_INFO::clear()
{
    if(mRcType != RC_INVALID){
        mRcFile = "";
        mRcType = RC_INVALID;
    }
    if(!mParameterHash.isEmpty())
        mParameterHash.clear();
}



RcContainer::RcContainer(ThemeParser *pthemeManager,QRectF pRect)
{
    themeManager = pthemeManager;
    m_ComRect[HVBoth] = pRect;

    releaseRC();
}

RcContainer::~RcContainer()
{
    releaseRC();
}

void RcContainer::releaseRC()
{
    if(!m_ComponentRC.isEmpty())
    {
        QList<RC_INFO*> tmpRcInfoList = m_ComponentRC.values();
        RC_INFO *tmpRcInfo = 0;
        foreach(tmpRcInfo, tmpRcInfoList)
        {
            if(tmpRcInfo)
                delete tmpRcInfo;
        }
        m_ComponentRC.clear();
    }
}

void RcContainer::setThemeManager(ThemeParser *pthemeManager)
{
    themeManager = pthemeManager;
}

void RcContainer::RemoveRc(int pKey)
{
    RC_INFO *tmpRcInfo = m_ComponentRC.take(pKey);
    if(tmpRcInfo)
        delete tmpRcInfo;
}

RC_INFO *RcContainer::LoadRc(int pKey, QDomElement pRcElement)
{
    if(pRcElement.isNull())
        return (RC_INFO *)0;
    QString tmpFile = pRcElement.text();
    if(tmpFile.isEmpty())
        return (RC_INFO *)0;
    quint32 tmpLocation = pRcElement.attribute("Location").toUInt();
    if(tmpLocation == 1)  //>@读取绝对地址中的内容
    {
        //>@直接使用绝对地址
    }
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    else if(m_UiContainer.data()->m_UiManager->RegulatRcFile(tmpFile) == PATH_TYPENONE)
    {
        IDE_TRACE_STR(tmpFile);
        return (RC_INFO *)0;
    }
#endif
    RC_INFO *tmpRcInfo = m_ComponentRC.value(pKey);
    if(!tmpRcInfo)
    {
        tmpRcInfo = new RC_INFO;
        if(!tmpRcInfo)
            return (RC_INFO *)0;
        m_ComponentRC.insert(pKey, tmpRcInfo);
    }

    tmpRcInfo->clear();
    tmpRcInfo->mElement = pRcElement;
    tmpRcInfo->mRcFile = tmpFile;
    tmpRcInfo->mRcType = getRcType(tmpFile);
    tmpRcInfo->mTipText = pRcElement.attribute(D_TIPTEXT);
    tmpRcInfo->mTipText.replace("\\n", "\n");  //>@将文本中的\n全部转化为换行符
    ParserParameter(tmpRcInfo, pRcElement.attribute(D_PARAMETER));
    return tmpRcInfo;
}

RC_INFO *RcContainer::LoadPath(int pKey, QString pPath)
{
    if(pPath.isEmpty())
        return (RC_INFO *)0;
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    if(!m_UiContainer.isNull() && m_UiContainer->m_UiManager->RegulatRcFile(pPath) == PATH_TYPENONE)
    {
        IDE_TRACE_STR(pPath);
        return (RC_INFO *)0;
    }
#endif
    RC_INFO *tmpRcInfo = m_ComponentRC.value(pKey);
    if(!tmpRcInfo)
    {
        tmpRcInfo = new RC_INFO;
        if(!tmpRcInfo)
            return (RC_INFO *)0;
        m_ComponentRC.insert(pKey, tmpRcInfo);
    }
    tmpRcInfo->mRcFile = pPath;
    tmpRcInfo->mRcType = getRcType(pPath);
    tmpRcInfo->clear();
    return tmpRcInfo;
}

RC_INFO *RcContainer::LoadPixmap(int pKey, QPixmap pPixmap)
{
    if(pPixmap.isNull())
        return (RC_INFO *)0;
    RC_INFO *tmpRcInfo = m_ComponentRC.value(pKey);
    if(!tmpRcInfo)
    {
        tmpRcInfo = new RC_INFO;
        if(!tmpRcInfo)
            return (RC_INFO *)0;
        m_ComponentRC.insert(pKey, tmpRcInfo);
    }
    tmpRcInfo->mRcFile.clear();
    tmpRcInfo->mRcType = RC_PNG;
    tmpRcInfo->mRcPix.mPNG = pPixmap;
    return tmpRcInfo;
}


bool RcContainer::refreshRC()
{
    RC_INFO *tmpRcInfo = 0;
    foreach(tmpRcInfo, m_ComponentRC)
    {
        if(tmpRcInfo)
            tmpRcInfo->clear();
    }
    return Start();
}



bool RcContainer::hasResource()
{
    if(m_ComponentRC.isEmpty())
        return false;
    return true;
}


bool RcContainer::InitComponent(int layout, DIRECTION direction,QDomElement &pElement, bool pOffset)
{
    if(pElement.isNull())
    {
        return false;
    }
    m_ComType = getComponetType(pElement.tagName());
    m_ComPath = getComponetPath(m_ComType);

    releaseRC();
    m_ComElement = pElement;
    QRect tmpHRect, tmpVRect;
    //if(themeManager->getComRect(m_UiContainer->m_UiLayout, m_ComElement, tmpHRect, tmpVRect))
    if(themeManager->getComRect(layout, m_ComElement, tmpHRect, tmpVRect))
    {
        if(pOffset)
        {
            m_ComRect[Horizontal] = QRectF(tmpHRect);
            m_ComRect[Vertical] = QRectF(tmpVRect);
        }
        else
        {
            m_ComRect[Horizontal] = QRectF(tmpHRect);
            m_ComRect[Vertical] = QRectF(tmpVRect);
        }
    }
    //>@SM
    QDomNodeList tmpStateList = themeManager->getComStateList(m_ComElement);
    for(int i=0;i<tmpStateList.count();i++)
    {
        QDomElement tmpElement = tmpStateList.at(i).toElement();
        initState(tmpElement);
    }

    return Start();
}


void RcContainer::InitRC(QString pRcPath, QString pPrefix, bool pInititial)
{
    if(!pRcPath.isEmpty())
    {
        if(!pRcPath.endsWith('/'))
            pRcPath.append('/');
        pRcPath.replace("//", "/");
        QStringList tmpFileList = getAllFileInFloders(pRcPath);
        QString tmpFile;
        foreach(tmpFile, tmpFileList)
        {
            QString tmpFileName = getFileName(tmpFile);
            int index = tmpFileName.indexOf('.');
            if(index)
                tmpFileName = tmpFileName.mid(0, index);
            int tmpKey = GetKey(tmpFileName, pPrefix);
            if(tmpKey != -1)
                LoadPath(tmpKey, tmpFile);
        }
    }
    if(pInititial)
    {
        m_RcList = m_ComponentRC.keys();
        if(!m_RcList.isEmpty())
        {
            qSort(m_RcList.begin(), m_RcList.end());
            m_PixmapPointer.mCurPointer = GetPixmap(m_RcList.first());
        }
    }
}


QRectF RcContainer::GetCurRect()
{
    QRectF tmpRect;
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    tmpRect = m_ComRect[m_UiContainer->m_UiDirection];
#else
    tmpRect = m_ComRect[HVBoth];
#endif
    return tmpRect;
}

bool RcContainer::initState(QDomElement pElement)
{
    if(pElement.isNull())
        return false;
    QString tmpStateName = pElement.tagName();
    AREA_OPERATE tmpOperate = getOperateType(tmpStateName);
    if(tmpOperate == OPERATE_NONE)
        return false;
    QString tmpType = pElement.attribute(D_EFFECTTYPE);

    IDE_DEBUG(QString("Init State[%1] type[%2]").arg(tmpStateName).arg(tmpType));

    if(tmpOperate == OPERATE_RC)
    {
        //>@添加资源
        m_RcPrefix = pElement.attribute(D_PREFIX);
        QDomNodeList tmpList = pElement.childNodes();
        int count = tmpList.count();
        for(int i=0;i<count;i++)
        {
            QDomElement tmpRcElement = tmpList.at(i).toElement();
            if(tmpRcElement.isNull())
                continue;
            QString tmpName = tmpRcElement.tagName();
            int index = getKey(tmpName);
            if(index == -1)
                continue;
            LoadRc(index, tmpRcElement);
        }
        //>@查看当前系统中是否已经有资源样式
        if(m_RcStyle)
            delete m_RcStyle;
        //>@添加资源样式
        m_RcStyle = new AREA_STYLE;
        m_RcStyle->mRcType = GetComRcType(tmpType);
        InitRcPara(m_RcStyle, pElement.attribute(D_EFFECTPARA));
        return true;
    }
    else
    {
        return false;
    }
    return true;
}

void RcContainer::ParserParameter(RC_INFO *pInfo, QString pParameter)
{
    if(!pInfo || pParameter.isEmpty())
        return;
    QStringList tmpParaList = pParameter.split(';');
    for(int i=0;i<tmpParaList.count();i++)
    {
        QStringList tmpList = tmpParaList.at(i).split(':');
        if(tmpList.count() != 2)
            continue;
        int key = StrToEnum(tmpList.at(0));
        if(key == -1)
            continue;
        QVariant value = StrToValue(key, tmpList.at(1));
        pInfo->mParameterHash.insert(key, value);
    }
}

bool RcContainer::InitEffectState(EffectType pStateType, AREA_OPERATE pOperate, QDomElement pElement)
{
    if(pStateType == EffectTypeNone)
        return false;
    //>@添加特效参数
    AREA_ANIMATE *tmpAreaAnimate = new  AREA_ANIMATE;
    tmpAreaAnimate->mEffectType = pStateType;
    InitEffectPara(tmpAreaAnimate, pElement.attribute(D_EFFECTPARA));
    //>@查看当前系统中是否已经有此状态机
    AREA_ANIMATE *tmpOldAreaAnimate = m_EffectGroup.take(pOperate);
    if(tmpOldAreaAnimate)
        delete tmpOldAreaAnimate;
    m_EffectGroup.insert(pOperate, tmpAreaAnimate);
    return true;
}

bool RcContainer::InitSubPara(QString pStateName, QString pLabel, QString pContent)
{
    AREA_OPERATE tmpOperate = getOperateType(pStateName);
    bool ret = false;
    if(tmpOperate == OPERATE_RC)  //>@common
        ret = InitSubRcPara(m_RcStyle, pLabel, pContent);
    else
        ret = InitSubEffectPara(m_EffectGroup.value(tmpOperate), pLabel, pContent);
    return ret;
}

bool RcContainer::InitRcPara(AREA_STYLE* pAreaStyle, QString pCommonPara)
{
    if(pCommonPara.isEmpty())
        return false;
    QStringList tmpList = pCommonPara.split(";", QString::SkipEmptyParts);
    for(int i=0;i<tmpList.count();i++)
    {
        QString tmpPara = tmpList.at(i);
        if(tmpPara.isEmpty())
            continue;
        int index = tmpPara.indexOf(':');
        if(index < 0)
            continue;
        InitSubRcPara(pAreaStyle, tmpPara.mid(0,index), tmpPara.mid(index+1));
    }
    return true;
}

bool RcContainer::InitEffectPara(AREA_ANIMATE* pAreaEffect, QString pEffectPara)
{
    if(pEffectPara.isEmpty())
        return false;
    QStringList tmpList = pEffectPara.split(";", QString::SkipEmptyParts);
    for(int i=0;i<tmpList.count();i++)
    {
        QStringList tmpSubList = tmpList.at(i).split(":", QString::SkipEmptyParts);
        if(tmpSubList.count()<2)
            continue;
        InitSubEffectPara(pAreaEffect, tmpSubList.at(0), tmpSubList.at(1));
    }
    return true;
}

bool RcContainer::InitSubEffectPara(AREA_ANIMATE* pAreaEffect, QString pLabel, QString pContent)
{
    if(pAreaEffect == 0)
        return false;

    if(!PreprocContent(pContent))
        return false;

    if(pLabel.compare(PARA_FRAMERANGE, Qt::CaseInsensitive) == 0)
    {
        bool ok;
        int dec = pContent.toInt(&ok, 10);  //>@10进制
        if(ok)
        {
            pAreaEffect->mFrameStart = 0;
            pAreaEffect->mFrameEnd = dec;
        }
    }
    else if(pLabel.compare(PARA_INTERVAL, Qt::CaseInsensitive) == 0)
    {
        bool ok;
        int dec = pContent.toInt(&ok, 10);  //>@10进制
        if(ok)
            pAreaEffect->mAnimateTime = dec;
    }
    else if(pLabel.compare(PARA_DUTYRATIO, Qt::CaseInsensitive) == 0)  //>@0~100
    {
        bool ok;
        int dec = pContent.toInt(&ok, 10);  //>@10进制
        if(ok)
            pAreaEffect->mDutyRatio = dec;
    }
    else if(pLabel.compare(PARA_CURVESHAPE, Qt::CaseInsensitive) == 0)
    {
        if(pContent.compare(QString("EaseIn"), Qt::CaseInsensitive) == 0)
            pAreaEffect->mAnimateType = QTimeLine::EaseInCurve;
        else if(pContent.compare(QString("EaseOut"), Qt::CaseInsensitive) == 0)
            pAreaEffect->mAnimateType = QTimeLine::EaseOutCurve;
        else if(pContent.compare(QString("EaseInOut"), Qt::CaseInsensitive) == 0)
            pAreaEffect->mAnimateType = QTimeLine::EaseInOutCurve;
        else if(pContent.compare(QString("Linear"), Qt::CaseInsensitive) == 0)
            pAreaEffect->mAnimateType = QTimeLine::LinearCurve;
        else if(pContent.compare(QString("Sine"), Qt::CaseInsensitive) == 0)
            pAreaEffect->mAnimateType = QTimeLine::SineCurve;
        else if(pContent.compare(QString("Cosine"), Qt::CaseInsensitive) == 0)
            pAreaEffect->mAnimateType = QTimeLine::CosineCurve;
    }
    else if(pLabel.compare(PARA_DIRECTION, Qt::CaseInsensitive) == 0)
    {
        if(pContent.compare(QString("H"), Qt::CaseInsensitive) == 0)
            pAreaEffect->mDirection = Horizontal;
        else if(pContent.compare(QString("V"), Qt::CaseInsensitive) == 0)
            pAreaEffect->mDirection = Vertical;
    }
    else if(pLabel.compare(PARA_ORIENTATION, Qt::CaseInsensitive) == 0)
    {
        if(pContent.compare(QString("P"), Qt::CaseInsensitive) == 0)
            pAreaEffect->mOrientation = Positive;
        else if(pContent.compare(QString("N"), Qt::CaseInsensitive) == 0)
            pAreaEffect->mOrientation = Negative;
    }
    return true;
}



QString RcContainer::getAttribute(QString pEffect, QString pName)
{
    QDomElement tmpSMElement = m_ComElement.firstChildElement("StateMachine");
    if(tmpSMElement.isNull())
        return QString();
    QDomElement tmpStateElement = tmpSMElement.firstChildElement(pEffect);
    if(tmpStateElement.isNull())
        return QString();
    QString tmpEffectPara = tmpStateElement.attribute(D_EFFECTPARA);
    if(tmpEffectPara.isEmpty())
        return QString();
    QStringList tmpParaList = tmpEffectPara.split(';');
    for(int m=0;m<tmpParaList.count();m++)
    {
        QStringList tmpValueList = tmpParaList.at(m).split(':');
        if(tmpValueList.count()!=2)
            continue;
        if(tmpValueList.at(0).compare(pName))
            continue;
        return tmpValueList.at(1);
    }
    return QString();
}

bool RcContainer::SetAttribute(QString pEffect, QString pName, QString pValue)
{
    QDomElement tmpSMElement = m_ComElement.firstChildElement("StateMachine");
    if(tmpSMElement.isNull())
        return false;
    QDomElement tmpStateElement = tmpSMElement.firstChildElement(pEffect);
    if(tmpStateElement.isNull())
        return false;
    QString tmpEffectPara = tmpStateElement.attribute(D_EFFECTPARA);
    if(tmpEffectPara.isEmpty())
        return false;
    QStringList tmpParaList = tmpEffectPara.split(';');
    for(int m=0;m<tmpParaList.count();m++)
    {
        QStringList tmpValueList = tmpParaList.at(m).split(':');
        if(tmpValueList.count()!=2)
            continue;
        if(tmpValueList.at(0).compare(pName))
            continue;
        tmpParaList.replace(m, QString("%1:%2").arg(pName).arg(pValue));
        QString tmpNewString = tmpParaList.join(QString(";"));
        tmpStateElement.setAttribute(D_EFFECTPARA, tmpNewString);
        return true;
    }
    tmpParaList.append(QString("%1:%2").arg(pName).arg(pValue));
    QString tmpNewString = tmpParaList.join(QString(";"));
    tmpStateElement.setAttribute(D_EFFECTPARA, tmpNewString);
    return true;
}

bool RcContainer::SetPara(QDomElement &pElement, XmlParse *pParse)
{
    if(pElement.isNull() || pParse==0)
        return false;

    QString tmpCmd = pElement.text();
    if(!tmpCmd.isEmpty())
    {
        pParse->setText(pElement, execCommand(tmpCmd));
    }
    QDomNodeList tmpList = pElement.childNodes();
    for(int i=0;i<tmpList.count();i++)
    {
        QDomElement tmpElement = tmpList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpName = tmpElement.tagName();
        if(tmpName.compare("Rect", Qt::CaseInsensitive) == 0)
        {
            //>@如果是在其父窗口中移动，则执行移动操作
#ifdef SUPPORT_SCENE
            UiScene *tmpScene = VAR_CAST<UiScene*>(m_UiContainer.data());
#elif SUPPORT_WIDGET
            UiWidget *tmpScene = m_UiContainer.data();
#endif
            if(tmpScene && tmpScene->m_LogicThemeManager && tmpScene->m_LogicThemeManager->hasTheme())
            {
                QStringList tmpGeoList;
                if(tmpScene->m_LogicThemeManager->m_UiContainer->m_UiDirection == Horizontal)
                    tmpGeoList = tmpElement.firstChildElement("Horizontal").text().split(',');
                else if(tmpScene->m_LogicThemeManager->m_UiContainer->m_UiDirection == Vertical)
                    tmpGeoList = tmpElement.firstChildElement("Vertical").text().split(',');
                if(tmpGeoList.count() != 4)
                    continue;
                ModifyGeomotery(tmpGeoList);
            }
        }
        else if(tmpName.compare("StateMachine", Qt::CaseInsensitive) == 0)
        {
            QDomElement tmpOldSMElement = m_ComElement.firstChildElement("StateMachine");
            if(tmpOldSMElement.isNull())
                continue;
            QDomNodeList tmpStateList = tmpElement.childNodes();
            for(int j=0;j<tmpStateList.count();j++)
            {
                QDomElement tmpNewStateElement = tmpStateList.at(j).toElement();
                if(tmpNewStateElement.isNull())
                    continue;
                QString tmpStateName = tmpNewStateElement.tagName();
                QDomElement tmpOldStateElement = tmpOldSMElement.firstChildElement(tmpStateName);
                if(tmpOldStateElement.isNull())
                    continue;
                uiManager* tmpUiManager = VAR_CAST<uiManager*>(pParse);
                if(tmpUiManager)
                {
                    QString tmpEffectType = tmpNewStateElement.attribute(D_EFFECTTYPE);
                    if(tmpUiManager->ModifyEffectType(tmpOldStateElement, tmpEffectType))
                    {
                        QString tmpEffectPara = tmpNewStateElement.attribute(D_EFFECTPARA);
                        if(tmpUiManager->ModifyEffectPara(tmpOldStateElement, tmpEffectPara))
                        {
                            InitState(tmpOldStateElement);
                        }
                    }
                }
            }
            Restart();
            //InitComponent(m_ComElement);
        }
    }
    return true;
}

bool RcContainer::GetPara(QDomElement &pElement, XmlParse *pParse)
{
    if(pElement.isNull() || pParse==0)
        return false;

    QString tmpCmd = pElement.text();
    if(!tmpCmd.isEmpty())
    {
        QString tmpRet;
        if(tmpCmd.compare("rclist", Qt::CaseInsensitive) == 0)
        {
            QList<RC_INFO*> tmpRcList = m_EffectRC.values();
            if(!tmpRcList.isEmpty())
            {
                int i=0;
                tmpRet.append(getFileDirectory(tmpRcList.at(0)->mRcFile));
                tmpRet.append(';');
                for(;i<tmpRcList.count()-1;i++)
                {
                    tmpRet.append(getFileName(tmpRcList.at(i)->mRcFile));
                    tmpRet.append(';');
                }
                tmpRet.append(getFileName(tmpRcList.at(i)->mRcFile));
            }
        }
        pParse->SetText(pElement, tmpRet);
    }
    QDomNodeList tmpList = pElement.childNodes();
    for(int i=0;i<tmpList.count();i++)
    {
        QDomElement tmpElement = tmpList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpName = tmpElement.tagName();
        if(tmpName.compare("Rect", Qt::CaseInsensitive) == 0)
        {
#ifdef SUPPORT_SCENE
            UiScene *tmpScene = VAR_CAST<UiScene*>(m_UiContainer.data());
#elif SUPPORT_WIDGET
            UiWidget *tmpScene = m_UiContainer.data();
#endif
            if(tmpScene && tmpScene->m_LogicThemeManager && tmpScene->m_LogicThemeManager->hasTheme())
            {
            }
        }
        else if(tmpName.compare("StateMachine", Qt::CaseInsensitive) == 0)
        {
            QDomElement tmpOldSMElement = m_ComElement.firstChildElement("StateMachine");
            if(tmpOldSMElement.isNull())
                continue;
            QDomNodeList tmpStateList = tmpElement.childNodes();
            for(int j=0;j<tmpStateList.count();j++)
            {
                QDomElement tmpNewStateElement = tmpStateList.at(j).toElement();
                if(tmpNewStateElement.isNull())
                    continue;
                QString tmpStateName = tmpNewStateElement.tagName();
                QDomElement tmpOldStateElement = tmpOldSMElement.firstChildElement(tmpStateName);
                if(tmpOldStateElement.isNull())
                    continue;
                QString tmpEffectPara = tmpNewStateElement.attribute(D_EFFECTPARA);
                if(!tmpEffectPara.isEmpty())
                {
                    QStringList tmpParaList = tmpEffectPara.split(';');
                    QStringList tmpOldParaList = tmpOldStateElement.attribute(D_EFFECTPARA).split(';');
                    for(int m=0;m<tmpParaList.count();m++)
                    {
                        for(int n=0;n<tmpOldParaList.count();n++)
                        {
                            QString tmpOldValue = tmpOldParaList.at(n);
                            if(tmpOldValue.contains(tmpParaList.at(m)))
                            {
                                tmpParaList.replace(m, tmpOldValue);
                                break;
                            }
                        }
                    }
                    QString tmpNewString = tmpParaList.join(QString(";"));
                    tmpNewStateElement.setAttribute(D_EFFECTPARA, tmpNewString);
                }
            }
        }
    }

    return true;
}


int RcContainer::getNextRc(int pCurRcIndex)
{
    if(m_RcList.count() > 0)
    {
        for(int i=0;i<m_RcList.count();i++)
        {
            int value = m_RcList.at(i);
            if(value == pCurRcIndex)
            {
                if((i+1) < m_RcList.count())
                    return m_RcList.at(i+1);
                else
                    return m_RcList.first();
            }
        }
        //>@如果找不到提供的资源标号，则默认返回最后一个资源的标号
        return m_RcList.last();
    }
    return -1;
}

int RcContainer::getPrevRc(int pCurRcIndex)
{
    if(m_RcList.count() > 0)
    {
        for(int i=0;i<m_RcList.count();i++)
        {
            int value = m_RcList.at(i);
            if(value == pCurRcIndex)
            {
                if((i-1) >= 0)
                    return m_RcList.at(i-1);
                else
                    return m_RcList.last();
            }
        }
        //>@如果找不到提供的资源标号，则默认返回第一个资源的标号
        return m_RcList.first();
    }
    return -1;
}

QPixmap RcContainer::getNextImg()
{
    bool find = false;
    RC_INFO* tmpRc=0;
    foreach(tmpRc, m_ComponentRC)
    {
        if(!tmpRc)
            continue;
        if(m_CurResource.compare(tmpRc->mRcFile,Qt::CaseInsensitive))
        {
            find = true;
            break;
        }
    }
    if(find)
    {
        int key = m_ComponentRC.key(tmpRc);
        tmpRc = m_ComponentRC.value(getNextRc(key));
        if(tmpRc)
            return tmpRc->mRcFile;
    }
    return "";
}

QPixmap RcContainer::getPrevImg()
{
    bool find = false;
    RC_INFO* tmpRc=0;
    foreach(tmpRc, m_ComponentRC)
    {
        if(!tmpRc)
            continue;
        if(m_CurResource.compare(tmpRc->mRcFile,Qt::CaseInsensitive))
        {
            find = true;
            break;
        }
    }
    if(find)
    {
        int key = m_ComponentRC.key(tmpRc);
        tmpRc = m_ComponentRC.value(getPrevRc(key));
        if(tmpRc)
            return tmpRc->mRcFile;
    }
    return "";
}

QString RcContainer::GetDefaultEffectPara(QString pStateName, QString pEffectName)
{
    EFFECT *tmpEffect = GetDefaultEffect(pStateName, pEffectName);
    if(!tmpEffect)
        return QString();
    QString tmpParaString;
    for(int i=0;i<tmpEffect->EffectParas.count();i++)
    {
        EFFECTPARA *tmpPara = tmpEffect->EffectParas.at(i);
        if(!tmpPara)
            continue;
        QStringList tmpSubPara = tmpPara->ParaValue.split('/');
        if(tmpSubPara.isEmpty())
            continue;
        tmpParaString.append(QString("%1:%2;").arg(tmpPara->ParaName).arg(tmpSubPara.first()));
    }
    return tmpParaString;
}

QDomElement RcContainer::GetComStateElement(QString pStateName, QString &pEffectName, QString &pEffectPara)
{
    QDomElement tmpStateElement;

    tmpStateElement = themeManager->getComState(m_ComElement, pStateName);
    if(!tmpStateElement.isNull())
    {
        pEffectName = tmpStateElement.attribute(D_EFFECTTYPE);
        pEffectPara = tmpStateElement.attribute(D_EFFECTPARA);
    }

    return tmpStateElement;
}

QString RcContainer::GetComStateEffect(QString pStateName)
{
    QDomElement tmpStateElement;

    tmpStateElement = themeManager->getComState(m_ComElement, pStateName);

    if(!tmpStateElement.isNull())
    {
        return tmpStateElement.attribute(D_EFFECTTYPE);
    }
    return QString();
}

QString RcContainer::GetComStateEffectPara(QString pStateName, QString pParaName)
{
    QDomElement tmpStateElement;

    tmpStateElement = themeManager->getComState(m_ComElement, pStateName);

    if(!tmpStateElement.isNull())
    {
        QStringList tmpParaList = tmpStateElement.attribute(D_EFFECTPARA).split(';');
        for(int i=0;i<tmpParaList.count();i++)
        {
            QStringList tmpParaValueList = tmpParaList.at(i).split(':');
            if(tmpParaValueList.count() != 2)
                continue;
            if(tmpParaValueList.first().compare(pParaName) == 0)
                return tmpParaValueList.at(1);
        }
    }
    return QString();
}

QString RcContainer::GetComStateRcFile(QString pStateName, QString pRcName)
{
    QDomElement tmpStateElement;
    tmpStateElement = themeManager->getComState(m_ComElement, pStateName);
    if(tmpStateElement.isNull())
        return QString();
    QDomElement tmpRcElement = tmpStateElement.firstChildElement(pRcName);
    if(tmpRcElement.isNull())
        return QString();
    return tmpRcElement.text();
}

QString RcContainer::GetRelativeRcLocation()
{
    QString tmpName = GetComStateEffect(QString("resource"));
    if(tmpName.isEmpty())
        return QString();
    QString tmpPath = m_ComPath + QString("/resource/%1").arg(tmpName);
    tmpPath.replace("//", "/");
    return tmpPath;
}
