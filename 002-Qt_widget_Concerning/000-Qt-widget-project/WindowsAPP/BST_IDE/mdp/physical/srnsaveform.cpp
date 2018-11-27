#include "srnsaveform.h"
#include "ui_srnsaveform.h"

SrnSaveForm::SrnSaveForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SrnSaveForm)
{
    ui->setupUi(this);
}

SrnSaveForm::~SrnSaveForm()
{
    delete ui;
}

QLabel *SrnSaveForm::GetLogoLabel()
{
    return ui->label_logo;
}
