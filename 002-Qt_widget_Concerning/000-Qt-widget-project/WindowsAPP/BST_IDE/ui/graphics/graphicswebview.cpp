#include "graphicswebview.h"

#ifdef IDE
    #include "diagramscene.h"
#elif DISPLAY
    #include "logicthememanager.h"
#endif

GraphicsWebview::GraphicsWebview(QGraphicsItem *parent) :
    GraphicsOperate(parent)
{
#ifdef SURRPORT_WEBKIT
    m_Webview = 0;
#endif
}

bool GraphicsWebview::Start()
{
    if(GraphicsOperate::Start() == false)
        return false;
    CreateWebview();
    m_PixmapPointer.mLastPointer = m_PixmapPointer.mCurPointer = GetPixmap(BGRC);

    updateEffect(m_RcList.first());
    return true;
}

bool GraphicsWebview::CreateWebview()
{
#ifdef SURRPORT_WEBKIT
    if(m_Webview)
        m_Webview->deleteLater();
    m_Webview = new QGraphicsWebView(this);
    m_Webview->setGeometry(rect());
    m_Webview->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    m_Webview->settings()->setAttribute(QWebSettings::PluginsEnabled, true);//??¨°?
    //?a??¡ä¨°????¨°????D¦Ì??t??¨¢¡ä?¨®?¨º¨¬a
    m_Webview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    m_Webview->show();
#endif
    return true;
}

bool GraphicsWebview::DestoryWebview()
{
#ifdef SURRPORT_WEBKIT
    if(m_Webview == 0)
        return true;
    m_Webview->stop();
    m_Webview->deleteLater();
    m_Webview = 0;
#endif
    return true;
}

void GraphicsWebview::updateEffect(AREA_OPERATE pOperate, QVariant pPara)
{
    switch(pOperate)
    {
        case OPERATE_CHANGE:
        {
            int index = pPara.toInt();
            updateEffect(index);
            break;
        }
        default:
            break;
    }
}

void GraphicsWebview::updateEffect(int pIndex)
{
    if(PREVRC == pIndex)
        pIndex = getPrevRc(m_CurRcLabel);
    else if(NEXTRC == pIndex)
        pIndex = getNextRc(m_CurRcLabel);

    if(BGRC == pIndex)
    {
        return;
    }

    bool tmpUrlChange = false;
    if(m_CurRcLabel != pIndex)
        tmpUrlChange = true;
#ifdef SURRPORT_WEBKIT
    if(tmpUrlChange && m_Webview)
    {
        QString tmpRc = m_EffectRC.value(pIndex)->mRcFile;
        FILEPATH_TYPE tmpType = PATH_TYPENETADDR;
#if 0
#ifndef SERVER
        tmpType = m_UiContainer->m_UiManager->GetRcFileType(tmpRc);
#endif
        if(tmpType == PATH_TYPENETADDR)
            m_Webview->load(QUrl(tmpRc));
        else
            m_Webview->setHtml(tmpRc);
#endif
    }
#endif
    m_CurRcLabel = pIndex;
}


DesignWebview::DesignWebview(QGraphicsItem *parent) :
    GraphicsWebview(parent)
{
    QAction *tmpDefault = m_ActionGroup->addAction(tr("0"));
    for(int i=1;i<20;i++)
    {
        m_ActionGroup->addAction(QString("%1").arg(i));
    }
    tmpDefault->setChecked(true);
}

QList<QAction*> DesignWebview::GetActionList()
{
    QList<QAction*> tmpList;
    if(m_ActionGroup)
    {
        tmpList = m_ActionGroup->actions();
        //>@?¨´?Y¦Ì¡À?¡ã¨°?¨®D¦Ì?¡Á¨º?¡ä¨º??¨¹?¨¬¨®|?|?¨¹Action
        for(int i=0;i<tmpList.count();i++)
        {
            QAction *tmpAction = tmpList.at(i);
            if(tmpAction == 0)
                continue;
            int tmpState = tmpAction->text().toInt();
            if(m_EffectRC.contains(tmpState))
                tmpAction->setEnabled(true);
            else
                tmpAction->setDisabled(true);
        }
    }
    return tmpList;
}

void DesignWebview::ExecAction(QAction *pAction)
{
    updateEffect(pAction->text().toInt());
}
