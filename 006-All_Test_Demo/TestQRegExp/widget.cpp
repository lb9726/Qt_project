#include "widget.h"
#include "ui_widget.h"
#include <QRegExp>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
//    QString str = "1720 * 1080";
    QRegExp rx("(\\d+)");  // 匹配数字，可以提取1720和1080
    int pos = 0;
    QString str = "T4u.png 6ju9j-060";
    while ((pos = rx.indexIn(str, pos)) != -1)
    {
        qDebug()<<rx.cap(0).toInt();
        pos += rx.matchedLength();
    }
}
