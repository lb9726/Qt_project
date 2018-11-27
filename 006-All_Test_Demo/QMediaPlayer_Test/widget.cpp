#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

const QString MediaPath = QString("/usr/bst/usrfs/Theme/sodimas/media/");

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    mPlayer = NULL;
    init();
}

void Widget::init()
{
    mPlayer = new QMediaPlayer;
    if (mPlayer)
    {
        connect(mPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
        mPlayer->setMedia(QUrl::fromLocalFile(MediaPath + "M43.wav"));
        mPlayer->setVolume(30);
        mPlayer->play();
    }
}

void Widget::positionChanged(qint64 pos)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"pos = "<<pos;
}

Widget::~Widget()
{
    delete ui;
}
