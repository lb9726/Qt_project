#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    label[0] = new GLabel(this,GLabel::Me);
    label[0]->setGeometry(05,10,100,66);
    label[0]->setOurText(tr("你好!"));
    label[0]->show();

    label[1] = new GLabel(this,GLabel::She);
    label[1]->setGeometry(5,label[0]->y()+label[0]->height(),100,66);
    label[1]->setOurText(tr("你好!"));
    label[1]->show();

    label[2] = new GLabel(this,GLabel::Me);
    label[2]->setGeometry(5,label[1]->y()+label[1]->height(),100,66);
    label[2]->setOurText(tr("我们好像在哪里见过？\n好像那是一个秋天夕阳西下，你美得让我不敢和你说话，你经过我时风起浮动我的发\n"));
    label[2]->show();

    label[3] = new GLabel(this,GLabel::She);
    label[3]->setGeometry(5,label[2]->y()+label[2]->height(),100,66);
    label[3]->setOurText(tr("？"));
    label[3]->show();

    label[4] = new GLabel(this,GLabel::Me);
    label[4]->setGeometry(5,label[3]->y()+label[3]->height(),100,66);
    label[4]->setOurText(tr("我们好像在哪见过你记得吗？好像那是一个秋天夕阳西下，你美得让我不敢和你说话，你经过我时风起浮动我的发"));
    label[4]->show();

    label[5] = new GLabel(this,GLabel::She);
    label[5]->setGeometry(5,label[4]->y()+label[4]->height(),100,66);
    label[5]->setOurText(tr("银杏还是会黄的，雪也会再次染白整个世界。安好"));
    label[5]->show();
}

Dialog::~Dialog()
{
    delete ui;
}
