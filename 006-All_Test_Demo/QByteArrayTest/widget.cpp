#include "widget.h"
#include "ui_widget.h"
#include <QByteArray>
#include <QDebug>

#define BVT_STX (0x80)        /* 帧起始字符 */
#define BVT_ETX (0x81)        /* 帧结束字符 */

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QByteArray byteArray("hello world");
    qDebug()<<"byteArray = "<<byteArray<<" "<<byteArray.length();;
    char ch = 0x21;
    byteArray.append(ch);
    qDebug()<<"byteArray = "<<byteArray<<" "<<byteArray.length();
    ch = 0x24;
    byteArray.insert(0, ch);
    qDebug()<<"byteArray = "<<byteArray<<" "<<byteArray.length();

    ch = 0x80;
    byteArray.insert(0, ch);
    qDebug()<<"byteArray = "<<byteArray<<" "<<byteArray.length();
    if ((unsigned char)byteArray.at(0) == BVT_STX)
    {
        qDebug()<<"byteArray.at(0) = 0x80";
    }
    else
    {
        qDebug()<<"byteArray.at(0) != 0x80";
    }
    getByteArrayReference(byteArray);
    qDebug()<<__PRETTY_FUNCTION__<<"byteArray.length = "<<byteArray.length()<<byteArray;
    getByteArrayReference2(byteArray);
    qDebug()<<__PRETTY_FUNCTION__<<"byteArray.length = "<<byteArray.length()<<byteArray;
    mByteArray = QByteArray("this is just test global QByteArray");
    qDebug()<<"mByteArray = "<<mByteArray<<"mByteArray.length() = "<<mByteArray.length();
    QByteArray tmp = mByteArray;
    mByteArray.clear();
    qDebug()<<"tmp = "<<tmp<<"tmp.length() = "<<tmp.length();

    qDebug()<<"mByteArray = "<<mByteArray<<"mByteArray.length() = "<<mByteArray.length();
    tmp.append("123456");
    qDebug()<<"tmp = "<<tmp<<"\n";

    tmp = getByteArrayReference(tmp);
    qDebug()<<"tmp = "<<tmp<<"\n";

    tmp.clear();
    qDebug()<<"tmp.length() = "<<tmp.length();
}

Widget::~Widget()
{
    delete ui;
}

QByteArray& Widget::getByteArrayReference(QByteArray &pArray)
{
    pArray.append("lb2616 is coming!");
    qDebug()<<__PRETTY_FUNCTION__<<"pArray.length = "<<pArray.length()<<pArray;
    return pArray;
}

QByteArray& Widget::getByteArrayReference2(QByteArray &pArray)
{
    QByteArray tmp;
    tmp = pArray;
    tmp.append("lb2616 is coming!");
    qDebug()<<__PRETTY_FUNCTION__<<"pArray.length = "<<tmp.length()<<tmp;
    return pArray;
}
