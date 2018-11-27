#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    musicPath(QString())
{
    ui->setupUi(this);
    init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    m_player = new Mplayer();
    musicPath = QString("/usr/bst/usrfs/Theme/sodimas/music/M0.mp3");
    IDE_TRACE_STR(musicPath);
    m_player->setBackgroundMuisc(musicPath, 30);
    m_player->startWork();
    m_player->playBackgroundPlayer();
}
