#include "uibase.h"

void RcItem::clear()
{
    if(mRcType != RC_INVALID){
        mRcFile = "";
        mRcType = RC_INVALID;
    }
    if(!mParameterHash.isEmpty())
        mParameterHash.clear();
}

QString UiBase::themeResourceDir = "";
QString UiBase::bundleResourceDir = "";
UiBase::UiBase(QObject *parent):QObject(parent)
{
    themeManager = NULL;
}

UiBase::UiBase(ThemeParser *pthemeManager)
{
    themeManager = pthemeManager;
}

UiBase::UiBase(ThemeParser *pthemeManager,QDomElement &pElement){
    themeManager = pthemeManager;
    initComponent(pElement);
}

void UiBase::setThemeManager(ThemeParser *pthemeManager)
{
    themeManager = pthemeManager;
}

UiBase::~UiBase()
{
    releaseRC();
}

void UiBase::releaseRC()
{
    if(!m_ComponentRCs.isEmpty())
    {
        QList<RcItem*> tmpRcInfoList = m_ComponentRCs.values();
        RcItem *tmpRcInfo = 0;
        foreach(tmpRcInfo, tmpRcInfoList)
        {
            if(tmpRcInfo)
                delete tmpRcInfo;
        }
        m_ComponentRCs.clear();
    }
}

bool UiBase::initComponent(QDomElement &pElement)
{
    if(pElement.isNull())
    {
        IDE_TRACE();
        return false;
    }

    if(themeManager == NULL)
    {
        IDE_TRACE();
        return false;
    }
    m_ComType = getComponetType(pElement.tagName());
    m_ComPath = getComponetPath(m_ComType);

    releaseRC();
    m_ComElement = pElement;
    QRect tmpHRect, tmpVRect;
    if(themeManager->getComRect(0, m_ComElement, tmpHRect, tmpVRect))
    {
        m_ComRectLayout[Horizontal] = QRectF(tmpHRect);
        m_ComRectLayout[Vertical] = QRectF(tmpVRect);

    }
    if(themeManager->getComRect(1, m_ComElement, tmpHRect, tmpVRect))
    {
        m_ComRectLayout1[Horizontal] = QRectF(tmpHRect);
        m_ComRectLayout1[Vertical] = QRectF(tmpVRect);

    }
    parserParameter(themeManager->getComRcElement(m_ComElement).text());
    QDomNodeList tmpRcList = themeManager->getComRcList(m_ComElement);
    //IDE_TRACE();
    for(int i=0;i<tmpRcList.count();i++)
    {
        QDomElement tmpElement = tmpRcList.at(i).toElement();
        QString tmpRcName = tmpElement.tagName();
        int index = getKey(tmpRcName);
        if(index == -1){
            IDE_TRACE_STR(tmpRcName);
            continue;
        }
        loadRc(index, tmpElement);
    }
    return true;
}

RcItem *UiBase::loadRc(int pKey, QDomElement pRcElement)
{
    //IDE_TRACE();
    if(pRcElement.isNull())
        return (RcItem *)0;
    QString tmpFile = pRcElement.text();
    if(tmpFile.isEmpty())
        return (RcItem *)0;
    RcItem *tmpRcInfo = m_ComponentRCs.value(pKey);
    if(!tmpRcInfo)
    {
        tmpRcInfo = new RcItem;
        if(!tmpRcInfo)
            return (RcItem *)0;
        m_ComponentRCs.insert(pKey, tmpRcInfo);
    }

    tmpRcInfo->clear();
    tmpRcInfo->mElement = pRcElement;
    tmpRcInfo->mRcFile = tmpFile;
    //IDE_TRACE_STR(tmpFile);
    tmpRcInfo->mRcType = getRcType(tmpFile);
    tmpRcInfo->mTipText = pRcElement.attribute(D_TIPTEXT);
    tmpRcInfo->mTipText.replace("\\n", "\n");
    return tmpRcInfo;
}

QString UiBase::getResourcePath(int index)
{
    RcItem *tmpRcInfo = m_ComponentRCs.value(index);
    if(!tmpRcInfo){
        IDE_TRACE();
        return "";
    }else{
        QString rcPath = themeResourceDir + tmpRcInfo->mRcFile;
        //IDE_TRACE_STR(rcPath);
        if(QFile::exists(rcPath)){
            return D_QMLRESSHCEL +  rcPath;
        }else{
            IDE_TRACE_STR(rcPath);
            return "";
        }
    }
}

bool UiBase::initGeometry(DIRECTION direction, int layout)
{

     Q_UNUSED(direction);
     Q_UNUSED(layout);
}

bool UiBase::initUi(DIRECTION direction)
{
    Q_UNUSED(direction);
}

void UiBase::parserParameter(QString pParameter)
{
    if(pParameter.isEmpty())
        return;
    //IDE_TRACE_STR(pParameter);
    QStringList tmpParaList = pParameter.split(';');
    for(int i=0;i<tmpParaList.count();i++)
    {
        QStringList tmpList = tmpParaList.at(i).split(':');
        if(tmpList.count() != 2)
            continue;
        m_ComponentParameter.insert(tmpList.at(0),tmpList.at(1));
    }
}


QRectF UiBase::getComRect(DIRECTION direction, int layout){
    QRectF rect;
    if(Horizontal == direction || Vertical == direction){
        if(layout == 0){
            rect = m_ComRectLayout[direction];
        }else if(layout == 1){
            rect = m_ComRectLayout1[direction];
        }else{

        }
    }
    return  rect;
}


bool UiBase::getComResource(int index,QString &rcPath,QString &tipText){
    RcItem *tmpRcInfo = m_ComponentRCs.value(index);
    if(!tmpRcInfo)
    {
        return false;
    }
    QString path = themeResourceDir + tmpRcInfo->mRcFile;
    if(QFile::exists(path)){
        rcPath = D_QMLRESSHCEL + path;
    }else{
        IDE_TRACE_STR(path);
        rcPath = "";
    }
    tipText = tmpRcInfo->mTipText;
    return true;
}

bool UiBase::setQmlProperty(char *name, const QVariant &value){
    if(m_QmlItem){
        return m_QmlItem->setProperty(name, value);
    }else
        return false;
}

bool UiBase::invokeQmlMethod(char *name,QVariant arg1){
    if(m_QmlItem){
        QVariant returnedValue;
        return   QMetaObject::invokeMethod(m_QmlItem, name,
                                           Q_RETURN_ARG(QVariant, returnedValue),
                                           Q_ARG(QVariant, arg1));
    }else
        return false;

}

bool UiBase::invokeQmlMethod(char *name, QVariant arg1, QVariant arg2){
    if(m_QmlItem){
        QVariant returnedValue;
        return   QMetaObject::invokeMethod(m_QmlItem, name,
                                           Q_RETURN_ARG(QVariant, returnedValue),
                                           Q_ARG(QVariant, arg1),
                                           Q_ARG(QVariant, arg2));
    }else
        return false;
}

bool UiBase::invokeQmlMethod(char *name,QVariant arg1,QVariant arg2,QVariant arg3){
    if(m_QmlItem){
        QVariant returnedValue;
        return   QMetaObject::invokeMethod(m_QmlItem, name,
                                           Q_RETURN_ARG(QVariant, returnedValue),
                                           Q_ARG(QVariant, arg1),
                                           Q_ARG(QVariant, arg2),
                                           Q_ARG(QVariant, arg3));
    }else
        return false;
}

bool UiBase::invokeQmlMethod(char *name,QVariant arg1,QVariant arg2,QVariant arg3,QVariant arg4){
    if(m_QmlItem){
        QVariant returnedValue;
        return   QMetaObject::invokeMethod(m_QmlItem, name,
                                           Q_RETURN_ARG(QVariant, returnedValue),
                                           Q_ARG(QVariant, arg1),
                                           Q_ARG(QVariant, arg2),
                                           Q_ARG(QVariant, arg3),
                                           Q_ARG(QVariant, arg4));
    }else
        return false;
}

