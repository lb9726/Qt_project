#include "uifloor.h"
#include "define.h"
#include "mainwindow.h"
#include <QFont>

UiFloor::UiFloor(QWidget* parent, Qt::WindowFlags f)
    :QWidget(parent, f)
{
    mHundred = 0;
    mTen = 0;
    mSingle = 0;
}

void UiFloor::Init(QDomElement pElement)
{
    bool tmpFlag = isVisible();
    QRect tmpRect = gUiLoader->m_themeParser->getComRect(gUiLoader->m_themeLayout, pElement, gUiLoader->m_themeDirection);

    IDE_DEBUG(QString("%1 %2 %3 %4").arg(tmpRect.x()).arg(tmpRect.y()).arg(tmpRect.width()).arg(tmpRect.height()));
    if(!tmpFlag)
    {
        this->setVisible(false);
    }

    mHundLabel = this->findChild<QLabel *>("Hundred");
    if(mHundLabel)
    {
        mHundLabel->setScaledContents(true);
    }
    mTenLabel = this->findChild<QLabel *>("Ten");
    if(mTenLabel)
    {
        mTenLabel->setScaledContents(true);
    }
    mSingleLabel = this->findChild<QLabel *>("Single");
    if(mSingleLabel)
    {
        mSingleLabel->setScaledContents(true);
    }

    mTimeText = this->findChild<QLabel *>("label_time");
    mDestText = this->findChild<QLabel *>("label_dest");
    QFont font;
    font.setPixelSize(16);
    if (mTimeText)
    {
        mTimeText->setFont(font);
    }
    if (mDestText)
    {
        mDestText->setFont(font);
    }

    //tmpRect.setHeight(tmpRect.height() + mTimeText->height() + mDestText->height());
    this->setGeometry(tmpRect);
    IDE_TRACE_INT(mTimeText->width());
    IDE_TRACE_INT(mTimeText->height());

    IDE_TRACE_INT(mDestText->width());
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
    if(mHundred == 48)
    {
          if(mTen == 48)
          {
              mHundLabel->setGeometry(0, 0, 0, 0);
              mTenLabel->setGeometry(66, 0, 0, 0);
              mSingleLabel->setGeometry(132, 0, 0, 0);

              mHundLabel->setFixedSize(66, 154);
              mTenLabel->setFixedSize(66, 154);
              mSingleLabel->setFixedSize(66, 154);

              mHundLabel->setPixmap(QPixmap());
              mTenLabel->setPixmap(mRcHash.value(mSingle));
              mSingleLabel->setPixmap(QPixmap());

          }
          else
          {
              mHundLabel->setGeometry(0, 0, 0, 0);
              mTenLabel->setGeometry(33, 0, 0, 0);
              mSingleLabel->setGeometry(99, 0, 0, 0);

              mHundLabel->setFixedSize(0, 0);
              mTenLabel->setFixedSize(66, 154);
              mSingleLabel->setFixedSize(66, 154);

              mHundLabel->setPixmap(QPixmap());
              mTenLabel->setPixmap(mRcHash.value(mTen));
              mSingleLabel->setPixmap(mRcHash.value(mSingle));

           }
      }
      else
      {
        mHundLabel->setGeometry(0, 0, 0, 0);
        mTenLabel->setGeometry(66, 0, 0, 0);
        mSingleLabel->setGeometry(132, 0, 0, 0);

        mHundLabel->setFixedSize(66, 154);
        mTenLabel->setFixedSize(66, 154);
        mSingleLabel->setFixedSize(66, 154);

        mHundLabel->setPixmap(mRcHash.value(mHundred));
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
//    static int count = 0;
//    count++;
//    if (10 == count)
//    {
//        IDE_TRACE_STR(dest);
//        IDE_TRACE_STR(time);
//        count = 0;
//    }
    mDestText->setText(dest.simplified());
    mTimeText->setText(time.simplified());
}
