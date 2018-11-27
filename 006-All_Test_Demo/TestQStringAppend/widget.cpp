#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QDebug>
#include <QByteArray>
#include <stdlib.h>
#include <stdio.h>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
//    QByteArray str;
    QString str;
    QString str1;
//    const char *a = "ABCDEDFG0abcdef0";
//    str.append(a);
    char a[10] = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46, '\0', 0x46, 0x47};
    str.append(a);
    qDebug()<<"str = "<< str;

    quint8 b[10] = {0x00, 0x42, 0x43, 0x44, '\0', 0x46, 0x00, 0x46, 0x47};
    str1.append((char *)b);
    qDebug()<<"str1 = "<<str1;
//    unsigned char s;
//    for (int i = 0; i < 10; ++i)
//    {
//        s = 0x40 + i;
//        if (i == 5)
//        {
//            s = 0x00;
//        }
//        str.append((char)s);
//    }

//    int pos = str.indexOf('\0');
//    qDebug()<<"pos = "<<pos;
//    for (int i = 0; i < str.length(); ++i)
//    {
//        if (0x00 == str.at(i))
//        {
//            qDebug()<<"is 0x00";
//            printf("%c", str.at(i));
//        }
//    }
//    qDebug()<<"str = "<<str;
}

Widget::~Widget()
{
    delete ui;
}
