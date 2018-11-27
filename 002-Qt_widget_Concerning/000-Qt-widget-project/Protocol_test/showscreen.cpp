#include "showscreen.h"
ShowScreen::ShowScreen(QObject *parent) : QObject(parent)
{
    qDebug()<< __PRETTY_FUNCTION__;
    qDebug()<<"ShowScreen主线程ID为："<<QThread::currentThreadId();
    pro = Protocoldeal::GetInstance();
    connect(pro, SIGNAL(ShowWhichScreen(int)), this, SLOT(ChoiceScreen(int)));
    connect(pro, SIGNAL(HideWhichScreen(int)), this, SLOT(OnHideScreen(int)));
}

ShowScreen::~ShowScreen()
{
    qDebug()<< __PRETTY_FUNCTION__;
    delete fileup;
    delete wid;
}

void ShowScreen::ChoiceScreen(int index)
{
    qDebug()<< __PRETTY_FUNCTION__<<"index = "<< index;
    if (0 == index)
    {
        fileup = new FileUpdate;
        fileup->showFullScreen();
    }
    else if (1 == index)
    {
        wid = new Widget;
        wid->showFullScreen();
    }
}

void ShowScreen::OnHideScreen(int index)
{
    qDebug()<< __PRETTY_FUNCTION__;
    if (0 == index)
    {
        if (NULL != fileup)
            fileup->hide();
    }
    else if (1 == index)
    {
        if (NULL != wid)
            wid->hide();
    }
}
