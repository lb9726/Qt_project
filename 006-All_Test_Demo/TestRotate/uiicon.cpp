#include "uiicon.h"
#include "mainwindow.h"

UiIcon::UiIcon(QWidget* parent, Qt::WindowFlags f)
    :QWidget(parent,f)
{
    mIconNum = -1;
    mIcon = 0;

    mTextLine1 = 0;
    mTextLine2 = 0;

    mBigTextLine1 = 0;
    mBigTextLine2 = 0;
    mBigTextLine3 = 0;
    mBigTextLine4 = 0;
    curStackWidgetIndex = 0;
    preStackWidgetIndex = 0;
    mHaveCopy = false;
//    connect(&mUsbAct, &UsbAction::sigUpdateProgress, this, &UiIcon::updateProgress);
}

void UiIcon::Init(QDomElement pElement)
{
    mDomElement = pElement;

    mIcon = this->findChild<QLabel *>("label_image");
    mTextLine1 = this->findChild<QLabel *>("label_text_1");
    mTextLine2 = this->findChild<QLabel *>("label_text_2");
    QStackedWidget *tmpStackedWidget = qobject_cast<QStackedWidget*>(parentWidget()->parentWidget());
    if(tmpStackedWidget)
    {
        QWidget *tmpBigPage = tmpStackedWidget->widget(1);
        mBigTextLine1 = tmpBigPage->findChild<QLabel *>("label_bigtext_1");
        mBigTextLine2 = tmpBigPage->findChild<QLabel *>("label_bigtext_2");
        mBigTextLine3 = tmpBigPage->findChild<QLabel *>("label_bigtext_3");
        mBigTextLine4 = tmpBigPage->findChild<QLabel *>("label_bigtext_4");

        mBigTextLine1->setAlignment(Qt::AlignCenter);
        mBigTextLine2->setAlignment(Qt::AlignCenter);
        mBigTextLine3->setAlignment(Qt::AlignCenter);
        mBigTextLine4->setAlignment(Qt::AlignCenter);

        QWidget *tmpBigPage2 = tmpStackedWidget->widget(2);
        mBlackBack = tmpBigPage2->findChild<QLabel *>("label_blackground");
        if (mBlackBack)
        {
            IDE_TRACE_STR(QString("mBlackBack is not null"));
            QPixmap tmp("/usr/bst/usrfs/Theme/sodimas/desktop/bootBg.png");
            mBlackBack->setPixmap(tmp);
        }
        QWidget *tmpPage3 = tmpStackedWidget->widget(3);
        mUsbPicture = tmpPage3->findChild<QLabel *>("label_usbpicture");
        mLabelWaring = tmpPage3->findChild<QLabel *>("label_waring");
        mTextView = tmpPage3->findChild<QListView *>("listView");
        mCopyProgress = tmpPage3->findChild<QProgressBar *>("progressBar");
    }

    bool tmpFlag = isVisible();
    QRect tmpRect = gUiLoader->m_themeParser->getComRect(gUiLoader->m_themeLayout, pElement, gUiLoader->m_themeDirection);
    IDE_TRACE_INT(mTextLine1->height());
    IDE_TRACE_INT(mTextLine2->height());
    IDE_TRACE_INT(tmpRect.height());
    tmpRect.setHeight(tmpRect.height() + mTextLine1->height() + mTextLine1->height());
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
//        IDE_TRACE_STR(tmpTagName);
        int tmpNum = tmpTagName.mid(2).toInt();
//        IDE_TRACE_INT(tmpNum);
//        IDE_TRACE_STR(tmpThemePath + tmpElement.text());
        mRcHash.insert(tmpNum, tmpThemePath + tmpElement.text());
    }

    updateIcon(); //确保更改图片后，能立即刷新到当前的显示内容上
}

void UiIcon::updateIcon()
{
    mIcon->setPixmap(mRcHash.value(mIconNum));

    //如果是小屏文本，则需要同步显示功能图标对应的文本
    QStackedWidget *tmpStackedWidget = qobject_cast<QStackedWidget*>(parentWidget()->parentWidget());
    if(tmpStackedWidget && tmpStackedWidget->currentIndex() == 0)
    {
        QDomElement tmpRcElement = mDomElement.firstChildElement("resource");
        QDomElement tmpElement =tmpRcElement.firstChildElement(QString("rc%1").arg(mIconNum));
        if(!tmpElement.isNull())
        {
            QString tmpText = tmpElement.attribute("text");
            QStringList tmpList = tmpText.split(";", QString::SkipEmptyParts);  //忽略空格
            if(tmpList.count() >= 1)
            {
                mTextLine1->setText(tmpList.at(0));
            }
            if(tmpList.count() >= 2)
            {
                mTextLine2->setText(tmpList.at(1));
            }
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
    mTextLine1->setText(pText1);
    mTextLine2->setText(pText2);

    mBigTextLine1->setText(pText1);
    mBigTextLine2->setText(pText2);
    mBigTextLine3->setText(pText3);
    mBigTextLine4->setText(pText4);
}

void UiIcon::setBigSrnMode(bool pFlag)
{
    QStackedWidget *tmpStackedWidget = qobject_cast<QStackedWidget*>(parentWidget()->parentWidget());
    if(!tmpStackedWidget)
        return;
    if(pFlag)
    {
        tmpStackedWidget->setCurrentIndex(1);
    }
    else
    {
        tmpStackedWidget->setCurrentIndex(0);
    }
}

int UiIcon::getStackWidgetIndex()
{
    QStackedWidget *tmpStackedWidget = qobject_cast<QStackedWidget*>(parentWidget()->parentWidget());
    if(!tmpStackedWidget)
    {
        return -1;
    }
    preStackWidgetIndex = tmpStackedWidget->currentIndex();
    return curStackWidgetIndex;
}

void UiIcon::setBlackGround(int index)
{
    IDE_TRACE_INT(index);
    QStackedWidget *tmpStackedWidget = qobject_cast<QStackedWidget*>(parentWidget()->parentWidget());
    if(!tmpStackedWidget)
    {
        return;
    }
    if (index > 2)
    {
        index = 2;
    }
    tmpStackedWidget->setCurrentIndex(index);
}

//void UiIcon::showCopyScreen()
//{
//    if (!mHaveCopy)   // U盘插入会检测到2次，排除这种情况,U盘移除时恢复为false
//    {
//        mHaveCopy = true;
//    }
//    else
//    {
//        return;
//    }
//    setUsbConnected();
//    mUsbAct.dirFileSize("/media/sodimas/resource");  // 设置文件夹路径
//    QPixmap mUsbPic("/usr/bst/usrfs/Theme/sodimas/usb.png");
//    // 判断文件夹中的文件的个数
//    if (0 >= mUsbAct.getFileNum())   // 没有对应的文件夹或者对应的文件夹下没有文件
//    {
//        if (mUsbPicture)
//        {
//            mUsbPicture->setPixmap(mUsbPic);
//            mUsbPicture->setScaledContents(true);  // 设置自动缩放，不然图片显示不清晰
//            mUsbPicture->show();
//        }
//        if (mLabelWaring)
//        {
//            mLabelWaring->setText("U disk has no resource, Please confirm!");
//        }
//        if (mCopyProgress)
//        {
//            mCopyProgress->setVisible(false);
//        }
//        if (mTextView)
//        {
//            mTextView->setVisible(false);
//        }
//    }
//    else
//    {
//        mUsbAct.copyFile("/media/sodimas/resource/", "/usr/bst/usrfs/");  // 执行拷贝文件
//        QStringList tmp = mUsbAct.readFile();
//        qDebug()<<"mUsbAct.readFile() = "<< tmp;
//        mUsbAct.sigStartProgress();
//        if (mUsbPicture)
//        {
//            mUsbPicture->setPixmap(mUsbPic);
//            mUsbPicture->setScaledContents(true);  // 设置自动缩放，不然图片显示不清晰
//            mUsbPicture->show();
//        }
//        if (mLabelWaring)
//        {
//            mLabelWaring->setVisible(false);
//        }
//        if (mCopyProgress)
//        {
//            mCopyProgress->setVisible(true);
//        }
//        if (mTextView)
//        {
//            model.setStringList(tmp);
//            mTextView->setVisible(true);
//            mTextView->setModel(&model);
//            mTextView->setSelectionMode(QAbstractItemView::NoSelection);
//        }
//    }
//}

void UiIcon::updateProgress(int num)
{
    if (mCopyProgress)
    {
        mCopyProgress->setValue(num);
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

void UiIcon::setUsbRemoveFlag()
{
    setHaveCopyFlag(false);
}

void UiIcon::setHaveCopyFlag(bool flag)
{
    mHaveCopy = flag;
    qDebug()<<"mHaveCopy flag = "<< mHaveCopy;
}

void UiIcon::setTimeAndDest(QString time, QString dest)
{
    IDE_TRACE_STR(time);
    mTimeText->setText(time);
    mDestText->setText(dest);
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

