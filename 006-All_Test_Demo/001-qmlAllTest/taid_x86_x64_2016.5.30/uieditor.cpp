#include "uieditor.h"
#include <QDomDocument>
#include <QtXml>
#include "compressclient.h"

UiEditor::UiEditor(QObject *rootobject):rootQmlObject(rootobject)
{
    mUiManager = NULL;
    mUiList.clear();
    mDirection = Horizontal;
    m_layout = 0;
    mProjectDir = "";
    init();
    compressClient = new CompressClient(this);
#ifdef WINDOWS
    qmlFilePrefix = "file:///";
#else
    qmlFilePrefix = "file://";
#endif
    //selectedUiFile("C:/Users/lisx/Desktop/theme/Theme.ui");
}

UiEditor::~UiEditor()
{
    if(mUiManager)
    {
        delete mUiManager;
    }

    foreach(UiComParser *com, mUiList)
    {
        if(com)
        {
            delete com;
        }
    }
    mUiList.clear();
}

void UiEditor::init(){
    QObject::connect(this, &UiEditor::sgn_loadui, this, &UiEditor::loadUiFile);
    QObject::connect(this, &UiEditor::sgn_functionTest, this, &UiEditor::slt_functionTest);
}

void UiEditor::setRootQmlObj(QObject *rootObject)
{
    rootQmlObject = rootObject;
}

bool UiEditor::loadUiFile(QString filepath)
{
    //IDE_TRACE_STR(filepath);
    filepath = shiftFilePrefix(filepath);
    if(InitCom(filepath))
    {
        mProjectDir = mUiManager->m_FileDir;
        ResourceManage();
        return true;
    }
    else
    {
        IDE_TRACE();
        return false;
    }
}

bool UiEditor::InitCom(QString pPath)
{
    if(pPath.isEmpty())
    {
        return false;
    }
    pPath.replace("\\", "/");
    if(!mUiManager)
    {
        mUiManager = new UiParser(pPath);
        mUiManager->SetForcesave(false);
        if(!mUiManager->OpenUI())
        {
            mUiManager->deleteLater();
            mUiManager = 0;
            return false;
        }
    }
    else  //查询是否与已经加载的文件相同
    {
        QString tmpFile = mUiManager->m_FileDir + mUiManager->m_FileName;
        tmpFile.replace("\\", "/");
        if(tmpFile.compare(pPath))
        {
            ReleaseUIs();
            mUiManager = new UiParser(pPath);
            mUiManager->SetForcesave(false);
            if(!mUiManager->OpenUI())
            {
                mUiManager->deleteLater();
                mUiManager = 0;
                return false;
            }
        }
    }
    m_layout = mUiManager->mLayout;

    QList<UI_TYPE> tmpComTypeList = mUiManager->m_ComElementGroup.keys();
    int count = tmpComTypeList.count();
    if(count <= 0)
    {
        return false;
    }
    qSort(tmpComTypeList.begin(), tmpComTypeList.end());

    for(int i= 0;i < count; ++i)
    {
        UI_TYPE tmpType = tmpComTypeList.at(i);
        QDomElement tmpElement = mUiManager->m_ComElementGroup.value(tmpType);
        if(tmpElement.isNull())
        {
            continue;
        }
        AddCom(tmpType, tmpElement);
    }
    return true;
}

bool UiEditor::AddCom(UI_TYPE pComType, QDomElement &pComElement)
{
    if(pComType < UI_DESKTOP)
    {
        return false;
    }
    if(pComElement.isNull())
    {
        return false;
    }

    //>@如果存在此控件，则不再重复创建
    if(mUiList.contains(pComType))
    {
        return true;
    }
    UiComParser *tmpComPtr = 0;
    switch(pComType)
    {
    case UI_DESKTOP:
    case UI_FLOOR:
    case UI_DTNELE:
    case UI_ARROW:
    case UI_FUNCTION:
    case UI_IMAGE:
    case UI_PICTURE:
    case UI_VIDEO:
    case UI_MEDIA:
    case UI_TEXT:
    case UI_TITLE:
    case UI_LOGO:
    case UI_WEATHER:
    case UI_CLOCK:
    case UI_DATE:
    case UI_TIME:
    case UI_NAMEPLATE:
    case UI_FLRBTN:
    case UI_CALLBTN:
    case UI_DOORBTN:
    case UI_PHONEBTN:
    case UI_EMERGENCYBTN:
    case UI_TALKBACKBTN:
    case UI_DISABILITYBTN:
    case UI_KEYBOARDBTN:
        tmpComPtr = new UiComParser();
        break;
    default:
        return false;
    }
    if(tmpComPtr == 0)
    {
        return false;
    }

    tmpComPtr->Init(pComElement, mUiManager);
    mUiList.insert(pComType, tmpComPtr);
    return true;
}

void UiEditor::ReleaseUIs()
{
    QList<UI_TYPE> tmpList = mUiList.keys();
    if(!tmpList.isEmpty())
    {
        qSort(tmpList.begin(), tmpList.end());
        for(int i = tmpList.count() - 1; i >= 0; --i)
        {
            UiComParser *tmpUi = mUiList.take(tmpList.at(i));
            if(tmpUi)
            {
                tmpUi->deleteLater();
            }
        }
    }
    if(mUiManager)
    {
        mUiManager->deleteLater();
        mUiManager = 0;
    }
}

bool UiEditor::RemoveUI(UI_TYPE pComType)
{

    return delCom(pComType);
    //    UiComParser* tmpComPtr = mUiList.take(pComType);
    //    if(tmpComPtr)
    //    {
    //        tmpComPtr->deleteLater();
    //    }

    //    return true;
}

void UiEditor::ResourceManage()
{
    if(mProjectDir.isEmpty())
    {
        return;
    }
    QList<UI_TYPE> uilist = mUiList.keys();
    QString tmpPath;
    for (int i = 0; i < uilist.size(); ++i)
    {
        tmpPath = mProjectDir + (GetUiName(uilist.at(i))).append("/Resource/Image/");
        CreatPath(tmpPath);
    }
}

void UiEditor::selectedUiFile(QString file)
{
    file = shiftFilePrefix(file);
    //qDebug()<<file;
    emit sgn_loadui(file);
}

QSize UiEditor::getReslution(bool direct)
{
    QSize size;
    if(!mUiList.contains(UI_DESKTOP))
    {
        return size;
    }
    UiComParser *desktop = mUiList.value(UI_DESKTOP);
    QRect rect ;
    if(direct)
    {
        rect = desktop->mComRect->GetRect(m_layout, Vertical);
    }
    else
    {
        rect = desktop->mComRect->GetRect(m_layout, Horizontal);
    }

    size.setWidth(rect.width());
    size.setHeight(rect.height());
    return size ;
}

QString UiEditor::getDeviceModel()
{
    bool flag = mUiManager->GetItemElement(QString("/DeviceModel"));
    if(flag)
    {
        return mUiManager->itemElement.text();
    }
    return "Unknow";
}

QString UiEditor::getProjectDir()
{
    return mProjectDir;
}

void UiEditor::setCurDirection(bool direction)
{
    if(direction)
    {
        mDirection = Vertical;
    }
    else
    {
        mDirection = Horizontal;
    }
}

void UiEditor::loadUICom()
{
    UI_TYPE uitype;
    UiComParser *uitemp;
    QRect rectH;
    QRect rectV;
    QString str;

    QVariant retVal;
    if(!rootQmlObject)
    {
        return;
    }
    QList<UI_TYPE> list = mUiList.keys();
    for (int i = 0; i < list.size(); ++i)
    {
        //IDE_TRACE();
        uitype = list.at(i);
        if(uitype <= UI_DESKTOP)
        {
            continue;
        }
        if(!mUiList.contains(uitype))
        {
            continue;
        }
        UiRectListParser* rectList = mUiList.value(uitype)->mComRect;
        if(!rectList)
        {
            //IDE_TRACE_STR(GetUiName(uitype));
            continue;
        }
        //IDE_TRACE_STR(GetUiName(uitype));
        rectH = rectList->GetRect(m_layout, Horizontal);
        rectV = rectList->GetRect(m_layout, Vertical);
        QMetaObject::invokeMethod(rootQmlObject, "dynamicCreate",
                                  Q_RETURN_ARG(QVariant, retVal),
                                  Q_ARG(QVariant, GetUiName(uitype)),
                                  Q_ARG(QVariant, rectH),
                                  Q_ARG(QVariant, rectV));
    }

    QMetaObject::invokeMethod(rootQmlObject, "dynamicCreate",
                              Q_RETURN_ARG(QVariant, retVal),
                              Q_ARG(QVariant, "Finish"),
                              Q_ARG(QVariant, rectH),
                              Q_ARG(QVariant, rectV));
}

bool UiEditor::createUICom(QString comName, QRect rectH, QRect rectV)
{
    bool flag = false;

    UI_TYPE uiType = GetUiType(comName);
    if(uiType == UI_INVALID)
    {
        return flag;
    }
    if (addNewCom(uiType))
    {
        flag = addRecPara(uiType, Horizontal, rectH);
        if(flag)
        {
            flag = addRecPara(uiType, Vertical, rectV);
        }
    }
    return flag;
}

bool UiEditor::addNewCom(UI_TYPE tmpComType)
{
    bool flag;
    if(tmpComType == UI_INVALID)
    {
        return false;
    }
    if(!delCom(tmpComType))
    {
        return false;
    }
    //IDE_TRACE();
    QString comPath = QString("/Ui/Desktop/SubUI/%1").arg(GetUiName(tmpComType));
    flag = mUiManager->CreateItemElement(comPath);
    if(!flag)
    {
        return false;
    }
    //IDE_TRACE();
    mUiManager->m_ComElementGroup.insert(tmpComType, mUiManager->itemElement);
    return AddCom(tmpComType, mUiManager->itemElement);
}

bool UiEditor::removeUIElement(QString comName)
{
    return delCom(GetUiType(comName));
}

bool UiEditor::delCom(UI_TYPE tmpComType)
{
    if(tmpComType == UI_INVALID)
    {
        return false;
    }

    if(mUiList.contains(tmpComType))
    {
        UiComParser * tmpComParser =  mUiList.take(tmpComType);
        delete tmpComParser;
    }

    if(mUiManager->m_ComElementGroup.contains(tmpComType))
    {
        QDomElement tempElement = mUiManager->m_ComElementGroup.take(tmpComType);
        mUiManager->DeleteItemElement(tempElement);
    }
    return true;
}

bool UiEditor::addRecPara(UI_TYPE uiType, DIRECTION direct, QRect rect)
{
    if(!mUiList.contains(uiType))
    {
        return false;
    }
    UiRectListParser *recList = mUiList.value(uiType)->mComRect;
    if(!recList)
    {
        bool flag = preperUIElement(uiType, "Rect");
        if(flag)
        {
            recList = mUiList.value(uiType)->mComRect;
        }
        else
        {
            return false;
        }
    }
    if(!recList)
    {
        return false;
    }

    recList->AddRect(m_layout, mUiManager);

    if(mDirection == Horizontal)
    {
        recList->SetRect(m_layout, direct, rect);
    }
    else if(mDirection == Vertical)
    {
        recList->SetRect(m_layout, direct, rect);
    }
    return true;
}

bool UiEditor::preperUIElement(UI_TYPE uiType, QString preperType)
{
    bool flag = false;
    if(mUiManager->m_ComElementGroup.contains(uiType))
    {
        if((preperType.compare(QString("Rect"), Qt::CaseSensitive) == 0)
                ||(preperType.compare(QString("Resource"), Qt::CaseSensitive) == 0)
                ||(preperType.compare(QString("Operation"), Qt::CaseSensitive) == 0)
                ||(preperType.compare(QString("SubUI"), Qt::CaseSensitive) == 0))
        {
            QDomElement tempElemnt = mUiManager->m_ComElementGroup.value(uiType);
            flag = mUiManager->CreateItemElement(tempElemnt, preperType);
            if(flag)
            {
                flag = mUiList.value(uiType)->Init(tempElemnt, mUiManager);
            }
        }
    }
    return flag;
}

QStringList UiEditor::pullResource(QString comName)
{
    //IDE_TRACE_STR(comName);
    UI_TYPE tempType = GetUiType(comName);
    QStringList resourceList;
    //return resourceList;
    switch(tempType)
    {
    case UI_DESKTOP:
        resourceList = pullDesktopResource();
        break;
    case UI_FLOOR:
        resourceList = pullFloorResource();
        break;
    case UI_ARROW:
        resourceList = pullArrowResource();
        break;
    case UI_FUNCTION:
        resourceList = pullFunctionResource();
        break;
    case UI_MEDIA:
        resourceList = pullMediaResource();
        break;
    case UI_PICTURE:
        resourceList = pullPictureResource();
        break;
    case UI_VIDEO:
        resourceList = pullVideoResource();
        break;
    case UI_TEXT:
        resourceList = pullTextResource();
        break;
    case UI_CLOCK:
        resourceList = pullClockResource();
        break;
    case UI_DATE:
        //resourceList = pullClockResource();
        break;
    case UI_TIME:
        //resourceList = pullClockResource();
        break;
    case UI_FLRBTN:
        resourceList = pullFlrBtnResource();
        break;
    case UI_CALLBTN:
        resourceList = pullCallBtnResource();
        break;
    case UI_KEYBOARDBTN:
        resourceList = pullKeyBoardResource();
        break;
    default:
        break;
    }
    return resourceList;
}

void UiEditor::pushResource(QString comName, QStringList list)
{
    UI_TYPE tempType = GetUiType(comName);
    switch(tempType)
    {
    case UI_DESKTOP:
        pushDesktopResource(list);
        break;
    case UI_FLOOR:
        pushFloorResource(list);
        break;
    case UI_ARROW:
        pushArrowResource(list);
        break;
    case UI_FUNCTION:
        pushFunctionResource(list);
        break;
    case UI_MEDIA:
        pushMediaResource(list);
        break;
    case UI_PICTURE:
        pushPictureResource(list);
        break;
    case UI_VIDEO:
        pushVideoResource(list);
        break;
    case UI_TEXT:
        pushTextResource(list);
        break;
    case UI_CLOCK:
        pushClockResource(list);
        break;
    case UI_FLRBTN:
        pushFlrBtnResource(list);
        break;
    case UI_CALLBTN:
        pushCallBtnResource(list);
        break;
    case UI_KEYBOARDBTN:
        pushKeyBoardResource(list);
        break;
    default:
        break;
    }
}

QStringList UiEditor::pullDesktopResource()
{
    QStringList resourceList;
    if(!mUiList.contains(UI_DESKTOP))
    {
        return resourceList;
    }
    UiRcListParser *rclist = mUiList.value(UI_DESKTOP)->mComResource;
    if(!rclist)
    {
        return resourceList;
    }
    UiRcParser * rcParser = rclist->GetRc(m_layout);
    if(!rcParser)
    {
        return resourceList;
    }
    QString path = rcParser->mFile[Horizontal];
    if(QFile::exists(path))
    {
        resourceList << path;
    }
    else
    {
        resourceList<<"";
    }
    path = rcParser->mFile[Vertical];
    if(QFile::exists(path))
    {
        resourceList << path;
    }
    else
    {
        resourceList<<"";
    }
    return resourceList;
}

QStringList UiEditor::pullArrowResource()
{
    QStringList resourceList;
    if(!mUiList.contains(UI_ARROW))
    {
        return resourceList;
    }
    UiRcListParser *rclist = mUiList.value(UI_ARROW)->mComResource;
    if(!rclist)
    {
        return resourceList;
    }
    for(int i = 0; i < 2; ++i)
    {
        UiRcParser * rcParser = rclist->GetRc(i + rclist->mCurKey);
        if(!rcParser)
        {
            resourceList<<"";
        }
        else
        {
            QString path = rcParser->mFile[Horizontal];//hv
            if(QFile::exists(path))
            {
                resourceList << path;
            }
            else
            {
                resourceList<<"";
            }
        }
    }
    //    UiRcParser * rcParser = rclist->GetRc(100);
    //    if(!rcParser){
    //        resourceList<<"";
    //    }else{
    //        QString path = rcParser->mFile[Horizontal];//hv
    //        if(QFile::exists(path))
    //            resourceList << path;
    //        else{
    //            resourceList<<"";
    //        }
    //    }
    return resourceList;
}

QStringList UiEditor::pullFloorResource()
{
    QStringList resourceList;
    if(!mUiList.contains(UI_FLOOR))
    {
        return resourceList;
    }
    UiRcListParser *rclist = mUiList.value(UI_FLOOR)->mComResource;
    if(!rclist)
    {
        return resourceList;
    }
    QList<int> rckeys =  rclist->mRCs.keys();
    int count = rckeys.count();
    if(count <= 0)
    {
        return resourceList;
    }
    qSort(rckeys.begin(), rckeys.end());
    UiRcParser * rcParser;
    for(int i= 0; i < count; ++i)
    {
        int rckey = rckeys.at(i);
        rcParser = rclist->mRCs.value(rckey);
        if(rcParser)
        {
            QString path = rcParser->mFile[Horizontal];//hv
            if(QFile::exists(path))
            {
                resourceList << path;
            }
        }
    }
    return resourceList;
}

QStringList UiEditor::pullFunctionResource()
{
    QStringList resourceList;
    if(!mUiList.contains(UI_FUNCTION))
    {
        return resourceList;
    }
    UiRcListParser *rclist = mUiList.value(UI_FUNCTION)->mComResource;
    if(!rclist)
    {
        IDE_TRACE();
        return resourceList;
    }
    QList<int> rckeys =  rclist->mRCs.keys();
    int count = rckeys.count();
    if(count <= 0)
    {
        return resourceList;
    }
    qSort(rckeys.begin(), rckeys.end());
    UiRcParser * rcParser;
    for(int i= 0; i < count; ++i)
    {
        int rckey = rckeys.at(i);
        rcParser = rclist->mRCs.value(rckey);
        if(rcParser)
        {
            QString path = rcParser->mFile[Horizontal];//hv
            QString text = rcParser->GetAttr("Text");
            if(QFile::exists(path))
            {
                resourceList << path+QString("@%1").arg(text);
            }
        }
    }
    return resourceList;
}

QStringList UiEditor::pullMediaResource()
{
    QStringList resourceList;
    if(!mUiList.contains(UI_MEDIA))
    {
        return resourceList;
    }
    UiRcListParser *rclist = mUiList.value(UI_MEDIA)->mComResource;
    if(!rclist)
    {
        //IDE_TRACE();
        return resourceList;
    }
    QList<int> rckeys =  rclist->mRCs.keys();
    int count = rckeys.count();
    //IDE_TRACE_INT(count);
    if(count <= 0)
    {
        //IDE_TRACE();
        return resourceList;
    }
    qSort(rckeys.begin(), rckeys.end());
    UiRcParser * rcParser;
    for(int i= 0;i < count; ++i)
    {
        int rckey = rckeys.at(i);
        rcParser = rclist->mRCs.value(rckey);
        if(rcParser)
        {
            QString path = rcParser->mFile[Horizontal];
            if(QFile::exists(path))
            {
                resourceList << qmlFilePrefix + path;
            }
        }
    }
    return resourceList;
}

QStringList UiEditor::pullPictureResource()
{
    QStringList resourceList;
    if(!mUiList.contains(UI_PICTURE))
    {
        return resourceList;
    }
    UiRcListParser *rclist = mUiList.value(UI_PICTURE)->mComResource;
    if(!rclist)
    {
        return resourceList;
    }
    QList<int> rckeys =  rclist->mRCs.keys();
    int count = rckeys.count();
    //IDE_TRACE_INT(count);
    if(count <= 0)
    {
        return resourceList;
    }
    qSort(rckeys.begin(), rckeys.end());
    UiRcParser * rcParser;
    for(int i= 0;i < count; ++i)
    {
        int rckey = rckeys.at(i);
        rcParser = rclist->mRCs.value(rckey);
        if(rcParser)
        {
            QString path = rcParser->mFile[Horizontal];
            if(QFile::exists(path))
            {
                resourceList << qmlFilePrefix + path;
            }
        }
    }
    return resourceList;
}

QStringList UiEditor::pullVideoResource()
{
    QStringList resourceList;
    if(!mUiList.contains(UI_VIDEO))
    {
        return resourceList;
    }
    UiRcListParser *rclist = mUiList.value(UI_VIDEO)->mComResource;
    if(!rclist)
    {
        return resourceList;
    }
    QList<int> rckeys =  rclist->mRCs.keys();
    int count = rckeys.count();
    if(count <= 0)
    {
        return resourceList;
    }
    qSort(rckeys.begin(), rckeys.end());
    UiRcParser * rcParser;
    for(int i= 0;i < count; ++i)
    {
        int rckey = rckeys.at(i);
        rcParser = rclist->mRCs.value(rckey);
        if(rcParser)
        {
            QString path = rcParser->mFile[Horizontal];
            if(QFile::exists(path))
            {
                resourceList << qmlFilePrefix + path;
            }
        }
    }
    return resourceList;
}

QStringList UiEditor::pullMediaResource(int mediaType)/*1-img;2-video;3-music*/
{
    QStringList list;
    QString scanPath;


    if(mediaType == 1)
    {
        scanPath = mProjectDir + "Media/Resource/Image";

    }
    else if(mediaType == 2)
    {
        scanPath = mProjectDir + "Media/Resource/Video";
    }
    else if(mediaType == 3)
    {
        scanPath = mProjectDir + "Media/Resource/Music";
    }
    else
    {

    }
    return list;
}

QStringList UiEditor::pullTextResource()
{
    QStringList list;
    QString tmpString;
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_ARROW))
    {
        return list;
    }
    UIDom = mUiList.value(UI_ARROW)->mElement;

    if(mUiManager->GetItemElement(UIDom, "/Operation/Run"))
    {
        tempDom = mUiManager->itemElement;
        tmpString = tempDom.text();
        list << tmpString;
    }
    return list;
}

QStringList UiEditor::pullCallBtnResource()
{
    QStringList resourceList;
    UiRcListParser *rclist = mUiList.value(UI_CALLBTN)->mComResource;
    if(!rclist)
    {
        return resourceList;
    }
    UiRcParser * rcParser = rclist->GetRc(rclist->mCurKey);
    if(rcParser)
    {
        QString path = rcParser->mFile[Horizontal];
        if(QFile::exists(path))
        {
            resourceList << path;
        }
        else
        {
            resourceList << "";
        }
    }
    else
    {
        IDE_TRACE();
        resourceList << "";
    }
    if(!mUiList.contains(UI_CALLBTN))
    {
        return resourceList;
    }

    UiComParser* com = mUiList.value(UI_CALLBTN);
    QDomElement curDom;
    QString pressPath, releasePath;
    bool upCall = false;
    bool dnCall = false;
    int btnStatu = 0;
    float spacing = 0.0;

    if(mUiManager->GetItemElement(com->mElement, "/SubUi/KeyUp"))
    {
        upCall = true;
        curDom = mUiManager->itemElement;
    }

    if(mUiManager->GetItemElement(com->mElement, "/SubUi/KeyDn"))
    {
        dnCall = true;
        curDom = mUiManager->itemElement;
    }

    if(upCall&&dnCall)
    {
        btnStatu = 3;
    }
    else if(upCall&&!dnCall)
    {
        btnStatu = 1;
    }
    else if(!upCall&&dnCall)
    {
        btnStatu = 2;
    }
    else
    {

    }

    resourceList << QString("%1").arg(btnStatu);
    QDomElement subDom;
    if(curDom.isNull())
        return resourceList;

    if((btnStatu == 3) && (mUiManager->GetItemElement(curDom, "/Rect/Layout"+QString("%1").arg(m_layout))))
    {
        subDom = mUiManager->itemElement;
        QRect rect = GetRectFromString(subDom.attribute("HV"));
        QRect rectH = mUiList.value(UI_CALLBTN)->mComRect->GetRect(m_layout, Horizontal);

        spacing = 1- (float)2*rect.height()/rectH.height();
        //qDebug()<<rect.height()<<" "<<rectH.height()<<" "<<spacing;
        resourceList << QString("%1").arg(spacing);
    }
    else
    {
        spacing = 0;
        resourceList << QString("%1").arg(spacing);
    }

    if(mUiManager->GetItemElement(curDom, "/Resource/RC0"))
    {
        subDom = mUiManager->itemElement;
        pressPath = mProjectDir + subDom.attribute("HV");
        resourceList << pressPath;
    }
    else
    {
        resourceList << "";
    }

    if(mUiManager->GetItemElement(curDom, "/Resource/RC1"))
    {
        subDom = mUiManager->itemElement;
        releasePath = mProjectDir + subDom.attribute("HV");
        resourceList << releasePath;
    }
    else
    {
        resourceList << "";
    }

    return resourceList;
}

QStringList UiEditor::pullKeyBoardResource()
{
    return scanKeyBoardImg(mProjectDir+"KeyboardBtn/Resource/Image");
}

QStringList UiEditor::scanKeyBoardImg(QString pDir)
{
    QStringList list;
    pDir = shiftFilePrefix(pDir);
    if(pDir.isEmpty())
    {
        return list;
    }
    pDir.replace("\\", "/");
    if(!pDir.endsWith('/'))
    {
        pDir.append('/');
    }
    QFileInfo fileInfo(pDir);

    if(!fileInfo.isDir())
    {
        return list;
    }

    QVector<int> folderName;
    folderName << 8 << 13;
    for(int i=48; i<58; ++i)
    {
        folderName << i;
    }

    folderName << 65 << 66;

    list<< qmlFilePrefix + pDir;
    QString dirPath;
    for(int j=0;j<folderName.size();j++)
    {
        dirPath = pDir + QString("%1/").arg(folderName.at(j));
        QDir tmpDir = QDir(dirPath);
        if(tmpDir.exists())
        {
            QString filePath = scanKeyImg(pDir, dirPath);
            list.append(QString("%1:%2").arg(folderName.at(j)).arg(filePath));
        }
    }
    return list;
}

int UiEditor::scanKeyImg(QString pDir)
{
    QFileInfo fileInfo(pDir);
    if(!fileInfo.isDir())
    {
        return 0;
    }

    QDir tmpDir = QDir(pDir);
    QFileInfoList tmpFolders = tmpDir.entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    int count = tmpFolders.count();

    QString tmp;
    QString path0 = "";
    QString path1 = "";

    for (int i = 0; i < count; ++i)
    {
        QFileInfo tmpSubFolder = tmpFolders.at(i);
        if(IsSupportImg(tmpSubFolder))
        {
            tmp = tmpSubFolder.baseName();
            if(tmp.compare("0", Qt::CaseInsensitive)==0)
            {
                path0 = tmpSubFolder.filePath();
            }

            if(tmp.compare("1", Qt::CaseInsensitive)==0)
            {
                path1 = tmpSubFolder.filePath();
            }
        }
    }
    if(path0 != "" && path1 == "")
    {
        return 1;
    }
    else if(path0 == "" && path1 != "")
    {
        return 2;
    }
    else if(path0 != "" && path1 != "")
    {
        return 3;
    }
    else
    {
        return 0;
    }
}

QString UiEditor::scanKeyImg(QString pPath, QString pDir)
{

    QFileInfo fileInfo(pDir);
    if(!fileInfo.isDir())
    {
        return QString(":");
    }

    QDir tmpDir = QDir(pDir);
    QFileInfoList tmpFolders = tmpDir.entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    int count = tmpFolders.count();

    QString tmp;
    QString path0 = "";
    QString path1 = "";

    for (int i = 0; i < count; ++i)
    {
        QFileInfo tmpSubFolder = tmpFolders.at(i);
        if(IsSupportImg(tmpSubFolder))
        {
            tmp = tmpSubFolder.baseName();
            if(tmp.compare("0", Qt::CaseInsensitive)==0)
            {
                path0 = tmpSubFolder.filePath().mid(pPath.length());
            }

            if(tmp.compare("1", Qt::CaseInsensitive)==0)
            {
                path1 = tmpSubFolder.filePath().mid(pPath.length());
            }
        }
    }
    return path0+":"+path1;
}

QStringList UiEditor::pullClockResource()
{
    return QStringList();
}

QStringList UiEditor::scanClockResource(bool dateOrtime/*true for Date*/, QString pDir)
{
    QStringList list;
    if(pDir.isEmpty())
    {
        pDir = mProjectDir+"Clock/Resource/Image";
    }
    pDir = shiftFilePrefix(pDir);
    if(pDir.isEmpty())
    {
        return list;
    }
    pDir.replace("\\", "/");
    if(!pDir.endsWith('/'))
    {
        pDir.append('/');
    }
    QFileInfo fileInfo(pDir);

    if(!fileInfo.isDir())
    {
        return list;
    }

    QVector<int> folderName;

    for(int i=0; i<10; ++i)
    {
        folderName << i;
    }
    folderName << 56;

    list<< qmlFilePrefix + pDir;
    QString tmpFile;

    if(dateOrtime)
    {
        for(int j = 0; j < folderName.size(); ++j)
        {
            tmpFile = scanClockImg(pDir, QString("d%1").arg(folderName.at(j)));
            if(!tmpFile.isEmpty())
            {
                list << tmpFile;
            }
        }
    }
    else
    {
        for(int j = 0; j < folderName.size(); j++)
        {
            tmpFile = scanClockImg(pDir, QString("t%1").arg(folderName.at(j)));
            if(!tmpFile.isEmpty())
            {
                list << tmpFile;
            }
        }
    }

    return list;
}

QString UiEditor::scanClockImg(QString pPath, QString bName)
{
    QFileInfo fileInfo(pPath);
    if(!fileInfo.isDir())
    {
        IDE_TRACE();
        return QString();
    }

    QDir tmpDir = QDir(pPath);
    QFileInfoList tmpFolders = tmpDir.entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    int count = tmpFolders.count();

    QString tmp;

    for (int i = 0; i < count; ++i)
    {
        QFileInfo tmpfile = tmpFolders.at(i);
        tmp = tmpfile.baseName();
        if(tmp.compare(bName, Qt::CaseInsensitive)==0)
        {
            if(IsSupportImg(tmpfile))
            {
                return tmpfile.fileName();
            }
        }
    }
    return QString();
}

QStringList UiEditor::pullFlrBtnResource()
{
    QDomElement subUi;
    QStringList list;
    QString themeDirPath = "FlrBtn/Resource/Image/";
    QString dstPath = mProjectDir+themeDirPath;
    list << qmlFilePrefix + dstPath;
    if(!mUiList.contains(UI_FLRBTN))
        return list;

    if(mUiManager->GetItemElement(mUiList.value(UI_FLRBTN)->mElement, "/SubUi"))
    {
        subUi = mUiManager->itemElement;
        QDomNodeList tmpList = subUi.childNodes();
        QString keyInfo;
        if(!tmpList.isEmpty())
        {
            for(int i= 0;i < tmpList.count();i++)
            {
                QDomNode tmpNode = tmpList.at(i);
                if(tmpNode.isNull())
                {
                    continue;
                }
                QString tagName = tmpNode.nodeName();
                QString keyIndex;
                if(tagName.startsWith("Key"))
                {
                    keyIndex = tagName.mid(3);
                    keyInfo = keyIndex;
                }
                else
                {
                    continue;
                }
                if(mUiManager->GetItemElement(subUi, QString("/%1/Resource/RC0").arg(tagName)))
                {
                    if(QFile::exists(dstPath+keyIndex+"/0.png"))
                    {
                        keyInfo += ":" + keyIndex+"/0.png";
                    }
                    else
                    {
                        keyInfo += ":" ;
                    }
                }
                if(mUiManager->GetItemElement(subUi, QString("/%1/Resource/RC1").arg(tagName)))
                {
                    if(QFile::exists(dstPath+keyIndex+"/1.png"))
                    {
                        keyInfo += ":" + keyIndex+"/1.png";
                    }
                    else
                    {
                        keyInfo += ":" ;
                    }
                }
                list << keyInfo;
            }
        }
    }

    return list;
}

QStringList UiEditor::pullFlrBtnResource(int startFlr, int endFlr, QString pPath)
{
    QStringList list;
    pPath = shiftFilePrefix(pPath);
    QFileInfo fileInfo(pPath);
    if(startFlr > endFlr)
    {
        return list;
    }

    if(!fileInfo.isDir())
    {
        pPath = mProjectDir+"FlrBtn/Resource/Image/";
    }

    pPath.replace("\\", "/");
    if(!pPath.endsWith('/'))
    {
        pPath.append('/');
    }

    QString dirPath;
    list << qmlFilePrefix + pPath;
    for(int j = startFlr; j <= endFlr; ++j)
    {
        dirPath = pPath + QString("%1/").arg(j);
        QDir tmpDir = QDir(dirPath);
        if(tmpDir.exists())
        {
            QString filePath = scanKeyImg(pPath, dirPath);
            list.append(QString("%1:%2").arg(j).arg(filePath));
        }
    }

    return list;
}

void UiEditor::convertAndTransmit(QString src, QString dst, UI_TYPE uiType, bool aspectRatio)
{
    if(!mUiList.contains(uiType))
    {
        return ;
    }
    UiRectListParser* rectList = mUiList.value(uiType)->mComRect;
    if(rectList)
    {
        QRect rect = rectList->GetRect(m_layout, Horizontal);
        QSize size(rect.width(), rect.height());
        if(QFile::exists(src))
        {
            QImage sImage(src);
            QImage dImage;
            if(aspectRatio)
            {
                size =  scaleToHeight(sImage.size(), size);
            }
            dImage = sImage.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

            if(QFile::exists(dst))
            {
                DelFile(dst);
            }
            if(!dImage.isNull())
            {
                dImage.save(dst);
            }
        }
    }
}

void UiEditor::convertImageTo(QString src, QString dst, QSize size, bool aspectRatio)
{
    if(0 == src.compare(dst, Qt::CaseSensitive))
    {
        return;
    }
    if(QFile::exists(src))
    {
        QImage sImage(src);
        QImage dImage;
        if(aspectRatio)
        {
            size =  scaleToHeight(sImage.size(), size);
        }
        dImage = sImage.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        if(QFile::exists(dst))
        {
            DelFile(dst);
        }

        if(!dImage.isNull())
        {
            dImage.save(dst);
        }
    }
}

QSize UiEditor::scaleToHeight(QSize sSize, QSize dSize)
{
    qreal dheight = dSize.height();
    qreal sheight = sSize.height();
    qreal heightRate = dheight / sheight;

    int dwith = heightRate * sSize.width();
    return QSize(dwith, dSize.height());
}

bool UiEditor::pushDesktopResource(QStringList list)
{
    if(list.isEmpty())
    {
        return true;
    }
    if(!mUiList.contains(UI_DESKTOP))
    {
        return false;
    }
    if(!mUiList.contains(UI_DESKTOP))
    {
        return false;
    }
    UiRcListParser *rclist = mUiList.value(UI_DESKTOP)->mComResource;
    if(!rclist)
    {
        preperUIElement(UI_DESKTOP, "Resource");
        rclist = mUiList.value(UI_DESKTOP)->mComResource;
        if(!rclist)
        {
            return false;
        }
    }

    UiRcParser * rcParser = rclist->AddRc(m_layout, mUiManager, true);
    if(!rcParser)
    {
        return false;
    }

    if(list.size()!=2)
    {
        return false;
    }

    QString pushPath = list.at(0);
    pushPath = shiftFilePrefix(pushPath);
    QString oldpath = rcParser->mFile[Horizontal];
    QString dstPath = mProjectDir+"Desktop/Resource/Image/";

    QString dst = dstPath +QString("H%1.png").arg(m_layout);
    CreatPath(dstPath);
    if(QFile::exists(pushPath))
    {
        if(pushPath.compare(oldpath, Qt::CaseSensitive) != 0)
        {
            DelFile(oldpath);
            convertAndTransmit(pushPath, dst, UI_DESKTOP);
            rcParser->SetRc(mProjectDir, dst, Horizontal);
        }
    }

    pushPath = list.at(1);
    pushPath = shiftFilePrefix(pushPath);
    oldpath = rcParser->mFile[Vertical];
    dst = dstPath + QString("V%1.png").arg(m_layout);
    if(QFile::exists(pushPath)){
        if(pushPath.compare(oldpath, Qt::CaseSensitive) != 0)
        {
            DelFile(oldpath);
            convertAndTransmit(pushPath, dst, UI_DESKTOP);
            rcParser->SetRc(mProjectDir, dst, Vertical);
        }
    }
    return true;
}

bool UiEditor::pushArrowResource(QStringList list)
{
    if(list.isEmpty() || list.size() != 2)
    {
        return true;
    }

    if(!mUiList.contains(UI_ARROW))
    {
        return false;
    }
    UiRcListParser *rclist = mUiList.value(UI_ARROW)->mComResource;
    if(!rclist)
    {
        preperUIElement(UI_ARROW, "Resource");
        rclist = mUiList.value(UI_ARROW)->mComResource;
        if(!rclist)
        {
            return false;
        }
    }

    UiRcParser * rcParser;
    QString pushPath;
    QString oldpath;
    QString dst;
    QString dstPath = mProjectDir+"Arrow/Resource/Image/";
    CreatPath(dstPath);
    for(int i = rclist->mCurKey, j = 0; i<rclist->mCurKey + 2; ++i, ++j)
    {
        rcParser = rclist->AddRc(i, mUiManager, true);
        if(!rcParser)
        {
            continue;
        }
        pushPath = list.at(j);
        pushPath = shiftFilePrefix(pushPath);
        oldpath = rcParser->mFile[Horizontal];

        dst = dstPath +QString("%1.png").arg(i);
        if(QFile::exists(pushPath))
        {
            if(pushPath.compare(oldpath, Qt::CaseSensitive) != 0)
            {
                DelFile(oldpath);
                convertAndTransmit(pushPath, dst, UI_ARROW);
                rcParser->SetRc(mProjectDir, dst, HVBoth);
            }
        }
    }

    //    rcParser = rclist->AddRc(100, mUiManager, true);
    //    if(rcParser){
    //        pushPath = list.at(3);
    //        pushPath = shiftFilePrefix(pushPath);

    //        oldpath = rcParser->mFile[Horizontal];
    //        dst = dstPath +QString("%1.png").arg(100);
    //        if(QFile::exists(pushPath)){
    //            if(pushPath.compare(oldpath, Qt::CaseSensitive) != 0)
    //            {
    //                DelFile(oldpath);
    //                convertAndTransmit(pushPath, dst, UI_ARROW);
    //                rcParser->SetRc(mProjectDir, dst, HVBoth);
    //            }
    //        }
    //    }
    return true;
}

bool UiEditor::pushFloorResource(QStringList list)
{
    if(list.isEmpty())
    {
        return false;
    }
    if(!mUiList.contains(UI_FLOOR))
    {
        return false;
    }
    UiRcListParser *rclist = mUiList.value(UI_FLOOR)->mComResource;
    if(!rclist)
    {
        preperUIElement(UI_FLOOR, "Resource");
        rclist = mUiList.value(UI_FLOOR)->mComResource;
        if(!rclist)
        {
            return false;
        }
    }

    UiRcParser * rcParser;
    QString pushPath;
    QString oldpath;
    QString dst;
    QString dstPath = mProjectDir+"Floor/Resource/Image/";
    QString fileName;
    bool convertFlag;
    for(int i = 0; i<list.size(); ++i)
    {
        pushPath = list.at(i);
        pushPath = shiftFilePrefix(pushPath);
        fileName = GetFileBaseName(pushPath);
        int keyNumber = fileName.toInt(&convertFlag);
        if(!convertFlag)
        {
            continue;
        }
        rcParser = rclist->AddRc(keyNumber, mUiManager, true);
        if(!rcParser)
        {
            continue;
        }
        oldpath = rcParser->mFile[Horizontal];

        dst = dstPath +QString("%1.png").arg(keyNumber);
        if(QFile::exists(pushPath))
        {
            if(pushPath.compare(oldpath, Qt::CaseSensitive) != 0)
            {
                DelFile(oldpath);
                convertAndTransmit(pushPath, dst, UI_FLOOR);
                rcParser->SetRc(mProjectDir, dst, HVBoth);
            }
        }
    }
}

bool UiEditor::pushFunctionResource(QStringList list)
{
    clearElementRcList(UI_FUNCTION);
    if(list.isEmpty())
    {
        return false;
    }
    if(!mUiList.contains(UI_FUNCTION))
    {
        return false;
    }
    UiRcListParser *rclist = mUiList.value(UI_FUNCTION)->mComResource;
    if(!rclist)
    {
        preperUIElement(UI_FUNCTION, "Resource");
        rclist = mUiList.value(UI_FUNCTION)->mComResource;
        if(!rclist)
        {
            return false;
        }
    }

    UiRcParser * rcParser;
    QString pushPath;
    QString oldpath;
    QString dst;
    QString dstPath = mProjectDir + "Function/Resource/Image/";

    QStringList templist;
    QString fileName;
    bool convertFlag;
    CreatPath(dstPath);
    for(int i = 0;i < list.size();i++)
    {
        QString temp = list.at(i);
        templist = temp.split("@");
        if(templist.size() != 2)
        {
            continue;
        }
        pushPath = templist.at(0);
        pushPath = shiftFilePrefix(pushPath);
        fileName = GetFileBaseName(pushPath);
        //IDE_TRACE_STR(fileName);
        int keyNumber = fileName.toInt(&convertFlag);
        if(!convertFlag)
        {
            IDE_TRACE();
            continue;
        }
        rcParser = rclist->AddRc(keyNumber, mUiManager, true);
        if(!rcParser)
        {
            continue;
        }
        oldpath = rcParser->mFile[Horizontal];

        dst = dstPath +QString("%1.png").arg(keyNumber);
        if(QFile::exists(pushPath)){
            if(pushPath.compare(oldpath, Qt::CaseSensitive) != 0)
            {
                DelFile(oldpath);
                convertAndTransmit(pushPath, dst, UI_FUNCTION);
                rcParser->SetRc(mProjectDir, dst, HVBoth);
                rcParser->SetAttr("Text", templist.at(1));
            }
            else
            {
                rcParser->SetAttr("Text", templist.at(1));
            }
        }
    }
    return true;
}

void UiEditor::clearElementRcList(UI_TYPE uiType)
{
    UiRcListParser *rclist = mUiList.value(uiType)->mComResource;
    if(rclist)
    {
        QHash<int, UiRcParser*>  rc =  rclist->mRCs;
        QList<int> rckeys = rc.keys();
        for(int i = 0; i<rckeys.count(); ++i)
        {
            rclist->DelRc(rckeys.at(i));
        }
    }
}

bool UiEditor::pushMediaResource(QStringList list)
{
    if(!mUiList.contains(UI_MEDIA))
    {
        IDE_TRACE();
        return false;
    }

    if(list.isEmpty())
    {
        //IDE_TRACE();
        clearElementRcList(UI_MEDIA);
        return false;
    }

    UiRcListParser *rclist = mUiList.value(UI_MEDIA)->mComResource;

    if(!rclist)
    {
        preperUIElement(UI_MEDIA, "Resource");
        rclist = mUiList.value(UI_MEDIA)->mComResource;
        if(!rclist)
        {
            IDE_TRACE();
            return false;
        }
    }

    UiRcParser * rcParser;
    QString pushPath, oldpath, dst;
    QString dstPath = mProjectDir+"Media/Resource/Audio/";
    CreatPath(dstPath);
    int keyNumber;
    for(int i = 0; i < list.size(); ++i)
    {
        pushPath = list.at(i);
        pushPath = shiftFilePrefix(pushPath);
        keyNumber = rclist->mCurKey + i;
        rcParser = rclist->AddRc(keyNumber, mUiManager, true);
        if(!rcParser)
        {
            IDE_TRACE();
            continue;
        }
        oldpath = rcParser->mFile[Horizontal];

        if(QFile::exists(pushPath))
        {
            //IDE_TRACE();
            QFileInfo fileInfo(pushPath);
            QString subfix = fileInfo.suffix();
            dst = dstPath +QString("%1.%2").arg(keyNumber).arg(subfix);
            if(pushPath.compare(oldpath, Qt::CaseSensitive) != 0)
            {
                /////IDE_TRACE_STR(oldpath);
                ///IDE_TRACE_STR(dst);
                DelFile(oldpath);
                TCopyFile(pushPath, dst);
                //convertAndTransmit(pushPath, dst, UI_VIDEO);
                rcParser->SetRc(mProjectDir, dst, HVBoth);
            }
        }
    }

    QHash<int, UiRcParser*>  rc =  rclist->mRCs;
    QList<int> rckeys = rc.keys();
    for(int i=0;i<rckeys.count();i++)
    {
        if(rckeys.at(i) >= list.size() + rclist->mCurKey)
            rclist->DelRc(rckeys.at(i));
    }

    return true;
}

bool UiEditor::pushPictureResource(QStringList list)
{
    if(!mUiList.contains(UI_PICTURE))
        return false;
    //clearElementRcList(UI_PICTURE);
    if(list.isEmpty())
    {
        clearElementRcList(UI_PICTURE);
        return false;
    }

    UiRcListParser *rclist = mUiList.value(UI_PICTURE)->mComResource;
    if(!rclist)
    {
        preperUIElement(UI_PICTURE, "Resource");
        rclist = mUiList.value(UI_PICTURE)->mComResource;
        if(!rclist)
        {
            return false;
        }
    }
    IDE_TRACE();
    UiRcParser * rcParser;
    QString pushPath;
    QString oldpath;
    QString dst;
    QString dstPath = mProjectDir+"Picture/Resource/Image/";
    CreatPath(dstPath);
    int keyNumber;
    for(int i = 0;i < list.size();i++)
    {
        pushPath = list.at(i);
        pushPath = shiftFilePrefix(pushPath);
        keyNumber = rclist->mCurKey + i;
        rcParser = rclist->AddRc(keyNumber, mUiManager, true);
        if(!rcParser)
        {
            continue;
        }
        oldpath = rcParser->mFile[Horizontal];

        dst = dstPath +QString("%1.png").arg(keyNumber);
        if(QFile::exists(pushPath))
        {
            if(pushPath.compare(oldpath, Qt::CaseSensitive) != 0)
            {
                DelFile(oldpath);
                convertAndTransmit(pushPath, dst, UI_PICTURE);
                rcParser->SetRc(mProjectDir, dst, HVBoth);
            }
        }
    }

    QHash<int, UiRcParser*>  rc =  rclist->mRCs;
    QList<int> rckeys = rc.keys();
    for(int i = 0; i<rckeys.count(); ++i)
    {
        if(rckeys.at(i) >= list.size() + rclist->mCurKey)
            rclist->DelRc(rckeys.at(i));
    }

    return true;
}

bool UiEditor::pushVideoResource(QStringList list)
{
    //clearElementRcList(UI_VIDEO);
    //IDE_TRACE();
    if(!mUiList.contains(UI_VIDEO))
    {
        return false;
    }

    if(list.isEmpty())
    {
        clearElementRcList(UI_VIDEO);
        return false;
    }

    //IDE_TRACE();
    UiRcListParser *rclist = mUiList.value(UI_VIDEO)->mComResource;
    if(!rclist)
    {
        preperUIElement(UI_VIDEO, "Resource");
        rclist = mUiList.value(UI_VIDEO)->mComResource;
        if(!rclist)
        {
            return false;
        }
    }

    UiRcParser * rcParser;
    QString pushPath, oldpath, dst;
    QString dstPath = mProjectDir+"Video/Resource/Video/";
    CreatPath(dstPath);
    int keyNumber;
    //IDE_TRACE();
    for(int i = 0; i < list.size(); ++i)
    {
        pushPath = list.at(i);
        pushPath = shiftFilePrefix(pushPath);
        keyNumber = rclist->mCurKey + i;
        rcParser = rclist->AddRc(keyNumber, mUiManager, true);
        if(!rcParser)
        {
            continue;
        }
        oldpath = rcParser->mFile[Horizontal];

        if(QFile::exists(pushPath))
        {
            QFileInfo fileInfo(pushPath);
            QString subfix = fileInfo.suffix();
            dst = dstPath + QString("%1.%2").arg(keyNumber).arg(subfix);
            if(pushPath.compare(oldpath, Qt::CaseSensitive) != 0)
            {
                //IDE_TRACE_STR(pushPath);
                // IDE_TRACE_STR(dst);
                DelFile(oldpath);
                TCopyFile(pushPath, dst);
                //convertAndTransmit(pushPath, dst, UI_VIDEO);
                rcParser->SetRc(mProjectDir, dst, HVBoth);
                //IDE_TRACE_STR(rcParser->mFile[Vertical]);
                //IDE_TRACE_STR(rcParser->mFile[Horizontal]);
            }
        }
    }


    QHash<int, UiRcParser*>  rc =  rclist->mRCs;
    QList<int> rckeys = rc.keys();
    for(int i = 0; i<rckeys.count(); ++i)
    {
        if(rckeys.at(i) >= list.size() + rclist->mCurKey)
        {
            rclist->DelRc(rckeys.at(i));
        }
    }

    return true;
}

bool UiEditor::pushTextResource(QStringList list)
{
    if(list.isEmpty())
    {
        return false;
    }
    if(!mUiList.contains(UI_TEXT))
    {
        return false;
    }
    UiRcListParser *rclist = mUiList.value(UI_TEXT)->mComResource;
    if(!rclist)
    {
        preperUIElement(UI_TEXT, "Resource");
        rclist = mUiList.value(UI_TEXT)->mComResource;
        if(!rclist)
        {
            return false;
        }
    }

    UiRcParser * rcParser;
    QString pushPath;
    QString oldpath;
    QString dst;
    QString dstPath = mProjectDir+"Text/Resource/Image/";
    rcParser = rclist->AddRc(rclist->mCurKey, mUiManager, true);
    if(rcParser){
        pushPath = list.at(0);
        pushPath = shiftFilePrefix(pushPath);
        oldpath = rcParser->mFile[Horizontal];
        dst = dstPath +QString("HV%1.png").arg(rclist->mCurKey);
        if(QFile::exists(pushPath))
        {
            if(pushPath.compare(oldpath, Qt::CaseSensitive) != 0)
            {
                DelFile(oldpath);
                convertAndTransmit(pushPath, dst, UI_TEXT, true);
                rcParser->SetRc(mProjectDir, dst, HVBoth);
            }
        }
    }
    return true;
}

bool UiEditor::pushCallBtnResource(QStringList list)
{
    if(list.size() != 5)
    {
        return false;
    }

    QString temp;

    QString pathBack;
    QString pathPress;
    QString pathRelease;
    int btnStatus;
    float btnSpacing;

    bool flag = false;
    pathBack = shiftFilePrefix(list.at(0));
    pathPress = shiftFilePrefix(list.at(1));
    pathRelease = shiftFilePrefix(list.at(2));
    temp = list.at(3);
    btnStatus = temp.toInt(&flag);
    if(!flag)
    {
        btnStatus = 3;
    }
    temp = list.at(4);
    btnSpacing = temp.toFloat(&flag);
    if(!flag)
    {
        btnSpacing = 0;
    }
    QRect rect;
    if(!mUiList.contains(UI_CALLBTN))
    {
        return false;
    }
    UiRectListParser* rectList = mUiList.value(UI_CALLBTN)->mComRect;
    if(rectList)
    {
        rect = rectList->GetRect(m_layout, Horizontal);
    }
    else
    {
        return false;
    }

    UiRcListParser *rclist = mUiList.value(UI_CALLBTN)->mComResource;
    if(!rclist)
    {
        preperUIElement(UI_CALLBTN, "Resource");
        rclist = mUiList.value(UI_CALLBTN)->mComResource;
        if(!rclist)
        {
            return false;
        }
    }

    UiRcParser * rcParser;
    QString oldpath;
    QString dst;
    QString dstPath = mProjectDir+"CallBtn/Resource/Image/";
    //IDE_TRACE();
    rcParser = rclist->AddRc(rclist->mCurKey, mUiManager, true);
    if(rcParser)
    {
        oldpath = rcParser->mFile[Horizontal];
        dst = dstPath +QString("%1.png").arg(rclist->mCurKey);
        if(QFile::exists(pathBack))
        {
            if(pathBack.compare(oldpath, Qt::CaseSensitive) != 0)
            {
                DelFile(oldpath);
                convertAndTransmit(pathBack, dst, UI_CALLBTN);
                rcParser->SetRc(mProjectDir, dst, HVBoth);
            }
        }
        if(QFile::exists(pathPress)&&QFile::exists(pathRelease))
        {
            createSubCallBtn(btnStatus, btnSpacing, pathPress, pathRelease);
        }
    }
    return true;
}

void UiEditor::createSubCallBtn(int btnStatu, float spacing, QString press, QString release)
{
    if(!mUiList.contains(UI_CALLBTN))
    {
        return;
    }
    if(mUiManager->GetItemElement(mUiList.value(UI_CALLBTN)->mElement, "/SubUi"))
    {
        mUiManager->DeleteItemElement(mUiManager->itemElement);
    }
    UiRectListParser *rectlist = mUiList.value(UI_CALLBTN)->mComRect;
    QRect rect = rectlist->GetRect(m_layout, Horizontal);
    QDomElement subUi, upDom, downDom, tempDom, tempSubDom;
    subUi = mUiManager->createElement("SubUi");
    QString themeDirPath = "CallBtn/Resource/Image/";
    QString dstPath = mProjectDir+"CallBtn/Resource/Image/";
    switch (btnStatu)
    {
    case 1:
    {
        QRect rectUp(0, 0, rect.width(), rect.height());
        upDom = mUiManager->createElement("KeyUp");
        tempDom = mUiManager->createElement("Rect");
        tempSubDom = mUiManager->createElement("Layout"+QString("%1").arg(m_layout));
        tempSubDom.setAttribute("HV", GetStringFromRect(rectUp));
        tempDom.appendChild(tempSubDom);
        upDom.appendChild(tempDom);

        tempDom = mUiManager->createElement("Resource");
        tempSubDom = mUiManager->createElement("RC0");
        tempSubDom.setAttribute("HV", themeDirPath +"RC0.png");
        tempDom.appendChild(tempSubDom);
        tempSubDom = mUiManager->createElement("RC1");
        tempSubDom.setAttribute("HV", themeDirPath +"RC1.png");
        tempDom.appendChild(tempSubDom);
        upDom.appendChild(tempDom);
        subUi.appendChild(upDom);
        convertImageTo(press, dstPath+"RC0.png", QSize(rectUp.width(), rectUp.height()));
        convertImageTo(release, dstPath+"RC1.png", QSize(rectUp.width(), rectUp.height()));
    }
        break;
    case 2:
    {
        QRect rectDn(0, 0, rect.width(), rect.height());
        downDom = mUiManager->createElement("KeyDn");
        tempDom = mUiManager->createElement("Rect");
        tempSubDom = mUiManager->createElement("Layout"+QString("%1").arg(m_layout));
        tempSubDom.setAttribute("HV", GetStringFromRect(rectDn));
        tempDom.appendChild(tempSubDom);
        downDom.appendChild(tempDom);

        tempDom = mUiManager->createElement("Resource");
        tempSubDom = mUiManager->createElement("RC0");
        tempSubDom.setAttribute("HV", themeDirPath +"RC0.png");
        tempDom.appendChild(tempSubDom);
        tempSubDom = mUiManager->createElement("RC1");
        tempSubDom.setAttribute("HV", themeDirPath +"RC1.png");
        tempDom.appendChild(tempSubDom);
        downDom.appendChild(tempDom);
        subUi.appendChild(downDom);
        convertImageTo(press, dstPath+"RC0.png", QSize(rectDn.width(), rectDn.height()));
        convertImageTo(release, dstPath+"RC1.png", QSize(rectDn.width(), rectDn.height()));
    }
        break;
    case 3:
    {
        if(spacing>1||spacing<0)
        {
            spacing = 0;
        }

        float rate = (1-spacing)/2;
        QRect rectUp(0, 0, rect.width(), rect.height()*rate);
        QRect rectDn(0, rect.width()*((1-spacing)/2), rect.width(), rect.height()*rate);

        upDom = mUiManager->createElement("KeyUp");
        tempDom = mUiManager->createElement("Rect");
        tempSubDom = mUiManager->createElement("Layout"+QString("%1").arg(m_layout));
        tempSubDom.setAttribute("HV", GetStringFromRect(rectUp));
        tempDom.appendChild(tempSubDom);
        upDom.appendChild(tempDom);
        tempDom = mUiManager->createElement("Resource");
        tempSubDom = mUiManager->createElement("RC0");
        tempSubDom.setAttribute("HV", themeDirPath +"RC0.png");
        tempDom.appendChild(tempSubDom);
        tempSubDom = mUiManager->createElement("RC1");
        tempSubDom.setAttribute("HV", themeDirPath +"RC1.png");
        tempDom.appendChild(tempSubDom);
        upDom.appendChild(tempDom);

        downDom = mUiManager->createElement("KeyDn");
        tempDom = mUiManager->createElement("Rect");
        tempSubDom = mUiManager->createElement("Layout"+QString("%1").arg(m_layout));
        tempSubDom.setAttribute("HV", GetStringFromRect(rectDn));
        tempDom.appendChild(tempSubDom);
        downDom.appendChild(tempDom);
        tempDom = mUiManager->createElement("Resource");
        tempSubDom = mUiManager->createElement("RC0");
        tempSubDom.setAttribute("HV", themeDirPath +"RC0.png");
        tempDom.appendChild(tempSubDom);
        tempSubDom = mUiManager->createElement("RC1");
        tempSubDom.setAttribute("HV", themeDirPath +"RC1.png");
        tempDom.appendChild(tempSubDom);
        downDom.appendChild(tempDom);

        subUi.appendChild(upDom);
        subUi.appendChild(downDom);
        convertImageTo(press, dstPath+"RC0.png", QSize(rectUp.width(), rectUp.height()));
        convertImageTo(release, dstPath+"RC1.png", QSize(rectUp.width(), rectUp.height()));
    }
        break;
    default:
        break;
    }
    mUiList.value(UI_CALLBTN)->mElement.appendChild(subUi);
}

bool UiEditor::pushKeyBoardResource(QStringList list)
{
    QString resBasePath;
    if(list.isEmpty())
    {
        return false;
    }
    if(list.size() != 15)
    {
        IDE_TRACE();
        return false;
    }
    resBasePath = shiftFilePrefix(list.at(0));
    if(!mUiList.contains(UI_KEYBOARDBTN))
    {
        return false;
    }
    QRect rect;
    UiRectListParser* rectList = mUiList.value(UI_KEYBOARDBTN)->mComRect;
    if(rectList)
    {
        rect = rectList->GetRect(m_layout, Horizontal);
    }
    else
    {
        return false;
    }

    bool flag = false;
    if(mUiManager->GetItemElement(mUiList.value(UI_KEYBOARDBTN)->mElement, "/SubUi"))
    {
        mUiManager->DeleteItemElement(mUiManager->itemElement);
    }

    QDomElement subUi = mUiManager->createElement("SubUi");
    mUiList.value(UI_KEYBOARDBTN)->mElement.appendChild(subUi);
    if(subUi.isNull())
    {
        return false;
    }
    int index = 0;
    int keyColume = 3;
    int keyRow = 4;
    int keyWidth = rect.width()/keyColume;
    int keyHeight = rect.height()/keyRow;
    QRect keyRect;

    QString keyInfo = list.at(1);
    QStringList keyInfolist = keyInfo.split(":");
    keyInfo = keyInfolist.at(0);
    index = keyInfo.toInt(&flag);
    keyRect = QRect(0, (rect.height()- keyHeight), (rect.width()/2), keyHeight);
    createKeyBtnKey(subUi, index, resBasePath+keyInfolist.at(1), resBasePath+keyInfolist.at(2), keyRect);

    keyInfo = list.at(2);
    keyInfolist = keyInfo.split(":");
    keyInfo = keyInfolist.at(0);
    index = keyInfo.toInt(&flag);
    keyRect = QRect((rect.width()/2), (rect.height()-keyHeight), (rect.width()/2), keyHeight);
    createKeyBtnKey(subUi, index, resBasePath+keyInfolist.at(1), resBasePath+keyInfolist.at(2), keyRect);

    for(int i= 0;i < keyRow; ++i)
    {
        for(int j= 0;j < keyColume; ++j)
        {
            keyInfo = list.at(i*keyColume + j + 3);
            keyInfolist = keyInfo.split(":");
            if(keyInfolist.size() != 3)
            {
                continue;
            }
            keyInfo = keyInfolist.at(0);
            index = keyInfo.toInt(&flag);
            keyRect = QRect(j*keyWidth, i*keyHeight, keyWidth, keyHeight);

            createKeyBtnKey(subUi, index, resBasePath+keyInfolist.at(1), resBasePath+keyInfolist.at(2), keyRect);
        }
    }
    return true;
}

void UiEditor::createKeyBtnKey(QDomElement subUi, int keyIndex, QString fImg, QString bImg, QRect rect)
{
    QDomElement keyDom, tempDom, tempSubDom;
    QString themeDirPath = "KeyboardBtn/Resource/Image/";
    QString dstPath = mProjectDir+themeDirPath+QString("%1/").arg(keyIndex);
    CreatPath(dstPath);

    keyDom = mUiManager->createElement("Key"+QString("%1").arg(keyIndex));
    tempDom = mUiManager->createElement("Rect");
    tempSubDom = mUiManager->createElement("Layout"+m_layout);
    tempSubDom.setAttribute("HV", GetStringFromRect(rect));
    tempDom.appendChild(tempSubDom);
    keyDom.appendChild(tempDom);

    tempDom = mUiManager->createElement("Resource");
    tempSubDom = mUiManager->createElement("RC0");
    tempSubDom.setAttribute("HV", themeDirPath +"0.png");
    tempDom.appendChild(tempSubDom);
    tempSubDom = mUiManager->createElement("RC1");
    tempSubDom.setAttribute("HV", themeDirPath +"1.png");
    tempDom.appendChild(tempSubDom);
    keyDom.appendChild(tempDom);
    subUi.appendChild(keyDom);
    if(fImg.compare((dstPath+"0.png"), Qt::CaseSensitive) != 0)
    {
        convertImageTo(fImg, dstPath+"0.png", QSize(rect.width(), rect.height()));
    }
    if(bImg.compare((dstPath+"1.png"), Qt::CaseSensitive) != 0)
    {
        convertImageTo(bImg, dstPath+"1.png", QSize(rect.width(), rect.height()));
    }
}

bool UiEditor::pushClockResource(QStringList list)
{
    return true;
}

bool UiEditor::createDateArea(QRect dateRect, QStringList list, QString dateStyle)
{
    QDomElement dateDom, tempDom, tempSubDom;
    QString resBasePath;
    if(list.isEmpty())
    {
        return false;
    }
    if(list.size() != 12)
    {
        return false;
    }

    resBasePath = shiftFilePrefix(list.at(0));
    QRect rect;
    if(!mUiList.contains(UI_CLOCK))
    {
        return false;
    }
    UiRectListParser* rectList = mUiList.value(UI_CLOCK)->mComRect;
    if(rectList)
    {
        rect = rectList->GetRect(m_layout, Horizontal);
    }
    else
    {
        return false;
    }
    int dateImgW, dateImgH;
    dateImgW = rect.width()/9;
    dateImgH = rect.height();
    QDomElement subUi;
    if(mUiManager->GetItemElement(mUiList.value(UI_CLOCK)->mElement, "/SubUi"))
    {
        if(mUiManager->GetItemElement(mUiManager->itemElement, "/Date"))
        {
            //IDE_TRACE();
            mUiManager->DeleteItemElement(mUiManager->itemElement);
        }
    }
    else
    {
        IDE_TRACE();
        subUi = mUiManager->createElement("SubUi");
        //tempDom =  mUiManager->createElement("Date");
        //subUi.appendChild(tempDom);
        mUiList.value(UI_CLOCK)->mElement.appendChild(subUi);
    }

    if(mUiManager->GetItemElement(mUiList.value(UI_CLOCK)->mElement, "/SubUi"))
    {
        subUi = mUiManager->itemElement;
    }
    if(subUi.isNull())
    {
        return false;
    }

    QString themeDirPath = "Clock/Resource/Image/";
    QString dstPath = mProjectDir+themeDirPath;
    CreatPath(dstPath);

    QString tempStr, baseName, sRes, dRes;

    dateDom = mUiManager->createElement("Date");
    dateDom.setAttribute("Format", dateStyle);
    tempDom = mUiManager->createElement("Rect");
    tempSubDom = mUiManager->createElement("Layout"+QString("%1").arg(m_layout));
    tempSubDom.setAttribute("HV", GetStringFromRect(dateRect));
    IDE_TRACE_STR(GetStringFromRect(dateRect));
    tempDom.appendChild(tempSubDom);
    dateDom.appendChild(tempDom);
    tempDom = mUiManager->createElement("Resource");

    for(int i = 1; i< 11; ++i)
    {
        tempStr = list.at(i);
        if(tempStr.isEmpty())
        {
            continue;
        }
        baseName = tempStr.mid(0, tempStr.lastIndexOf("."));
        tempSubDom = mUiManager->createElement("RC"+QString("%1").arg(i - 1));
        tempSubDom.setAttribute("HV", "Clock/Resource/Image/"+tempStr);
        tempDom.appendChild(tempSubDom);
        sRes = resBasePath + tempStr;
        dRes = dstPath + tempStr;
        if(sRes.compare(dRes, Qt::CaseSensitive) != 0)
        {
            convertImageTo(sRes, dRes, QSize(dateImgW*2, dateImgH));
        }
    }

    tempStr = list.at(11);
    if(!tempStr.isEmpty())
    {
        baseName = tempStr.mid(0, tempStr.lastIndexOf("."));
        tempSubDom = mUiManager->createElement("RC56");
        tempSubDom.setAttribute("HV", "Clock/Resource/Image/"+tempStr);
        tempDom.appendChild(tempSubDom);
        sRes = resBasePath + tempStr;
        dRes = dstPath + tempStr;
        if(sRes.compare(dRes, Qt::CaseSensitive)!= 0)
        {
            convertImageTo(sRes, dRes, QSize(dateImgW, dateImgH));
        }
    }

    dateDom.appendChild(tempDom);
    subUi.appendChild(dateDom);
    return true;
}

bool UiEditor::createTimeArea(QRect timeRect, QStringList list, bool withSecond)
{
    QDomElement dateDom, tempDom, tempSubDom;
    QString resBasePath;
    if(list.isEmpty())
    {
        return false;
    }
    if(list.size()!=12)
    {
        return false;
    }
    resBasePath = shiftFilePrefix(list.at(0));
    QRect rect;
    if(!mUiList.contains(UI_CLOCK))
    {
        return false;
    }
    UiRectListParser* rectList = mUiList.value(UI_CLOCK)->mComRect;
    if(rectList)
    {
        rect = rectList->GetRect(m_layout, Horizontal);
    }
    else
    {
        return false;
    }
    int dateImgW, dateImgH;
    if(withSecond)
    {
        dateImgW = rect.width()/14;
        dateImgH = rect.height();
    }
    else
    {
        dateImgW = rect.width()/9;
        dateImgH = rect.height();
    }
    QDomElement subUi;
    if(mUiManager->GetItemElement(mUiList.value(UI_CLOCK)->mElement, "/SubUi"))
    {
        if(mUiManager->GetItemElement(mUiList.value(UI_CLOCK)->mElement, "/SubUi/Time"))
        {
            mUiManager->DeleteItemElement(mUiManager->itemElement);
        }
    }
    else
    {
        subUi = mUiManager->createElement("SubUi");
        tempDom =  mUiManager->createElement("Date");
        subUi.appendChild(tempDom);
        mUiList.value(UI_CLOCK)->mElement.appendChild(subUi);
    }

    if(mUiManager->GetItemElement(mUiList.value(UI_CLOCK)->mElement, "/SubUi"))
    {
        subUi = mUiManager->itemElement;
    }

    if(subUi.isNull())
    {
        return false;
    }

    QString themeDirPath = "Clock/Resource/Image/";
    QString dstPath = mProjectDir+themeDirPath;
    CreatPath(dstPath);

    QString tempStr, baseName, sRes, dRes;

    resBasePath = shiftFilePrefix(list.at(0));
    dateDom = mUiManager->createElement("Time");
    if(withSecond)
    {
        dateDom.setAttribute("WithSecond", "Yes");
    }
    else
    {
        dateDom.setAttribute("WithSecond", "No");
    }
    tempDom = mUiManager->createElement("Rect");
    tempSubDom = mUiManager->createElement("Layout"+QString("%1").arg(m_layout));
    tempSubDom.setAttribute("HV", GetStringFromRect(timeRect));
    tempDom.appendChild(tempSubDom);
    dateDom.appendChild(tempDom);

    tempDom = mUiManager->createElement("Resource");
    for(int i = 1; i< 11; ++i)
    {
        tempStr = list.at(i);
        if(tempStr.isEmpty())
        {
            continue;
        }
        baseName = tempStr.mid(0, tempStr.lastIndexOf("."));
        tempSubDom = mUiManager->createElement("RC"+QString("%1").arg(i-1));
        tempSubDom.setAttribute("HV", "Clock/Resource/Image/"+tempStr);
        tempDom.appendChild(tempSubDom);
        sRes = resBasePath + tempStr;
        dRes = dstPath + tempStr;
        if(sRes.compare(dRes, Qt::CaseSensitive) != 0)
        {
            convertImageTo(sRes, dRes, QSize(dateImgW*2, dateImgH));
        }
    }

    tempStr = list.at(11);
    if(!tempStr.isEmpty())
    {
        baseName = tempStr.mid(0, tempStr.lastIndexOf("."));
        tempSubDom = mUiManager->createElement("RC56");
        tempSubDom.setAttribute("HV", "Clock/Resource/Image/"+tempStr);
        tempDom.appendChild(tempSubDom);
        sRes = resBasePath + tempStr;
        dRes = dstPath + tempStr;
        if(sRes.compare(dRes, Qt::CaseSensitive) != 0)
        {
            convertImageTo(sRes, dRes, QSize(dateImgW, dateImgH));
        }
    }

    dateDom.appendChild(tempDom);
    subUi.appendChild(dateDom);
}

bool UiEditor::pushFlrBtnResource(QStringList list)
{
    return true;
}

bool UiEditor::pushFlrBtnResource(QSize size, bool isHori, QStringList list)
{
    QString resBasePath;
    if(list.isEmpty())
    {
        return false;
    }
    if(list.size()!= size.width() * size.height() + 1)
    {
        IDE_TRACE();
        return false;
    }

    if(!mUiList.contains(UI_FLRBTN))
    {
        return false;
    }
    resBasePath = shiftFilePrefix(list.at(0));
    QRect rect;
    UiRectListParser* rectList = mUiList.value(UI_FLRBTN)->mComRect;
    if(rectList)
    {
        rect = rectList->GetRect(m_layout, Horizontal);
    }
    else
    {
        return false;
    }

    bool flag = false;

    if(mUiManager->GetItemElement(mUiList.value(UI_FLRBTN)->mElement, "/SubUi"))
    {
        mUiManager->DeleteItemElement(mUiManager->itemElement);
    }
    QDomElement subUi = mUiManager->createElement("SubUi");
    mUiList.value(UI_FLRBTN)->mElement.appendChild(subUi);
    if(subUi.isNull())
    {
        return false;
    }

    int keyColume = size.height();
    int keyRow = size.width();
    int keyWidth = rect.width()/keyColume;
    int keyHeight = rect.height()/keyRow;
    QRect keyRect;

    for(int i= 0;i < keyRow; i++)
    {
        for(int j= 0;j < keyColume; j++)
        {
            QString keyInfo = list.at(i*keyColume+j+1);
            QStringList keyInfolist = keyInfo.split(":");

            if(keyInfolist.size() != 3)
            {
                continue;
            }
            keyInfo = keyInfolist.at(0);

            int index = keyInfo.toInt(&flag);

            if(isHori)
            {
                keyRect = QRect(j*keyWidth, rect.height() - i*keyHeight , keyWidth, keyHeight);
            }
            else
            {
                keyRect = QRect(j*keyWidth, i*keyHeight, keyWidth, keyHeight);
            }

            createFlrBtnKey(subUi, index, resBasePath+keyInfolist.at(1), resBasePath+keyInfolist.at(2), keyRect);
        }
    }
}

void UiEditor::createFlrBtnKey(QDomElement subUi, int keyIndex, QString fImg, QString bImg, QRect rect)
{
    QDomElement keyDom, tempDom, tempSubDom;
    QString themeDirPath = "FlrBtn/Resource/Image/";
    QString dstPath = mProjectDir+themeDirPath+QString("%1/").arg(keyIndex);
    CreatPath(dstPath);

    keyDom = mUiManager->createElement("Key"+QString("%1").arg(keyIndex));
    tempDom = mUiManager->createElement("Rect");
    tempSubDom = mUiManager->createElement("Layout"+QString("%1").arg(m_layout));
    tempSubDom.setAttribute("HV", GetStringFromRect(rect));
    tempDom.appendChild(tempSubDom);
    keyDom.appendChild(tempDom);

    tempDom = mUiManager->createElement("Resource");
    tempSubDom = mUiManager->createElement("RC0");
    tempSubDom.setAttribute("HV", themeDirPath +"0.png");
    tempDom.appendChild(tempSubDom);
    tempSubDom = mUiManager->createElement("RC1");
    tempSubDom.setAttribute("HV", themeDirPath +"1.png");
    tempDom.appendChild(tempSubDom);
    keyDom.appendChild(tempDom);
    subUi.appendChild(keyDom);
    if(fImg.compare((dstPath+"0.png"), Qt::CaseSensitive) != 0)
    {
        convertImageTo(fImg, dstPath+"0.png", QSize(rect.width(), rect.height()));
    }
    if(bImg.compare((dstPath+"1.png"), Qt::CaseSensitive) != 0)
    {
        convertImageTo(bImg, dstPath+"1.png", QSize(rect.width(), rect.height()));
    }
}

bool UiEditor::pushExeResource(QString exportPath, QString filePath)
{
    filePath = shiftFilePrefix(filePath);
    if(exportPath.isEmpty())
    {
        return false;
    }
    if(QFile::exists(filePath))
    {
        QFileInfo info(filePath);
        QString tmpPath;
        exportPath = shiftFilePrefix(exportPath);
        if(!exportPath.endsWith('/'))
        {
            exportPath.append('/');
        }

        tmpPath = exportPath.append("EXE");
        if(!tmpPath.endsWith('/'))
        {
            tmpPath.append('/');
        }

        tmpPath.append(info.fileName());
        //IDE_TRACE_STR(tmpPath);
        //IDE_TRACE_STR(filePath);
        TCopyFile(filePath, tmpPath);
    }
}

QString UiEditor::shiftFilePrefix(QString filePath)
{
    if(filePath.startsWith(qmlFilePrefix))
    {
        return filePath.mid(qmlFilePrefix.length());
    }
    else
    {
        return filePath;
    }
}

void UiEditor::savePara()
{
    if(!mUiManager->isNull())
    {
        mUiManager->SaveUI();
    }
}

bool UiEditor::publishFile(QString exportPath)
{
    QString tmpPath;
    if(exportPath.isEmpty())
    {
        return false;
    }

    exportPath = shiftFilePrefix(exportPath);
    if(!exportPath.endsWith('/'))
    {
        exportPath.append('/');
    }
    tmpPath = exportPath.append("Theme");
    if(!tmpPath.endsWith('/'))
    {
        tmpPath.append('/');
    }

    QDir themeDir(tmpPath);
    if(themeDir.exists())
    {
        themeDir.removeRecursively();
    }
    themeDir.mkdir("Theme");
    //QList<UI_TYPE> uilist = mUiList.keys();
    //QString sDir, dDir;
    //    for(int i = 0; i < uilist.size(); ++i)
    //    {
    //        sDir = mProjectDir + GetUiName(uilist.at(i));
    //        dDir = tmpPath+GetUiName(uilist.at(i));
    //        CreatPath(dDir);
    //        QDir srcDir(sDir);
    //        QDir dstDir(dDir);
    //        qCopyDirectory(srcDir, dstDir, true);
    //    }
    //sDir = mProjectDir;
    //dDir = tmpPath;
    QDir srcDir(mProjectDir);
    QDir dstDir(tmpPath);
    qCopyDirectory(srcDir, dstDir, true);

    return true;
}

bool UiEditor::qCopyDirectory(const QDir& fromDir, const QDir& toDir, bool bCoverIfFileExists)
{
    QDir formDir_ = fromDir;
    QDir toDir_ = toDir;

    if(!toDir_.exists())
    {
        if(!toDir_.mkdir(toDir.absolutePath()))
        {
            return false;
        }
    }

    QFileInfoList fileInfoList = formDir_.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
        {
            continue;
        }

        //拷贝子目录
        if(fileInfo.isDir())
        {
            //递归调用拷贝
            if(!qCopyDirectory(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName()), true))
            {
                return false;
            }
        }
        //拷贝子文件
        else
        {
            if(bCoverIfFileExists && toDir_.exists(fileInfo.fileName()))
            {
                toDir_.remove(fileInfo.fileName());
            }
            if(!QFile::copy(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName())))
            {
                return false;
            }
        }
    }
    return true;
}
bool UiEditor::removeElement(QString comName)
{
    return delCom(GetUiType(comName));
}

void UiEditor::orderResouce()
{
    if(!mUiList.contains(UI_ARROW))
    {
        IDE_TRACE();
        DeleteDirectory(mProjectDir+"Arrow/");
    }
    else
    {
        //IDE_TRACE();
        orderArrow();
    }

    if(!mUiList.contains(UI_FUNCTION))
    {
        //IDE_TRACE();
        DeleteDirectory(mProjectDir+"Function/");
    }
    else
    {
        //IDE_TRACE();
        orderFunction();
    }

    if(!mUiList.contains(UI_PICTURE))
    {
        //IDE_TRACE();
        DeleteDirectory(mProjectDir+"Picture/");
    }
    else
    {
        //IDE_TRACE();
        orderPicture();
    }

    if(!mUiList.contains(UI_MEDIA))
    {
        //IDE_TRACE();
        DeleteDirectory(mProjectDir+"Media/");
    }
    else
    {
        //IDE_TRACE();
        orderMedia();
    }

    if(!mUiList.contains(UI_VIDEO))
    {
        //IDE_TRACE();
        //IDE_TRACE_STR(mProjectDir+"Video/");
        //DelFolder(mProjectDir+"Video/");
        DeleteDirectory(mProjectDir+"Video/");
    }
    else
    {
        //
        //IDE_TRACE();
        orderVideo();
    }

}

void UiEditor::orderMedia()
{
    QStringList rcs;
    UiRcListParser *rclist = mUiList.value(UI_MEDIA)->mComResource;
    if(rclist)
    {
        QList<int> rckeys =  rclist->mRCs.keys();
        int count = rckeys.count();
        qSort(rckeys.begin(), rckeys.end());
        UiRcParser * rcParser;
        for(int i= 0; i < count; ++i)
        {
            int rckey = rckeys.at(i);
            rcParser = rclist->mRCs.value(rckey);
            if(rcParser)
            {
                QString path = rcParser->mFile[Vertical];
                QFile file(path);
                if(file.exists())
                {
                    rcs << (QFileInfo(file)).fileName();
                }
            }
        }
    }
    QDir formDir(mProjectDir+"Media/Resource/Audio/");
    if(!formDir.exists())
    {
        return;
    }

    QFileInfoList fileInfoList = formDir.entryInfoList();
    bool delflag = true;
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        //拷贝子目录
        //IDE_TRACE_STR(fileInfo.fileName());
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
        {
            continue;
        }
        else if(fileInfo.isDir())
        {
            DeleteDirectory(fileInfo.filePath());
        }
        else
        {
            delflag = true;
            foreach(QString rcItem, rcs)
            {
                if(!rcItem.compare(fileInfo.fileName()))
                {
                    delflag = false;
                    break;
                }
            }
            if(delflag)
            {
                //fileInfo.fileName()
                IDE_TRACE_STR(fileInfo.absoluteFilePath());
                DelFile(fileInfo.absoluteFilePath());
            }
        }
    }
}

void UiEditor::orderArrow()
{
    QStringList rcs;
    UiRcListParser *rclist = mUiList.value(UI_ARROW)->mComResource;
    if(rclist)
    {

        QList<int> rckeys =  rclist->mRCs.keys();
        int count = rckeys.count();
        qSort(rckeys.begin(), rckeys.end());
        UiRcParser * rcParser;
        for(int i= 0;i < count; ++i)
        {
            int rckey = rckeys.at(i);
            rcParser = rclist->mRCs.value(rckey);
            if(rcParser)
            {
                QString path = rcParser->mFile[Vertical];
                QFile file(path);
                if(file.exists())
                {
                    rcs << (QFileInfo(file)).fileName();
                }
            }
        }
    }
    QDir formDir(mProjectDir+"Arrow/Resource/Image/");
    if(!formDir.exists())
    {
        return;
    }
    QFileInfoList fileInfoList = formDir.entryInfoList();
    bool delflag = true;
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        //拷贝子目录
        //IDE_TRACE_STR(fileInfo.fileName());
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
        {
            continue;
        }
        else if(fileInfo.isDir())
        {
            DeleteDirectory(fileInfo.filePath());
        }
        else
        {
            delflag = true;
            foreach(QString rcItem, rcs)
            {
                if(!rcItem.compare(fileInfo.fileName()))
                {
                    delflag = false;
                    break;
                }
            }
            if(delflag)
            {
                DelFile(fileInfo.absoluteFilePath());
            }
        }
    }

}

void UiEditor::orderFunction()
{
    QStringList rcs;
    UiRcListParser *rclist = mUiList.value(UI_FUNCTION)->mComResource;
    if(rclist)
    {
        QList<int> rckeys =  rclist->mRCs.keys();
        int count = rckeys.count();
        qSort(rckeys.begin(), rckeys.end());
        UiRcParser * rcParser;
        for(int i= 0;i < count; ++i)
        {
            int rckey = rckeys.at(i);
            rcParser = rclist->mRCs.value(rckey);
            if(rcParser)
            {
                QString path = rcParser->mFile[Vertical];
                QFile file(path);
                if(file.exists())
                {
                    rcs << (QFileInfo(file)).fileName();
                }
            }
        }
    }
    QDir formDir(mProjectDir+"Function/Resource/Image/");
    if(!formDir.exists())
    {
        return;
    }
    QFileInfoList fileInfoList = formDir.entryInfoList();
    bool delflag = true;
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        //拷贝子目录
        //IDE_TRACE_STR(fileInfo.fileName());
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
        {
            continue;
        }
        else if(fileInfo.isDir())
        {
            DeleteDirectory(fileInfo.filePath());
        }
        else
        {
            delflag = true;
            foreach(QString rcItem, rcs)
            {
                if(!rcItem.compare(fileInfo.fileName()))
                {
                    delflag = false;
                    break;
                }
            }
            if(delflag)
            {
                DelFile(fileInfo.absoluteFilePath());
            }
        }
    }
}

void UiEditor::orderPicture()
{
    QStringList rcs;
    UiRcListParser *rclist = mUiList.value(UI_PICTURE)->mComResource;
    if(rclist)
    {
        QList<int> rckeys =  rclist->mRCs.keys();
        int count = rckeys.count();
        qSort(rckeys.begin(), rckeys.end());
        UiRcParser * rcParser;
        for(int i= 0;i < count; ++i)
        {
            int rckey = rckeys.at(i);
            rcParser = rclist->mRCs.value(rckey);
            if(rcParser)
            {
                QString path = rcParser->mFile[Vertical];
                QFile file(path);
                if(file.exists())
                {
                    //IDE_TRACE_STR(file.fileName());
                    rcs << (QFileInfo(file)).fileName();
                }
            }
        }
        //qDebug()<<rcs;
    }
    QDir formDir(mProjectDir+"Picture/Resource/Image/");
    if(!formDir.exists())
    {
        return;
    }

    QFileInfoList fileInfoList = formDir.entryInfoList();
    bool delflag = true;
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        //拷贝子目录
        //IDE_TRACE_STR(fileInfo.fileName());
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
        {
            continue;
        }
        else if(fileInfo.isDir())
        {
            DeleteDirectory(fileInfo.filePath());
        }
        else
        {
            delflag = true;
            foreach(QString rcItem, rcs)
            {
                if(!rcItem.compare(fileInfo.fileName()))
                {
                    delflag = false;
                    break;
                }
            }
            if(delflag)
            {
                //fileInfo.fileName()
                //IDE_TRACE_STR(fileInfo.absoluteFilePath());
                DelFile(fileInfo.absoluteFilePath());
            }
        }
    }
}

void UiEditor::orderVideo()
{
    QStringList rcs;
    UiRcListParser *rclist = mUiList.value(UI_VIDEO)->mComResource;
    if(rclist)
    {
        QList<int> rckeys =  rclist->mRCs.keys();
        int count = rckeys.count();
        qSort(rckeys.begin(), rckeys.end());
        UiRcParser * rcParser;
        for(int i= 0;i < count; ++i)
        {
            int rckey = rckeys.at(i);
            rcParser = rclist->mRCs.value(rckey);
            if(rcParser)
            {
                QString path = rcParser->mFile[Vertical];
                //IDE_TRACE_STR(path);
                QFile file(path);
                if(file.exists())
                {
                    //IDE_TRACE_STR(file.fileName());
                    rcs << (QFileInfo(file)).fileName();
                }
            }
        }
    }

    QDir formDir(mProjectDir+"Video/Resource/Video/");
    if(!formDir.exists())
    {
        return;
    }

    QFileInfoList fileInfoList = formDir.entryInfoList();
    bool delflag = true;
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        //拷贝子目录
        //IDE_TRACE_STR(fileInfo.fileName());
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
        {
            continue;
        }
        else if(fileInfo.isDir())
        {
            DeleteDirectory(fileInfo.filePath());
        }
        else
        {
            delflag = true;
            foreach(QString rcItem, rcs){
                if(!rcItem.compare(fileInfo.fileName()))
                {
                    delflag = false;
                    break;
                }
            }
            if(delflag)
            {
                //fileInfo.fileName()
                //IDE_TRACE_STR(fileInfo.absoluteFilePath());
                DelFile(fileInfo.absoluteFilePath());
            }
        }
    }
}

QStringList UiEditor::pullProperty(QString comName)
{
    UI_TYPE tempType = GetUiType(comName);
    QStringList resourceList;
    switch(tempType)
    {
    case UI_DESKTOP:
        //resourceList = pullDesktopResource();
        break;
    case UI_FLOOR:
        resourceList = pullFloorProperty();
        break;
    case UI_ARROW:
        resourceList = pullArrowProperty();
        break;
    case UI_FUNCTION:
        //resourceList = pullFunctionResource();
        break;
    case UI_MEDIA:
        resourceList = pullMediaProperty();
        break;
    case UI_VIDEO:
        resourceList = pullVideoProperty();
        break;
    case UI_TEXT:
        resourceList = pullTextProperty();
        break;
    case UI_TITLE:
        resourceList = pullTitleProperty();
        break;
    case UI_CLOCK:
        resourceList = pullClockProperty();
        break;
    case UI_TIME:
        resourceList = pullTimeProperty();
        break;
    case UI_DATE:
        resourceList = pullDateProperty();
        break;
    case UI_FLRBTN:
        resourceList = pullFlrBtnProperty();
        break;
    case UI_CALLBTN:
        //resourceList = pullCallBtnResource();
        break;
    case UI_KEYBOARDBTN:
        //resourceList = pullKeyBoardResource();
        break;
    default:
        break;
    }
    return resourceList;
}

QStringList UiEditor::pullTimeProperty()
{
    QStringList list;
    QString tmpString;
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_TIME))
    {
        return list;
    }
    UIDom = mUiList.value(UI_TIME)->mElement;

    if(mUiManager->GetItemElement(UIDom, "/Operation/Run"))
    {
        tempDom = mUiManager->itemElement;

        tmpString = tempDom.attribute("Font");
        if(!tmpString.isEmpty())
        {
            list << "Font@" + tmpString;
        }

        tmpString = tempDom.attribute("PixelSize");
        if(!tmpString.isEmpty())
        {
            list << "PixelSize@" + tmpString;
        }

        tmpString = tempDom.attribute("Color");
        if(!tmpString.isEmpty())
        {
            list << "Color@" + tmpString;
        }

        tmpString = tempDom.attribute("Bold");
        if(!tmpString.isEmpty())
        {
            list << "Bold@" + tmpString;
        }

        tmpString = tempDom.attribute("Split");
        if(!tmpString.isEmpty())
        {
            list << "Split@" + tmpString;
        }
        tmpString = tempDom.attribute("Format");
        if(!tmpString.isEmpty())
        {
            list << "Format@" + tmpString;
        }
    }
    return list;
}

QStringList UiEditor::pullDateProperty()
{
    QStringList list;
    QString tmpString;
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_DATE))
    {
        return list;
    }
    UIDom = mUiList.value(UI_DATE)->mElement;

    if(mUiManager->GetItemElement(UIDom, "/Operation/Run"))
    {
        tempDom = mUiManager->itemElement;

        tmpString = tempDom.attribute("Font");
        if(!tmpString.isEmpty())
        {
            list << "Font@" + tmpString;
        }

        tmpString = tempDom.attribute("PixelSize");
        if(!tmpString.isEmpty())
        {
            list << "PixelSize@" + tmpString;
        }

        tmpString = tempDom.attribute("Color");
        if(!tmpString.isEmpty())
        {
            list << "Color@" + tmpString;
        }

        tmpString = tempDom.attribute("Bold");
        if(!tmpString.isEmpty())
        {
            list << "Bold@" + tmpString;
        }

        tmpString = tempDom.attribute("Split");
        if(!tmpString.isEmpty())
        {
            list << "Split@" + tmpString;
        }
        tmpString = tempDom.attribute("Format");
        if(!tmpString.isEmpty())
        {
            list << "Format@" + tmpString;
        }
    }
    return list;
}


QStringList UiEditor::pullDesktopProperty()
{
    QStringList list;
    return list;
}

QStringList UiEditor::pullFloorProperty()
{
    QStringList list;
    QString tmpString;
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_FLOOR))
    {
        return list;
    }
    UIDom = mUiList.value(UI_FLOOR)->mElement;

    if(mUiManager->GetItemElement(UIDom, "/Operation/Run"))
    {
        tempDom = mUiManager->itemElement;

        tmpString = tempDom.attribute("Font");
        if(!tmpString.isEmpty())
        {
            list << "Font:" + tmpString;
        }

        tmpString = tempDom.attribute("PixelSize");
        if(!tmpString.isEmpty())
        {
            list << "PixelSize:" + tmpString;
        }

        tmpString = tempDom.attribute("Color");
        if(!tmpString.isEmpty())
        {
            list << "Color:" + tmpString;
        }

        tmpString = tempDom.attribute("Bold");
        if(!tmpString.isEmpty())
        {
            list << "Bold:" + tmpString;
        }
    }
    return list;
}

QStringList UiEditor::pullArrowProperty()
{
    QStringList list;
    QString tmpString;
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_ARROW))
        return list;
    UIDom = mUiList.value(UI_ARROW)->mElement;

    if(mUiManager->GetItemElement(UIDom, "/Operation/Run"))
    {
        tempDom = mUiManager->itemElement;
        tmpString = tempDom.attribute("Speed");
        if(!tmpString.isEmpty())
        {
            list << "Speed:" + tmpString;
        }
    }
    return list;
}

QStringList UiEditor::pullFunctionProperty()
{
    QStringList list;
    return list;
}

QStringList UiEditor::pullMediaProperty()
{
    QStringList list;
    QString tmpString;
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_MEDIA))
    {
        return list;
    }
    UIDom = mUiList.value(UI_MEDIA)->mElement;

    if(mUiManager->GetItemElement(UIDom, "/Operation/Run"))
    {
        tempDom = mUiManager->itemElement;
        tmpString = tempDom.attribute("Volume");
        if(!tmpString.isEmpty())
        {
            list << "Volume:" + tmpString;
        }
    }
    return list;
}

QStringList UiEditor::pullVideoProperty()
{
    QStringList list;
    QString tmpString;
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_VIDEO))
    {
        return list;
    }
    UIDom = mUiList.value(UI_VIDEO)->mElement;

    if(mUiManager->GetItemElement(UIDom, "/Operation/Run"))
    {
        tempDom = mUiManager->itemElement;
        tmpString = tempDom.attribute("Volume");
        if(!tmpString.isEmpty())
        {
            list << "Volume:" + tmpString;
        }
    }
    return list;
}

QStringList UiEditor::pullTextProperty()
{
    QStringList list;
    QString tmpString;
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_TEXT))
    {
        return list;
    }
    UIDom = mUiList.value(UI_TEXT)->mElement;

    if(mUiManager->GetItemElement(UIDom, "/Operation/Run"))
    {
        tempDom = mUiManager->itemElement;

        tmpString = tempDom.text();
        if(!tmpString.isEmpty())
        {
            list << "Text:" + tmpString;
        }

        tmpString = tempDom.attribute("Font");
        if(!tmpString.isEmpty())
        {
            list << "Font:" + tmpString;
        }

        tmpString = tempDom.attribute("PixelSize");
        if(!tmpString.isEmpty())
        {
            list << "PixelSize:" + tmpString;
        }

        tmpString = tempDom.attribute("Color");
        if(!tmpString.isEmpty())
        {
            list << "Color:" + tmpString;
        }

        tmpString = tempDom.attribute("Bold");
        if(!tmpString.isEmpty())
        {
            list << "Bold:" + tmpString;
        }
    }
    return list;
}

QStringList UiEditor::pullTitleProperty()
{
    QStringList list;
    QString tmpString;
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_TITLE))
    {
        return list;
    }
    UIDom = mUiList.value(UI_TITLE)->mElement;

    if(mUiManager->GetItemElement(UIDom, "/Operation/Run"))
    {
        tempDom = mUiManager->itemElement;

        tmpString = tempDom.text();
        if(!tmpString.isEmpty())
        {
            list << "Text:" + tmpString;
        }
        tmpString = tempDom.attribute("Font");
        if(!tmpString.isEmpty())
        {
            list << "Font:" + tmpString;
        }

        tmpString = tempDom.attribute("PixelSize");
        if(!tmpString.isEmpty())
        {
            list << "PixelSize:" + tmpString;
        }

        tmpString = tempDom.attribute("Color");
        if(!tmpString.isEmpty())
        {
            list << "Color:" + tmpString;
        }

        tmpString = tempDom.attribute("Bold");
        if(!tmpString.isEmpty())
        {
            list << "Bold:" + tmpString;
        }
    }
    return list;
}


QStringList UiEditor::pullClockProperty()
{
    QStringList list;
    QString tmpString;
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_CLOCK))
    {
        return list;
    }
    UIDom = mUiList.value(UI_CLOCK)->mElement;

    if(mUiManager->GetItemElement(UIDom, "/SubUi/Date"))
    {
        tempDom = mUiManager->itemElement;
        tmpString = tempDom.attribute("Format");
        if(!tmpString.isEmpty())
        {
            list << "Format:" + tmpString;
        }
    }

    if(mUiManager->GetItemElement(UIDom, "/SubUi/Date/Rect/Layout"+QString("%1").arg(m_layout)))
    {
        tempDom = mUiManager->itemElement;
        tmpString = tempDom.attribute("HV");
        if(!tmpString.isEmpty())
        {
            list << "dRect:" + tmpString;
        }
    }

    if(mUiManager->GetItemElement(UIDom, "/SubUi/Time"))
    {
        tempDom = mUiManager->itemElement;
        tmpString = tempDom.attribute("WithSecond");
        if(!tmpString.isEmpty())
        {
            list << "WithSecond:" + tmpString;
        }
    }

    if(mUiManager->GetItemElement(mUiList.value(UI_CLOCK)->mElement, "/SubUi/Time/Rect/Layout"+QString("%1").arg(m_layout)))
    {
        tempDom = mUiManager->itemElement;
        tmpString = tempDom.attribute("HV");
        if(!tmpString.isEmpty())
        {
            list << "tRect:" + tmpString;
        }
    }

    return list;
}

QStringList UiEditor::pullFlrBtnProperty()
{
    QStringList list;
    QString tmpString;
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_FLRBTN))
    {
        return list;
    }
    UIDom = mUiList.value(UI_FLRBTN)->mElement;

    if(mUiManager->GetItemElement(UIDom, "/SubUi"))
    {
        tempDom = mUiManager->itemElement;
        tmpString = tempDom.attribute("Row");
        if(!tmpString.isEmpty())
        {
            list << "Row:" + tmpString;
        }
        tmpString = tempDom.attribute("Colume");
        if(!tmpString.isEmpty())
        {
            list << "Colume:" + tmpString;
        }
        tmpString = tempDom.attribute("StartFlr");
        if(!tmpString.isEmpty())
        {
            list << "StartFlr:" + tmpString;
        }
        tmpString = tempDom.attribute("InvaildFlr");
        if(!tmpString.isEmpty())
        {
            list << "InvaildFlr:" + tmpString;
        }
        tmpString = tempDom.attribute("Flow");
        if(!tmpString.isEmpty())
        {
            list << "Flow:" + tmpString;
        }
        tmpString = tempDom.attribute("Direction");
        if(!tmpString.isEmpty())
        {
            list << "Direction:" + tmpString;
        }
    }

    return list;
}

QStringList UiEditor::pullCallBtnProperty()
{
    QStringList list;
    return list;
}

QStringList UiEditor::pullKeyBoardProperty()
{
    QStringList list;
    return list;
}

void UiEditor::pushProperty(QString comName, QStringList list)
{
    UI_TYPE tempType = GetUiType(comName);
    switch(tempType)
    {
    case UI_DESKTOP:
        pushDesktopResource(list);
        break;
    case UI_FLOOR:
        pushFloorResource(list);
        break;
    case UI_ARROW:
        pushArrowResource(list);
        break;
    case UI_FUNCTION:
        pushFunctionResource(list);
        break;
    case UI_MEDIA:
        pushMediaResource(list);
        break;
    case UI_TEXT:
        pushTextResource(list);
        break;
    case UI_CLOCK:
        pushClockResource(list);
        break;
    case UI_TIME:
        //resourceList = pullClockProperty();
        break;
    case UI_DATE:
        //resourceList = pullClockProperty();
        break;
    case UI_FLRBTN:
        pushFlrBtnResource(list);
        break;
    case UI_CALLBTN:
        pushCallBtnResource(list);
        break;
    case UI_KEYBOARDBTN:
        pushKeyBoardResource(list);
        break;
    default:
        break;
    }
}

bool UiEditor::pushDesktopProperty(QStringList list)
{
    return true;
}

bool UiEditor::pushFloorProperty(QStringList list)
{
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_FLOOR))
    {
        return false;
    }
    QStringList templist;
    UIDom = mUiList.value(UI_FLOOR)->mElement;
    if(!mUiManager->GetItemElement(UIDom, "/Operation/Run"))
    {
        if(mUiManager->CreateElementPath(UIDom, "/Operation/Run/"))
        {
            tempDom = mUiManager->itemElement;
            for(int i = 0; i < list.size(); ++i)
            {
                QString temp = list.at(i);
                templist = temp.split(":");
                if(templist.size() != 2)
                {
                    continue;
                }
                tempDom.setAttribute(templist[0], templist[1]);
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        tempDom = mUiManager->itemElement;
        for(int i = 0;i < list.size();i++)
        {
            QString temp = list.at(i);
            templist = temp.split(":");
            if(templist.size() != 2)
            {
                continue;
            }
            tempDom.setAttribute(templist[0], templist[1]);
        }
    }
    //mUiManager->SaveUI();
    return true;
}

bool UiEditor::pushArrowProperty(QStringList list)
{
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_ARROW))
    {
        return false;
    }
    QStringList templist;
    UIDom = mUiList.value(UI_ARROW)->mElement;
    if(!mUiManager->GetItemElement(UIDom, "/Operation/Run"))
    {
        if(mUiManager->CreateElementPath(UIDom, "/Operation/Run/"))
        {
            tempDom = mUiManager->itemElement;
            for(int i = 0;i < list.size();i++)
            {
                QString temp = list.at(i);
                templist = temp.split(":");
                if(templist.size() != 2)
                {
                    continue;
                }
                tempDom.setAttribute(templist[0], templist[1]);
            }

        }
        else
        {
            return false;
        }
    }
    else
    {
        tempDom = mUiManager->itemElement;
        for(int i = 0; i < list.size(); ++i)
        {
            QString temp = list.at(i);
            templist = temp.split(":");
            if(templist.size() != 2)
            {
                continue;
            }
            tempDom.setAttribute(templist[0], templist[1]);
        }
    }
    return true;
}

bool UiEditor::pushFunctionProperty(QStringList list)
{
    return true;
}

bool UiEditor::pushMediaProperty(QStringList list)
{
    //qDebug()<<list;
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_MEDIA))
    {
        return false;
    }
    QStringList templist;
    UIDom = mUiList.value(UI_MEDIA)->mElement;
    if(!mUiManager->GetItemElement(UIDom, "/Operation/Run"))
    {
        if(mUiManager->CreateElementPath(UIDom, "/Operation/Run/"))
        {
            tempDom = mUiManager->itemElement;
            for(int i = 0; i < list.size(); ++i)
            {
                QString temp = list.at(i);
                templist = temp.split(":");
                if(templist.size() != 2)
                {
                    continue;
                }
                tempDom.setAttribute(templist[0], templist[1]);
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        tempDom = mUiManager->itemElement;
        for(int i = 0; i < list.size(); ++i)
        {
            QString temp = list.at(i);
            templist = temp.split(":");
            if(templist.size() != 2)
            {
                continue;
            }
            tempDom.setAttribute(templist[0], templist[1]);
        }
    }
    return true;
}

bool UiEditor::pushVideoProperty(QStringList list)
{
    //qDebug()<<list;
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_VIDEO))
    {
        return false;
    }
    QStringList templist;
    UIDom = mUiList.value(UI_VIDEO)->mElement;
    if(!mUiManager->GetItemElement(UIDom, "/Operation/Run"))
    {
        if(mUiManager->CreateElementPath(UIDom, "/Operation/Run/"))
        {
            tempDom = mUiManager->itemElement;
            for(int i = 0; i < list.size(); ++i)
            {
                QString temp = list.at(i);
                templist = temp.split(":");
                if(templist.size() != 2)
                {
                    continue;
                }
                tempDom.setAttribute(templist[0], templist[1]);
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        tempDom = mUiManager->itemElement;
        for(int i = 0; i < list.size(); ++i)
        {
            QString temp = list.at(i);
            templist = temp.split(":");
            if(templist.size() != 2)
            {
                continue;
            }
            tempDom.setAttribute(templist[0], templist[1]);
        }
    }
    return true;
}

bool UiEditor::pushTextProperty(QStringList list)
{
    //qDebug()<<list;
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_TEXT))
    {
        return false;
    }
    QStringList templist;
    UIDom = mUiList.value(UI_TEXT)->mElement;
    if(!mUiManager->GetItemElement(UIDom, "/Operation/Run"))
    {
        if(mUiManager->CreateElementPath(UIDom, "/Operation/Run/"))
        {
            tempDom = mUiManager->itemElement;
            for(int i = 0; i < list.size(); i++)
            {
                QString temp = list.at(i);
                templist = temp.split(":");
                if(templist.size() != 2)
                {
                    continue;
                }
                if(!templist[0].compare("Text"))
                {
                    mUiManager->SetText(tempDom, templist[1]);
                }
                else
                {
                    tempDom.setAttribute(templist[0], templist[1]);
                }
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        tempDom = mUiManager->itemElement;
        for(int i = 0; i < list.size(); i++)
        {
            QString temp = list.at(i);
            templist = temp.split(":");
            if(templist.size() != 2)
            {
                continue;
            }
            if(!templist[0].compare("Text"))
            {
                mUiManager->SetText(tempDom, templist[1]);
            }
            else
            {
                tempDom.setAttribute(templist[0], templist[1]);
            }
        }
    }
    return true;
}

bool UiEditor::pushTitleProperty(QStringList list)
{
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_TITLE))
    {
        return false;
    }
    QStringList templist;
    UIDom = mUiList.value(UI_TITLE)->mElement;
    if(!mUiManager->GetItemElement(UIDom, "/Operation/Run"))
    {
        if(mUiManager->CreateElementPath(UIDom, "/Operation/Run/"))
        {
            tempDom = mUiManager->itemElement;
            for(int i = 0; i < list.size(); ++i)
            {
                QString temp = list.at(i);
                templist = temp.split(":");
                if(templist.size() != 2)
                {
                    continue;
                }
                if(!templist[0].compare("Text"))
                {
                    mUiManager->SetText(tempDom, templist[1]);
                }
                else
                {
                    tempDom.setAttribute(templist[0], templist[1]);
                }
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        tempDom = mUiManager->itemElement;
        for(int i = 0; i < list.size(); ++i)
        {
            QString temp = list.at(i);
            templist = temp.split(":");
            if(templist.size() != 2)
            {
                continue;
            }
            if(!templist[0].compare("Text"))
            {
                mUiManager->SetText(tempDom, templist[1]);
            }
            else
            {
                tempDom.setAttribute(templist[0], templist[1]);
            }
        }
    }
    return true;
}

bool UiEditor::pushClockProperty(QStringList list)
{
    return true;
}

bool UiEditor::pushTimeProperty(QStringList list)
{
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_TIME))
    {
        return false;
    }
    QStringList templist;
    UIDom = mUiList.value(UI_TIME)->mElement;
    if(!mUiManager->GetItemElement(UIDom, "/Operation/Run"))
    {
        if(mUiManager->CreateElementPath(UIDom, "/Operation/Run/"))
        {
            tempDom = mUiManager->itemElement;
            for(int i = 0; i < list.size(); ++i)
            {
                QString temp = list.at(i);
                templist = temp.split("@");
                //                if(templist.size()==3){
                //                    if(QString::compare(templist[0], "Format")){
                //                        tempDom.setAttribute(templist[0], "hh:mm");
                //                    }
                //                    continue;
                //                }
                //                if(templist.size()== 4){
                //                    if(QString::compare(templist[0], "Format")){
                //                        tempDom.setAttribute(templist[0], "hh:mm:ss");
                //                    }
                //                    continue;
                //                }
                if(templist.size() != 2)
                {
                    continue;
                }
                tempDom.setAttribute(templist[0], templist[1]);
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        tempDom = mUiManager->itemElement;
        for(int i = 0; i < list.size(); ++i)
        {
            QString temp = list.at(i);
            templist = temp.split("@");
            //            if(templist.size()==3){
            //                if(QString::compare(templist[0], "Format")){
            //                    tempDom.setAttribute(templist[0], "hh:mm");
            //                }
            //                continue;
            //            }
            //            if(templist.size()== 4){
            //                if(QString::compare(templist[0], "Format")){
            //                    tempDom.setAttribute(templist[0], "hh:mm:ss");
            //                }
            //                continue;
            //            }
            if(templist.size() != 2)
            {
                continue;
            }
            tempDom.setAttribute(templist[0], templist[1]);
        }
    }
    return true;
}

bool UiEditor::pushDateProperty(QStringList list)
{
    QDomElement UIDom, tempDom;
    if(!mUiList.contains(UI_DATE))
    {
        return false;
    }
    QStringList templist;
    UIDom = mUiList.value(UI_DATE)->mElement;
    if(!mUiManager->GetItemElement(UIDom, "/Operation/Run"))
    {
        if(mUiManager->CreateElementPath(UIDom, "/Operation/Run/"))
        {
            tempDom = mUiManager->itemElement;
            for(int i = 0; i < list.size(); ++i)
            {
                QString temp = list.at(i);
                templist = temp.split("@");
                if(templist.size() != 2)
                {
                    continue;
                }
                tempDom.setAttribute(templist[0], templist[1]);
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        tempDom = mUiManager->itemElement;
        for(int i = 0; i < list.size(); ++i){
            QString temp = list.at(i);
            templist = temp.split("@");
            if(templist.size()!=2)
            {
                continue;
            }
            tempDom.setAttribute(templist[0], templist[1]);
        }
    }
    return true;
}

bool UiEditor::pushFlrBtnProperty(QStringList list)
{
    return true;
}

bool UiEditor::pushCallBtnProperty(QStringList list)
{
    return true;
}

bool UiEditor::pushKeyBoardProperty(QStringList list)
{
    return true;
}

void UiEditor::resizeRect(QString elementType, QRect rect)
{
    UI_TYPE tempType = GetUiType(elementType);
    if(!mUiList.contains(tempType))
    {
        return;
    }
    UiComParser* tmpComPtr = mUiList.value(tempType);
    if(tmpComPtr)
    {
        tmpComPtr->mComRect->SetRect(m_layout, mDirection, rect);
    }
    else
    {
        IDE_TRACE();
    }
    //mUiManager->SaveUI();
}

void UiEditor::functionTest()
{
    emit sgn_functionTest();
}

void UiEditor::slt_functionTest()
{
    //IDE_TRACE_STR(mProjectDir);
    //JlCompress::compressDir("ni.zip", "C:/Users/js198/Desktop/BVT910");
    //TCopyFile("C:/Users/js198/Desktop/a_H.avi", "C:/Users/js198/Desktop/nihao/Video/Resource/Video/0.avi");
}


void UiEditor::archiveCurTheme()
{
    //判断文件夹大小是否超过500MB
    QDir dir(mProjectDir);
    if(dir.exists())
    {
        if(QFile::exists("taidResource.zip"))
        {
            DelFile("taidResource.zip");
        }
        compressClient->compressDir("taidResource.zip", mProjectDir);
        //JlCompress::compressDir("taidResource.zip", mProjectDir);
    }
}
