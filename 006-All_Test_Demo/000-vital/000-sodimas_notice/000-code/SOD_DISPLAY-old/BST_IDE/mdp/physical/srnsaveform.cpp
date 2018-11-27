#include "srnsaveform.h"
#include "ui_srnsaveform.h"

SrnSaveForm::SrnSaveForm(QWidget *parent) :
    QWidget(parent)
{
    //ui->setupUi(this);
    mLabel = new QLabel(this);
    mLabel->resize(this->width(),this->height());
}

SrnSaveForm::~SrnSaveForm()
{
    //delete ui;
}

QLabel *SrnSaveForm::GetLogoLabel()
{
    return mLabel;
}

