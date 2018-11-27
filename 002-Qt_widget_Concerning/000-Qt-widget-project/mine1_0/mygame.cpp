#include "mygame.h"
#include "ui_mygame.h"

mygame::mygame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mygame)
{
    ui->setupUi(this);
    length_flag = width_flag = mine_flag = 0;
}

mygame::~mygame()
{
    delete ui;
}
//===================================================
void mygame::setbutton()
{
    if(length_flag == 1 && width_flag == 1 && mine_flag == 1)
        ui->mynewgame->setEnabled("true");
}
void mygame::on_length_textChanged(const QString &arg1)
{
    length_flag = 1;
    setbutton();
}

void mygame::on_width_textChanged(const QString &arg1)
{
    width_flag = 1;
    setbutton();
}

void mygame::on_mine_textChanged(const QString &arg1)
{
    mine_flag = 1;
    setbutton();
}
//=====================================================
void mygame::on_mynewgame_clicked()
{
    length = ui->length->text().toInt();
    width = ui->width->text().toInt();
    mine = ui->mine->text().toInt();
    this->close();
}
