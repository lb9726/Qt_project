#include "widget.h"
#include "ui_widget.h"
#include <QPushButton>
#include <QDebug>
#include "open/display.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);    
    canOpenClient = new OpenClient();
    //connect(canOpenClient,&OpenClient::sigNodeState,this,&Widget::testSlot);
    connect(canOpenClient,SIGNAL(sigNodeState(int)),this,SLOT(testSlot(int)));

    canOpenClient->initCanopenClient();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::testSlot(int state)
{
    qDebug()<<"testSlot"<<state;
}


void Widget::on_pushButton_clicked()
{
    canOpenClient->sendUserPdo();
}
