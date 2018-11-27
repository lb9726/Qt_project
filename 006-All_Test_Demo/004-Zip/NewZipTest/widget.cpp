#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    mRunPath = QCoreApplication::applicationDirPath();
    QString pSrc = "/home/libo/Desktop/Sodimas.zip";
//            QString pSrc = mRunPath + "/CanFestival-3-8bfe0ac00cdb.zip";/home/libo/Desktop
    QString pDst = mRunPath + "/usrfs/";
    connect(&mZipFunction, &::ZipFunction::zipProgress, this, &Widget::zipProgressUpdate, Qt::QueuedConnection);
    connect(&mZipFunction, &::ZipFunction::unZipProgress, this, &Widget::unZipProgressUpdate, Qt::QueuedConnection);
//    mZipFunction.startUnZip(pSrc, pDst, true, true);
    mZipFunction.startZip(pDst, pSrc, true, true);
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

