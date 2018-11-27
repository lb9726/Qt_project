#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QLineEdit>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    MyLineEdit* tmpLineEdit = new MyLineEdit();

    tmpLineEdit->setParent(this);
    tmpLineEdit->setGeometry(10, 10, 380, 35);
    tmpLineEdit->setPlaceholderText(QString::fromLocal8Bit("请输入搜索内容"));

    tmpLineEdit->setStyleSheet("QLineEdit {border: 4px solid rgb(41, 57, 85);border-radius: 13px;\
        selection-background-color: green;font-size: 14px ;}QLineEdit:hover {border: 1px solid blue;}");

    connect(tmpLineEdit, &MyLineEdit::doubleClicked, this, &Widget::slot_Print);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::slot_Print()
{
    qDebug()<<"myLineEdit is double clicked";
}
