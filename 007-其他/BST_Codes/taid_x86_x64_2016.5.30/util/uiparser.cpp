#include "uiparser.h"
const QString UiParser::UiSuffix = ".ui";
const QString UiParser::UiFileType = "BST UI File";

QRect GetRectFromString(QString pRectString)
{
    if(pRectString.isEmpty())
        return QRect();
    QStringList tmpList = pRectString.split(",");
    if(tmpList.count() != 4)
    {
        return QRect();
    }
    int tmpX = tmpList.at(0).toInt();
    int tmpY = tmpList.at(1).toInt();
    int tmpW = tmpList.at(2).toInt();
    int tmpH = tmpList.at(3).toInt();
    if(tmpW<=0 || tmpH<=0)
    {
        return QRect();
    }
    return QRect(tmpX, tmpY, tmpW, tmpH);
}

QString GetStringFromRect(QRect pRect)
{
    if(pRect.isEmpty())
        return QString();
    return QString("%1,%2,%3,%4").arg((int)pRect.left()).arg((int)pRect.top()).arg((int)pRect.width()).arg((int)pRect.height());
}

QRectF GetRotateRect(QSizeF pParentSize, QRectF pRect, ROTATE_ANGLE pAngle)
{
    if(pParentSize.isValid() == false || pRect.isValid() == false)
        return QRectF();
    qreal ParWidth = pParentSize.width();
    qreal ParHeight = pParentSize.height();
    QRectF tmpRect;
    int value;
    switch(pAngle)
    {
    case ROTATE_90: //>@均为顺时针旋转
    {
        //>@获取左下角顶点相对于父窗口左下角顶点的距离
        value = ParHeight - pRect.top() - pRect.height();
        tmpRect.setLeft(value);
        value = pRect.left();
        tmpRect.setTop(value);
        tmpRect.setWidth(pRect.height());
        tmpRect.setHeight(pRect.width());
        break;
    }
    case ROTATE_180:
    {
        //>@获取右下角顶点为起点
        value = ParHeight - pRect.top() - pRect.height();
        tmpRect.setLeft(value);
        value = ParWidth - pRect.left() - pRect.width();
        tmpRect.setTop(value);
        tmpRect.setWidth(pRect.width());
        tmpRect.setHeight(pRect.height());
        break;
    }
    case ROTATE_270:
    {
        //>@获取右上角顶点为起点
        value = pRect.top();
        tmpRect.setLeft(value);
        value = ParWidth - pRect.left() - pRect.width();
        tmpRect.setTop(value);
        tmpRect.setWidth(pRect.height());
        tmpRect.setHeight(pRect.width());
        break;
    }
    default:
        return QRectF();
    }
    return tmpRect;
}

CONSTRAINT GetRotateConstraint(CONSTRAINT pConstraint, ROTATE_ANGLE pAngle)
{
    CONSTRAINT tmpConstraint;
    switch(pAngle)
    {
    case ROTATE_90: //>@均为顺时针旋转
    {
        tmpConstraint.mDirection = (DIRECTION)((pConstraint.mDirection + 1) % 2);
        tmpConstraint.mOrientation = (ORIENTATION)((pConstraint.mOrientation + 1) % 2);
        break;
    }
    case ROTATE_180:
    {
        tmpConstraint.mDirection = (DIRECTION)(pConstraint.mDirection);
        tmpConstraint.mOrientation = (ORIENTATION)((pConstraint.mOrientation + 1) % 2);
        break;
    }
    case ROTATE_270:
    {
        tmpConstraint.mDirection = (DIRECTION)((pConstraint.mDirection + 1) % 2);
        tmpConstraint.mOrientation = (ORIENTATION)(pConstraint.mOrientation);
        break;
    }
    default:
        return pConstraint;
    }
    return tmpConstraint;
}

UI_TYPE GetUiType(QString pUiName)
{
    UI_TYPE tmpComType = UI_INVALID;
    if (pUiName.isNull())
        return tmpComType;
    else if (pUiName.isEmpty() ||
             pUiName.compare(QString("Desktop"), Qt::CaseInsensitive) == 0 ||
             pUiName.compare(QString("/"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = UI_DESKTOP;
    }
    else if(pUiName.compare(QString("SetBar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = UI_SETBAR;
    }
    else if(pUiName.compare(QString("MessageBar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = UI_MESSAGEBAR;
    }
    else if(pUiName.compare(QString("ProgressBar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = UI_PROGRESSBAR;
    }
    else if(pUiName.compare(QString("SrnSaveForm"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = UI_SRNSAVEFORM;
    }
    else if(!pUiName.compare(QString("Animal"), Qt::CaseInsensitive))
    {
        tmpComType = UI_ANIMAL;
    }
    else if(!pUiName.compare(QString("Floor"), Qt::CaseInsensitive))
    {
        tmpComType = UI_FLOOR;
    }
    else if(!pUiName.compare(QString("DtnEle"), Qt::CaseInsensitive))
    {
        tmpComType = UI_DTNELE;
    }
    else if(!pUiName.compare(QString("Arrow"), Qt::CaseInsensitive))
    {
        tmpComType = UI_ARROW;
    }
    else if(!pUiName.compare(QString("Function"), Qt::CaseInsensitive))
    {
        tmpComType = UI_FUNCTION;
    }
    else if(!pUiName.compare(QString("FlrSign"), Qt::CaseInsensitive))
    {
        tmpComType = UI_FLRSIGN;
    }
    else if(!pUiName.compare(QString("RenCode"), Qt::CaseInsensitive))
    {
        tmpComType = UI_RENCODE;
    }
    else if(!pUiName.compare(QString("Media"), Qt::CaseInsensitive))
    {
        tmpComType = UI_MEDIA;
    }
    else if(!pUiName.compare(QString("Image"), Qt::CaseInsensitive))
    {
        tmpComType = UI_IMAGE;
    }
    else if(!pUiName.compare(QString("Picture"), Qt::CaseInsensitive))
    {
        tmpComType = UI_PICTURE;
    }
    else if(!pUiName.compare(QString("Video"), Qt::CaseInsensitive))
    {
        tmpComType = UI_VIDEO;
    }
    else if(!pUiName.compare(QString("WebView"), Qt::CaseInsensitive))
    {
        tmpComType = UI_WEBVIEW;
    }
    else if(!pUiName.compare(QString("Phone"), Qt::CaseInsensitive))
    {
        tmpComType = UI_PHONE;
    }
    else if(!pUiName.compare(QString("Text"), Qt::CaseInsensitive))
    {
        tmpComType = UI_TEXT;
    }
    else if(!pUiName.compare(QString("Title"), Qt::CaseInsensitive))
    {
        tmpComType = UI_TITLE;
    }
    else if(!pUiName.compare(QString("Logo"), Qt::CaseInsensitive))
    {
        tmpComType = UI_LOGO;
    }
    else if(!pUiName.compare(QString("Weather"), Qt::CaseInsensitive))
    {
        tmpComType = UI_WEATHER;
    }
    else if(!pUiName.compare(QString("Clock"), Qt::CaseInsensitive))
    {
        tmpComType = UI_CLOCK;
    }
    else if(!pUiName.compare(QString("Date"), Qt::CaseInsensitive))
    {
        tmpComType = UI_DATE;
    }
    else if(!pUiName.compare(QString("Time"), Qt::CaseInsensitive))
    {
        tmpComType = UI_TIME;
    }
    else if(!pUiName.compare(QString("NamePlate"), Qt::CaseInsensitive))
    {
        tmpComType = UI_NAMEPLATE;
    }
    else if(!pUiName.compare(QString("FlrLCD"), Qt::CaseInsensitive))
    {
        tmpComType = UI_FLRLCD;
    }
    else if(!pUiName.compare(QString("FlrBtn"), Qt::CaseInsensitive))
    {
        tmpComType = UI_FLRBTN;
    }
    else if(!pUiName.compare(QString("KeyboardBtn"), Qt::CaseInsensitive))
    {
        tmpComType = UI_KEYBOARDBTN;
    }
    else if(!pUiName.compare(QString("CallBtn"), Qt::CaseInsensitive))
    {
        tmpComType = UI_CALLBTN;
    }
    else if(!pUiName.compare(QString("Talkback"), Qt::CaseInsensitive))
    {
        tmpComType = UI_TALKBACKBTN;
    }
    else if(!pUiName.compare(QString("EmergencyBtn"), Qt::CaseInsensitive))
    {
        tmpComType = UI_EMERGENCYBTN;
    }
    else if(!pUiName.compare(QString("TalkbackBtn"), Qt::CaseInsensitive))
    {
        tmpComType = UI_TALKBACKBTN;
    }
    else if(!pUiName.compare(QString("DoorBtn"), Qt::CaseInsensitive))
    {
        tmpComType = UI_DOORBTN;
    }
    else if(!pUiName.compare(QString("SetBtn"), Qt::CaseInsensitive))
    {
        tmpComType = UI_SETBTN;
    }
    else if(!pUiName.compare(QString("DisabilityBtn"), Qt::CaseInsensitive))
    {
        tmpComType = UI_DISABILITYBTN;
    }
    else if(!pUiName.compare(QString("Status"), Qt::CaseInsensitive))
    {
        tmpComType = UI_STATUS;
    }
    else if(pUiName.compare(QString("enum"), Qt::CaseInsensitive) == 0)
        return UI_ENUM;
    else if(pUiName.compare(QString("string"), Qt::CaseInsensitive) == 0)
        return UI_STRING;
    else if(pUiName.compare(QString("enetaddr"), Qt::CaseInsensitive) == 0)
        return UI_ENETADDR;
    else if(pUiName.compare(QString("num"), Qt::CaseInsensitive) == 0)
        return UI_NUM;
    else if(pUiName.compare(QString("check"), Qt::CaseInsensitive) == 0)
        return UI_CHECK;
    else if(pUiName.compare(QString("button"), Qt::CaseInsensitive) == 0)
        return UI_BUTTON;
    else if(pUiName.compare(QString("bool"), Qt::CaseInsensitive) == 0)
        return UI_BOOL;
    else if(pUiName.compare(QString("link"), Qt::CaseInsensitive) == 0)
        return UI_LINK;
    else if(pUiName.compare(QString("label"), Qt::CaseInsensitive) == 0)
        return UI_LABEL;
    return tmpComType;
}

QString GetUiName(UI_TYPE pUiType)
{
    switch(pUiType)
    {
    case UI_DESKTOP:
        return QString("Desktop");
    case UI_SETBAR:
        return QString("SetBar");
    case UI_MESSAGEBAR:
        return QString("MessageBar");
    case UI_PROGRESSBAR:
        return QString("ProgressBar");
    case UI_SRNSAVEFORM:
        return QString("SrnSaveForm");
    case UI_ANIMAL:
        return QString("Animal");
    case UI_FLOOR:
        return QString("Floor");
    case UI_DTNELE:
        return QString("DtnEle");
    case UI_ARROW:
        return QString("Arrow");
    case UI_FUNCTION:
        return QString("Function");
    case UI_RENCODE:
        return QString("RenCode");
    case UI_FLRSIGN:
        return QString("FlrSign");
    case UI_MEDIA:
        return QString("Media");
    case UI_IMAGE:
        return QString("Image");
    case UI_PICTURE:
        return QString("Picture");
    case UI_VIDEO:
        return QString("Video");
    case UI_WEBVIEW:
        return QString("WebView");
    case UI_PHONE:
        return QString("Phone");
    case UI_TEXT:
        return QString("Text");
    case UI_TITLE:
        return QString("Title");
    case UI_LOGO:
        return QString("Logo");
    case UI_WEATHER:
        return QString("Weather");
    case UI_CLOCK:
        return QString("Clock");
    case UI_DATE:
        return QString("Date");
    case UI_TIME:
        return QString("Time");
    case UI_NAMEPLATE:
        return QString("NamePlate");
    case UI_FLRLCD:
        return QString("FlrLCD");
    case UI_FLRBTN:
        return QString("FlrBtn");
    case UI_KEYBOARDBTN:
        return QString("KeyboardBtn");
    case UI_CALLBTN:
        return QString("CallBtn");
    case UI_PHONEBTN:
        return QString("PhoneBtn");
    case UI_EMERGENCYBTN:
        return QString("EmergencyBtn");
    case UI_TALKBACKBTN:
        return QString("TalkbackBtn");
    case UI_DOORBTN:
        return QString("DoorBtn");
    case UI_SETBTN:
        return QString("SetBtn");
    case UI_DISABILITYBTN:
        return QString("DisabilityBtn");
    case UI_STATUS:
        return QString("Status");
    case UI_ENUM:
        return QString("enum");
    case UI_STRING:
        return QString("string");
    case UI_ENETADDR:
        return QString("enetaddr");
    case UI_NUM:
        return QString("num");
    case UI_CHECK:
        return QString("check");
    case UI_BUTTON:
        return QString("button");
    case UI_BOOL:
        return QString("bool");
    case UI_LINK:
        return QString("link");
    case UI_LABEL:
        return QString("label");
    default:
        return QString();
    }
    return QString();
}

QString GetUiPath(UI_TYPE pType)
{
    if(pType == UI_DESKTOP)
        return QString("/");
    QString tmpName = GetUiName(pType);
    if(!tmpName.isEmpty())
        return tmpName.prepend('/');
    return tmpName;
}

UI_TYPE GetParentUiType(UI_TYPE pType)
{
    if(pType > UI_AREA && pType < UI_COMPONENT)
    {
        return UI_DESKTOP;
    }
    return UI_INVALID;
}

UI_TYPE GetParentUiType(QString pUiPath)
{
    if(pUiPath.isEmpty())
        return UI_INVALID;
    int index1,index2;
    QString tmpString;
    index1 = pUiPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
        return UI_INVALID;
    index2 = pUiPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0) //>@desktop and rect
    {
        tmpString = pUiPath.mid(index1);
        if(tmpString.length() == 1)
        {
            return UI_INVALID;
        }
        return UI_DESKTOP;
    }
    else
    {
        tmpString = pUiPath.mid(index1+1, index2-index1-1);
    }
    return GetUiType(tmpString);
}

QString GetParentUiPath(QString pUiPath)
{
    int index1,index2;
    QString tmpString;
    index1 = pUiPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
        return tmpString;
    index2 = pUiPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0)
    {
        tmpString = pUiPath.mid(index1);
        if(tmpString.length() == 1)
            return QString("");     //>@Empty表示当前控件为Desktop
        else
            return QString("/");
    }
    else
    {
        tmpString = pUiPath.mid(index1, index2-index1);
    }
    return tmpString;
}

QString GetCurUiName(QString pUiPath)
{
    if(pUiPath.isEmpty())
        return QString();
    int index1,index2;
    index1 = pUiPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
    {
        if(pUiPath.compare(QString("Desktop"), Qt::CaseInsensitive) == 0)
            return QString("Desktop");
        else
            return QString();
    }
    QString tmpString;
    index2 = pUiPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0)
    {
        tmpString = pUiPath.mid(index1+1);
        if(tmpString.isEmpty())
            return QString("Desktop");     //>@Empty表示Desktop
        else
            return tmpString;
    }
    else
    {
        tmpString = pUiPath.mid(index2+1);
    }
    return tmpString;
}

UI_TYPE GetCurUiType(QString pUiPath)
{
    QString tmpString;
    int index1,index2;
    index1 = pUiPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
    {
        return GetUiType(pUiPath);
    }
    index2 = pUiPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0)
    {
        tmpString = pUiPath.mid(index1+1);
        if(tmpString.isEmpty())
            return UI_DESKTOP;     //>@Empty表示Desktop
    }
    else
    {
        tmpString = pUiPath.mid(index2+1);
    }
    return GetUiType(tmpString);
}

/*
Para参数类型：
    Cache:0@true,false;
    Volume:80@0~100;
    Interval:2@1000,1500,2000,3000;
*/
QStringList PreprocPara(QString &pContent)
{
    QStringList tmpRetList;
    int index = pContent.indexOf('@');
    if(index > 0)
    {
        QString tmpValue = pContent.mid(0, index);

        tmpRetList = pContent.mid(index+1).split(D_ENUMSPLIT);
        if(!tmpRetList.isEmpty())
        {
            int tmpNum = tmpValue.toInt();
            if(tmpNum >= tmpRetList.count())
                tmpNum = tmpRetList.count() - 1;
            pContent = tmpRetList.at(tmpNum);
            tmpRetList.prepend(D_ENUMSPLIT);
        }

        tmpRetList = pContent.mid(index+1).split(D_RANGESPLIT);
        if(tmpRetList.count() == 2)
        {
            int tmpNum = tmpValue.toInt();
            int tmpMax = tmpRetList.at(1).toInt();
            int tmpMin = tmpRetList.at(0).toInt();
            if(tmpNum < tmpMin)
                tmpNum = tmpMin;
            else if(tmpNum > tmpMax)
                tmpNum = tmpMax;
            pContent = QString::number(tmpNum);
            tmpRetList.prepend(D_RANGESPLIT);
        }
    }
    return tmpRetList;
}

UiParaParser::UiParaParser(UiComParser *pComParser)
{
    mComParser = pComParser;
}

//xx:xx;
UiParaParser::UiParaParser(QString pString, UiComParser *pComParser)
{
    mComParser = pComParser;
    SetString(pString);
}

UiParaParser::~UiParaParser()
{
    mParaHash.clear();
}

void UiParaParser::SetString(QString pString)
{
    if(pString.isEmpty())
        return;
    QStringList tmpParaList = pString.split(';');
    foreach(QString tmpPara, tmpParaList)
    {
        if(tmpPara.isEmpty())
            continue;
        QStringList tmpList = tmpPara.split(':');
        if(tmpList.count() != 2)
            continue;
        mComParser->ParsePara(tmpList.at(0), tmpList.at(1), mParaHash);
    }
}

QVariant UiParaParser::GetPara(int pName)
{
    QVariant tmpVar = mParaHash.value(pName);
    if(tmpVar.type() == QVariant::List)
    {
        QList<QVariant> tmpList1 = tmpVar.toList();
        int index = tmpList1.at(0).toInt();
        QList<QVariant> tmpList2 = tmpList1.at(1).toList();
        if(index < tmpList2.count())
            return tmpList2.at(index);
        else
            return QVariant();
    }
    return tmpVar;
}

void UiParaParser::SetPara(int pName, QVariant pValue)
{
    QVariant tmpVar = mParaHash.value(pName);
    if(tmpVar.isNull())
    {
        mParaHash.insert(pName, pValue);
    }
    else
    {
        if(tmpVar.type() == QVariant::List) //>@如果是XX:1@xx,xx类型的，则pValue是序号
        {
            QList<QVariant> tmpList1 = tmpVar.toList();

            QList<QVariant> tmpList;
            tmpList.append(pValue);
            tmpList.append(tmpList1.at(1));

            mParaHash.insert(pName, tmpList);
        }
        else
        {
            mParaHash.insert(pName, pValue);
        }
    }
}

QString UiParaParser::GetString(_MergePara pMergeParaFun)
{
    QString tmpParaString;
    QHash<int, QVariant>::const_iterator i = mParaHash.constBegin();
    while (i != mParaHash.constEnd())
    {
        int tmpKey = i.key();
        tmpParaString.append(pMergeParaFun(tmpKey, i.value()));
        tmpParaString.append(';');
        ++i;
    }
    return tmpParaString;
}

UiRectParser::UiRectParser(QDomElement pElement, UiComParser *pComParser) : UiParaParser(pComParser)
{
    mHVSame = pElement.hasAttribute("HV");
    if(mHVSame)
    {
        mRect[0] = GetRectFromString(pElement.attribute("HV"));
    }
    else
    {
        mRect[Vertical] = GetRectFromString(pElement.attribute("V"));
        mRect[Horizontal] = GetRectFromString(pElement.attribute("H"));
    }
    mElement = pElement;
}

QRect UiRectParser::GetRect(DIRECTION pDirection)
{
    if(pDirection < HVBoth)
        return mRect[pDirection];
    return QRect();
}

void UiRectParser::SetRect(DIRECTION pDirection, QRect pRect)
{
    if(pDirection < HVBoth)
    {
        mHVSame = false;
        mRect[pDirection] = pRect;
        mElement.setAttribute(GetDirectionString(pDirection), GetStringFromRect(pRect));
        //>@查看横竖坐标是否存在，不存在则创建
        //>@在同步修改另一种类型坐标时，只修改宽高度，不修改左上角坐标；
        //>@并且如果是新插入坐标，则把左上角坐标设为0；如果是修改坐标，则保持左上角坐标不变
        DIRECTION tmpOppside;
        if(pDirection == Horizontal)
            tmpOppside = Vertical;
        else
            tmpOppside = Horizontal;
        mRect[tmpOppside] = QRect(mRect[tmpOppside].topLeft(), pRect.size());
        mElement.setAttribute(GetDirectionString(tmpOppside), GetStringFromRect(mRect[tmpOppside]));
    }
    else if(pDirection == HVBoth)
    {
        mHVSame = true;
        mRect[0] = pRect;
        mElement.removeAttribute("H");
        mElement.removeAttribute("V");
        mElement.setAttribute("HV", GetStringFromRect(pRect));
    }
}

UiRectListParser::UiRectListParser(QDomElement pElement, UiComParser *pComParser) : UiParaParser(pComParser)
{
    QDomNodeList tmpRectList = pElement.childNodes();
    for(int i=0;i<tmpRectList.count();i++)
    {
        QDomElement tmpRectElement = tmpRectList.at(i).toElement();
        if(tmpRectElement.isNull())
            continue;
        QString tmpLayoutStr = tmpRectElement.tagName();
        if(!tmpLayoutStr.startsWith("Layout", Qt::CaseInsensitive))
            continue;
        int tmpLayout = tmpLayoutStr.mid(6).toInt();//layout
        UiRectParser *tmpRectParser = new UiRectParser(tmpRectElement, mComParser);
        mRects.insert(tmpLayout, tmpRectParser);
    }
    SetString(pElement.attribute(D_PARA));
    mElement = pElement;
}

UiRectListParser::~UiRectListParser()
{
    foreach(UiRectParser* tmpRectPair, mRects)
    {
        if(tmpRectPair)
            delete tmpRectPair;
    }
}

UiRectParser* UiRectListParser::GetRect(int pLayout)
{
    //IDE_TRACE();
    return mRects.value(pLayout);
}

QRect UiRectListParser::GetRect(int pLayout, DIRECTION pDirection)
{
    UiRectParser *tmpRectPair = GetRect(pLayout);
    if(!tmpRectPair)
        return QRect();
    if(pDirection < HVBoth)
        return tmpRectPair->mRect[pDirection];
    return QRect();
}

bool UiRectListParser::SetRect(int pLayout, DIRECTION pDirection, QRect pRect)
{
    UiRectParser *tmpRectPair = GetRect(pLayout);
    if(!tmpRectPair)
        return false;
    tmpRectPair->SetRect(pDirection, pRect);
    return true;
}

UiRectParser* UiRectListParser::AddRect(int pLayout, XmlParser* pParser)
{
    UiRectParser *tmpRectPair = GetRect(pLayout);
    if(!tmpRectPair)
    {
        QDomElement tmpRectElement = pParser->createElement(QString("Layout%1").arg(pLayout));
        //>@顺序插入
        QList<int> tmpList = mRects.keys();
        qSort(tmpList.begin(), tmpList.end());
        QDomElement tmpAfter;
        foreach(int tmpLayout, tmpList)
        {
            if(pLayout < tmpLayout)
            {
                tmpAfter = mRects.value(tmpLayout)->mElement;
                break;
            }
        }
        mElement.insertBefore(tmpRectElement, tmpAfter);
        //
        tmpRectPair = new UiRectParser(tmpRectElement, mComParser);
        mRects.insert(pLayout, tmpRectPair);
    }
    return tmpRectPair;
}

void UiRectListParser::DelRect(int pLayout)
{
    UiRectParser *tmpRectPair = mRects.take(pLayout);
    if(tmpRectPair)
    {
        mElement.removeChild(tmpRectPair->mElement);
        delete tmpRectPair;
    }
}




UiRcParser::UiRcParser(QDomElement pElement, UiComParser *pComParser) : UiParaParser(pComParser)
{
    mHVSame = pElement.hasAttribute("HV");
    if(!mHVSame)
    {
        mFile[Vertical] = pElement.attribute("V");
        mFile[Horizontal] = pElement.attribute("H");
        if(!mFile[Vertical].isEmpty() && !mFile[Vertical].compare(mFile[Horizontal]))
        {
            UpdateHV(true);
            mElement.setAttribute("HV", mFile[Vertical]);
        }
    }
    else
    {
        mFile[Vertical] = mFile[Horizontal] = pElement.attribute("HV");
    }
    if(!mFile[Vertical].isEmpty())
    {

        mFile[Vertical].prepend(pComParser->mUiParser->m_FileDir);

        if(!QFile::exists(mFile[Vertical])){
            mElement.removeAttribute("V");
            mFile[Vertical] = "";
        }
    }
    if(!mFile[Horizontal].isEmpty())
    {
        mFile[Horizontal].prepend(pComParser->mUiParser->m_FileDir);
        if(!QFile::exists(mFile[Horizontal])){
            mElement.removeAttribute("H");
            mFile[Horizontal] = "";
        }
    }
    //IDE_TRACE_STR(mFile[Horizontal]);
    mElement = pElement;
    mCurIndex = HVNone;
}

UiRcParser::~UiRcParser()
{
    foreach(UD_MAPITEM *tmpItem, mMaps)
    {
        if(tmpItem)
            delete tmpItem;
    }
    mMaps.clear();
}

void UiRcParser::UpdateHV(bool pSame)
{
    if(pSame)
    {
        mHVSame = true;
        mElement.removeAttribute("H");
        mElement.removeAttribute("V");
    }
    else
    {
        mHVSame = false;
        mElement.removeAttribute("HV");
    }
}

bool UiRcParser::SetRc(QString pDir,QString pFile, DIRECTION pDirection)
{

    if(!pFile.startsWith(pDir, Qt::CaseInsensitive)){
        IDE_TRACE();
        return true;
    }
    if(!QFile::exists(pFile)){
        IDE_TRACE_STR(pFile);
        IDE_TRACE();
        return true;
    }

    QString tmpFile = pFile.mid(pDir.size());

    if(pDirection < HVBoth)
    {
        DIRECTION tmpOppside;
        if(pDirection == Horizontal){
            tmpOppside = Vertical;
        }else{
            tmpOppside = Horizontal;
        }

        if(pFile.compare(mFile[tmpOppside], Qt::CaseInsensitive))
        {
            mHVSame = false;
            mFile[pDirection] = pFile;
            mElement.setAttribute(GetDirectionString(pDirection), tmpFile);
            mType[pDirection] = GetRcType(pFile);
        }
        else
        {
            mFile[0] = pFile;
            mType[0] = GetRcType(pFile);
            UpdateHV(true);
            mElement.setAttribute("HV", tmpFile);
        }
    }
    else if(pDirection == HVBoth)
    {
        //IDE_TRACE_STR(pFile);
        mFile[0] = pFile;
        mFile[1] = pFile;
        mType[0] = GetRcType(pFile);
        UpdateHV(true);
        mElement.setAttribute("HV", tmpFile);
    }

    return true;

}

QString UiRcParser::GetAttr(QString attrName){
    return mElement.attribute(attrName);
}

void UiRcParser::SetAttr(QString attrName,QString value){
    mElement.setAttribute(attrName, value);
}

UiRcListParser::UiRcListParser(QDomElement pElement, UiComParser *pComParser) : UiParaParser(pComParser)
{
    mCurKey = 0;
    mElement = pElement;
    QString tmpInitValue = pElement.attribute(D_INITVALUE);
    if(!tmpInitValue.isEmpty())
        mCurKey = tmpInitValue.toInt();
    else
        pElement.setAttribute(D_INITVALUE, mCurKey);/////
    mPrefix = pElement.attribute(D_PREFIX);
    if(mPrefix.isEmpty())
        mPrefix = D_DEFAULT_PREFIX;
    QDomNodeList tmpNodeList = pElement.childNodes();
    for(int i=0;i<tmpNodeList.count();i++)              //>@插入ui的子元素
    {
        QDomElement tmpElement = tmpNodeList.at(i).toElement();
        QString tmpName = tmpElement.tagName();
        int tmpKey = GetKey(tmpName);
        if(tmpKey < 0)
            continue;
        UiRcParser* tmpRcInfo = mRCs.take(tmpKey);
        if(tmpRcInfo)
            delete tmpRcInfo;
        mRCs.insert(tmpKey, new UiRcParser(tmpElement, mComParser));
    }
    QStringList tmpParaList = pElement.attribute(D_PARA).split(';');
    foreach(QString tmpPara, tmpParaList)
    {
        if(tmpPara.isEmpty())
            continue;
        QStringList tmpList = tmpPara.split(':');
        if(tmpList.count() != 2)
            continue;
        mComParser->ParseRcPara(tmpList.at(0), tmpList.at(1), mParaHash);
    }
}

UiRcListParser::~UiRcListParser()
{
    mCurKey = 0;
    if(!mRCs.isEmpty())
    {
        foreach(UiRcParser* tmpRcInfo, mRCs)
        {
            if(tmpRcInfo)
                delete tmpRcInfo;
        }
        mRCs.clear();
    }
}

bool UiRcListParser::Contains(int pKey)
{
    return mRCs.contains(pKey);
}

UiRcParser* UiRcListParser::GetRc(int pKey)
{
    return mRCs.value(pKey);
}

UiRcParser* UiRcListParser::AddRc(int pKey, XmlParser* pParser,bool forceAdd)
{
    if(!mRCs.contains(pKey))
    {
        //IDE_TRACE();
        QDomElement tmpElement = pParser->createElement(QString("RC%1").arg(pKey));
        //>@顺序插入
        QList<int> tmpList = mRCs.keys();
        qSort(tmpList.begin(), tmpList.end());
        QDomElement tmpAfter;
        foreach(int tmpKey, tmpList)
        {
            if(pKey < tmpKey)
            {
                tmpAfter = mRCs.value(tmpKey)->mElement;
                break;
            }
        }
        mElement.insertBefore(tmpElement, tmpAfter);
        UiRcParser *tmpRcParser = new UiRcParser(tmpElement, mComParser);
        //IDE_TRACE_INT(pKey);
        mRCs.insert(pKey, tmpRcParser);
    }
    return GetRc(pKey);
}

void UiRcListParser::AddRc(int pKey, UiRcParser* pRcInfo)
{
    if(pKey < 0 || !pRcInfo)
        return;
    UiRcParser* tmpRcInfo = mRCs.take(pKey);
    if(tmpRcInfo)
    {
        mElement.removeChild(tmpRcInfo->mElement);
        delete tmpRcInfo;
    }
    //>@顺序插入
    QList<int> tmpList = mRCs.keys();
    qSort(tmpList.begin(), tmpList.end());
    QDomElement tmpAfter;
    foreach(int tmpKey, tmpList)
    {
        if(pKey < tmpKey)
        {
            tmpAfter = mRCs.value(tmpKey)->mElement;
            break;
        }
    }
    mElement.insertBefore(pRcInfo->mElement, tmpAfter);
    mRCs.insert(pKey, pRcInfo);
}

void UiRcListParser::DelRc(UiRcParser* pRcInfo)
{
    if(pRcInfo)
    {
        int tmpKey = mRCs.key(pRcInfo);
        //if(mCurKey == tmpKey)
            //mCurKey = INVALIDRC;
        mRCs.remove(tmpKey);
        mElement.removeChild(pRcInfo->mElement);
        delete pRcInfo;
    }
}

void UiRcListParser::DelRc(int pKey)
{
    //if(mCurKey == pKey)
       // mCurKey = INVALIDRC;
    UiRcParser *tmpRcInfo = mRCs.take(pKey);
    if(tmpRcInfo)
    {
        mElement.removeChild(tmpRcInfo->mElement);
        delete tmpRcInfo;
    }
}

UiOperateParser::UiOperateParser(EffectType pType, QDomElement pElement, UiComParser *pComParser) : QObject()
{
    mRunMode = QTimeLine::NotRunning;
    mComParser = pComParser;
    mAnimatePara = 0;

    QStringList tmpParaList = pElement.attribute("Para").split(';');
    if(!tmpParaList.isEmpty())
    {
        mAnimatePara = new AREA_ANIMATE;
        mAnimatePara->mEffectType = pType;
        foreach(QString tmpPara, tmpParaList)
        {
            if(tmpPara.isEmpty())
                continue;
            QStringList tmpList = tmpPara.split(':');
            if(tmpList.count() != 2)
                continue;
            mComParser->ParseSubOperationPara(tmpList.at(0), tmpList.at(1), mAnimatePara);
        }
    }
}

UiOperateParser::~UiOperateParser()
{

    if(mAnimatePara)
        delete mAnimatePara;
}


void UiOperateParser::SetPara(UiOperateParser::ParaType pType, int pPara)
{
    switch(pType)
    {
    case E_DIRECTION:
    {
        mAnimatePara->mDirection = (DIRECTION)pPara;
        break;
    }
    case E_ORIENTATION:
    {
        mAnimatePara->mOrientation = (ORIENTATION)pPara;
        break;
    }
    case E_LOOP:
    {
        mAnimatePara->mLoopCount = pPara;
        break;
    }
    case E_FRASTART:
    {
        mAnimatePara->mFrameStart = pPara;
        mAnimatePara->mFrameCount = mAnimatePara->mFrameEnd - mAnimatePara->mFrameStart -1;
        break;
    }
    case E_FRAEND:
    {
        mAnimatePara->mFrameEnd = pPara;
        mAnimatePara->mFrameCount = mAnimatePara->mFrameEnd - mAnimatePara->mFrameStart -1;
        break;
    }
    case E_TIME:
    {
        mAnimatePara->mAnimateTime = pPara;
        break;
    }
    case E_DUTYRATIO:
    {
        mAnimatePara->mDutyRatio = pPara;
        break;
    }
    case E_CURVETYPE:
    {
        mAnimatePara->mAnimateType = (QTimeLine::CurveShape)pPara;
        break;
    }
    default:
        break;
    }
}

int  UiOperateParser::GetPara(UiOperateParser::ParaType pType)
{
    switch(pType)
    {
    case E_DIRECTION:
    {
        return mAnimatePara->mDirection;
    }
    case E_ORIENTATION:
    {
        return mAnimatePara->mOrientation;
    }
    case E_LOOP:
    {
        return mAnimatePara->mLoopCount;
    }
    case E_FRASTART:
    {
        return mAnimatePara->mFrameStart;
    }
    case E_FRAEND:
    {
        return mAnimatePara->mFrameEnd;
    }
    case E_TIME:
    {
        return mAnimatePara->mAnimateTime;
    }
    case E_DUTYRATIO:
    {
        return mAnimatePara->mDutyRatio;
    }
    case E_CURVETYPE:
    {
        return mAnimatePara->mAnimateType;
    }
    default:
        break;
    }
    return -1;
}

UiOperateListParser::UiOperateListParser(QDomElement pElement, UiComParser *pComParser) : UiParaParser(pComParser)
{
    mElement = pElement;
    QDomNodeList tmpNodeList = pElement.childNodes();
    for(int i=0;i<tmpNodeList.count();i++)
    {
        QDomElement tmpElement = tmpNodeList.at(i).toElement();
        QString tmpName = tmpElement.tagName();
        AREA_OPERATE tmpOperate = GetOperateType(tmpName);
        if(tmpOperate == OPERATE_NONE)
            continue;
        EffectType tmpType = GetEffectType(tmpElement.attribute("Type"));
        if(tmpType == EffectTypeNone)
            continue;
        UiOperateParser* tmpInfo = mOperations.take(tmpOperate);
        if(tmpInfo)
            delete tmpInfo;
        mOperations.insert(tmpOperate, new UiOperateParser(tmpType, tmpElement, mComParser));
    }
    QStringList tmpParaList = pElement.attribute(D_PARA).split(';');
    foreach(QString tmpPara, tmpParaList)
    {
        if(tmpPara.isEmpty())
            continue;
        QStringList tmpList = tmpPara.split(':');
        if(tmpList.count() != 2)
            continue;
        mComParser->ParseOperationPara(tmpList.at(0), tmpList.at(1), mParaHash);
    }
}

UiOperateListParser::~UiOperateListParser()
{
    foreach(UiOperateParser* tmpInfo, mOperations)
    {
        if(tmpInfo)
            delete tmpInfo;
    }
    mOperations.clear();
}

UiOperateParser* UiOperateListParser::GetOperate(AREA_OPERATE pKey)
{
    return mOperations.value(pKey);
}

AREA_ANIMATE* UiOperateListParser::GetOperatePara(AREA_OPERATE pKey)
{
    UiOperateParser *tmpInfo = mOperations.value(pKey);
    if(tmpInfo)
        return tmpInfo->mAnimatePara;
    return (AREA_ANIMATE*)0;
}

UiOperateParser * UiOperateListParser::AddOperate(AREA_OPERATE pKey, XmlParser* pParser)
{
    UiOperateParser *tmpParser = GetOperate(pKey);
    if(!tmpParser)
    {
        QDomElement tmpElement = pParser->createElement(GetOperateString(pKey));
        EffectType tmpType = EffectTypeNone;
        if(tmpElement.isNull() || ((tmpType = GetEffectType(tmpElement.attribute("Type"))) == EffectTypeNone))
        {
            return (UiOperateParser *)0;
        }
        //>@顺序插入
        QList<AREA_OPERATE> tmpList = mOperations.keys();
        qSort(tmpList.begin(), tmpList.end());
        QDomElement tmpAfter;
        foreach(AREA_OPERATE tmpKey, tmpList)
        {
            if(pKey < tmpKey)
            {
                tmpAfter = mOperations.value(tmpKey)->mElement;
                break;
            }
        }
        mElement.insertBefore(tmpElement, tmpAfter);
        //
        tmpParser = new UiOperateParser(tmpType, tmpElement, mComParser);
        mOperations.insert(pKey, tmpParser);
    }
    return tmpParser;
}

void UiOperateListParser::AddOperate(AREA_OPERATE pKey, UiOperateParser* pInfo)
{
    if(pKey <= OPERATE_NONE || !pInfo)
        return;
    UiOperateParser* tmpInfo = mOperations.take(pKey);
    if(tmpInfo)
    {
        mElement.removeChild(tmpInfo->mElement);
        delete tmpInfo;
    }
    //>@顺序插入
    QList<AREA_OPERATE> tmpList = mOperations.keys();
    qSort(tmpList.begin(), tmpList.end());
    QDomElement tmpAfter;
    foreach(AREA_OPERATE tmpKey, tmpList)
    {
        if(pKey < tmpKey)
        {
            tmpAfter = mOperations.value(tmpKey)->mElement;
            break;
        }
    }
    mElement.insertBefore(pInfo->mElement, tmpAfter);
    mOperations.insert(pKey, pInfo);
}

void UiOperateListParser::DelOperate(AREA_OPERATE pKey)
{
    UiOperateParser *tmpInfo = mOperations.take(pKey);
    if(tmpInfo)
    {
        mElement.removeChild(tmpInfo->mElement);
        delete tmpInfo;
    }
}

UiComParser::UiComParser():QObject(), UiParaParser(this)
{
    mUiParser = 0;
    Clear(false);

    mLayout = 0;
    mStyle = 0;
}

UiComParser::~UiComParser()
{
    Clear(!mExternPara);
}

void UiComParser::Clear(bool pSync)
{
    if(pSync)
    {
        if(mComRect)
        {
            delete mComRect;
            mComRect = 0;
        }
        if(mComResource)
        {
            delete mComResource;
            mComResource = 0;
        }
        if(mComOperation)
        {
            delete mComOperation;
            mComOperation = 0;
        }
    }
    else
    {
        mComRect = 0;
        mComResource = 0;
        mComOperation = 0;
    }
}

bool UiComParser::Init(QDomElement pElement, UiParser *pParser)
{
    mUiParser = pParser;
    if(pElement.isNull())
        return false;
    mElement = pElement;
    mLayout = pElement.attribute("Layout").toInt();
    mStyle = pElement.attribute("Style").toInt();
    //>@Rect
    QDomElement tmpRectElement = pElement.firstChildElement(QString("Rect"));
    if(!tmpRectElement.isNull())
    {
        if(!mExternPara && mComRect)
            delete mComRect;
        mComRect = new UiRectListParser(tmpRectElement, this);
    }
    //>@Resource
    QDomElement tmpResourceElement = pElement.firstChildElement(QString("Resource"));
    if(!tmpResourceElement.isNull())
    {
        if(!mExternPara && mComResource)
            delete mComResource;
        mComResource = new UiRcListParser(tmpResourceElement, this);
    }
    //>@Operation
    QDomElement tmpOperateElement = pElement.firstChildElement(QString("Operation"));
    if(!tmpOperateElement.isNull())
    {
        if(!mExternPara && mComOperation)
            delete mComOperation;
        mComOperation = new UiOperateListParser(tmpOperateElement, this);
    }
    //>@SubUI
    QDomElement tmpSubUiElement = pElement.firstChildElement(QString("SubUI"));
    if(!tmpSubUiElement.isNull())
    {
        QDomNodeList tmpList = tmpSubUiElement.childNodes();
        for(int i=0;i<tmpList.count();i++)
        {
            QDomElement tmpElement = tmpList.at(i).toElement();
            if(tmpElement.isNull())
                continue;
            InitSubUI(tmpElement, pParser);
        }
    }
    return true;
}

bool UiComParser::InitSubElement(QString elementName)
{
    QDomElement tmpElement = mElement.firstChildElement(elementName);
    if(!tmpElement.isNull()){
        return false;
    }
    if(elementName.compare(QString("Rect"), Qt::CaseInsensitive) == 0)
    {
        if(!mExternPara && mComRect)
            delete mComRect;
        mComRect = new UiRectListParser(tmpElement, this);
    }else if(elementName.compare(QString("Resource"), Qt::CaseInsensitive) == 0)
    {
        if(!mExternPara && mComResource)
            delete mComResource;
        mComResource = new UiRcListParser(tmpElement, this);
    }else if(elementName.compare(QString("Operation"), Qt::CaseInsensitive) == 0)
    {
        if(!mExternPara && mComOperation)
            delete mComOperation;
        mComOperation = new UiOperateListParser(tmpElement, this);
    }else if(elementName.compare(QString("SubUI"), Qt::CaseInsensitive) == 0)
    {
        QDomNodeList tmpList = tmpElement.childNodes();
        for(int i=0;i<tmpList.count();i++)
        {
            QDomElement subtmpElement = tmpList.at(i).toElement();
            if(subtmpElement.isNull())
                continue;
            InitSubUI(subtmpElement, mUiParser);
        }
    }else{
        return false;
    }
    return true;
}

bool UiComParser::InitSubUI(QDomElement pElement, UiParser *pParser)
{
    return true;
}

bool UiComParser::ParseSubOperationPara(QString pLabel, QString pContent, AREA_ANIMATE* pAreaEffect)
{
    if(pAreaEffect == 0)
        return false;

    PreprocPara(pContent);

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
        pAreaEffect->mAnimateType = GetTimelineType(pContent);
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

bool UiComParser::PaintBuffer(UiOperateParser* pAnimate, int pFrame){
    return true;
}

UiParser::UiParser(QString fileName) :
    XmlParser(fileName)
{
    int tmpProNum, tmpSuffixNum;
    tmpProNum = m_FileName.length();
    tmpSuffixNum = UiSuffix.length();
    if(tmpProNum <= tmpSuffixNum)
    {
        IDE_TRACE_INT(tmpProNum);
        return;
    }
    QString suffix = m_FileName.mid(tmpProNum-tmpSuffixNum, tmpSuffixNum);
    if(suffix != UiSuffix)
        m_FileName = QString("%1%2").arg(m_FileName).arg(UiSuffix);
}

UiParser::~UiParser(){
    m_ComElementGroup.clear();
}

bool UiParser::NewUI()
{
    bool flag = New();
    if(flag == false)
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

    QDomElement tmpElement = createElement("FileType");
    tmpElement.appendChild(createTextNode("BST UI File"));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("CreateTime");
    QDateTime time = QDateTime::currentDateTime();
    tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("LastModiTime");
    tmpElement.appendChild(createTextNode(time.toString("yyyy-MM-dd HH:mm:ss")));
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("Solution");
    rootElement.appendChild(tmpElement);

    tmpElement = createElement("Ui");
    QDomElement tmpChildElement = createElement("SubUI");
    tmpElement.appendChild(tmpChildElement);
    rootElement.appendChild(tmpElement);

    m_IsChanged = true;
    Save();

    GetUiGroup();
    return true;
}

bool UiParser::OpenUI()
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
    if(tmpString.compare(UiFileType, Qt::CaseInsensitive) != 0)
    {
        m_Valid = false;
        IDE_TRACE_STR(tmpString);
        return false;
    }
    GetUiGroup();
    return true;
}

bool UiParser::CloseUI()
{
    SaveUI();
    return true;
}

bool UiParser::DelUI()
{
    QFile file(m_FileDir + m_FileName);
    if(file.exists())
    {
        file.remove();
        return true;
    }
    return false;
}

bool UiParser::SaveUI()
{
    QDateTime time = QDateTime::currentDateTime();
    ModifyItemElement("/LastModiTime", time.toString("yyyy-MM-dd HH:mm:ss"));
    return Save();
}

bool UiParser::RenameUI(QString FileName)
{
    QString tmpString = FileName;
    if(!tmpString.endsWith(UiSuffix))
        tmpString.append(UiSuffix);
    bool flag = Rename(tmpString);
    if(flag)
        m_FileName = tmpString;     //>@如果重命名成功，则修改m_FileName为重命名后的值。
    return flag;
}

bool UiParser::isValid()
{
    if(!m_Valid)
        return false;
    if(m_FileDir.endsWith('/') == false)
        m_FileDir.append('/');
    QDir tmpDir(m_FileDir);
    if(tmpDir.exists() == false)
        return tmpDir.mkpath(m_FileDir);
    return true;
}

int UiParser::GetLayout()
{
    if(!m_Valid)
        return D_TYPE_USER_DEFINE;
    bool flag = GetItemElement(QString("/Layout"));
    if(flag)
    {
        QString tmpNum = itemElement.text();
        if(tmpNum.isEmpty())
            return D_TYPE_USER_DEFINE;
        return tmpNum.toInt();
    }
    return D_TYPE_USER_DEFINE;
}

bool UiParser::SetLayout(int pLayout)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Layout", QString::number(pLayout));
}

int UiParser::GetStyle()
{
    if(!m_Valid)
        return D_TYPE_USER_DEFINE;
    bool flag = GetItemElement(QString("/Style"));
    if(flag)
    {
        QString tmpNum = itemElement.text();
        if(tmpNum.isEmpty())
            return D_TYPE_USER_DEFINE;
        return tmpNum.toInt();
    }
    return D_TYPE_USER_DEFINE;
}

bool UiParser::SetStyle(int pStyle)
{
    if(!m_Valid)
        return false;
    return ModifyItemElement("/Style", QString::number(pStyle));
}

//>@用于更新Global类中的m_ComElementGroup
void UiParser::GetUiGroup()
{
    if(!m_Valid)
    {
        IDE_TRACE();
        return;
    }

    mLayout = GetLayout();
    mStyle = GetStyle();

    bool flag = GetItemElement("/Ui");
    if(flag)
    {
        m_ComElementGroup.clear();

        QDomElement tmpElement;
        QDomNodeList UiNodeList = itemElement.childNodes();
        for(int i=0;i<UiNodeList.count();i++)
        {
            tmpElement = UiNodeList.at(i).toElement();
            QString tmpString = tmpElement.tagName();
            UI_TYPE tmpComType = GetUiType(tmpString);
            if(tmpComType < UI_DESKTOP)
                continue;
            m_ComElementGroup.insert(tmpComType, tmpElement);//>@插入ui元素

            QString tmpSubUIString = QString("/Ui/%1/SubUI").arg(tmpString);
            flag = GetItemElement(tmpSubUIString);
            if(flag)
            {
                QDomNodeList SubUINodeList = itemElement.childNodes();
                for(int i=0;i<SubUINodeList.count();i++)              //>@插入ui的子元素
                {
                    tmpElement = SubUINodeList.at(i).toElement();
                    QString tmpSubString = tmpElement.tagName();
                    UI_TYPE tmpSubComType = GetUiType(tmpSubString);
                    if(tmpSubComType < UI_DESKTOP)
                        continue;
                    m_ComElementGroup.insert(tmpSubComType, tmpElement);

                    QString tmpSubSubUIString = QString("/Ui/%1/SubUI/%2/SubUI").arg(tmpString).arg(tmpSubString);
                    flag = GetItemElement(tmpSubSubUIString);
                    if(flag)
                    {
                        QDomNodeList SubSubUINodeList = itemElement.childNodes();
                        for(int i=0;i<SubSubUINodeList.count();i++)              //>@插入subui的子元素
                        {
                            tmpElement = SubSubUINodeList.at(i).toElement();
                            QString tmpSubSubString = tmpElement.tagName();
                            UI_TYPE tmpSubSubComType = GetUiType(tmpSubSubString);
                            if(tmpSubSubComType < UI_DESKTOP)
                                continue;
                            m_ComElementGroup.insert(tmpSubSubComType, tmpElement);
                        }
                    }
                }
            }
        }
    }
}

