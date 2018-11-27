#include "aloneprogessbar.h"
#include "ui_aloneprogessbar.h"

AloneProgessBar::AloneProgessBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AloneProgessBar)
{
    ui->setupUi(this);
    ui->progressBar->setValue(88);
    ui->progressBar->setWindowOpacity(0.1);
    this->setWindowOpacity(0.2);
//    ui->progressBar->setGeometry(20, 20, 180, this->height());
}

AloneProgessBar::~AloneProgessBar()
{
    delete ui;
}
