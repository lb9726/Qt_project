#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QFile>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &Widget::onClicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onClicked()
{
    copyFile("1", "2");
}

bool Widget::copyFile(const QString &srcfile, const QString &destfile)
{
    Q_UNUSED(srcfile);Q_UNUSED(destfile);
    QFileDialog fd;
    fd.setFileMode(QFileDialog::AnyFile);
    //fd.setNameFilter("Audio(*.wav)");
    QString RarPath = fd.getOpenFileName();
    qDebug()<<"RarPath = "<< RarPath;
    if(!RarPath.isEmpty())
    {
        RarPath.replace("/", "\\");
        qDebug()<<"RarPath = "<< RarPath;
        QString RarPathTo = "C:\\Users\\yonghao\\Desktop\\TestGifPlayer\\"/* QDir::currentPath()*/;
        QFileInfo info(RarPath);
        if (info.exists())
        {
            qDebug()<<"file is exists";
        }
        else
        {
            qDebug()<<"file is not exists";
        }
        QDir destDir(RarPathTo);
        if (destDir.exists())
        {
            qDebug()<<"dir is exists";
        }
        else
        {
            qDebug()<<"dir is not exists";
        }
        qDebug()<<"RarPath = "<<RarPath;
        qDebug()<<"RarPathTo = "<<RarPathTo;
//        QFile cpfile(RarPath);
//        bool flag = cpfile.copy(RarPathTo + RarPath.split("\\").last());
        bool flag = QFile::copy(RarPath, RarPathTo + RarPath.split("\\").last());
        qDebug()<<"flag = "<< flag;
    }
    return true;
}
