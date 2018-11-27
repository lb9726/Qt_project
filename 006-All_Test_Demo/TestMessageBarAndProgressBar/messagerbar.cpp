#include "messagerbar.h"
#include "ui_messagerbar.h"

MessagerBar::MessagerBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessagerBar)
{
    ui->setupUi(this);
}

MessagerBar::~MessagerBar()
{
    delete ui;
}
