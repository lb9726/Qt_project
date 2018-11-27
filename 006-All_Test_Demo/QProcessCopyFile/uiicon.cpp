#include "uiicon.h"
#include "ui_uiicon.h"
#include <QDebug>

UiIcon::UiIcon(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiIcon)
{
    ui->setupUi(this);
    mUsbPicture = nullptr;
    mLabelWaring = nullptr;
    mTextView = nullptr;
    mCopyProgress = nullptr;
    mHaveCopy = false;
    mBackUp = false;
    mUpdateComplete = true;
    initComponent();

    mUsbAct.copyFile("/home/libo/Desktop/usrfs", "/home/libo/Desktop/usrfs_new");
    mUsbAct.initProcess();
    mUsbAct.startWork();
}

UiIcon::~UiIcon()
{
    delete ui;
}

void UiIcon::initComponent()
{
    mUsbPicture = ui->label_usbpicture;
    mLabelWaring = ui->label_waring;
    mTextView  = ui->listView;
    mCopyProgress = ui->progressBar;
}

void UiIcon::showCopyScreen()
{
    qDebug()<<__PRETTY_FUNCTION__<<"is enter showCopyScreen";
    if (!mHaveCopy)   //
    {
        mHaveCopy = true;
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"already enter showCopyScreen will return";
        return;
    }
//    mUsbAct.resetCopyParameter();
//    setUsbConnected();
//    mUsbAct.dirFileSize("/media/sodimas/usrfs");  //
    QtSleep(1000);
//    setUsbUI(false, "BackUp is Completing!");
    QtSleep(1000);
//    setUsbUI(false, "Waiting to update...");
    QtSleep(1000);
//    if (0 >= mUsbAct.getFileNum())   //
//    {
//        setUsbUI(false, "U disk has no resource!");
//    }
//    else
//    {
//        //        mUsbAct.removeFile("/usr/bst/usrfs/");
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"delete rm code";
//        int flag = mUsbAct.copyFile("/media/sodimas/usrfs/*", "/usr/bst/usrfs_new/");  //
//        qDebug()<<__PRETTY_FUNCTION__<<"flag = "<<flag;
//        if (0 == flag) // if copy suceess
//        {
//            setUsbUI(true, QString());
//            if (mTextView)
//            {
//                model.setStringList(mUsbAct.getFileList(false));
//                mTextView->setVisible(true);
//                mTextView->setModel(&model);
//                mTextView->setSelectionMode(QAbstractItemView::NoSelection);
//            }
//            mUsbAct.sigStartProgress(false);
//        }
//        else   // copy failed
//        {
//            setUsbUI(false, "copy resource failed!");
//        }
//    }
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
        while (mUpdateComplete)
        {
            mLabelWaring->setText("Update success!");
            if (0 == tmp)
            {
                mLabelWaring->setStyleSheet("QLabel {color:black}");
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

void UiIcon::showBackUpScreen()
{

}

void UiIcon::setUsbRemoveFlag()
{

}
