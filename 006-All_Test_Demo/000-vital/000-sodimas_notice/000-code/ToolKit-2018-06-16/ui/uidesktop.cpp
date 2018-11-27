#include "uidesktop.h"
#include "qapplication.h"
#include <QPainter>
#include "mainwindow.h"
#include <QGraphicsScene>

UiDesktop::UiDesktop(QWidget* parent, Qt::WindowFlags f)
    :QWidget(parent,f)
{
    mSplashForm = 0;
}

void UiDesktop::Init(QDomElement pElement)
{
    IDE_TRACE_STR(QString("UiDesktop::Init(QDomElement pElement)"));
    QRect tmpRect = gUiLoader->m_themeParser->getComRect(gUiLoader->m_themeLayout, pElement, gUiLoader->m_themeDirection);
    QRect tmpRect2 = QRect(tmpRect.x() - 5, tmpRect.y() -5 , tmpRect.width() + 10, tmpRect.height() + 10);
    resize(tmpRect2.size());
    gUiLoader->mView->scene()->setSceneRect(tmpRect);

    QString tmpThemePath = gUiLoader->m_themeParser->m_FileDir;

    QDomElement tmpRcElement = pElement.firstChildElement("resource");
    QDomElement tmpRc0Element =tmpRcElement.firstChildElement("rc0");
    if(!tmpRc0Element.isNull())
    {
        mHorizontalPixmap = QPixmap(tmpThemePath + tmpRc0Element.text());
    }
    QDomElement tmpRc1Element =tmpRcElement.firstChildElement("rc1");
    if(!tmpRc1Element.isNull())
    {
        mVerticalPixmap = QPixmap(tmpThemePath + tmpRc1Element.text());
    }

    if (gUiLoader->m_themeDirection == Horizontal)
    {
        if (mSplashForm)
        {
            mSplashForm->setGeometry(-5, -5, 490, 282);
        }
    }
    else if (gUiLoader->m_themeDirection == Vertical)
    {
        if (mSplashForm)
        {
            mSplashForm->setGeometry(-5, -5, 282, 490);
        }
    }


   if (gUiLoader->m_themeDirection == Horizontal)
   {
       if (m_messageBar)
       {
           m_messageBar->setGeometry(115, 10, 252, 100);
       }
   }
   else if (gUiLoader->m_themeDirection == Vertical)
   {
       if (m_messageBar)
       {
           m_messageBar->setGeometry(20, 20, 232, 100);
       }
   }
    update(); //确保更改图片后，能立即刷新到当前的显示内容上
}

void UiDesktop::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);

    if(gUiLoader->m_themeDirection == Horizontal)
    {
        p.drawPixmap(rect(), mHorizontalPixmap);
    }
    else
    {
        p.drawPixmap(rect(), mVerticalPixmap);
    }
}

void UiDesktop::setSplashForm(SplashForm *pSplash)
{
    mSplashForm = pSplash;
}

void UiDesktop::setMessageBar(MessageBar *pMessageBar)
{
    m_messageBar = pMessageBar;
}
