#include "playerdialog.h"

/***********************************************************************
函数名字：playerDialog(QWidget *parent)
函数功能：playerDialog构造函数
函数流程：
    初始化成员
************************************************************************/
playerDialog::playerDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle("游戏对话");
    label = new QLabel("\t恭喜你刷新了纪录！\n现在你可以砸掉电脑，或者输入你的名字：");
    lineEdit = new QLineEdit;
    pushButtonOK = new QPushButton("确定");
    this->connect(pushButtonOK,SIGNAL(clicked()),this,SLOT(BtnOKClicked()));
    this->connect(pushButtonOK,SIGNAL(clicked()),this,SLOT(close()));

    glayout = new QGridLayout(this);
    glayout->addWidget(label,0,0,1,2);
    glayout->addWidget(lineEdit,1,0,1,2);
    glayout->addWidget(pushButtonOK,2,0,1,2);
}

/***********************************************************************
函数名字：BtnOKClicked()
函数功能：（槽）OK按钮被点击槽
函数流程：
    接收对话框输入的玩家名
    发送玩家名
************************************************************************/
void playerDialog::BtnOKClicked()//（槽）OK按钮被点击槽
{
    QString playername = this->lineEdit->text();
    if(playername.isEmpty())
        playername = "没输入名字的脑残玩家";
    emit sendPlayerName(playername);//发送玩家（信号）
}
