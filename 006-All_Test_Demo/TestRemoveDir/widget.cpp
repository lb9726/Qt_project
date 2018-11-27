#include "widget.h"
#include "ui_widget.h"
#include <QDir>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    mTimeCount.start();
    RemoveDir("/usr/bst/usrfs/");
    int overtime = mTimeCount.elapsed();
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"overtime = "<<overtime;
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::RemoveDir(const QString path)
{
    qDebug()<<"is call "<<__PRETTY_FUNCTION__<<"path = "<<path;
    if (path.isEmpty())
    {
        return false;
    }
    QDir dir(path);
    if(!dir.exists())
    {
        qDebug()<<"path is not exists";
        return true;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
    QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
    foreach (QFileInfo file, fileList)
    { //遍历文件信息
        if (file.isFile())
        { // 是文件，删除
            file.dir().remove(file.fileName());
        }
        else
        { // 递归删除
            RemoveDir(file.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath()); // 删除文件夹
}
