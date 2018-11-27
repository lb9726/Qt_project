#include "splashscreen.h"
#include <QPainter>

SplashScreen::SplashScreen(const QPixmap &pixmap, Qt::WindowFlags f) :
    QSplashScreen(pixmap, f)
{
    setFont(QFont("Arial", 10, QFont::DemiBold, false));
    m_ProgressBar = 0;
    int w = pixmap.width();
    int h = pixmap.height();
    if(w <= 0)
        w = 800;
    if(h <= 0)
        h = 70;
    QPoint tmpCenter = QApplication::desktop()->geometry().center();
    setGeometry(QRect(tmpCenter - QPoint(w/2, h/2), QSize(w,h)));
}

SplashScreen::~SplashScreen()
{
}

void SplashScreen::slot_Progress(QString pMsg, quint32 pPrg)
{
    updateProgressBar(pPrg);
    ShowMsg(pMsg, Qt::cyan);
}

void SplashScreen::updateProgressBar(int value)
{
    if(m_ProgressBar == 0)
    {
        m_ProgressBar = new QProgressBar(this);
        QSize size = this->size();
        m_ProgressBar->setGeometry(20, size.height()-30, size.width()-40, 20);
    }
    if(value == 0 || value == 100)
        m_ProgressBar->hide();
    else
    {
        m_ProgressBar->show();
        m_ProgressBar->setValue(value);
    }
}

