#include "globalfun.h"

const QString xmlParse::XmlHead = "version=\"1.0\" encoding=\"UTF-8\"";
const QString xmlParse::XmlSuffix = ".xml";

int CalcPixelFromPoint(int pPoint)
{
    return (DPI * pPoint / 72);
}
int CalcPointFromPixel(int pPixel)
{
    return (pPixel * 72 / DPI);
}

qreal RegularAngle(qreal angle)
{
    while (angle < 0)
        angle += 2.0*PI;
    while (angle > 2.0*PI)
        angle -= 2.0*PI;
    return angle;
}

//>@计算相对于270度的差值
qreal CalDiffBeginAngle(qreal angle)
{
    qreal tmpAngle = RegularAngle(angle);
    if(tmpAngle >= 0 && tmpAngle < PI/2.0)
        return PI/2.0 + tmpAngle;
    else if(tmpAngle >= PI/2.0 && tmpAngle < 3.0*PI/2.0)
        return 3.0*PI/2.0 - tmpAngle;
    else if(tmpAngle >= 3.0*PI/2.0 && tmpAngle < 2.0*PI)
        return tmpAngle - 3.0*PI/2.0;
    return 0;
}

//>@"Arial", pPrior表示宽度优先或者高度优先
int CalcFontSizeFromSize(QString pFontType, QSize pSize, PRIOR pPrior)
{
    if(pFontType.isEmpty())
        return -1;
    if(pSize.isValid() == false)
        return -1;
    int fontsize=1;
    while(fontsize < 200) //>@确保fontsize最大为200，防止死循环
    {
        QFont tmp(pFontType,fontsize);
        tmp.setPointSize(fontsize);
        QFontMetricsF fm(tmp);
        qreal pixelsWide = fm.width("8");
        qreal pixelsHigh = fm.height();
        if(pPrior == PRIOR_WIDTH)
        {
            if(pixelsWide>=pSize.width())
                break;
        }
        else if(pPrior == PRIOR_HEIGHT)
        {
            if(pixelsHigh >= pSize.height())
                break;
        }
        ++fontsize;
    }
    return fontsize;
}

int CalcFontSizeFromRect(QString pFontType, QSize pSize)
{
    if(pFontType.isEmpty())
        return -1;
    if(pSize.isValid() == false)
        return -1;
    int fontsize=1;
    while(fontsize < 200) //>@确保fontsize最大为200，防止死循环
    {
        QFont tmp(pFontType,fontsize);
        tmp.setPointSize(fontsize);
        QFontMetricsF fm(tmp);
        qreal pixelsWide = fm.width("8");
        qreal pixelsHigh = fm.height();
        if(pixelsWide>=pSize.width() || pixelsHigh >= pSize.height())
            break;
        ++fontsize;
    }
    return fontsize;
}

QSize CalcSizeFromFontSize(QString pFontType, int pFontSize)
{
    QFont tmp(pFontType,pFontSize);
    tmp.setPointSize(pFontSize);
    QFontMetricsF fm(tmp);
    return QSize(fm.width('8'), fm.height());
}

QSize CalcSizeFromFontSize(QString pFontType, QChar pChar, int pFontSize)
{
    QFont tmp(pFontType,pFontSize);
    tmp.setPointSize(pFontSize);
    QFontMetricsF fm(tmp);
    return QSize(fm.width(pChar), fm.height());
}

QSize CalcSizeFromFontSize(QString pFontType, QString pText, int pFontSize)
{
    QFont tmp(pFontType,pFontSize);
    tmp.setPointSize(pFontSize);
    QFontMetricsF fm(tmp);
    return QSize(fm.width(pText), fm.height());
}

QPointF CalcIntersectPos(QRectF pRect, QLineF pLine)
{
    QPointF p1 = pRect.topLeft();
    QList<QPointF>  tmpRectPoints;
    tmpRectPoints << pRect.topRight() << pRect.bottomRight() << pRect.bottomLeft();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;
    for(int i=0; i<tmpRectPoints.count(); ++i)
    {
        p2 = tmpRectPoints.at(i);
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType = polyLine.intersect(pLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
            return intersectPoint;
        p1 = p2;
    }
    return QPointF();
}

void WriteText(QPainter *pPainter, QString pText, QRect pRect, QFont pFont, QColor pColor)
{
    int num = pText.length();
    if(num <= 0)
        return;
    if(pRect.isEmpty() == false)
        return;
    pPainter->save();
    pPainter->setFont(pFont);
    pPainter->setPen(pColor);
    pPainter->drawText(pRect, pText);
    pPainter->restore();
}

//>@所有文字写在一行
void WriteText(QPainter *pPainter, QString pText, QRect pRect, QString pFontType, QColor pColor, PRIOR pPrior)
{
    int num = pText.length();
    if(num <= 0)
        return;
    if(pRect.isEmpty() == false)
        return;
    int fontsize = 1;
    int fontwidth = pRect.width()/num;
    int fontheight = pRect.height();
    fontsize = CalcFontSizeFromSize(pFontType, QSize(fontwidth, fontheight), pPrior);
    QFont font(pFontType, fontsize, QFont::DemiBold, false);
    font.setUnderline(false);
    WriteText(pPainter, pText, pRect, font, pColor);
}

/*
    QFontMetricsF fm(tmpFontInfo.mFont);
    textRect =  QRectF(fm.boundingRect(rect.toRect(), Qt::AlignLeft, tmpContent));
    p.drawText(rect, tmpContent);
*/

//>@根据字体大小自动排列文字并进行换行
void WriteText(QPainter *pPainter, QString pText, QRect pRect, QString pFontType, QColor pColor, int pFontSize)
{
    int num = pText.length();
    if(num <= 0)
        return;
    if(pRect.isValid() == false)
        return;
    QSize tmpSize = CalcSizeFromFontSize(pFontType, pFontSize);
    QFont font(pFontType, pFontSize, QFont::DemiBold, false);
    font.setUnderline(false);
    pPainter->save();
    pPainter->setFont(font);
    pPainter->setPen(pColor);
    quint32 tmpRowCharCount = (qreal)(pRect.width()) / (qreal)(tmpSize.width());
    if(tmpRowCharCount <= 0) //>@防止死循环bug
        tmpRowCharCount = 1;
    quint32 tmpColumnCount = (qreal)(pText.count()) / (qreal)(tmpRowCharCount) + 1;
    for(int i=0;i<tmpColumnCount;i++)
    {
        int y = pRect.y()+i*(tmpSize.height()+2);
        if(y >= pRect.y() + pRect.height() - tmpSize.height())
            break;
        pPainter->drawText(pRect.x(),
                           y,
                           pText.mid(tmpRowCharCount*i, tmpRowCharCount));
    }
    pPainter->restore();
}

qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += 2.0*PI;
    while (angle > 2.0*PI)
        angle -= 2.0*PI;
    return angle;
}

bool isFileExist(QString FileName)
{
    if(FileName.isEmpty())
        return false;
    QFile file(FileName);
    bool flag = file.exists();
    return flag;
}

bool delFile(QString FileName)
{
    if(FileName.isEmpty())
        return false;
    QFile file(FileName);
    bool flag = file.remove();
    return flag;
}


bool is_file(QString fullFileName)
{
    if (!fullFileName.size() > 0) {
      return false;
    }
    QDir diro(fullFileName);
       if (diro.exists()) {
         return false;
       }
    QFile f( fullFileName );
    if ( f.exists(fullFileName) ) {
    return true;
    } else {
    return false;
    }
}

bool StringToInt(QString pString,int &pInt)
{
    bool ok;
    int dec = pString.toInt(&ok, 10);  //>@10进制
    if(ok)
        pInt = dec;
    return ok;
}

QString getFileIconName(QString pFileName)
{
    QString tmpIconString;
    if (pFileName.isEmpty())
        return tmpIconString;
    int index = pFileName.lastIndexOf(".");
    QString fileSuffix;
    if(index >= 0)
        fileSuffix = pFileName.mid(index);
    else
        return tmpIconString;
    if(!QString::compare(fileSuffix, ".xml", Qt::CaseInsensitive))
    {
        tmpIconString = QString(":/");
    }
    return tmpIconString;
}


FILE_TYPE getFileType(QString FileName)
{
    FILE_TYPE tmpFileType = TYPE_INVALID;
    if (FileName.isEmpty())
        return tmpFileType;
    int index = FileName.lastIndexOf(".");
    QString fileSuffix;
    if(index >= 0)
        fileSuffix = FileName.mid(index);
    else
        return TYPE_INVALID;
    if(!QString::compare(fileSuffix, ".bstpro", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_PRO;
    }
    else if(!QString::compare(fileSuffix, ".ui", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_UI;
    }
    else if(!QString::compare(fileSuffix, ".mdp", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_MDP;
    }
    else if(!QString::compare(fileSuffix, ".stc", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_STC;
    }
    else if(!QString::compare(fileSuffix, ".sto", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_STO;
    }
    else if(!QString::compare(fileSuffix, ".rc", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_RC;
    }
    else if(!QString::compare(fileSuffix, ".editor", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_EDITOR;
    }
    else if(!QString::compare(fileSuffix, ".devinfo", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_DEVINFO;
    }
    else if(!QString::compare(fileSuffix, ".map", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_MAP;
    }
    else if(fileSuffix.contains(".avi", Qt::CaseInsensitive) ||
            fileSuffix.contains(".mp4", Qt::CaseInsensitive) ||
            fileSuffix.contains(".flv", Qt::CaseInsensitive) ||
            fileSuffix.contains(".rmvb", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_VIDEOEDITOR;
    }
    else if(fileSuffix.contains(".png", Qt::CaseInsensitive) ||
            fileSuffix.contains(".jpg", Qt::CaseInsensitive) ||
            fileSuffix.contains(".svg", Qt::CaseInsensitive) ||
            fileSuffix.contains(".gif", Qt::CaseInsensitive) ||
            fileSuffix.contains(".bmp", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_IMAGEEDITOR;
    }
    else if(fileSuffix.contains(".wav", Qt::CaseInsensitive) ||
            fileSuffix.contains(".mp3", Qt::CaseInsensitive))
    {
        tmpFileType = TYPE_AUDIOEDITOR;
    }
    return tmpFileType;
}

FILE_TYPE getFileType(FILE_INFO *pFileInfo)
{
    if(pFileInfo->CategoryType.compare(QString("Solution"), Qt::CaseInsensitive)==0)
    {
        return TYPE_PRO;
    }
    else if(pFileInfo->CategoryType.compare(QString("File"), Qt::CaseInsensitive)==0)
    {
        return getFileType(pFileInfo->ItemType);
    }
    return TYPE_INVALID;
}

QString getFileSuffix(FILE_TYPE pFileType)
{
    QString tmpString;
    switch(pFileType)
    {
        case TYPE_START:
        {
            break;
        }
        case TYPE_MANUFACTURE:
        {
            tmpString.append("mft");
            break;
        }
        case TYPE_VIDEOEDITOR:
        {
            tmpString.append("veditor");
            break;
        }
        case TYPE_IMAGEEDITOR:
        {
            tmpString.append("ieditor");
            break;
        }
        case TYPE_AUDIOEDITOR:
        {
            tmpString.append("aeditor");
            break;
        }
        case TYPE_PRO:
        {
            tmpString.append("bstpro");
            break;
        }
        case TYPE_UI:
        {
            tmpString.append("ui");
            break;
        }
        case TYPE_MDP:
        {
            tmpString.append("mdp");
            break;
        }
        case TYPE_STC:
        {
            tmpString.append("stc");
            break;
        }
        case TYPE_MAP:
        {
            tmpString.append("map");
            break;
        }
        case TYPE_DEVINFO:
        {
            tmpString.append("devinfo");
            break;
        }
        case TYPE_STO:
        {
            tmpString.append("sto");
            break;
        }
        case TYPE_LOG:
        {
            tmpString.append("log");
            break;
        }
        case TYPE_RC:
        {
            tmpString.append("rc");
            break;
        }
        default:
            break;
    }
    return tmpString;
}

QString getNewFileName(FILE_TYPE pType)
{
    return QString("New.") + getFileSuffix(pType);
}



//>@将pSrcFile文件规整到某个文件夹下，返回规整后的文件名
//>@如果pRcName不为空，则按照此名进行规整文件的名字
QString regularFile(QString pRcName, QString pSrcFile, QString pDstPath, QSize pSize)
{

}

int GetKeyType(QString pKey)
{
    if(pKey.length() == 1)
        return (int)(pKey.toLatin1().at(0));
    else if(pKey.compare("Esc") == 0)
        return (QTASCII)?(Qt::Key_Escape):(0x1b);
    else if(pKey.compare("Tab") == 0)
        return (QTASCII)?(Qt::Key_Tab):(0x0b);
    else if(pKey.compare("Backspace") == 0)
        return (QTASCII)?(Qt::Key_Backspace):(0x08);
    else if(pKey.compare("Enter") == 0)
        return (QTASCII)?(Qt::Key_Enter):(0x0d);
    else if(pKey.compare("Del") == 0)
        return (QTASCII)?(Qt::Key_Delete):(0x7f);
    else if(pKey.compare("Home") == 0)
        return (QTASCII)?(Qt::Key_Home):(0x02);
    else if(pKey.compare("End") == 0)
        return (QTASCII)?(Qt::Key_End):(0x03);
    else if(pKey.compare("Left") == 0)
        return Qt::Key_Left;
    else if(pKey.compare("Up") == 0)
        return Qt::Key_Up;
    else if(pKey.compare("Right") == 0)
        return Qt::Key_Right;
    else if(pKey.compare("Down") == 0)
        return Qt::Key_Down;
    else if(pKey.compare("Shift") == 0)
        return (QTASCII)?(Qt::Key_Shift):(0x0f);
    else if(pKey.compare("Ctrl") == 0)
        return (QTASCII)?(Qt::Key_Control):(0x11);
    else if(pKey.compare("Alt") == 0)
        return (QTASCII)?(Qt::Key_Alt):(0x10);
    else if(pKey.compare("Caps") == 0)
        return Qt::Key_CapsLock;
    else if(pKey.compare("Space") == 0)
        return Qt::Key_Space;
    else if(pKey.compare("Disablity") == 0)
        return RCDISABILITY;
    return 0;
}

MDPConfigParaInfo::MDPConfigParaInfo(const QDomElement &pElement)
{
    if(pElement.isNull())
        return;
    mName = pElement.tagName();
    //>@获取权限
    QString tmpString = pElement.attribute("Permission");
    if(!tmpString.compare("RO", Qt::CaseInsensitive))
        mPermission = RO;
    else if(!tmpString.compare("EDIT", Qt::CaseInsensitive))
        mPermission = EDIT;
    else if(!tmpString.compare("RW", Qt::CaseInsensitive))
        mPermission = RW;
    mParaType = GetUiType(pElement.attribute("Para"));
    mParas = pElement.attribute("List");
    if(1)
    {
        mValue = pElement.text();
    }
    else
    {
        mValue = pElement.attribute("Active");
        QDomNodeList tmpList = pElement.childNodes();
        for(int i=0;i<tmpList.count();i++)
        {
            QDomElement tmpElement = tmpList.at(i).toElement();
            if(tmpElement.isNull())
                continue;
            SubConfigParaInfo *tmpParaInfo = new SubConfigParaInfo;
            tmpParaInfo->Name = tmpElement.tagName();
            QDomNodeList tmpSubList = tmpElement.childNodes();
            for(int i=0;i<tmpSubList.count();i++)
            {
                QDomElement tmpSubElement = tmpSubList.at(i).toElement();
                if(tmpSubElement.isNull())
                    continue;
                tmpParaInfo->ParaInfoList.append(new MDPConfigParaInfo(tmpSubElement));
            }
            mSubInfos.append(tmpParaInfo);
        }
    }
}

MDPConfigParaInfo::~MDPConfigParaInfo()
{
    if(mSubInfos.count() <= 0)
        return;
    for(int i=mSubInfos.count()-1;i>=0;i--)
    {
        SubConfigParaInfo* tmpParaInfo = mSubInfos.takeAt(i);
        if(tmpParaInfo)
        {
            if(tmpParaInfo->ParaInfoList.count() <= 0)
                continue;
            for(int j=tmpParaInfo->ParaInfoList.count()-1;j>=0;j--)
            {
                MDPConfigParaInfo *tmpInfo = tmpParaInfo->ParaInfoList.takeAt(j);
                if(tmpInfo)
                    delete tmpInfo;
            }
            delete tmpParaInfo;
        }
    }
}

bool MDPConfigParaInfo::IsNull()
{
    if((mName.isEmpty()))
        return true;
    return false;
}

MDPConfigInfo::MDPConfigInfo(const QDomElement &pElement)
{
    mDrag = false;
    mZValue = 0;
    if(pElement.isNull())
        return;
    mName = pElement.tagName();
    mRc = pElement.attribute("img");
    QDomNodeList nodeList = pElement.childNodes();
    for(int i=0;i<nodeList.count();i++)
    {
        QDomElement cldElement = nodeList.at(i).toElement();
        if(cldElement.isNull())
            continue;
        QString cldName = cldElement.tagName();
        if(!cldName.compare("Drag", Qt::CaseInsensitive))
        {
            mDrag =  (!cldElement.text().compare("True", Qt::CaseInsensitive))?true:false;
        }
        else if(!cldName.compare("geometry", Qt::CaseInsensitive))
        {
            QString text = cldElement.text();
            QStringList list = text.split(",", QString::SkipEmptyParts);
            if(list.count()!=4)
                continue;
            mRectF = QRectF(list.at(0).toDouble(),list.at(1).toDouble(),list.at(2).toDouble(),list.at(3).toDouble()).normalized();
        }
        else if(!cldName.compare("ZValue", Qt::CaseInsensitive))
        {
            mZValue = cldElement.text().toInt();
        }
        else if(!cldName.compare("Parameter", Qt::CaseInsensitive))
        {
            QDomNodeList cldParaNodeList = cldElement.childNodes();
            for(int i=0;i<cldParaNodeList.count();i++)
            {
                QDomElement cldParaElement = cldParaNodeList.at(i).toElement();
                if(cldParaElement.isNull())
                    continue;
                mParaInfoList.append(new MDPConfigParaInfo(cldParaElement));
            }
        }
    }
}
MDPConfigInfo::~MDPConfigInfo()
{
    mName = QString();
    mDrag = false;
    mRc = QString();
    mRectF = QRectF();
    mZValue = 0;
    if(mParaInfoList.count() > 0)
    {
        for(int i=mParaInfoList.count();i>=0;i--)
        {
            MDPConfigParaInfo *tmpParaInfo = mParaInfoList.takeAt(i);
            if(tmpParaInfo)
                delete tmpParaInfo;
        }
    }
}
bool MDPConfigInfo::IsNull()
{
    if((mName.isEmpty())||
       (mRc.isEmpty())||
       (!mRectF.isValid()))
        return true;
    return false;
}

COM_TYPE getComponetType(QString ComponentName)
{
    COM_TYPE tmpComType = COM_INVALID;
    if (ComponentName.isNull())
        return tmpComType;
    else if (ComponentName.isEmpty() ||
             ComponentName.compare(QString("Desktop"), Qt::CaseInsensitive) == 0 ||
             ComponentName.compare(QString("/"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_DESKTOP;

    }
    else if(ComponentName.compare(QString("SetBar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_SETBAR;
    }
    else if(ComponentName.compare(QString("MessageBar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_MESSAGEBAR;
    }
    else if(ComponentName.compare(QString("ProgressBar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_PROGRESSBAR;
    }
    else if(!ComponentName.compare(QString("Animal"), Qt::CaseInsensitive))
    {
        tmpComType = COM_ANIMAL;
    }
    else if(!ComponentName.compare(QString("Floor"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FLOOR;
    }
    else if(!ComponentName.compare(QString("Arrow"), Qt::CaseInsensitive))
    {
        tmpComType = COM_ARROW;
    }
    else if(!ComponentName.compare(QString("Function"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FUNCTION;
    }
    else if(!ComponentName.compare(QString("Video"), Qt::CaseInsensitive))
    {
        tmpComType = COM_VIDEO;
    }
    else if(!ComponentName.compare(QString("Image"), Qt::CaseInsensitive))
    {
        tmpComType = COM_IMAGE;
    }
    else if(!ComponentName.compare(QString("WebView"), Qt::CaseInsensitive))
    {
        tmpComType = COM_WEBVIEW;
    }
    else if(!ComponentName.compare(QString("QML"), Qt::CaseInsensitive))
    {
        tmpComType = COM_QML;
    }
    else if(!ComponentName.compare(QString("OpenGL"), Qt::CaseInsensitive))
    {
        tmpComType = COM_OPENGL;
    }
    else if(!ComponentName.compare(QString("Audio"), Qt::CaseInsensitive))
    {
        tmpComType = COM_AUDIO;
    }
    else if(!ComponentName.compare(QString("Phone"), Qt::CaseInsensitive))
    {
        tmpComType = COM_PHONE;
    }
    else if(!ComponentName.compare(QString("Text"), Qt::CaseInsensitive))
    {
        tmpComType = COM_TEXT;
    }
    else if(!ComponentName.compare(QString("Logo"), Qt::CaseInsensitive))
    {
        tmpComType = COM_LOGO;
    }
    else if(!ComponentName.compare(QString("Weather"), Qt::CaseInsensitive))
    {
        tmpComType = COM_WEATHER;
    }
    else if(!ComponentName.compare(QString("Time"), Qt::CaseInsensitive))
    {
        tmpComType = COM_TIME;
    }
    else if(!ComponentName.compare(QString("NamePlate"), Qt::CaseInsensitive))
    {
        tmpComType = COM_NAMEPLATE;
    }
    else if(!ComponentName.compare(QString("FloorLCD"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FLOORLCD;
    }
    else if(!ComponentName.compare(QString("FloorBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FLOORBTN;
    }
    else if(!ComponentName.compare(QString("KeyboardBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_KEYBOARDBTN;
    }
    else if(!ComponentName.compare(QString("PhoneBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_PHONEBTN;
    }
    else if(!ComponentName.compare(QString("EmergencyBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_EMERGENCYBTN;
    }
    else if(!ComponentName.compare(QString("OpenBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_OPENBTN;
    }
    else if(!ComponentName.compare(QString("CloseBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_CLOSEBTN;
    }
    else if(!ComponentName.compare(QString("SetBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_SETBTN;
    }
    else if(!ComponentName.compare(QString("DisabilityBtn"), Qt::CaseInsensitive))
    {
        tmpComType = COM_DISABILITYBTN;
    }
    return tmpComType;
}

QString getComponetName(COM_TYPE ComponentType)
{
    switch(ComponentType)
    {
    case COM_DESKTOP:
        return QString("Desktop");
    case COM_SETBAR:
        return QString("SetBar");
    case COM_MESSAGEBAR:
        return QString("MessageBar");
    case COM_PROGRESSBAR:
        return QString("ProgressBar");
    case COM_ANIMAL:
        return QString("Animal");
    case COM_FLOOR:
        return QString("Floor");
    case COM_ARROW:
        return QString("Arrow");
    case COM_FUNCTION:
        return QString("Function");
    case COM_VIDEO:
        return QString("Video");
    case COM_IMAGE:
        return QString("Image");
    case COM_WEBVIEW:
        return QString("WebView");
    case COM_QML:
        return QString("QML");
    case COM_OPENGL:
        return QString("OpenGL");
    case COM_AUDIO:
        return QString("Audio");
    case COM_PHONE:
        return QString("Phone");
    case COM_TEXT:
        return QString("Text");
    case COM_LOGO:
        return QString("Logo");
    case COM_WEATHER:
        return QString("Weather");
    case COM_TIME:
        return QString("Time");
    case COM_NAMEPLATE:
        return QString("NamePlate");
    case COM_FLOORLCD:
        return QString("FloorLCD");
    case COM_FLOORBTN:
        return QString("FloorBtn");
    case COM_KEYBOARDBTN:
        return QString("KeyboardBtn");
    case COM_PHONEBTN:
        return QString("PhoneBtn");
    case COM_EMERGENCYBTN:
        return QString("EmergencyBtn");
    case COM_OPENBTN:
        return QString("OpenBtn");
    case COM_CLOSEBTN:
        return QString("CloseBtn");
    case COM_SETBTN:
        return QString("SetBtn");
    case COM_DISABILITYBTN:
        return QString("DisabilityBtn");
    default:
        return QString();
    }
    return QString();
}

QString getComponetPath(COM_TYPE pType)
{
    switch(pType)
    {
    case COM_DESKTOP:
        return QString("/");
    //>@设置窗口
    case COM_SETBAR:
        return QString("/SetBar");
    case COM_MESSAGEBAR:
        return QString("/MessageBar");
    case COM_PROGRESSBAR:
        return QString("/ProgressBar");
    case COM_ANIMAL:
        return QString("/Animal");
    case COM_FLOOR:
        return QString("/Floor");
    case COM_ARROW:
        return QString("/Arrow");
    case COM_FUNCTION:
        return QString("/Function");
    case COM_VIDEO:
        return QString("/Video");
    case COM_IMAGE:
        return QString("/Image");
    case COM_WEBVIEW:
        return QString("/WebView");
    case COM_QML:
        return QString("/QML");
    case COM_OPENGL:
        return QString("/OpenGL");
    case COM_AUDIO:
        return QString("/Audio");
    case COM_PHONE:
        return QString("/Phone");
    case COM_TEXT:
        return QString("/Text");
    case COM_LOGO:
        return QString("/Logo");
    case COM_WEATHER:
        return QString("/Weather");
    case COM_TIME:
        return QString("/Time");
    case COM_NAMEPLATE:
        return QString("/NamePlate");
    case COM_FLOORLCD:
        return QString("/FloorLCD");
    case COM_FLOORBTN:
        return QString("/FloorBtn");
    case COM_KEYBOARDBTN:
        return QString("/KeyboardBtn");
    case COM_PHONEBTN:
        return QString("/PhoneBtn");
    case COM_EMERGENCYBTN:
        return QString("/EmergencyBtn");
    case COM_OPENBTN:
        return QString("/OpenBtn");
    case COM_CLOSEBTN:
        return QString("/CloseBtn");
    case COM_SETBTN:
        return QString("/SetBtn");
    case COM_DISABILITYBTN:
        return QString("/DisabilityBtn");
    default:
        return QString();
    }
    return QString();
}

COM_TYPE getParentComType(COM_TYPE pType)
{
    if(pType > COM_AREA && pType < COM_COMPONENT)
    {
        return COM_DESKTOP;
    }
    return COM_INVALID;
}

COM_TYPE getParentComType(QString ComPath)
{
    if(ComPath.isEmpty())
        return COM_INVALID;
    int index1,index2;
    QString tmpString;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
        return COM_INVALID;
    index2 = ComPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0) //>@desktop and rect
    {
        tmpString = ComPath.mid(index1);
        if(tmpString.length() == 1)
        {
            return COM_INVALID;
        }
        return COM_DESKTOP;
    }
    else
    {
        tmpString = ComPath.mid(index1+1, index2-index1-1);
    }
    return getComponetType(tmpString);
}

QString getComPath(QString ComName)
{
    QString tmpString;
    if (ComName.isNull())
        return tmpString;
    else if (ComName.isEmpty() ||
             ComName.compare(QString("Desktop"), Qt::CaseInsensitive) == 0 ||
             ComName.compare(QString("/"), Qt::CaseInsensitive) == 0)
    {
        return QString("/");
    }
    else if(!ComName.compare(QString("SetBar"), Qt::CaseInsensitive))
    {
        tmpString = QString("/SetBar");
    }
    else if(!ComName.compare(QString("MessageBar"), Qt::CaseInsensitive))
    {
        tmpString = QString("/MessageBar");
    }
    else if(!ComName.compare(QString("ProgressBar"), Qt::CaseInsensitive))
    {
        tmpString = QString("/ProgressBar");
    }
    else if(!ComName.compare(QString("Floor"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Floor");
    }
    else if(!ComName.compare(QString("Arrow"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Arrow");
    }
    else if(!ComName.compare(QString("Function"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Function");
    }
    else if(!ComName.compare(QString("Video"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Video");
    }
    else if(!ComName.compare(QString("Image"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Image");
    }
    else if(!ComName.compare(QString("WebView"), Qt::CaseInsensitive))
    {
        tmpString = QString("/WebView");
    }
    else if(!ComName.compare(QString("QML"), Qt::CaseInsensitive))
    {
        tmpString = QString("/QML");
    }
    else if(!ComName.compare(QString("OpenGL"), Qt::CaseInsensitive))
    {
        tmpString = QString("/OpenGL");
    }
    else if(!ComName.compare(QString("Audio"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Audio");
    }
    else if(!ComName.compare(QString("Phone"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Phone");
    }
    else if(!ComName.compare(QString("Text"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Text");
    }
    else if(!ComName.compare(QString("Logo"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Logo");
    }
    else if(!ComName.compare(QString("Weather"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Weather");
    }
    else if(!ComName.compare(QString("Time"), Qt::CaseInsensitive))
    {
        tmpString = QString("/Time");
    }
    else if(!ComName.compare(QString("NamePlate"), Qt::CaseInsensitive))
    {
        tmpString = QString("/NamePlate");
    }
    else if(!ComName.compare(QString("FloorBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/FloorBtn");
    }
    else if(!ComName.compare(QString("PhoneBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/PhoneBtn");
    }
    else if(!ComName.compare(QString("EmergencyBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/EmergencyBtn");
    }
    else if(!ComName.compare(QString("OpenBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/OpenBtn");
    }
    else if(!ComName.compare(QString("CloseBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/CloseBtn");
    }
    else if(!ComName.compare(QString("SetBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/SetBtn");
    }
    else if(!ComName.compare(QString("DisabilityBtn"), Qt::CaseInsensitive))
    {
        tmpString = QString("/DisabilityBtn");
    }
    return tmpString;
}

QString getParentComPath(QString ComPath)
{
    int index1,index2;
    QString tmpString;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
        return tmpString;
    index2 = ComPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0)
    {
        tmpString = ComPath.mid(index1);
        if(tmpString.length() == 1)
            return QString("");     //>@Empty表示当前控件为Desktop
        else
            return QString("/");
    }
    else
    {
        tmpString = ComPath.mid(index1, index2-index1);
    }
    return tmpString;
}

QString getCurComName(QString ComPath)
{
    if(ComPath.isEmpty())
        return QString();
    int index1,index2;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
    {
        if(ComPath.compare(QString("Desktop"), Qt::CaseInsensitive) == 0)
            return QString("Desktop");
        else
            return QString();
    }
    QString tmpString;
    index2 = ComPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0)
    {
        tmpString = ComPath.mid(index1+1);
        if(tmpString.isEmpty())
            return QString("Desktop");     //>@Empty表示Desktop
        else
            return tmpString;
    }
    else
    {
        tmpString = ComPath.mid(index2+1);
    }
    return tmpString;
}

COM_TYPE getCurComType(QString ComPath)
{
    QString tmpString;
    int index1,index2;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
    {
        return getComponetType(ComPath);
    }
    index2 = ComPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0)
    {
        tmpString = ComPath.mid(index1+1);
        if(tmpString.isEmpty())
            return COM_DESKTOP;     //>@Empty表示Desktop
    }
    else
    {
        tmpString = ComPath.mid(index2+1);
    }
    return getComponetType(tmpString);
}

QString getFileName(QString FilePath)
{
    QString tmpFileName;
    if (FilePath.isEmpty())
        return tmpFileName;
    int index = FilePath.lastIndexOf("/");
    if(index >= 0)
        tmpFileName =  FilePath.mid(index + 1);
    else
        tmpFileName = FilePath;
    return tmpFileName;
}

QString getFileDirectory(QString FilePath)   //>@FilePath中必须有后缀名，否则会认为是路径
{
    QString tmpFileName;
    if (FilePath.isEmpty())
        return tmpFileName;
    int index1 = FilePath.lastIndexOf("/");
    int index2 = FilePath.lastIndexOf(".");
    if(index1 < 0)
        return tmpFileName;
    if(index2 < 0)
        return FilePath;
    if(index1 < index2)
        tmpFileName =  FilePath.mid(0, index1 + 1);
    else
        return FilePath;
    return tmpFileName;
}

//>@ e:/xx/xx/
QString getParentDirectory(QString DirPath)
{
    QString tmpDirPath;
    if (DirPath.isEmpty())
        return tmpDirPath;
    if(!DirPath.endsWith("/"))
        DirPath.append("/");
    int index1 = DirPath.lastIndexOf("/");
    if(index1 < 0)
        return tmpDirPath;
    int index2 = DirPath.lastIndexOf("/", index1-1);
    if(index2 < 0)
        return tmpDirPath;
    tmpDirPath =  DirPath.mid(0, index2 + 1);
    return tmpDirPath;
}

QString GetDirectoryName(QString DirPath)
{
    QString tmpDirName;
    if (DirPath.isEmpty())
        return tmpDirName;
    if(!DirPath.endsWith("/"))
        DirPath.append("/");
    int index1 = DirPath.lastIndexOf("/");
    if(index1 < 0)
        return tmpDirName;
    int index2 = DirPath.lastIndexOf("/", index1-1);
    if(index2 < 0)
        return tmpDirName;
    tmpDirName =  DirPath.mid(index2 + 1, index1 - index2 - 1);
    return tmpDirName;
}

//>@返回文件的绝对路径
QStringList findFiles(const QDir &dir,
                      const QString &fileName,
                      const QString &txt,
                      Qt::CaseSensitivity sensitive)
{
    QString tmpDirPath = dir.path();
    int index = tmpDirPath.lastIndexOf('/');
    if(index < tmpDirPath.length()-1)
        tmpDirPath.append('/');
    QStringList foundFiles;
    QStringList files;
    if(fileName.isEmpty())
        files = dir.entryList(QDir::Files | QDir::NoSymLinks);
    else
        files = dir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);
    if(txt.isEmpty())
    {
        for(int i=0;i<files.count();i++)
        {
            foundFiles.append(tmpDirPath + files.at(i));
        }
    }
    else
    {
        for (int i = 0; i < files.size(); ++i)
        {
            QFile file(dir.absoluteFilePath(files[i]));
            if (file.open(QIODevice::ReadOnly))
            {
                QString line;
                QTextStream in(&file);
                while (!in.atEnd())
                {
                    line = in.readLine();
                    if (line.contains(txt, sensitive))
                    {
                        foundFiles.append(tmpDirPath + files[i]);
                        break;
                    }
                }
            }
        }
    }
    return foundFiles;
}

QStringList findFilesInFloder(QString DirName, QString fileName, bool subfolder)
{
    int index = DirName.lastIndexOf('/');
    if(index < DirName.length()-1)
        DirName.append('/');
    QDir dir = QDir(DirName);
    QStringList findFileList;
    if(subfolder)
    {
        QStringList folders;
        folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        if(folders.size() > 0)
        {
            for (int i = 0; i < folders.size(); ++i)
            {
                findFileList << findFilesInFloder(DirName + folders[i], fileName, true);
            }
        }
    }
    findFileList << findFiles(dir, fileName, QString(""), Qt::CaseSensitive);
    return findFileList;
}

//>@查找某种类型的文件
QStringList SearchFiles(QString pDirPath, QString pSuffix, bool pSubfolder)
{
    if(pDirPath.endsWith('/')==false)
        pDirPath.append('/');
    QDir dir = QDir(pDirPath);
    QStringList findFileList;
    if(pSubfolder)
    {
        QStringList folders;
        folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        if(folders.size() > 0)
        {
            for (int i = 0; i < folders.size(); ++i)
            {
                findFileList << SearchFiles(pDirPath + folders[i], pSuffix, pSubfolder);
            }
        }
    }

    QStringList tmpFileList = dir.entryList(QStringList(QString("*.%1").arg(pSuffix)), QDir::Files | QDir::NoSymLinks);
    for(int i=0;i<tmpFileList.count();i++)
    {
        findFileList.append(pDirPath + tmpFileList.at(i));
    }
    return findFileList;
}

QStringList ErgodicDirectory(const QString d)
{
    QStringList tmpFileList;
    QDir dir(d);
    if (dir.exists())
    {
        const QFileInfoList list = dir.entryInfoList();
        QFileInfo fi;
        for (int l = 0; l < list.size(); l++)
        {
            fi = list.at(l);
            if (fi.isDir() && fi.fileName() != "." && fi.fileName() != "..")
            {
                tmpFileList << ErgodicDirectory(fi.absoluteFilePath());
            }
            else if(fi.isFile())
            {
                tmpFileList.append(fi.absoluteFilePath());
            }
        }
    }
    return tmpFileList;
}

QStringList getAllInFloders(QString pDir)
{
    if(!pDir.endsWith("/"))
        pDir.append("/");
    QDir dir = QDir(pDir);
    QStringList getAllList,tmpList;
    QString tmpString;
    getAllList.clear();
    tmpList.clear();
    if(!dir.exists())
        return getAllList;
    tmpList = dir.entryList(QDir::Dirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot);
    for(int i = 0;i<tmpList.count();i++)
    {
        tmpString = QString("%1%2").arg(pDir).arg(tmpList.at(i));
        tmpString.replace(QString("//"),QString("/"));
        getAllList.append(tmpString);
    }
    return getAllList;
}

QStringList getAllFileInFloders(QString pDir)
{
    if(!pDir.endsWith("/"))
        pDir.append("/");
    QDir dir = QDir(pDir);
    QStringList getAllList,tmpList;
    QString tmpString;
    if(!dir.exists())
        return getAllList;
    tmpList = dir.entryList(QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot);
    for(int i = 0;i<tmpList.count();i++)
    {
        tmpString = QString("%1%2").arg(pDir).arg(tmpList.at(i));
        tmpString.replace(QString("//"),QString("/"));
        getAllList.append(tmpString);
    }
    return getAllList;
}


//>@pValidRect:为0表示全部拷贝  pResolution:目标图片分辨率  先裁剪原图，在转化为目标分辨率。
//>@ConvertImage(QString("d:/a.jpg"), QRect(0,0,640,480), QString("d:/b.bmp"), QSize(320,240));
bool ConvertImage(QString pSrcFileName,
                  QRect pValidRect,
                  QString pDstFileName,
                  QSize pResolution)
{
    if(pSrcFileName.isEmpty() || pSrcFileName.indexOf(QRegExp(".")) < 0)
    {
        IDE_TRACE();
        return false;
    }
    if(pDstFileName.isEmpty() || pDstFileName.indexOf(QRegExp(".")) < 0)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = false;
    QImage tmpImage;
    tmpImage.load(pSrcFileName);
    QRect tmpRect = tmpImage.rect();
    if(pValidRect.left() != tmpRect.left() ||
       pValidRect.right() != tmpRect.right() ||
       pValidRect.top() != tmpRect.top() ||
       pValidRect.bottom() != tmpRect.bottom())
    {
        QImage newImage = tmpImage.copy(pValidRect);
        QImage dstImage = newImage.scaled(pResolution, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        flag = dstImage.save(pDstFileName);
    }
    else
    {
        QImage dstImage = tmpImage.scaled(pResolution, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        flag = dstImage.save(pDstFileName);
    }
    return flag;
}

QString getFileSuffix(QString pFileName)
{
    int index = pFileName.lastIndexOf(".");
    QString fileSuffix;
    if(index >= 0)
        fileSuffix = pFileName.mid(index+1);
    else
        return fileSuffix;
    if(fileSuffix.length() < 2)
        return QString();
    return fileSuffix;
}

//>@根据参数判断图片缩放大小
bool ConvertImage(QString pFileName, QSize pSize, PRIOR pPrior)
{
    QFile tmpFile(pFileName);
    if(tmpFile.exists() == false)
    {
        IDE_TRACE();
        return false;
    }
    QImage tmpImage, tmpNewImage;
    if(tmpImage.load(pFileName) == false)
    {
        IDE_TRACE();
        return false;
    }
    if(pPrior == PRIOR_WIDTH)
    {
        tmpNewImage = tmpImage.scaledToWidth(pSize.width(), Qt::SmoothTransformation);
    }
    else
    {
        tmpNewImage = tmpImage.scaledToHeight(pSize.height(), Qt::SmoothTransformation);
    }
    if(tmpFile.remove() == false)
    {
        IDE_TRACE();
        return false;
    }
    if(tmpNewImage.save(pFileName) == false)
    {
        IDE_TRACE();
        return false;
    }
    return true;
}

QImage GetScaledImage(QString pFileName, QSize pSize, PRIOR pPrior)
{
    QFile tmpFile(pFileName);
    if(tmpFile.exists() == false)
    {
        return QImage();
    }
    QImage tmpImage, tmpNewImage;
    if(tmpImage.load(pFileName) == false)
    {
        return QImage();
    }
    if(pPrior == PRIOR_WIDTH)
    {
        tmpNewImage = tmpImage.scaledToWidth(pSize.width(), Qt::SmoothTransformation);
    }
    else
    {
        tmpNewImage = tmpImage.scaledToHeight(pSize.height(), Qt::SmoothTransformation);
    }
    return tmpNewImage;
}

QSize GetScaledImageSize(QString pFileName, QSize pSize, PRIOR pPrior)
{
    QImage tmpImage= GetScaledImage(pFileName, pSize, pPrior);
    if(tmpImage.isNull())
    {
        return QSize();
    }
    return tmpImage.size();
}

//>@如果pResolution无效，则不缩放
bool ConvertImage(QString pSrcFileName,
                  QString pDstFileName,
                  QSize pResolution)
{
    QFile tmpSrcFile(pSrcFileName);
    if(tmpSrcFile.exists() == false)
    {
        IDE_TRACE();
        return false;
    }
    QImage tmpImage;
    if(!tmpImage.load(pSrcFileName))
    {
        IDE_TRACE();
        return false;
    }
    if(pDstFileName.isEmpty())
    {
        IDE_TRACE();
        return false;
    }
    QString suffix = getFileSuffix(pDstFileName);
    if(suffix.isEmpty())
    {
        IDE_TRACE();
        return false;
    }
    QString tmpDstString;
    QFile tmpDstFile(pDstFileName);
    if(tmpDstFile.exists())
    {
        QFile::Permissions tmpPermissions = tmpDstFile.permissions(pDstFileName);
        if(!(tmpPermissions & QFile::WriteUser))
            tmpDstFile.setPermissions(tmpPermissions | QFile::WriteUser);
        tmpDstFile.remove();
    }
    else
    {
        tmpDstString = getFileDirectory(pDstFileName);
        QDir tmpDir;
        tmpDir.mkpath(tmpDstString);
    }

    QImage dstImage;
    //>@检查目标图片分辨率是否合理，如果大小无效或者大于1920x1920，则保持原大小不变。
    if(pResolution.isEmpty() || pResolution.isValid()==false ||
       pResolution.width() > 1920 ||
       pResolution.height() > 1920)
    {
        dstImage = tmpImage;
    }
    else
    {
        dstImage = tmpImage.scaled(pResolution, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    if(!dstImage.save(pDstFileName, suffix.toLatin1().data()))
    {
        IDE_TRACE();
        return false;
    }
    return true;
}

QString GetStrBehindKey
    (QString pString, QString pKey,
     quint8 pStart, quint8 pLenth,
     Qt::CaseSensitivity pCaps)
{
    QString tmpString;
    tmpString.clear();

    if((pLenth==0)||(pString.isEmpty()))
        return tmpString;
    int tmpDex = pString.indexOf(pKey,pCaps);
    if(tmpDex==-1)
        return tmpString;
    if((tmpDex+pKey.length()+pStart,pLenth) > pString.length())
        return tmpString;

    tmpString = pString.mid(tmpDex+pKey.length()+pStart,pLenth);
    return tmpString;
}

QString GetStrBetweenPreAndAfter
    (QString pString,
     QString pPre, QString pAfter,
     int pPreindent, int pAfterindent,
     Qt::CaseSensitivity pCaps)
{
    QString tmpString;
    tmpString.clear();

    int tmpPreDex = pString.indexOf(pPre,pCaps);
    if(tmpPreDex==-1)
        return tmpString;
    int tmpAfterDex = pString.indexOf(pAfter,pCaps);
    if(tmpAfterDex==-1)
        return tmpString;

    tmpString = pString.mid(tmpPreDex+pPre.length()+pPreindent,
                            tmpAfterDex-tmpPreDex-pPre.length()-pAfterindent);
    return tmpString;
}

/************************************************************************
//>@
************************************************************************/
MULTIMEDIA_CONVERT::MULTIMEDIA_CONVERT(QObject *parent) :
    QObject(parent)
{
    m_Process = 0;
    mMutex = new QMutex(QMutex::Recursive);
}

MULTIMEDIA_CONVERT::~MULTIMEDIA_CONVERT()
{
    KillProcess();
}

bool MULTIMEDIA_CONVERT::GetFileInfo(QString pSrc, int pTimeout)
{
    QFile file(pSrc);
    if(!file.exists())
    {
        IDE_TRACE();
        return false;
    }
    m_FileInfo.mPath = pSrc;
    m_FileInfo.mName = getFileName(m_FileInfo.mPath);
    m_FileInfo.mSize = file.size();
    QString cmd = QString("%1 -i %2").arg((QString)D_FFMPEG_PATH).arg(pSrc);
    return StartProcess(cmd, pTimeout);
}

bool MULTIMEDIA_CONVERT::ConvertAudio(QString pSrc, QString pDst, int pTimeout)
{
    if(!PreHandel(pSrc, pDst))
    {
        IDE_TRACE();
        return false;
    }
//    ./ffmpeg -y -i test.mp3 -ac 1 -ar 22050 -ab 32 -y test.wav
    QString cmd = QString("%1 -y -i %2 -ac 1 -ar 44100 -y %3").
                         arg((QString)D_FFMPEG_PATH).
                         arg(pSrc).
                         arg(pDst);
    return StartProcess(cmd, pTimeout);
}

bool MULTIMEDIA_CONVERT::ConvertVideo(QString pSrc, QSize pResolution, int pFrame, QString pDst, int pTimeout)
{
    if(!PreHandel(pSrc, pDst))
        return false;
    if(!pResolution.isValid())
        return false;
//    ffmpeg -y -i test.mp3 -ac 1 -ar 22050 -ab 32 -qscale 6 -s 640x480 -r 29.97 -y test.wav
    QString resolution = SizeToStr(pResolution);
    QString cmd = QString("%1 -y -i %2 -vcodec mpeg4 -s %3 -r %4 -y %5").
                         arg((QString)D_FFMPEG_PATH).
                         arg(pSrc).
                         arg(resolution).
                         arg(pFrame).
                         arg(pDst);
    return StartProcess(cmd, pTimeout);
}

bool MULTIMEDIA_CONVERT::ConvertVideo(QString pSrc, QRect pRect, QSize pSize, int pFrame, QString pDst, int pTimeout)
{
    if(!PreHandel(pSrc, pDst))
        return false;
    if(!pSize.isValid())
        return false;
    if(!GetFileInfo(pSrc, pTimeout/10))
        return false;
    QtSleep(100);
    QSize srcResolution = m_FileInfo.mResolution;
    quint16 left =0,top=0,right=0,bottom=0;
    left = pRect.left();
    top = pRect.top();
    right = srcResolution.width() - pRect.width() - left;
    bottom = srcResolution.height() - pRect.height() - top;
    if(((left + right)>=srcResolution.width())||((top + bottom)>=srcResolution.height()))
        return false;
//    ffmpeg -y -i test.mp3 -ac 1 -ar 22050 -ab 32 -qscale 6 -croptop 100 -cropbottom 100 -cropleft 100 -cropright 100 -s 640x480 -r 29.97 -y test.wav
    QString tempDst = QString("%1.%2").arg(pSrc).arg(getFileSuffix(pSrc));
    QString cutCmd = QString("%1 -y -i %2 -croptop %3 -cropbottom %4 -cropleft %5 -cropright %6 -r %7 -y %8").
                         arg((QString)D_FFMPEG_PATH).
                         arg(pSrc).
                         arg(top).arg(bottom).arg(left).arg(right).
                         arg(pFrame).
                         arg(tempDst);
    if(!StartProcess(cutCmd, pTimeout*3/10))
       return false;
    if(!PreHandel(tempDst, pDst))
        return false;
    QString resolution = SizeToStr(pSize);
    QString cmd = QString("%1 -y -i %2 -vcodec mpeg4 -s %3 -y %4").
                         arg((QString)D_FFMPEG_PATH).
                         arg(tempDst).
                         arg(resolution).
                         arg(pDst);
    if(!StartProcess(cmd, pTimeout*3/5))
       return false;
    QFile tempFile(tempDst);
    if(tempFile.exists())
    {
        tempFile.setPermissions(QFile::WriteOther);
        if(!tempFile.remove())
        {
            IDE_TRACE();
            return false;
        }
    }
    return true;
}
//>@----------------------------------------------------------------
void MULTIMEDIA_CONVERT::slot_CatchStandardOutput()
{
    quint8 tmpCnt = 0;
    QString tmpString,tmpInfo,tmpKeyInfo,tmpKey0,tmpKey1;
    QStringList tmpStrList;
    tmpStrList.clear();
    if(m_Process != 0)
    {
        if(m_Process->state())
            tmpInfo = m_Process->readAllStandardOutput();
    }
    qDebug()<<"---"<<tmpInfo;
//>@Type
    tmpKey0 = QString("Input #");
    tmpKey1 = QString(", from '");
    tmpKeyInfo = GetStrBetweenPreAndAfter(tmpInfo,tmpKey0,tmpKey1,0,0,Qt::CaseInsensitive);
    if(!tmpKeyInfo.isEmpty())
    {
        m_FileInfo.mType = GetSrcType(tmpKeyInfo);
    }
//>@Lenth
    tmpKey0 = QString("Duration:");
    tmpKeyInfo = GetStrBehindKey(tmpInfo,tmpKey0,1,8,Qt::CaseSensitive);
    if(!tmpKeyInfo.isEmpty())
    {
        m_FileInfo.mLenth = GetLenth(tmpKeyInfo);
    }
//>@Resolution
    tmpKey0 = QString("Stream #0.0");
    tmpKey1 = QString("[PAR");
    tmpKeyInfo = GetStrBetweenPreAndAfter(tmpInfo,tmpKey0,tmpKey1,0,0,Qt::CaseInsensitive);
    if(!tmpKeyInfo.isEmpty())
    {
        m_FileInfo.mResolution = GetResolution(tmpKeyInfo);
    }
//>@Progress
    tmpKey0 = QString("time=");
    tmpKey1 = QString("bitrate=");
    tmpStrList = tmpInfo.split(tmpKey0, QString::KeepEmptyParts, Qt::CaseInsensitive);
    tmpCnt = tmpStrList.count();
    for(int i=1;i<tmpCnt;i++)
    {
        tmpString = QString(" time=%1").arg(tmpStrList.at(i));
        tmpKeyInfo = GetStrBetweenPreAndAfter(tmpString,tmpKey0,tmpKey1,0,1,Qt::CaseSensitive);
        if(!tmpKeyInfo.isEmpty())
        {
            emit sProgress(GetProgress(tmpKeyInfo,m_FileInfo.mLenth));
        }
    }
}

void MULTIMEDIA_CONVERT::slot_Finished(int pID, QProcess::ExitStatus pState)
{
    switch(pState)
    {
        case 0:
            mTaskState = Success;
            break;
        case 1:
            mTaskState = Failed;
            break;
    }
}

QPixmap GenerateSpectrum(QString pMediaName, QRectF pRect, QPixmap pBgPixmap)
{
    QRect tmpRect;
    if(pBgPixmap.isNull() == false)
        tmpRect = pBgPixmap.rect();
    else
        tmpRect = pRect.toRect();
    QImage tmpImage(tmpRect.width(), tmpRect.height(), QImage::Format_ARGB32_Premultiplied);
    if(tmpImage.isNull())
        return QPixmap();
    tmpImage.fill(Qt::transparent); //>@创建透明图层
    QPainter painter(&tmpImage);
    if(pBgPixmap.isNull() == false)
        painter.drawPixmap(0,0,pBgPixmap);
    MULTIMEDIA_CONVERT tmpMediaCtrl;
    if(tmpMediaCtrl.GetFileInfo(pMediaName, 500))
    {
        qreal tmpDiffHeight = pRect.height() / 6.0;
        qreal tmpDiffWidth = tmpDiffHeight * 2.0 /3.0;

        QString tmpString;
        if(tmpMediaCtrl.m_FileInfo.mType == RC_INVALID)
        {
            tmpString= QString("[MediaType] : INVALID");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::red, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_MP3)
        {
            tmpString= QString("[MediaType] : MP3");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_WAV)
        {
            tmpString= QString("[MediaType] : WAV");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_MP4)
        {
            tmpString= QString("[MediaType] : MP4");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_AVI)
        {
            tmpString= QString("[MediaType] : AVI");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_RMVB)
        {
            tmpString= QString("[MediaType] : RMVB");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        else if(tmpMediaCtrl.m_FileInfo.mType == RC_FLV)
        {
            tmpString= QString("[MediaType] : FLV");
            WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top(), tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
        }
        tmpString = QString("[Path] : %1").arg(tmpMediaCtrl.m_FileInfo.mPath);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight, tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);

        tmpString = QString("[Name] : %1").arg(tmpMediaCtrl.m_FileInfo.mName);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight*2, tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);

        tmpString = QString("[TotalSize] : %1").arg(tmpMediaCtrl.m_FileInfo.mSize);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight*3, tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);

        tmpString = QString("[Resolution] : %1x%2").arg(tmpMediaCtrl.m_FileInfo.mResolution.width()).arg(tmpMediaCtrl.m_FileInfo.mResolution.height());
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight*4, tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);

        tmpString = QString("[TotalLength] : %1").arg(tmpMediaCtrl.m_FileInfo.mLenth);
        WriteText(&painter, tmpString, QRect(pRect.left(), pRect.top()+tmpDiffHeight*5, tmpDiffWidth*tmpString.length(), tmpDiffHeight), QString("Arial"), Qt::green, PRIOR_HEIGHT);
    }
    painter.end();
    return QPixmap::fromImage(tmpImage);
}

bool ConvertAudio(QString pSrcFileName, QString pDstFileName)
{
    QString tmpString = getFileSuffix(pSrcFileName);
    if(tmpString.isEmpty())
        return false;
    if(pDstFileName.endsWith(".wav", Qt::CaseInsensitive) == false)
        pDstFileName.append(".wav");
    if(tmpString.compare("wav", Qt::CaseInsensitive) == 0)
    {
        return CopyFile(pSrcFileName, pDstFileName);
    }
    else if(tmpString.compare("mp3", Qt::CaseInsensitive) == 0)
    {
        MULTIMEDIA_CONVERT tmpAudioConvert;
        return tmpAudioConvert.ConvertAudio(pSrcFileName, pDstFileName, 100000);
    }
    else
    {
        IDE_TRACE();
        return false;
    }
    return true;
}

bool ConvertVideo(QString pSrcFileName, QString pDstFileName, QSize pResolution)
{
    return ConvertVideo(pSrcFileName, QRect(), pDstFileName, pResolution);
}

bool ConvertVideo(QString pSrcFileName, QRect pValidRect, QString pDstFileName, QSize pResolution)
{
    QString tmpString = getFileSuffix(pSrcFileName);
    if(tmpString.isEmpty())
        return false;
    if(pDstFileName.endsWith(".mp4", Qt::CaseInsensitive) == false)
        pDstFileName.append(".mp4");
    MULTIMEDIA_CONVERT tmpVideoConvert;
    return tmpVideoConvert.ConvertVideo(pSrcFileName, pResolution, 10, pDstFileName, 1000000);
}

/****************************************************************************************************
            格式化输入输出函数
***************************************************************************************************/
/************************************************************
_scanf目前可支持：
仅支持字符，字符串，整形数的格式化输入；
一次性输入MAX_SCANF_COUNT个数据；
fmt中没有分隔符时默认使用空格和制表符；
分隔符可使用任意单字节数据；
以换行作为输入的结束符；
exp：
_scanf((pInputFun)myFun,"%d,%s,%c",&a,&b,&c) 输入123,jinwenjie,A\n
************************************************************/

#ifdef WINDOWS

size_t _Scanf(pInputCharFun InputFun, const char* fmt,...)
{
    //解析fmt
    if(fmt == NULL)
        return 0;
    size_t count=0;
    char tmpBuf[MAX_SCANF_COUNT][2]={{0}};  //最大支持一次读入10个数据类型
    size_t __tmp = strlen(fmt);
    for(size_t tmp=0;tmp<__tmp;tmp++)
    {
        if(fmt[tmp] == '%')
        {
            //防止溢出fmt范围
            if(tmp+1 >= __tmp)
                return 0;
            if((fmt[tmp+1]=='c')||(fmt[tmp+1]=='s'||(fmt[tmp+1]=='d')))  //目前仅支持%d,%s,%c
            {
                tmpBuf[count][0]	= fmt[++tmp];
                //printf("%c\t",tmpBuf[count][0]);
                //防止溢出fmt范围
                if(tmp+1 >= __tmp+1)
                    return 0;
                if((fmt[tmp+1]=='%')) //分隔符支持除%以外的所有字符，但只允许是单字符
                {
                    tmpBuf[count][1]	= 0;
                }
                else if(fmt[tmp+1]=='\0')  //fmt结束
                {
                    tmpBuf[count][1]	= 1;
                }
                else
                {
                    tmpBuf[count][1]	= fmt[tmp+1];
                }
                count++;
                if(count == MAX_SCANF_COUNT)
                    break;
            }
            else
                return 0;//格式不合法
        }
    }
    //此处count值为fmt中有多个少数据类型
    //printf(" total : %d\r\n",count);

    char key;
    char * p_arg_c = 0;
    char * p_arg_s = 0;
    int * p_arg_d = 0;
    va_list argbuf;
    va_start(argbuf, fmt);
    size_t i = 0;  //用于指示实际存入的数据数
    bool InReading = false;	//指示读取一个数据类型是否结束，true表还在读取
    size_t StrCount=0; //读取的字符串长度
    while(1)
    {
        if( i == count) //如果已经读取了指定的数据类型个数，则直接退出
        {
            goto exit;
        }
        key = InputFun();
        //如果没有分隔符默认以空格和\t进行分割，否则就以指定字符进行分割。注意如果有连续多个分隔符存在则默认看作一个分隔符
        if((((tmpBuf[i][1]==0)||(tmpBuf[i][1]==1))?((key != ' ')&&(key != '\t')):(key != tmpBuf[i][1]))&&(key != '\r')&&(key != '\n'))   //注意如果以其他字符作为输入终止符，则需要更改key != '\r' key != '\n'
        {
            switch(tmpBuf[i][0])
            {
            case 'c':  //只读入一个字符
                if((key >= 'a' && key <= 'z')||(key >= 'A' && key <= 'Z'))  //只取字母，否则认为不合法。如果有特殊需要要修改此处
                {
                    p_arg_c= va_arg(argbuf, char*);
                    *p_arg_c = key;
                    InReading = true;
                }
                else
                {
                    goto exit;
                }
                break;
            case 's':
                if((key >= 'a' && key <= 'z')||(key >= 'A' && key <= 'Z'))  //只取字母，否则认为不合法。如果有特殊需要要修改此处
                {
                    if(InReading == false)
                    {
                        p_arg_s = va_arg(argbuf, char*);
                        *p_arg_s = key;
                        StrCount++;
                        InReading = true;
                    }
                    else
                    {
                        *(p_arg_s+StrCount++) = key;   //此处没有加判断是否溢出的判断
                    }
                }
                else
                {
                    goto exit;
                }
                break;
            case 'd':
                if(key <= '9' && key >= '0')
                {
                    if(InReading == false)
                    {
                        p_arg_d = va_arg(argbuf, int*);
                        *p_arg_d = key - '0';
                        InReading = true;
                    }
                    else
                    {
                        *p_arg_d *= 10;
                        *p_arg_d += key - '0';
                    }
                }
                else
                {
                    goto exit;
                }
                break;
            }
        }
        else
        {
            if(InReading == true)  //确认成功读取了一个数据结构时才自加1.
                i++;
            InReading = false;  //当遇到分隔符时，表示数据结构取值结束
            if(StrCount>0)  //如果是字符串结束，则在最后补上\0
            {
                *(p_arg_s+StrCount++) = '\0';
                StrCount = 0;
            }
            if(key == '\n')	//以换行符作为输入结束
            {
                goto exit;
            }
        }
    }
exit:
    va_end(argbuf);
    return i;
}


size_t _Printf(pOutputStrFun OutputStrFun, const char* fmt,...)
{
    char *tmpBuf = new char[1024];
    size_t len;
    va_list argptr;
    va_start(argptr, fmt);
    vsprintf(tmpBuf, fmt, argptr);
    va_end(argptr);
    len = OutputStrFun(tmpBuf, strlen(tmpBuf));
    delete []tmpBuf;
    return len;
}

size_t _Printf(pOutputCharFun OutputCharFun, const char* fmt,...)
{
    char *tmpBuf = new char[1024];
    size_t len;
    va_list argptr;
    va_start(argptr, fmt);
    vsprintf(tmpBuf, fmt, argptr);
    va_end(argptr);
    for( len=0;len<strlen(tmpBuf);len++ )
    {
        if(OutputCharFun(tmpBuf[len]) == false)
            break;
    }
    return len;
}
#endif

bool InitLogPath(QString pLogPath, QString pLogName, int pMaxLogFileNum)
{
    if(pLogName.isEmpty())
        return false;
    if(pLogPath.endsWith("/") == false)
        pLogPath.append("/");
    QDir tmpDir(pLogPath);
    if(tmpDir.exists() == false)
        tmpDir.mkpath(pLogPath);
    QString FilePath = QString("%1%2").arg(pLogPath).arg(pLogName);
    QString tmppath;
    int i;
    for(i=1;i<=pMaxLogFileNum;i++)
    {
        tmppath = QString("%1.%2").arg(FilePath).arg(i);
        QFile file(tmppath);
        if(file.exists() == true)
            continue;
        else
            break;
    }
    QString srcFile;
    QString dstFile;
    if(i == pMaxLogFileNum+1)  //>@日志文件已满
    {
        for(int j=1;j<=pMaxLogFileNum;j++)
        {
            srcFile = QString("%1.%2").arg(FilePath).arg(j);
            dstFile = QString("%1.%2").arg(FilePath).arg(j-1);
            CopyFile(srcFile,dstFile);
        }
    }
    srcFile = FilePath;
    dstFile = QString("%1.%2").arg(FilePath).arg(i);
    CopyFile(srcFile,dstFile);
    return true;
}

#if(QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
void SetFuzzy(QGraphicsItem *pItem, qreal pBlurRadius)
{
    if(!pItem) return;
    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect;
    effect->setBlurRadius(pBlurRadius);
    pItem->setGraphicsEffect(effect);
}
void SetColor(QGraphicsItem *pItem, QColor pColor)
{
    if(!pItem) return;
    QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect;
    effect->setColor(pColor);
    pItem->setGraphicsEffect(effect);
}
void SetShadow(QGraphicsItem *pItem, QColor pColor, qreal pBlurRadius, QPointF pPointF)
{
    if(!pItem) return;
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setColor(pColor);
    effect->setBlurRadius(pBlurRadius);
    effect->setOffset(pPointF);
    pItem->setGraphicsEffect(effect);
}
void SetOpacity(QGraphicsItem *pItem, qreal pOpacity)
{
    if(!pItem) return;
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect;
    effect->setOpacity(pOpacity);
    pItem->setGraphicsEffect(effect);
}
void ClearFuzzy(QGraphicsItem *pItem)
{
    if(!pItem) return;
    QGraphicsBlurEffect* effect = VAR_CAST<QGraphicsBlurEffect*>(pItem->graphicsEffect());
    if(effect)effect->deleteLater();
}
void ClearColor(QGraphicsItem *pItem)
{
    if(!pItem) return;
    QGraphicsColorizeEffect* effect = VAR_CAST<QGraphicsColorizeEffect*>(pItem->graphicsEffect());
    if(effect)effect->deleteLater();
}
void ClearShadow(QGraphicsItem *pItem)
{
    if(!pItem) return;
    QGraphicsDropShadowEffect* effect = VAR_CAST<QGraphicsDropShadowEffect*>(pItem->graphicsEffect());
    if(effect)effect->deleteLater();
}
void ClearOpacity(QGraphicsItem *pItem)
{
    if(!pItem) return;
    QGraphicsOpacityEffect* effect = VAR_CAST<QGraphicsOpacityEffect*>(pItem->graphicsEffect());
    if(effect)effect->deleteLater();
}

void Effect_Fuzzy(QGraphicsItem *Item,qreal n)
{
    if(!Item) return;
    QGraphicsBlurEffect *uEffect = new QGraphicsBlurEffect();
    uEffect->setBlurRadius(n);
    Item->setGraphicsEffect(uEffect);
}

void Effect_color(QGraphicsItem *Item,int r, int g, int b)
{
    if(!Item) return;
    if(r==-1 || g==-1 || b==-1)
    {
        Item->setGraphicsEffect(0);
    }
    else
    {
        QGraphicsColorizeEffect *uEffect = new QGraphicsColorizeEffect();
        uEffect->setColor(QColor(r,g,b));
        Item->setGraphicsEffect(uEffect);
    }
}

void Effect_Shadow(QGraphicsItem *Item,qreal dx, qreal dy)
{
    if(!Item) return;
    QGraphicsDropShadowEffect *uEffect = new QGraphicsDropShadowEffect();
    uEffect->setOffset(dx,dy);
    Item->setGraphicsEffect(uEffect);
}

void Effect_Transparent(QGraphicsItem *Item,qreal opacity)
{
    if(!Item) return;
    QGraphicsOpacityEffect *uEffect = new QGraphicsOpacityEffect();
    uEffect->setOpacity(opacity);
    Item->setGraphicsEffect(uEffect);
}
#endif

GraphicsSceneBase::GraphicsSceneBase(QObject *parent, QRect pRect) :
    QGraphicsScene(parent)
{
    m_Rect = pRect;
    setSceneRect(pRect);
}

GraphicsViewBase::GraphicsViewBase(QWidget *parent)
{
    QRectF rect = this->geometry();
    m_Scene = new GraphicsSceneBase(0, QRect(0,0,rect.width(),180));
    setScene(m_Scene);

    setRenderHint(QPainter::Antialiasing);
    //setBackgroundBrush(QPixmap("d:\\bg.png"));
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setDragMode(QGraphicsView::ScrollHandDrag);

    setAttribute(Qt::WA_TranslucentBackground, true);
    setAutoFillBackground(true);
    //setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

GraphicsViewBase::GraphicsViewBase(QWidget *parent, QRect pRect):
    QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setGeometry(pRect);

    m_Scene = new GraphicsSceneBase(0, pRect);
    setScene(m_Scene);

    setRenderHint(QPainter::Antialiasing);
    //setBackgroundBrush(QPixmap("d:\\bg.png"));
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setDragMode(QGraphicsView::RubberBandDrag);

    setAttribute(Qt::WA_TranslucentBackground, true);
    setAutoFillBackground(true);
    //setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

xmlParse::xmlParse() :
    QObject(), QDomDocument()
{
    m_Valid = false;
    m_IsChanged = true;
}

xmlParse::xmlParse(QString fileName) :
    QObject(), QDomDocument()
{
    m_Valid = false;
    m_IsChanged = true;
    int index = fileName.lastIndexOf('/');
    if(index < 0)
    {
        m_FileName = fileName;
        m_FileDir = "";
    }
    else
    {
        m_FileName = fileName.mid(index+1);
        m_FileDir = fileName.mid(0,index+1);
        if(m_FileDir.endsWith('/') == false)
            m_FileDir.append('/');
    }
}

xmlParse::~xmlParse()
{
    if(m_IsChanged)
        SaveAs(m_FileDir + m_FileName);
    //clear();
    IDE_TRACE();
}

bool xmlParse::GetItemElement(QString itemPath)
{
    bool nodeFlag = true;          //>@是否为/test/subtest/类型的路径
    QString tmpString;
    QDomElement tmpElement, newElement;
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
        tmpString = itemPath.mid(indexbk+1);
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

bool xmlParse::GetItemElement(QDomElement itemDomElement, QString itemPath)
{
    bool nodeFlag = true;          //>@是否为/test/subtest/类型的路径
    QString tmpString;
    QDomElement tmpElement, newElement;
    if (itemPath.at(0) != '/')
    {
        IDE_TRACE();
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
            IDE_TRACE();
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
            IDE_TRACE();
            return false;
        }
        tmpElement = newElement;
    }
    itemElement = tmpElement;
    return true;
}

QString xmlParse::GetItemElementValue()
{
    QDomNode tmpNode = itemElement.firstChild();
    if(tmpNode.isNull() == false)
       return itemElement.text();
    return QString("");
}

QString xmlParse::GetItemElementValue(QString itemPath)
{
    if(GetItemElement(itemPath))
    {
        return GetItemElementValue();
    }
    return QString("");
}

QList<QDomAttr> xmlParse::GetItemElementAttrs(QDomElement itemDomElement)
{
    QList<QDomAttr> tmpAttrs;
    QDomNamedNodeMap tmpNodesMap = itemDomElement.attributes();
    for(int i=0;i<tmpNodesMap.count();i++)
        tmpAttrs.append(tmpNodesMap.item(i).toAttr());
    return tmpAttrs;
}

QList<QDomAttr> xmlParse::GetItemElementAttrs()
{
    return GetItemElementAttrs(itemElement);
}

QList<QDomAttr> xmlParse::GetItemElementAttrs(QString itemPath)
{
    QList<QDomAttr> tmpAttrs;
    if(GetItemElement(itemPath))
        return GetItemElementAttrs();
    return tmpAttrs;
}

bool xmlParse::SearchItemElement(QString itemName)
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

bool xmlParse::CreateItemElement(QString itemPath)
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
    m_IsChanged = true;
    return true;
}

bool xmlParse::CreateItemElement(QDomElement pParElement, QString pName, QString pValue)
{
    if(pParElement.isNull())
        return false;
    QDomElement newElement = createElement(pName);
    QDomText newTitleText = createTextNode(pValue);
    newElement.appendChild(newTitleText);
    pParElement.appendChild(newElement);
    return true;
}

bool xmlParse::SaveItemElement(QDomElement itemDomElement, QString& pContent)
{
    if(itemDomElement.isNull())
        return false;
    QTextStream out(&pContent);
    itemDomElement.save(out, 0);
    return true;
}

bool xmlParse::SaveItemElement(QString& pContent)
{
    return SaveItemElement(itemElement, pContent);
}

bool xmlParse::SaveItemElement(QDomElement itemDomElement, QIODevice *pDevice)
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

bool xmlParse::SaveItemElement(QIODevice *pDevice)
{
    return SaveItemElement(itemElement, pDevice);
}

bool xmlParse::ModifyItemElement(QString itemPath, QString value)
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

bool xmlParse::ModifyItemElement(QDomElement itemDomElement, QString value)
{
    if(itemDomElement.isElement() == false)
    {
        IDE_TRACE();
        return false;
    }
    //>@如果值为空，则表示清空当前text
    //>@如果已经有文字节点了，则替换，否则添加即可
    QDomNodeList tmpList = itemDomElement.childNodes();
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
            return false;
        }
    }
    return true;
}

bool xmlParse::SetText(QDomElement itemDomElement, QString value)
{
    return ModifyItemElement(itemDomElement, value);
}

bool xmlParse::DeleteItemElement(QDomElement itemDomElement)
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

bool xmlParse::ClearItemElement(QDomElement itemDomElement)
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
            newNode.clear();
        }
    }
    m_IsChanged = true;
    return true;
}

bool xmlParse::write(QIODevice *device)
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

bool xmlParse::read(QIODevice *device)
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

bool xmlParse::Open()
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

bool xmlParse::New()
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

bool xmlParse::Save()
{
    if(m_FileDir.isEmpty() || m_FileName.isEmpty())
        return false;
    if(m_IsChanged)
    {
        //将当前文件备份
        QString tmpString = m_FileDir +m_FileName + QString("_bk");
        bool flag = SaveAs(tmpString);
        IDE_TRACE_STR(tmpString);
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
        m_IsChanged = false;
        QtSleep(10);
    }
    return true;
}

bool xmlParse::Rename(QString fileName)
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

bool xmlParse::SaveAs(QString fileName)
{
    if(m_FileDir.isEmpty() || m_FileName.isEmpty() || fileName.isEmpty())
        return false;
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

iniParse::iniParse(QString pfileName) :
    QSettings(pfileName, QSettings::IniFormat)
{
}

bool iniParse::getProperty(QString pkey, QString &pValue)
{
    QVariant tmpVariant = value(pkey);
    if(tmpVariant.isNull())
        return false;
    pValue = tmpVariant.toString();
    return true;
}

int  iniParse::getProperty(QString pkey, int &pValue)
{
    QVariant tmpVariant = value(pkey);
    if(tmpVariant.isNull())
        return false;
    bool ok = false;
    int tmpValue = tmpVariant.toInt(&ok);
    if(ok == false)
        return false;
    pValue = tmpValue;
    return true;
}
