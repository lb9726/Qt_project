#include "uidesktop.h"
#include "qapplication.h"
#include <QPainter>
#include "mainwindow.h"
#include <QGraphicsScene>

UiDesktop::UiDesktop(QWidget* parent, Qt::WindowFlags f)
    :QWidget(parent,f)
{
}

void UiDesktop::Init(QDomElement pElement)
{
    QRect tmpRect = gUiLoader->m_themeParser->getComRect(gUiLoader->m_themeLayout, pElement, gUiLoader->m_themeDirection);
    gUiLoader->mView->scene()->setSceneRect(tmpRect);

//    if (gUiLoader->mView)
//    {
//        gUiLoader->mView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//        gUiLoader->mView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    }
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
    update(); //确保更改图片后，能立即刷新到当前的显示内容上
}

void UiDesktop::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);

    if(gUiLoader->m_themeDirection == Horizontal)
        p.drawPixmap(rect(), mHorizontalPixmap);
    else
        p.drawPixmap(rect(), mVerticalPixmap);
}
