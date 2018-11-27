#include "uieditor.h"

UiEditor::UiEditor(QObject *parent) : QObject(parent)
{
    qDebug()<< "UiEditor";
    mUiManager = NULL;
    mUiList.clear();
    mDirection = HVNone;
}

bool UiEditor::InitCom(QString pPath)
{
    if(pPath.isEmpty())
        return false;
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

    //>@加载控件
    QList<UI_TYPE> tmpComTypeList = mUiManager->m_ComElementGroup.keys();
    int count = tmpComTypeList.count();
    if(count <= 0)
        return false;

    qSort(tmpComTypeList.begin(), tmpComTypeList.end());
    for(int i= 0;i < count; i++)
    {
        UI_TYPE tmpType = tmpComTypeList.at(i);
        QDomElement tmpElement = mUiManager->m_ComElementGroup.value(tmpType);
        if(tmpElement.isNull())
            continue;
        AddCom(tmpType, tmpElement);
    }
    return true;
}

bool UiEditor::AddCom(UI_TYPE pComType, QDomElement &pComElement)
{
    if(pComType < UI_DESKTOP)
        return false;
    if(pComElement.isNull())
        return false;

    //>@如果存在此控件，则不再重复创建
    if(mUiList.contains(pComType))
        return true;

    UiComParser *tmpComPtr = 0;
    switch(pComType)
    {
    case UI_DESKTOP:
    case UI_FLOOR:
    case UI_DTNELE:
    case UI_ARROW:
    case UI_FUNCTION:
    case UI_IMAGE:
    case UI_MEDIA:
    case UI_TEXT:
    case UI_LOGO:
    case UI_WEATHER:
    case UI_CLOCK:
    case UI_NAMEPLATE:
    case UI_FLRBTN:
    case UI_CALLBTN:
    case UI_DOORBTN:
    case UI_PHONEBTN:
    case UI_EMERGENCYBTN:
    case UI_TALKBACKBTN:
    case UI_DISABILITYBTN:
        tmpComPtr = new UiComParser();
        break;
    default:
        return false;
    }
    if(tmpComPtr == 0)
        return false;
    tmpComPtr->Init(pComElement, mUiManager);
    mUiList.insert(pComType, tmpComPtr);
    return true;
}

void UiEditor::ReleaseUIs()
{
    QList<int> tmpList = mUiList.keys();
    if(!tmpList.isEmpty())
    {
        qSort(tmpList.begin(), tmpList.end());
        for(int i=tmpList.count()-1;i>=0;i--)
        {
            UiComParser *tmpUi = mUiList.take(tmpList.at(i));
            if(tmpUi)
                tmpUi->deleteLater();
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
    UiComParser* tmpComPtr = mUiList.take(pComType);
    if(tmpComPtr)
    {
        //tmpComPtr->UnbindLayout(mLayoutStates);
        tmpComPtr->deleteLater();
    }
    return true;
}


