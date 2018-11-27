#include "newwiner.h"
#include "ui_newwiner.h"
#include <QFile>
#include <QTextStream>
#include  <QList>
#include <QMessageBox>

newwiner::newwiner(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newwiner)
{
    ui->setupUi(this);

}

newwiner::~newwiner()
{
    delete ui;
}

void newwiner::on_lineEdit_textChanged(const QString &arg1)
{
    ui->ok->setEnabled("ture");
}

void newwiner::setwintext(QString a, QString b, int c)
{
    time = a;  mine = b; mine_num = c;
    QString winstr ="恭喜您获得了" +mine+","+QString::number(mine_num)+"雷的胜利!\n用时:"+time;
    ui->winlable->setText(winstr);
}

void newwiner::on_ok_clicked()
{
    //保存信息
    QFile file("gamewin.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        return;
    QTextStream out(&file);
   // QString name = ui->lineEdit->text();
    QString num = QString::number(mine_num);
    //out<<name<<" "<<mine<<" "<<num<<" "<<time<<endl;
    this->close();
}

void newwiner::on_quit_clicked()
{
    this->close();
}
