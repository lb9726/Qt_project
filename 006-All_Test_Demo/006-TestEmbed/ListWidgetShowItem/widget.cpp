#include "widget.h"
#include "ui_widget.h"
#include "define.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Widget);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("border: 2px solid black;border-radius: 8px;");

    setGeometry(135, 10, 232, 120);
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->addItem("ni hao");
    ui->listWidget->addItem("hello world");
    ui->listWidget->addItem("ni hao");
    ui->listWidget->addItem("hello world");
    ui->listWidget->addItem("ni hao");
    ui->listWidget->addItem("hello world");
    ui->listWidget->addItem("ni hao");
    ui->listWidget->addItem("hello world");
    ui->listWidget->addItem("ni hao");
    ui->listWidget->addItem("hello world");
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->listWidget->setCurrentRow(ui->listWidget->count() - 1);
    ui->listWidget->setSelectionMode(QAbstractItemView::NoSelection);
//    QtSleep(10000);
//    ui->listWidget->clear();

}

Widget::~Widget()
{
    delete ui;
}
