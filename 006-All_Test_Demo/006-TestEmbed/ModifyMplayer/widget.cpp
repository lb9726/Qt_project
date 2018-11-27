#include "widget.h"
#include "ui_widget.h"

const QString MediaPath = QString("/usr/bst/usrfs/Theme/sodimas/media/");

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_player = new Mplayer();
    m_liftarvvol = 10;
    m_liftflrvol = 10;
    if (m_player)
    {
        m_player->setBackgroundMuisc("/usr/bst/usrfs/Theme/sodimas/music/M0.wav", 10);
        m_player->startWork();
        m_player->setBackgroundPlayerVolume(10);
        m_player->playBackgroundPlayer();
    }
    connect(&m_TimePlay, &QTimer::timeout, this, &Widget::slot_TimeOut);
    m_TimePlay.start(8000);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::slot_TimeOut()
{
    static int num = 0;
    QString mMusicNumber = QString::number(num);
    if (num > 70)
    {
        num = 0;
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"set num = 0";
    }
    if (m_player)
    {
        QString floorMusic = MediaPath + QString("M%1.wav").arg(mMusicNumber);
        if (41 == num || 42 == num || 43 == num)
        {
            m_player->addMedia(AO_LIFTARV, floorMusic, 1, false, m_liftarvvol);

        }
        else
        {
            m_player->addMedia(AO_LIFTFLR, floorMusic, 1, false, m_liftflrvol);
        }
        ++num;
    }
}
