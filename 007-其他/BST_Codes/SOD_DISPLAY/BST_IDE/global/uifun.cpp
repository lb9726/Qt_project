#include "uifun.h"

#ifdef SUPPORT_SCENE
#include "uiscene.h"
#elif SUPPORT_WIDGET
#include "uiwidget.h"
#elif SUPPORT_COMEDIT
#include "diagramscene.h"
#endif

#ifdef DISPLAY  //#ifndef DESIGNER
#include "logicthememanager.h"
#endif

void RC_INFO::clear()
{
    if(mRcType != RC_INVALID)
        mRcPix.clear(mRcType);
    if(!mParaHash.isEmpty())
        mParaHash.clear();
}

RcContainer::RcContainer(
#ifdef SUPPORT_SCENE
                        GraphicsScene *pContainer,
#elif SUPPORT_WIDGET
                        UiWidget *pContainer,
#elif SUPPORT_COMEDIT
                        GraphicsScene *pContainer,
#endif
                        QRectF pRect)
{
    m_ExternRC = false;
    m_EffectValid = false;
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    m_UiContainer = pContainer;
#endif
    m_ComRect[HVBoth] = pRect;
    m_RcStyle = 0;
#ifdef IDE
    m_ComCategory = 0;
#endif
    ReleaseRC();
}

//>@从ui文件中删除此ComElement
RcContainer::~RcContainer()
{
    ReleaseRC();
}

void RcContainer::SetUiContainer(
#ifdef SUPPORT_SCENE
                        GraphicsScene *pContainer
#elif SUPPORT_WIDGET
                        UiWidget *pContainer
#elif SUPPORT_COMEDIT
                        GraphicsScene *pContainer
#endif
        )
{
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    m_UiContainer = pContainer;
#endif
}

void RcContainer::RemoveRc(int pKey)
{
    RC_INFO *tmpRcInfo = m_EffectRC.take(pKey);
    if(tmpRcInfo)
        delete tmpRcInfo;
}

FORM_INFO *RcContainer::LoadForm(int pKey, QDomElement pRcElement)
{
    if(pRcElement.isNull())
        return (FORM_INFO *)0;
    QString tmpFile = pRcElement.text();
    if(tmpFile.isEmpty())
        return (FORM_INFO *)0;
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    if(m_UiContainer.data()->m_UiManager->RegulatRcFile(tmpFile) == PATH_TYPENONE)
    {
        IDE_TRACE_STR(tmpFile);
        return (FORM_INFO *)0;
    }
#endif
    FORM_INFO *tmpRcInfo = m_FormList.value(pKey);
    if(!tmpRcInfo)
    {
        tmpRcInfo = new FORM_INFO;
        m_FormList.insert(pKey, tmpRcInfo);
    }
    tmpRcInfo->clear();
    tmpRcInfo->mFile = tmpFile;
    tmpRcInfo->mStyleSheet = pRcElement.attribute("StyleSheet");
    ParserParameter(tmpRcInfo, pRcElement.attribute(D_PARAMETER));
    return tmpRcInfo;
}

MAP_INFO* RcContainer::LoadMap(QDomElement pElement)
{
    if(pElement.isNull())
        return (MAP_INFO *)0;
    int tmpKey = GetKey(pElement.tagName());
    MAP_INFO *tmpInfo = m_MapList.value(tmpKey);
    if(!tmpInfo)
    {
        tmpInfo = new MAP_INFO;
        m_MapList.insert(tmpKey, tmpInfo);
    }
    bool ret = false;
    quint32 tmpMap = pElement.text().toInt(&ret, 10);
    if(ret)
        tmpInfo->mMap = tmpMap;
    else
        tmpInfo->mMap = -1;
    ParserParameter(tmpInfo, pElement.attribute(D_PARAMETER));
    return tmpInfo;
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
    RC_INFO *tmpRcInfo = m_EffectRC.value(pKey);
    if(!tmpRcInfo)
    {
        tmpRcInfo = new RC_INFO;
        if(!tmpRcInfo)
            return (RC_INFO *)0;
        m_EffectRC.insert(pKey, tmpRcInfo);
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
    RC_INFO *tmpRcInfo = m_EffectRC.value(pKey);
    if(!tmpRcInfo)
    {
        tmpRcInfo = new RC_INFO;
        if(!tmpRcInfo)
            return (RC_INFO *)0;
        m_EffectRC.insert(pKey, tmpRcInfo);
    }
    tmpRcInfo->mRcFile = pPath;
    tmpRcInfo->mRcType = getRcType(pPath);
    tmpRcInfo->clear();
    return tmpRcInfo;
}

RC_INFO *RcContainer::CreateRcInfo(QPixmap pPixmap)
{
    if(pPixmap.isNull())
        return (RC_INFO *)0;
    RC_INFO *tmpRcInfo = new RC_INFO;
    if(!tmpRcInfo)
        return (RC_INFO *)0;
    tmpRcInfo->mRcFile.clear();
    tmpRcInfo->mRcType = RC_PNG;
    tmpRcInfo->mRcPix.mPNG = pPixmap;
    return tmpRcInfo;
}

RC_INFO *RcContainer::LoadPixmap(int pKey, QPixmap pPixmap)
{
    if(pPixmap.isNull())
        return (RC_INFO *)0;
    RC_INFO *tmpRcInfo = m_EffectRC.value(pKey);
    if(!tmpRcInfo)
    {
        tmpRcInfo = new RC_INFO;
        if(!tmpRcInfo)
            return (RC_INFO *)0;
        m_EffectRC.insert(pKey, tmpRcInfo);
    }
    tmpRcInfo->mRcFile.clear();
    tmpRcInfo->mRcType = RC_PNG;
    tmpRcInfo->mRcPix.mPNG = pPixmap;
    return tmpRcInfo;
}

#ifdef  SURRPORT_SVG
QPixmap SvgToPixmap(QSvgRenderer *pRenderer, QSizeF pSize)
{
    if(pRenderer && pRenderer->isValid())
    {
        if(pSize.isEmpty())
            return QPixmap();
        QImage tmpImage(pSize.width(), pSize.height(), QImage::Format_ARGB32_Premultiplied);
        if(tmpImage.isNull())
            return QPixmap();
        tmpImage.fill(Qt::transparent); //>@创建透明图层
        QPainter p(&tmpImage);
        pRenderer->render(&p);
        p.end();
#ifdef RGB565  //>@主要是对图像进行像素调整
        if(ConvertImageTo565(tmpImage) == false)
            return QPixmap();
#endif
        return QPixmap::fromImage(tmpImage);
    }
}
#endif

QPixmap RcContainer::LoadPixmap(QString pPixmapName, QSizeF pSize)
{
    if(pPixmapName.isEmpty())
        return QPixmap();
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    FILEPATH_TYPE tmpType = m_UiContainer->m_UiManager->RegulatRcFile(pPixmapName);
    if(tmpType == PATH_TYPENONE || tmpType == PATH_TYPENETADDR)
        return QPixmap();
#endif
    RC_TYPE tmpRcType = getRcType(pPixmapName);
    switch(tmpRcType)
    {
        case RC_BMP:
        case RC_JPG:
        case RC_PNG:
        {
            QPixmap tmpPixmap;
#ifdef RGB565  //>@主要是对图像进行像素调整
            QImage tmpImage;
            if(tmpImage.load(pPixmapName) == false)
                return QPixmap();
            if(ConvertImageTo565(tmpImage) == false)
                return QPixmap();
            tmpPixmap = QPixmap::fromImage(tmpImage);
#else
            if(tmpPixmap.load(pPixmapName) == false)
                return QPixmap();
#endif
            return ScalePixmap(tmpPixmap, pSize);
        }
        case RC_FONT:
        {
            QPixmap tmpPixmap;
            fontManager *tmpFontManager = new fontManager(pPixmapName);
            if(tmpFontManager->m_Valid)
            {
                if(pSize.isEmpty())
                    return QPixmap();
                QImage tmpImage(pSize.width(), pSize.height(), QImage::Format_ARGB32_Premultiplied);
                if(tmpImage.isNull())
                    return QPixmap();
                tmpImage.fill(Qt::transparent); //>@创建透明图层
                QPainter p(&tmpImage);
                //>@获取内容
                QStringList tmpDescList = tmpFontManager->m_ParagraphGroup.keys();
                int count = tmpDescList.count();
                int diff = (qreal)(pSize.height()) / (qreal)count;
                for(int i=0;i<count;i++)
                {
                    QString tmpString;
                    FONT_INFO tmpFontInfo;
                    if(tmpFontManager->GetParagraph(tmpDescList.at(i), tmpString, tmpFontInfo) == false)
                        continue;
                    WriteText(&p, tmpString, QRect(0, diff*i, pSize.width(), diff), tmpFontInfo.mFont.family(), tmpFontInfo.mColor, PRIOR_HEIGHT);
                }
                p.end();
#ifdef RGB565  //>@主要是对图像进行像素调整
                if(ConvertImageTo565(tmpImage) == false)
                    return QPixmap();
#endif
                tmpPixmap = QPixmap::fromImage(tmpImage);
            }
            tmpFontManager->CloseFont();
            delete tmpFontManager;
            return tmpPixmap;
        }
        default:break;
    }
    return QPixmap();
}

void RcContainer::ReleaseEffectGroup()
{
    if(m_RcStyle)
    {
        delete m_RcStyle;
        m_RcStyle = 0;
    }
    if(!m_EffectGroup.isEmpty())
    {
        QHash<AREA_OPERATE, AREA_ANIMATE*>::const_iterator i = m_EffectGroup.constBegin();
        while (i != m_EffectGroup.constEnd())   //>@使用此方法会有一个bug，当只有一个元素时无法执行
        {
            AREA_ANIMATE *tmpAnimate = i.value();
            if(tmpAnimate)
                delete tmpAnimate;
            ++i;
        }
        m_EffectGroup.clear();
    }
}

void RcContainer::ReleaseRC()
{
    if(m_ExternRC)
        return;
    m_EffectValid = false;
    if(!m_EffectRC.isEmpty())
    {
        QList<RC_INFO*> tmpRcInfoList = m_EffectRC.values();
        RC_INFO *tmpRcInfo = 0;
        foreach(tmpRcInfo, tmpRcInfoList)
        {
            if(tmpRcInfo)
                delete tmpRcInfo;
        }
        m_EffectRC.clear();
    }
    ReleaseEffectGroup();
}

bool RcContainer::RefreshRC()
{
    RC_INFO *tmpRcInfo = 0;
    foreach(tmpRcInfo, m_EffectRC)
    {
        if(tmpRcInfo)
            tmpRcInfo->clear();
    }
    return Start();
}

void RcContainer::InitScene(
#ifdef SUPPORT_WIDGET
                            UiWidget* pScene
#elif SUPPORT_SCENE
                            GraphicsScene* pScene
#elif SUPPORT_COMEDIT
                            GraphicsScene* pScene
#endif
                            )
{
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    m_UiContainer = pScene;
#endif
}

void RcContainer::DeleteComponent()
{
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    if(m_UiContainer)
        m_UiContainer->m_UiManager->DeleteComDom(m_ComType);
#endif
}

bool RcContainer::hasResource()
{
    if(m_EffectRC.isEmpty())
        return false;
    return true;
}

bool RcContainer::InitComponent(QHash<int, RC_INFO*> pRcList, QHash<AREA_OPERATE, AREA_ANIMATE*>  pEffectGroup, QRectF pRect)
{
    ReleaseRC();
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    m_ComRect[m_UiContainer->m_UiDirection] = pRect;
#else
    m_ComRect[HVBoth] = pRect;
#endif
    m_EffectRC = pRcList;
    m_EffectGroup = pEffectGroup;
    m_ExternRC = true;
    Start();
    return true;
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
        m_RcList = m_EffectRC.keys();
        if(!m_RcList.isEmpty())
        {
            qSort(m_RcList.begin(), m_RcList.end());
            m_PixmapPointer.mCurPointer = GetPixmap(m_RcList.first());
        }
    }
}

bool RcContainer::InitComponent(QDomElement &pElement, bool pOffset)
{
    if(pElement.isNull())
    {
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
        if(m_ComRect[m_UiContainer->m_UiDirection].isNull())
            return false;
#if 0
        pElement = m_UiContainer->m_UiManager->AddComDom(pComType, m_ComRect);
        if(pElement.isNull())
#endif
#endif
            return false;
    }
    m_ComType = getComponetType(pElement.tagName());
    m_ComPath = getComponetPath(m_ComType);
#ifdef IDE
    m_ComCategory = GlobalPara.m_ComCategoryGroup.value(m_ComPath);
    if(!m_ComCategory)
        return false;
#endif
    ReleaseRC();
    m_ComElement = pElement;
    //>@Rect
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    QRect tmpHRect, tmpVRect;
    if(m_UiContainer->m_UiManager->GetComRect(m_UiContainer->m_UiLayout, m_ComElement, tmpHRect, tmpVRect))  //m_UiContainer->m_UiDirection
    {
        if(pOffset)
        {
            m_ComRect[Horizontal] = ToSceneRect(QRectF(tmpHRect));
            m_ComRect[Vertical] = ToSceneRect(QRectF(tmpVRect));
        }
        else
        {
            m_ComRect[Horizontal] = QRectF(tmpHRect);
            m_ComRect[Vertical] = QRectF(tmpVRect);
        }
    }
    //>@SM
    QDomNodeList tmpStateList = m_UiContainer->m_UiManager->GetComStateList(m_ComElement);
    for(int i=0;i<tmpStateList.count();i++)
    {
        QDomElement tmpElement = tmpStateList.at(i).toElement();
        InitState(tmpElement);
    }
#endif
    return Start();
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

bool RcContainer::InitState(QDomElement pElement)
{
    if(pElement.isNull())
        return false;
    QString tmpStateName = pElement.tagName();
    AREA_OPERATE tmpOperate = GetOperateType(tmpStateName);
    if(tmpOperate == OPERATE_NONE)
        return false;
    QString tmpType = pElement.attribute(D_EFFECTTYPE);

    IDE_DEBUG(QString("Init State[%1] type[%2]").arg(tmpStateName).arg(tmpType));

    //>@初始化;
    if(tmpOperate == OPERATE_FORM)
    {
        //>@添加资源
        QDomNodeList tmpList = pElement.childNodes();
        int count = tmpList.count();
        for(int i=0;i<count;i++)
        {
            QDomElement tmpRcElement = tmpList.at(i).toElement();
            if(tmpRcElement.isNull())
                continue;
            int index = GetKey(tmpRcElement.tagName());
            if(index == -1)
                continue;
            LoadForm(index, tmpRcElement);
        }
    }
    else if(tmpOperate == OPERATE_RC)
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
            int index = GetKey(tmpName);
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
    }
    else if(tmpOperate == OPERATE_MAP)
    {
        QDomNodeList tmpList = pElement.childNodes();
        int count = tmpList.count();
        for(int i=0;i<count;i++)
        {
            QDomElement tmpMapElement = tmpList.at(i).toElement();
            if(tmpMapElement.isNull())
                continue;
            LoadMap(tmpMapElement);
        }
        m_MapStyle = new MAP_STYLE;
        m_MapStyle->mType = GetComRcType(tmpType);
        InitMapPara(m_MapStyle, pElement.attribute(D_EFFECTPARA));
    }
    else
    {
        EffectType tmpEffectType = GetEffectType(tmpType);
        InitEffectState(tmpEffectType, tmpOperate, pElement);
    }
    return true;
}

void RcContainer::ParserParameter(FORM_INFO *pInfo, QString pParameter)
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
        pInfo->mParaHash.insert(key, value);
    }
}

void RcContainer::ParserParameter(MAP_INFO *pInfo, QString pParameter)
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
        pInfo->mParaHash.insert(key, value);
    }
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
        pInfo->mParaHash.insert(key, value);
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
    AREA_OPERATE tmpOperate = GetOperateType(pStateName);
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

bool RcContainer::InitMapPara(MAP_STYLE* pStyle, QString pPara)
{
    if(pPara.isEmpty())
        return false;
    QStringList tmpList = pPara.split(";", QString::SkipEmptyParts);
    for(int i=0;i<tmpList.count();i++)
    {
        QStringList tmpSubList = tmpList.at(i).split(":", QString::SkipEmptyParts);
        if(tmpSubList.count()<2)
            continue;
        InitSubMapPara(pStyle, tmpSubList.at(0), tmpSubList.at(1));
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

bool PreprocContent(QString &pContent)
{
    int index = pContent.indexOf('@');
    if(index > 0)
    {
        bool ok = false;
        int tmpNum = pContent.mid(0, index).toInt(&ok, 10);
        if(!ok)
            return false;
        QStringList tmpValueList = pContent.mid(index+1).split(',');
        if(tmpValueList.isEmpty())
            return false;
        if(tmpNum >= tmpValueList.count())
            tmpNum = tmpValueList.count() - 1;
        pContent = tmpValueList.at(tmpNum);
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

QPixmap RcContainer::ScalePixmap(QPixmap pPixmap, QSizeF pSize)
{
    QSize tmpSize = pSize.toSize();
    if(!(tmpSize.isEmpty()) && !(pPixmap.isNull()) && pPixmap.size() != tmpSize)
        return pPixmap.scaled(tmpSize);
    return pPixmap;
}

QString RcContainer::GetPixmapPath(int pKey)
{
    RC_INFO *tmpRcInfo = m_EffectRC.value(pKey);
    if(!tmpRcInfo)
        return QString();
    return tmpRcInfo->mRcFile;
}

QPixmap RcContainer::GetPixmap(int pKey, QSizeF pSize)
{
    return GetPixmap(m_EffectRC.value(pKey), pSize);
}

QMovie* RcContainer::GetGif(RC_INFO *pRcInfo, QSizeF pSize)
{
    QMovie *tmpMovie = 0;
    if(!pRcInfo)
        return tmpMovie;
    if(pRcInfo->mRcType != RC_GIF)
        return tmpMovie;
    //>@首先查看PIX
    tmpMovie = pRcInfo->mRcPix.mGIF;
    if(tmpMovie)
    {
        if(pSize.isValid())
            tmpMovie->setScaledSize(pSize.toSize());
        return tmpMovie;
    }
    //>@其次查找m_EffectPath，并将其插入m_EffectImage
    tmpMovie = new QMovie(pRcInfo->mRcFile);
    if(pSize.isValid())
        tmpMovie->setScaledSize(pSize.toSize());
    pRcInfo->mRcPix.mGIF = tmpMovie;
    return tmpMovie;
}

QPixmap RcContainer::GetPixmap(RC_INFO *pRcInfo, QSizeF pSize)
{
    QPixmap tmpPixmap;
    if(!pRcInfo)
        return tmpPixmap;
    if(pRcInfo->mRcType != RC_PNG)
        return tmpPixmap;
#ifdef PIXBUFFER
    //>@首先查看PIX
    tmpPixmap = pRcInfo->mRcPix.mPNG;
    if(tmpPixmap.isNull() == false)
    {
        return ScalePixmap(tmpPixmap, pSize);
    }
    //>@其次查找m_EffectPath，并将其插入m_EffectImage
    tmpPixmap = LoadPixmap(pRcInfo->mRcFile, pSize);
    if(tmpPixmap.isNull() == false)
    {
        pRcInfo->mRcPix.mPNG = tmpPixmap;
        return tmpPixmap;
    }
#else
    tmpPixmap = LoadPixmap(pRcInfo->mRcFile, pSize);
#endif
    return tmpPixmap;
}

//>@根据提供的资源标号，获取下一个资源的标号
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
    foreach(tmpRc, m_EffectRC)
    {
        if(!tmpRc)
            continue;
        if(tmpRc->mRcPix.mPNG == m_PixmapPointer.mCurPointer)
        {
            find = true;
            break;
        }
    }
    if(find)
    {
        int key = m_EffectRC.key(tmpRc);
        tmpRc = m_EffectRC.value(getNextRc(key));
        if(tmpRc)
            return tmpRc->mRcPix.mPNG;
    }
    return QPixmap();
}

QPixmap RcContainer::getPrevImg()
{
    bool find = false;
    RC_INFO* tmpRc=0;
    foreach(tmpRc, m_EffectRC)
    {
        if(!tmpRc)
            continue;
        if(tmpRc->mRcPix.mPNG == m_PixmapPointer.mCurPointer)
        {
            find = true;
            break;
        }
    }
    if(find)
    {
        int key = m_EffectRC.key(tmpRc);
        tmpRc = m_EffectRC.value(getPrevRc(key));
        if(tmpRc)
            return tmpRc->mRcPix.mPNG;
    }
    return QPixmap();
}

MAP_INFO* RcContainer::AddMap(int pSrc, int pMap)
{
    MAP_INFO *tmpMapInfo = 0;
    //>@同步修改ui文件
    QDomElement tmpSMElement = m_ComElement.firstChildElement("StateMachine");
    if(tmpSMElement.isNull())
        return tmpMapInfo;
    QDomElement tmpStateElement = tmpSMElement.firstChildElement("Map");
    if(tmpStateElement.isNull())
        return tmpMapInfo;
    RC_MAP tmpMap;
    tmpMap.mSrc = QString::number(pSrc);
    tmpMap.mMap = QString::number(pMap);
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    if(!m_UiContainer->m_UiManager->AddMap(tmpStateElement, tmpMap))
        return tmpMapInfo;
#endif
    tmpMapInfo = new MAP_INFO;
    tmpMapInfo->mMap = pMap;
    m_MapList.insert(pSrc, tmpMapInfo);
    return tmpMapInfo;
}

bool RcContainer::DelMap(int pSrc)
{
    //>@同步修改ui文件
    QDomElement tmpSMElement = m_ComElement.firstChildElement("StateMachine");
    if(tmpSMElement.isNull())
        return false;
    QDomElement tmpStateElement = tmpSMElement.firstChildElement("Map");
    if(tmpStateElement.isNull())
        return false;
    RC_MAP tmpMap;
    tmpMap.mSrc = QString::number(pSrc);
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    if(!m_UiContainer->m_UiManager->DelMap(tmpStateElement, tmpMap))
        return false;
#endif
    //>@
    MAP_INFO *tmpMapInfo = m_MapList.take(pSrc);
    delete tmpMapInfo;
    return true;
}

bool RcContainer::DelAllMap()
{
    //>@同步修改ui文件
    QDomElement tmpSMElement = m_ComElement.firstChildElement("StateMachine");
    if(tmpSMElement.isNull())
        return false;
    QDomElement tmpStateElement = tmpSMElement.firstChildElement("Map");
    if(tmpStateElement.isNull())
        return false;
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    if(!m_UiContainer->m_UiManager->ClearItemElement(tmpStateElement))
        return false;
#endif
    QList<int> tmpList = m_MapList.keys();
    for(int i=0;i<tmpList.count();i++)
    {
        MAP_INFO *tmpMapInfo = m_MapList.take(tmpList.at(i));
        if(tmpMapInfo)
            delete tmpMapInfo;
    }
    return true;
}

QString RcContainer::GetAttribute(QString pEffect, QString pName)
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

//>@路径 《属性列表》 值
bool RcContainer::SetPara(QDomElement &pElement, xmlParse *pParse)
{
    if(pElement.isNull() || pParse==0)
        return false;
#ifdef DISPLAY
    QString tmpCmd = pElement.text();
    if(!tmpCmd.isEmpty())
    {
        pParse->SetText(pElement, ExecCommand(tmpCmd));
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
#endif
    return true;
}

bool RcContainer::GetPara(QDomElement &pElement, xmlParse *pParse)
{
    if(pElement.isNull() || pParse==0)
        return false;
#ifdef DISPLAY
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
#endif
    return true;
}

QList<EFFECT*> RcContainer::GetDefaultEffects(QString pStateName)
{
    QList<EFFECT*> tmpList;
#ifdef IDE
    for(int i=0;i<m_ComCategory->StateMachines.count();i++)
    {
        STATEMACHINE *tmpSM = m_ComCategory->StateMachines.at(i);
        if(!tmpSM)
            continue;
        if(tmpSM->StateName.compare(pStateName))
            continue;
        tmpList = tmpSM->Effects;
        break;
    }
#endif
    return tmpList;
}

EFFECT* RcContainer::GetDefaultEffect(QList<EFFECT*> pEffects, QString pEffectName)
{
    for(int j=0;j<pEffects.count();j++)
    {
        EFFECT *tmpFindEffect = pEffects.at(j);
        if(!tmpFindEffect)
            continue;
        if(tmpFindEffect->EffectName.compare(pEffectName))
            continue;
        return tmpFindEffect;
    }
    return (EFFECT *)0;
}

EFFECT* RcContainer::GetDefaultEffect(QString pStateName, QString pEffectName)
{
    return GetDefaultEffect(GetDefaultEffects(pStateName), pEffectName);
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
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    tmpStateElement = m_UiContainer->m_UiManager->GetComState(m_ComElement, pStateName);
    if(!tmpStateElement.isNull())
    {
        pEffectName = tmpStateElement.attribute(D_EFFECTTYPE);
        pEffectPara = tmpStateElement.attribute(D_EFFECTPARA);
    }
#endif
    return tmpStateElement;
}

QString RcContainer::GetComStateEffect(QString pStateName)
{
    QDomElement tmpStateElement;
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    tmpStateElement = m_UiContainer->m_UiManager->GetComState(m_ComElement, pStateName);
#endif
    if(!tmpStateElement.isNull())
    {
        return tmpStateElement.attribute(D_EFFECTTYPE);
    }
    return QString();
}

QString RcContainer::GetComStateEffectPara(QString pStateName, QString pParaName)
{
    QDomElement tmpStateElement;
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    tmpStateElement = m_UiContainer->m_UiManager->GetComState(m_ComElement, pStateName);
#endif
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
#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET) || defined(SUPPORT_COMEDIT))
    tmpStateElement = m_UiContainer->m_UiManager->GetComState(m_ComElement, pStateName);
#endif
    if(tmpStateElement.isNull())
        return QString();
    QDomElement tmpRcElement = tmpStateElement.firstChildElement(pRcName);
    if(tmpRcElement.isNull())
        return QString();
    return tmpRcElement.text();
}

QString RcContainer::GetRelativeRcLocation()
{
    QString tmpName = GetComStateEffect(QString("Resource"));
    if(tmpName.isEmpty())
        return QString();
    QString tmpPath = m_ComPath + QString("/Resource/%1").arg(tmpName);
    tmpPath.replace("//", "/");
    return tmpPath;
}


