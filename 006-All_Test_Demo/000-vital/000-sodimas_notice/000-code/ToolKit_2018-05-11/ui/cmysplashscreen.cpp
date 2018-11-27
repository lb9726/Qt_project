#include "cmysplashscreen.h"
#include <QTime>
#include <QTimer>
#include <QPalette>
#include <QFont>
#include <QCoreApplication>
#include <QDebug>
#include <QRect>
#include <QFontMetrics>

CMySplashScreen::CMySplashScreen(int time) :
    elapseTime(time),
    runPath(QCoreApplication::applicationDirPath())
{
    bootLogo = new QLabel(this);
    ProgressBar = new QProgressBar(this);
    sysInfo = new QLabel(this);
    appInfo = new QLabel(this);
}

CMySplashScreen::~CMySplashScreen()
{
    qDebug()<<"is call "<<__FUNCTION__;
    ProgressBar->deleteLater();
    ProgressBar = 0;
    sysInfo->deleteLater();
    sysInfo = 0;
    appInfo->deleteLater();
    appInfo = 0;
}

void CMySplashScreen::init()
{
    setBackGround(runPath + "/Theme/sodimas/desktop/bootBg.png");
    setLogoPosition(runPath + "/Theme/sodimas/desktop/logo.png");
    setProgressBarPositon();
    setTextSysInfo("SYS:0.1.k.180507");
    setTextAppInfo("APP:0.9.1.180507");
    setSysAndAppPosition();
}

void CMySplashScreen::setBackGround(QString path)
{
    if (m_direction == 1)
    {
        this->setGeometry(0, 0, 480, 272);
    }
    else if (m_direction == 0)
    {
        this->setGeometry(0, 0, 272, 480);
    }
    qDebug()<<__FUNCTION__ <<" path = "<< path;
    QPixmap tmpixbg(path);
    this->setPixmap(tmpixbg);
}

void CMySplashScreen::setLogoPosition(QString path)
{
    qDebug()<<__FUNCTION__ <<" path = "<< path;
    QPixmap tmpix(path);
    if (NULL != bootLogo)
    {
        if (m_direction == 1)
        {
            bootLogo->setGeometry((480 - tmpix.width())/2, 50 , tmpix.width(), tmpix.height());
        }
        else if (m_direction == 0)
        {
            bootLogo->setGeometry((272 - tmpix.width())/2, 50 , tmpix.width(), tmpix.height());
        }
        bootLogo->setPixmap(tmpix);
    }
}

void CMySplashScreen::setProgressBarPositon()
{
    if (NULL != ProgressBar)
    {
        if (m_direction == 1)
        {
            ProgressBar->setGeometry((480 - 252)/2, 130, 252, 30); //设置进度条的位置
        }
        else if (m_direction == 0)
        {
            ProgressBar->setGeometry((272 - 200)/2, 400, 200, 30); //设置进度条的位置
        }

        //设置进度条的样式
        ProgressBar->setStyleSheet("QProgressBar { border: 5px solid grey;border-radius: 10px; }\
                QProgressBar {color:grey;font:15px;text-align:center; }\
                QProgressBar::chunk {background-color: rgb(202, 165, 14);}");
        //设置进度条的范围
        ProgressBar->setRange(0, 100); //设置进度条的当前进
        ProgressBar->setValue(0);
        generateAscendRandomNumber();
        setProgress();
    }
}

void CMySplashScreen::setTextSysInfo(QString pSys)
{
    if (NULL != sysInfo)
    {
        sysInfo->setText(pSys);
        sysInfo->setStyleSheet("QLabel {color: white}");
    }
}

void CMySplashScreen::setTextAppInfo(QString pApp)
{
    if (NULL != appInfo)
    {
        appInfo->setText(pApp);
        appInfo->setStyleSheet("QLabel {color: white}");
    }
}

void CMySplashScreen::setSysAndAppPosition()
{
    QFont ft;                    // 设置字号
    ft.setPointSize(12);
    if ( NULL != sysInfo)
    {
        sysInfo->setFont(ft);
    }
    if (NULL != appInfo)
    {
        appInfo->setFont(ft);
    }

    QFontMetrics fm(ft);
    QRect rec = fm.boundingRect(sysInfo->text());
    int sysLength = rec.width() + 10;//这个就获得了字符串所占的像素宽度

    QRect rec2 = fm.boundingRect(appInfo->text());
    int appLength = rec2.width()+ 10;//这个就获得了字符串所占的像素宽度

    if (m_direction == 1)
    {
        sysInfo->setGeometry((480 - sysLength)/2, 200, sysLength, 45);
        appInfo->setGeometry((480 - appLength)/2, 218, appLength, 45);
    }
    else if (m_direction == 0)
    {
        sysInfo->setGeometry((272 - sysLength)/2, 200, sysLength, 45);
        appInfo->setGeometry((272 - appLength)/2, 218, appLength, 45);
    }
}

void CMySplashScreen::setDirection(int pDirection)
{
    m_direction = pDirection;
}

void CMySplashScreen::setProgress()
{
    int tempTime = elapseTime/100;
    for(int i = 0; i < 100; ++i)
    {
       QTimer::singleShot(i*tempTime, this, SLOT(slotUpdateProgress()));
    }
}

void CMySplashScreen::slotUpdateProgress()
{
    static int num = 0;
    if (num < 96)
    {
        ProgressBar->setValue(numbersList[num]);
    }
    else
    {
        ProgressBar->setValue(100);
    }
    ++num;
}

void CMySplashScreen::generateAscendRandomNumber()
{
    int i;
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    for(i = 0; i < 99; ++i) //生成100个大小在[0, 100]之间的随机数
    {
        numbersList.append(qrand()%101);
    }
    numbersList.append(100);
    //递增排序
    std::stable_sort(numbersList.begin(), numbersList.end());
}
