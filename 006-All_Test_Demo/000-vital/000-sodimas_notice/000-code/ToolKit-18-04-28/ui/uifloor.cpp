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
    mFloorMapLabel = 0;
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
    /*floor map */

    mFloorMapLabel = this->findChild<QLabel *>("mapLabel");

    if(mFloorMapLabel)
    {
        //qDebug()<<__PRETTY_FUNCTION__<<"mFloorMapLabel is not null";
        mFloorMapLabel->setScaledContents(true);
    }
    else
    {
        //qDebug()<<__PRETTY_FUNCTION__<<"mFloorMapLabel is null";
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

    tmpRect.setHeight(tmpRect.height() + mTimeText->height() + mDestText->height());
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
    /*floor map*/
    QDomElement tmpRcElement1 = pElement.firstChildElement("Map");
    if(!tmpRcElement1.isNull())
    {
        QDomNodeList tmpList1 = tmpRcElement1 .childNodes();
        for(int i = 0; i < tmpList1.count(); i++)
        {
            QDomElement tmpElement = tmpList1.at(i).toElement();
            if(tmpElement.isNull())
            {
                continue;
            }
            QString tmpTagName = tmpElement.tagName();
            int tmpNum = tmpTagName.mid(2).toInt();
            mFloorHash.insert(tmpNum,tmpElement.text());
            //qDebug()<<"mFloorHash tmpNum = "<<tmpNum;
            //qDebug()<<"mFloorHash tmpElement.text = "<<tmpElement.text();
        }
    }
    updateFloor();//确保更改图片后，能立即刷新到当前的显示内容上
}
//floor map
void UiFloor::initAudio(QDomElement pElement)
{
    QDomElement tmpRcElement = pElement.firstChildElement("Map");
    if(!tmpRcElement.isNull())
    {
        QDomNodeList tmpList = tmpRcElement.childNodes();
        for(int i=0; i<tmpList.count(); i++)
        {
            QDomElement tmpElement = tmpList.at(i).toElement();
            if(tmpElement.isNull())
                continue;
            QString tmpTagName = tmpElement.tagName();
            int tmpNum = tmpTagName.mid(2).toInt();
            mAudioHash.insert(tmpNum, tmpElement.text());
            //qDebug()<<"mAudioHash tmpNum = "<<tmpNum;
            //qDebug()<<"mAudioHash tmpElement.text = "<<tmpElement.text();
        }
    }
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
              if (mFloorMapLabel)
              {
                  mFloorMapLabel->setPixmap(QPixmap());
              }
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
              if (mFloorMapLabel)
              {
                  mFloorMapLabel->setPixmap(QPixmap());
              }
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
        if (mFloorMapLabel)
        {
            mFloorMapLabel->setPixmap(QPixmap());
        }
      }
    qDebug()<<"mSingleLabel->geometry() = "<<mSingleLabel->geometry();
    qDebug()<<"mTenLabel->geometry() = "<<mTenLabel->geometry();
    qDebug()<<"mHundLabel->geometry() = "<<mHundLabel->geometry();
    qDebug()<<"uifloor->geometry() = "<<this->geometry();
    //mSingleLabel->setStyleSheet("font:bold larger\"Arial\"; font-size:100px;color:white");
}

void UiFloor::setFloor(int pGe, int pShi, int pBai, int frameCount)
{
    /*
    qDebug()<<"setFloor = "<<pGe
           <<pShi
           <<pBai;
    */
    //qDebug()<<"frameCount = "<<frameCount;
    int key = getFloorHashKey(frameCount);//floor map
    QString mFloorHashValue = mFloorHash.value(key);
    //qDebug()<<"key = "<<key;
    //qDebug()<<"mFloorHashValue = "<<mFloorHashValue;

    if(mFloorHashValue.count() == 6)
    {
        mHundred = pBai+48;
        mTen = pShi+48;
        mSingle = pGe+48;

        updateFloor();
    }
    else
    {
        //qDebug()<<"mFloorHashValue = "<<mFloorHashValue;
        updateMap(mFloorHashValue);
    }
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
//floor map
QString UiFloor::setMapMedia(int key)
{
    QString keyString = QString::number(key);
    QString audioHashValue = mAudioHash.value(key);
    if(keyString != audioHashValue)
    {
        QString path = gUiLoader->m_themeParser->m_FileDir + QString("media/M%1.wav").arg(audioHashValue);
        return path;
    }

    return "false";
}

//floor map
int UiFloor::getFloorHashKey(int frame)
{
    //qDebug()<<"frame = "<<frame;
    int safFrame = frame;
    int key = 1000;
    if(safFrame == 0)
    {
        key = 323248;
    }
    else if(safFrame == 1)
    {
        key = 323249;
    }
    else if(safFrame == 2)
    {
        key = 323250;
    }
    else if(safFrame == 3)
    {
        key = 323251;
    }
    else if(safFrame == 4)
    {
        key = 323252;
    }
    else if(safFrame == 5)
    {
        key = 323253;
    }
    else if(safFrame == 6)
    {
        key = 323254;
    }
    else if(safFrame == 7)
    {
        key = 323255;
    }
    else if(safFrame == 8)
    {
        key = 323256;
    }
    else if(safFrame == 9)
    {
        key = 323257;
    }
    else if(safFrame == 10)
    {
        key = 324948;
    }
    else if(safFrame == 11)
    {
        key = 324949;
    }
    else if(safFrame == 12)
    {
        key = 324950;
    }
    else if(safFrame == 13)
    {
        key = 324951;
    }
    else if(safFrame == 14)
    {
        key = 324952;
    }
    else if(safFrame == 15)
    {
        key = 324953;
    }
    else if(safFrame == 16)
    {
        key = 324954;
    }
    else if(safFrame == 17)
    {
        key = 324955;
    }
    else if(safFrame == 18)
    {
        key = 324956;
    }
    else if(safFrame == 19)
    {
        key = 324957;
    }
    else if(safFrame == 21)
    {
        key = 324549;
    }
    else if(safFrame == 22)
    {
        key = 324550;
    }
    else if(safFrame == 23)
    {
        key = 324551;
    }
    else if(safFrame == 24)
    {
        key = 324552;
    }
    else if(safFrame == 25)
    {
        key = 324553;
    }
    else if(safFrame == 26)
    {
        key = 324554;
    }
    else if(safFrame == 27)
    {
        key = 324555;
    }
    else if(safFrame == 28)
    {
        key = 325148;
    }
    else if(safFrame == 29)
    {
        key = 325149;
    }
    else if(safFrame == 30)
    {
        key = 325048;
    }
    else if(safFrame == 31)
    {
        key = 325049;
    }
    else if(safFrame == 32)
    {
        key = 325050;
    }
    else if(safFrame == 33)
    {
        key = 325051;
    }
    else if(safFrame == 34)
    {
        key = 325052;
    }
    else if(safFrame == 35)
    {
        key = 325053;
    }
    else if(safFrame == 36)
    {
        key = 325054;
    }
    else if(safFrame == 37)
    {
        key = 325055;
    }
    else if(safFrame == 38)
    {
        key = 325056;
    }
    else if(safFrame == 39)
    {
        key = 325057;
    }

    return key;
}

void UiFloor::updateMap(QString path)
{
    QString tmpThemePath = gUiLoader->m_themeParser->m_FileDir;
    QString path1 = tmpThemePath + "floor/" + "T" + path + ".png";
    //qDebug()<<"floor map path1 = "<<path1;
    if (mFloorMapLabel)
    {
        mFloorMapLabel->setFixedSize(200,200);
        mFloorMapLabel->setPixmap(path1);
    }


    mHundLabel->setPixmap(QPixmap());
    mTenLabel->setPixmap(QPixmap());
    mSingleLabel->setPixmap(QPixmap());
}
