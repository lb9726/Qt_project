#include "widget.h"
#include <QDebug>
#include "define.h"
#include <QDir>
#include <stdio.h>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    /*
     * __FILE__显示的路径为相对于可执行所在路径的相对路径
     * 所以可以根据可执行程序所在的绝对路径和__FILE__的相对路径来合成当前文件的绝对路径是什么
    */
    QString tmp(__FILE__);
    qDebug()<<__FILE__<<tmp;
    QDir temDir(tmp);
    QString absDir = temDir.absolutePath();
    IDE_TRACE();
    qDebug()<<QDir::homePath();
    qDebug()<<"absDir = " <<absDir;
    QDir dir;
    qDebug()<<dir.absoluteFilePath("Widget.cpp");
    QString cmd = QString("pwd > %1/%2").arg(QCoreApplication::applicationDirPath()).arg("path.txt");
    qDebug()<<"cmd = "<<cmd;
    system(cmd.toLatin1().data());
}

Widget::~Widget()
{

}

QString Widget::getCurrenFileAbsolutePath(QString appPath, QString curFileRelativePath)
{
    if (curFileRelativePath.contains(".."))
    {

    }
}
