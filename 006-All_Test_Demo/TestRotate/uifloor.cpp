#include "uifloor.h"
#include "define.h"
#include "mainwindow.h"

UiFloor::UiFloor(QWidget* parent, Qt::WindowFlags f)
    :QWidget(parent,f)
{
    mHundred = 0;
    mTen = 0;
    mSingle = 0;
}

void UiFloor::Init(QDomElement pElement)
{
    bool tmpFlag = isVisible();
    QRect tmpRect = gUiLoader->m_themeParser->getComRect(gUiLoader->m_themeLayout, pElement, gUiLoader->m_themeDirection);
    this->setGeometry(tmpRect);
    IDE_DEBUG(QString("%1 %2 %3 %4").arg(tmpRect.x()).arg(tmpRect.y()).arg(tmpRect.width()).arg(tmpRect.height()));
    if(!tmpFlag)
        this->setVisible(false);

    mHundLabel = this->findChild<QLabel *>("Hundred");
    if(mHundLabel)
        mHundLabel->setScaledContents(true);
    mTenLabel = this->findChild<QLabel *>("Ten");
    if(mTenLabel)
        mTenLabel->setScaledContents(true);
    mSingleLabel = this->findChild<QLabel *>("Single");
    if(mSingleLabel)
        mSingleLabel->setScaledContents(true);

    mTimeText = this->findChild<QLabel *>("label_time");
    mDestText = this->findChild<QLabel *>("label_dest");
    mTimeText->setAlignment(Qt::AlignCenter);
    mDestText->setAlignment(Qt::AlignCenter);

    tmpRect.setHeight(tmpRect.height() + mTimeText->height() + mDestText->height());
    this->setGeometry(tmpRect);
    IDE_TRACE_INT(mTimeText->height());
    IDE_TRACE_INT(mDestText->height());

    QString tmpThemePath = gUiLoader->m_themeParser->m_FileDir;

    QDomElement tmpRcElement = pElement.firstChildElement("resource");
    QDomNodeList tmpList = tmpRcElement.childNodes();
    for(int i=0; i<tmpList.count(); i++)
    {
        QDomElement tmpElement = tmpList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpTagName = tmpElement.tagName();
        int tmpNum = tmpTagName.mid(2).toInt();
        mRcHash.insert(tmpNum, tmpThemePath + tmpElement.text());
    }

    updateFloor();//确保更改图片后，能立即刷新到当前的显示内容上
}

void UiFloor::updateFloor()
{
    if(mTen == 48)
    {
        mTenLabel->setPixmap(mRcHash.value(mSingle));
        mSingleLabel->setPixmap(QPixmap());
    }
    else
    {
        mTenLabel->setPixmap(mRcHash.value(mTen));
        mSingleLabel->setPixmap(mRcHash.value(mSingle));
    }
    //mSingleLabel->setStyleSheet("font:bold larger\"Arial\"; font-size:100px;color:white");
}

void UiFloor::setFloor(int pGe, int pShi, int pBai)
{
    mHundred = pBai+48;
    mTen = pShi+48;
    mSingle = pGe+48;

    updateFloor();
}

void UiFloor::setTimeAndDest(QString dest, QString time)
{
    mTimeText->setText(time);
    mDestText->setText(dest);
}
