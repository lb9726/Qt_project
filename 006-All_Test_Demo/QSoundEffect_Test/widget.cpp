#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    mPlayWav = new PlayWav;
    if (mPlayWav)
    {
        mPlayWav->initPlayer();
        mPlayWav->setLoopCount(-1);
        mPlayWav->setVolume(0.3f);
        mPlayWav->play();
    }
}

Widget::~Widget()
{
    delete ui;
}
