#include "adminitem.h"
#include "ui_adminitem.h"

AdminItem::AdminItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminItem)
{
    ui->setupUi(this);
}

AdminItem::~AdminItem()
{
    delete ui;
}

void AdminItem::on_pushButton_clicked()
{
    emit sMadeCardsNum();
}

void AdminItem::on_pushButton_2_clicked()
{
    emit sMakeAdminCard();
}

void AdminItem::on_btn_destory_clicked()
{
    emit sDestoryCard();
}

void AdminItem::on_pushButton_3_clicked()
{
    emit setPassword();
}
