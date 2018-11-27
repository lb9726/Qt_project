#include "introducemine.h"
#include "ui_introducemine.h"

IntroduceMine::IntroduceMine(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IntroduceMine)
{
    ui->setupUi(this);
}

IntroduceMine::~IntroduceMine()
{
    delete ui;
}
