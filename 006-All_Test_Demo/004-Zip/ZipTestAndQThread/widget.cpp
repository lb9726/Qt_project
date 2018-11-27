#include "widget.h"
#include "ui_widget.h"
#include <QCoreApplication>
#include "define.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(&mTmpZip, &::ZipFunction::zipProgress, this, &Widget::zipProgressUpdate, Qt::QueuedConnection);
    connect(&mTmpZip, &::ZipFunction::unZipProgress, this, &Widget::unZipProgressUpdate, Qt::QueuedConnection);

    mRunPath = QCoreApplication::applicationDirPath();
    QString pZip = "/home/libo/Desktop/Sodimas.zip";
//            QString pSrc = mRunPath + "/CanFestival-3-8bfe0ac00cdb.zip";/home/libo/Desktop
    QString pDir = mRunPath;
//    mTmpZip.setZipFilePath(pDir, pZip);
//    mTmpZip.setZipFlag(true, true);
//    QtSleep(200);
//    mTmpZip.start();

    mTmpZip.setZipFilePath(pDir, pZip);
    mTmpZip.setZipFlag(false, true);
    QtSleep(200);
    mTmpZip.start();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::zipProgressUpdate(int value)
{
    ui->zip_progress->setValue(value);
}

void Widget::unZipProgressUpdate(int value)
{
    ui->unzip_progress->setValue(value);
}
