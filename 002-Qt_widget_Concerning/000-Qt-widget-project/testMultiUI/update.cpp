#include "update.h"
#include "ui_update.h"

update::update(QWidget *parent) :
    QWidget(parent),
      up(new Ui::Form)
{
    up->setupUi(this);
}

update::~update()
{
    delete up;
}
