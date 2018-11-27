#include "splashform.h"
#include "ui_splashform.h"

SplashForm::SplashForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SplashForm)
{
    ui->setupUi(this);
}

SplashForm::~SplashForm()
{
    delete ui;
}
