#include "widget.h"
#include "ui_widget.h"

#include <QCoreApplication>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    mRunPath = QCoreApplication::applicationDirPath();
    QString pSrc = mRunPath + "/CanFestival-3-8bfe0ac00cdb.zip";
    QString pDst = mRunPath + "/000Test/";
    connect(&mZipFunction, &::ZipFunction::progress, this, &Widget::update_Progress, Qt::QueuedConnection);
//    mZipFunction.startUnZip(pSrc, pDst, true, true);
    mZipFunction.startZip(pDst, pSrc, true, true);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::update_Progress(int value)
{
    ui->progressBar->setValue(value);
}
