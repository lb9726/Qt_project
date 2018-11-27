#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include "globalfun.h"
#include <QDesktopWidget>
#include <QWidget>
#include <QSplashScreen>
#include <QProgressBar>

class SplashScreen : public QSplashScreen
{
    Q_OBJECT
    
public:
    explicit SplashScreen(const QPixmap &pixmap = QPixmap(), Qt::WindowFlags f = Qt::SplashScreen);
    ~SplashScreen();

    void ShowMsg(QString pString, Qt::GlobalColor pColor)
    {
        Qt::Alignment topRight = Qt::AlignLeft | Qt::AlignTop;
        showMessage(pString, topRight, pColor);
    }

    void updateProgressBar(int value);

public slots:
    void slot_Progress(QString pMsg, quint32 pPrg);

public:
    QProgressBar    *m_ProgressBar;
};

#endif // SPLASHSCREEN_H
