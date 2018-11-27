#include "uiicon.h"
#include "mainwindow.h"
#include <QStackedWidget>
#include <QFont>
#include <QModelIndex>
#include <QThread>
#include <QDebug>

UiIcon::UiIcon(QWidget* parent, Qt::WindowFlags f)
    :QWidget(parent, f)
{
    mIconNum = -1;
    mIcon = 0;

    mTextLine1 = 0;
    //mTextLine2 = 0;

    mBigTextLine1 = 0;
    mBigTextLine2 = 0;
    mBigTextLine3 = 0;
    mBigTextLine4 = 0;
    mHaveCopy = false;
    mBackUp = false;
    mUpdateComplete = true;
    connect(&mUsbAct, &UsbAction::sigUpdateProgress, this, &UiIcon::updateProgress, Qt::UniqueConnection);
    connect(&mUsbAct, &UsbAction::sigBackUpEnded, this, &UiIcon::showCopyScreen, Qt::UniqueConnection);
    connect(&mUsbAct, &UsbAction::sigUpdateEnded, this, &UiIcon::promptUpdateComplete, Qt::UniqueConnection);
}

void UiIcon::Init(QDomElement pElement)
{
    mDomElement = pElement;

    mIcon = this->findChild<QLabel *>("label_image");
    mTextLine1 = this->findChild<QLabel *>("label_text_1");
    //mTextLine2 = this->findChild<QLabel *>("label_text_2");
    QStackedWidget *tmpStackedWidget = qobject_cast<QStackedWidget*>(parentWidget()->parentWidget());
    if(tmpStackedWidget)
    {
        QWidget *tmpBigPage = tmpStackedWidget->widget(1);
        mBigTextLine1 = tmpBigPage->findChild<QLabel *>("label_bigtext_1");
        mBigTextLine2 = tmpBigPage->findChild<QLabel *>("label_bigtext_2");
        mBigTextLine3 = tmpBigPage->findChild<QLabel *>("label_bigtext_3");
        mBigTextLine4 = tmpBigPage->findChild<QLabel *>("label_bigtext_4");

        QWidget *tmpPage3 = tmpStackedWidget->widget(3);
        mUsbPicture = tmpPage3->findChild<QLabel *>("label_usbpicture");
        mLabelWaring = tmpPage3->findChild<QLabel *>("label_waring");
        mTextView = tmpPage3->findChild<QListView *>("listView");
        mCopyProgress = tmpPage3->findChild<QProgressBar *>("progressBar");

        setLabelGeometry();
        setLabelStyleSheet();
        setUsbUI(false, QString());
    }

    bool tmpFlag = isVisible();
    QRect tmpRect = gUiLoader->m_themeParser->getComRect(gUiLoader->m_themeLayout, pElement, gUiLoader->m_themeDirection);

    IDE_TRACE_INT(mTextLine1->height());
    IDE_TRACE_INT(tmpRect.height());
    //tmpRect.setHeight(tmpRect.height() + mTextLine1->height() + mTextLine1->height());
    this->setGeometry(tmpRect);

    if(!tmpFlag)
    {
        this->setVisible(false);
    }

    QString tmpThemePath = gUiLoader->m_themeParser->m_FileDir;

    QDomElement tmpRcElement = pElement.firstChildElement("resource");
    QDomNodeList tmpList = tmpRcElement.childNodes();
    for(int i=0;i<tmpList.count();i++)
    {
        QDomElement tmpElement = tmpList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpTagName = tmpElement.tagName();
        int tmpNum = tmpTagName.mid(2).toInt();
        mRcHash.insert(tmpNum, tmpThemePath + tmpElement.text());
    }

    updateIcon(); //
}

void UiIcon::updateIcon()
{
    mIcon->setPixmap(mRcHash.value(mIconNum));

    QStackedWidget *tmpStackedWidget = qobject_cast<QStackedWidget*>(parentWidget()->parentWidget());
    if(tmpStackedWidget && tmpStackedWidget->currentIndex() == 0)
    {
        QDomElement tmpRcElement = mDomElement.firstChildElement("resource");
        QDomElement tmpElement =tmpRcElement.firstChildElement(QString("rc%1").arg(mIconNum));
        if(!tmpElement.isNull())
        {
            QString tmpText = tmpElement.attribute("text");
            QStringList tmpList = tmpText.split(";", QString::SkipEmptyParts);  //���Կո�
            if(tmpList.count() >= 1)
            {
                mTextLine1->setText(tmpList.at(0));
            }
//            if(tmpList.count() >= 2)
//            {
//                mTextLine2->setText(tmpList.at(1));
//            }
        }
    }
}

void UiIcon::setIcon(int pIcon)
{
    mIconNum = pIcon;
    updateIcon();
}

void UiIcon::setText(QString pText1, QString pText2, QString pText3, QString pText4)
{
    if (mBigTextLine1)
    {
        mBigTextLine1->setText(pText1);
    }
    if (mBigTextLine2)
    {
        mBigTextLine2->setText(pText2);
    }
    if (mBigTextLine3)
    {
        mBigTextLine3->setText(pText3);
    }
    if (mBigTextLine4)
    {
        mBigTextLine4->setText(pText4);
    }
}

void UiIcon::setBigSrnMode(bool pFlag)
{
    QStackedWidget *tmpStackedWidget = qobject_cast<QStackedWidget*>(parentWidget()->parentWidget());
    if(!tmpStackedWidget)
        return;
    if(pFlag)
    {
        tmpStackedWidget->setCurrentIndex(1);  // 大屏幕文本
    }
    else
    {
        tmpStackedWidget->setCurrentIndex(0); // 小屏幕文本
    }
}

int UiIcon::getStackWidgetIndex()
{
    IDE_TRACE();
    int curStackWidgetIndex = 0;
    QStackedWidget *tmpStackedWidget = qobject_cast<QStackedWidget*>(parentWidget()->parentWidget());
    if(!tmpStackedWidget)
    {
        IDE_TRACE_STR(QString("tmpStackedWidget is empty"));
        return -1;
    }    
    curStackWidgetIndex = tmpStackedWidget->currentIndex();
    IDE_TRACE_INT(curStackWidgetIndex);
    return curStackWidgetIndex;
}

void UiIcon::setUsbUI(bool flag, QString str)
{
    QPixmap mUsbPic("/usr/bst/usrfs/Theme/sodimas/usb.png");  //
    if (mUsbPicture)
    {
        mUsbPicture->setPixmap(mUsbPic);
        mUsbPicture->setScaledContents(true);  //
        mUsbPicture->show();
    }
    if (mLabelWaring)
    {
        mLabelWaring->setAlignment(Qt::AlignCenter);
        mLabelWaring->setText(str);
        mLabelWaring->setVisible(!flag);
    }
    if (mCopyProgress)
    {
        mCopyProgress->setVisible(flag);
    }
    if (mTextView)
    {
        mTextView->setVisible(flag);
    }
}

void UiIcon::setChangedGround(int index)
{
    IDE_TRACE_INT(index);
    QStackedWidget *tmpStackedWidget = qobject_cast<QStackedWidget*>(parentWidget()->parentWidget());
    if(!tmpStackedWidget)
    {
        return;
    }
    tmpStackedWidget->setCurrentIndex(index);
}

void UiIcon::showCopyScreen()
{
    IDE_TRACE_STR(QString("is enter showCopyScreen"));
    if (!mHaveCopy)   //
    {
        mHaveCopy = true;
    }
    else
    {
        IDE_TRACE_STR(QString("already enter showCopyScreen will return"));
        return;
    }
    mUsbAct.resetCopyParameter();
    setUsbConnected();
    mUsbAct.dirFileSize("/media/sodimas/usrfs");  //
    QtSleep(3000);
    setUsbUI(false, "BackUp is Completing!");
    QtSleep(3000);
    setUsbUI(false, "Will Update files!");
    QtSleep(3000);
    if (0 >= mUsbAct.getFileNum())   //
    {
        setUsbUI(false, "U disk has no resource!");
    }
    else
    {
        mUsbAct.removeFile("/usr/bst/usrfs/");
        int flag = mUsbAct.copyFile("/media/sodimas/usrfs/", "/usr/bst/usrfs/");  //
        qDebug()<<__FUNCTION__<<"() "<<"flag = "<<flag;
        if (0 == flag) // if copy suceess
        {            
            setUsbUI(true, QString());
            if (mTextView)
            {
                model.setStringList(mUsbAct.getFileList(false));
                mTextView->setVisible(true);
                mTextView->setModel(&model);
                mTextView->setSelectionMode(QAbstractItemView::NoSelection);
            }
            mUsbAct.sigStartProgress(false);
        }
        else   // copy failed
        {
            setUsbUI(false, "copy resource failed!");
        }
    }
}

void UiIcon::updateProgress(int num)
{
    if (mCopyProgress)
    {
        mCopyProgress->setValue(num);
        if (mTextView)
        {
            QModelIndex tmpCurIndex =  mTextView->currentIndex();
            QModelIndex tmpNextIndex = mTextView->model()->index(tmpCurIndex.row() + 1, 0);
            mTextView->setCurrentIndex(tmpNextIndex);
        }
        if (mUsbPicture->isHidden())
        {
            mUsbPicture->show();
        }
        else
        {
            mUsbPicture->hide();
        }
        if (100 == num)
        {
            mUsbPicture->show();
        }
    }
}

void UiIcon::promptUpdateComplete()
{
    qDebug()<<__FUNCTION__<<"() mLabelWaring";
    QtSleep(1500);
    if (mLabelWaring)
    {
        int tmp = 0;
        mLabelWaring->setVisible(true);
        if (mTextView)
        {
            mTextView->setVisible(false);
        }
        while(mUpdateComplete)
        {
            mLabelWaring->setText("Updating is Completing!");
            if (0 == tmp)
            {
                mLabelWaring->setStyleSheet("QLabel {color:green}");
                tmp = 1;
            }
            else
            {
                mLabelWaring->setStyleSheet("QLabel {color:red}");
                tmp = 0;
            }
            QtSleep(1500);
        }
    }
}

void UiIcon::setUsbRemoveFlag()
{
    setHaveCopyFlag(false);
}

void UiIcon::setLabelStyleSheet()
{
    QFont font;
    font.setPixelSize(23);
    if (mBigTextLine1)
    {
        mBigTextLine1->setFont(font);
        mBigTextLine1->setAlignment(Qt::AlignCenter);
    }
    if (mBigTextLine2)
    {
        mBigTextLine2->setFont(font);
        mBigTextLine2->setAlignment(Qt::AlignCenter);
    }
    if (mBigTextLine3)
    {
        mBigTextLine3->setFont(font);
        mBigTextLine3->setAlignment(Qt::AlignCenter);
    }
    if (mBigTextLine4)
    {
        mBigTextLine4->setFont(font);
        mBigTextLine4->setAlignment(Qt::AlignCenter);
    }
    if (mTextView)
    {
        mTextView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mTextView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

void UiIcon::setLabelGeometry()
{
    if(gUiLoader->m_themeDirection == Horizontal)
    {
        if (mBigTextLine1)
        {
            mBigTextLine1->setGeometry(9, 9, 462, 59);
        }
        if (mBigTextLine2)
        {
            mBigTextLine2->setGeometry(9, 74, 462, 59);
        }
        if (mBigTextLine3)
        {
            mBigTextLine3->setGeometry(9, 139, 462, 59);
        }
        if (mBigTextLine4)
        {
            mBigTextLine4->setGeometry(9, 204, 462, 59);
        }
        if (mUsbPicture)
        {
            mUsbPicture->setGeometry(400, 10, 45, 20);
        }
        if (mLabelWaring)
        {
            mLabelWaring->setGeometry(95, 110, 291, 31);
        }
        if (mTextView)
        {
            mTextView->setGeometry(90, 30, 301, 188);
        }
        if (mCopyProgress)
        {
            mCopyProgress->setGeometry(90, 230, 301, 23);
        }
    }
    else
    {
        if (mBigTextLine1)
        {
            mBigTextLine1->setGeometry(9, 20, 254, 110);
        }
        if (mBigTextLine2)
        {
            mBigTextLine2->setGeometry(9, 130, 254, 110);
        }
        if (mBigTextLine3)
        {
            mBigTextLine3->setGeometry(9, 240, 254, 110);
        }
        if (mBigTextLine4)
        {
            mBigTextLine4->setGeometry(9, 350, 254, 110);
        }
        if (mUsbPicture)
        {
            mUsbPicture->setGeometry(200, 10, 45, 20);
        }
        if (mLabelWaring)
        {
            mLabelWaring->setGeometry(30, 190, 210, 31);
        }
        if (mTextView)
        {
            mTextView->setGeometry(30, 40, 210, 309);
        }
        if (mCopyProgress)
        {
            mCopyProgress->setGeometry(30, 365, 210, 23);
        }
    }
}

void UiIcon::setHaveCopyFlag(bool flag)
{
    mHaveCopy = flag;
    qDebug()<<"mHaveCopy flag = "<< mHaveCopy;
}

bool UiIcon::getHaveCopyFlag()
{
    IDE_TRACE_INT(mHaveCopy);
    return mHaveCopy;
}

void UiIcon::setUpdateCompleteFlag(bool flag)
{
    mUpdateComplete = flag;
}

void UiIcon::setUsbConnected()
{
    IDE_TRACE();
    QStackedWidget *tmpStackedWidget = qobject_cast<QStackedWidget*>(parentWidget()->parentWidget());
    if(!tmpStackedWidget)
    {
        return;
    }
    tmpStackedWidget->setCurrentIndex(3);
}


void UiIcon::showBackUpScreen()
{
    IDE_TRACE_STR(QString("is enter showBackUpScreen"));
    if (!mBackUp)   //
    {
        mBackUp = true;
    }
    else
    {
        IDE_TRACE_STR(QString("already enter showBackUpScreen will return"));
        return;
    }
    mUsbAct.resetCopyParameter();
    setUsbConnected();
    mUsbAct.dirFileSize("/usr/bst/usrfs/");  //
    if (0 >= mUsbAct.getFileNum())   //
    {
        setUsbUI(false, "BackUp resource is empty!");
    }
    else
    {
        mUsbAct.removeFile("/media/sodimas/usrfs_backUp/");
        int flag = mUsbAct.copyFile("/usr/bst/usrfs/", "/media/sodimas/usrfs_backUp/");  //
        qDebug()<<__FUNCTION__<<"() "<<"flag = "<<flag;
        if (0 == flag) // if copy suceess
        {            
            setUsbUI(true, QString());
            if (mTextView)
            {
                model.setStringList(mUsbAct.getFileList(true)); // true is backup
                mTextView->setVisible(true);
                mTextView->setModel(&model);
                mTextView->setSelectionMode(QAbstractItemView::NoSelection);
            }
            mUsbAct.sigStartProgress(true);
        }
        else   // copy failed
        {
            setUsbUI(false, "BackUp resource failed!");
        }
    }
}
