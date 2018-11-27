#include "usritem.h"
#include "ui_usritem.h"

UsrItem::UsrItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsrItem)
{
    ui->setupUi(this);
}

UsrItem::~UsrItem()
{
    delete ui;
}

void UsrItem::on_pushButton_clicked()
{
    emit sCardRead();
}

void UsrItem::on_pushButton_2_clicked()
{
    emit sMakeUsrCard();
}

void UsrItem::on_btn_destory_clicked()
{
    emit sDestoryCard();
}
