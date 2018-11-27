#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "Xml/mdpparser.h"
#include "Xml/themeparser.h"
#include "Xml/xmlparse.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void callOpenMdp();
    void init();
    void getRunPath();

private:
    QString                     mRunPath;
    Ui::Widget *ui;
    ThemeParser                 *m_themeParser;
    MdpParser                   *m_mdpParser;
    int                          m_admusicvol;
    int                          m_beepvol;
    int                          m_liftarvvol;
    int                          m_liftflrvol;

    QString                      m_lednormalFreq;
    QString                      m_ledbusyFreq;
    QString                      m_lederrorFreq;

    int                          m_rotation;
    int                          m_luminance;
    int                          m_standby;
    int                          m_standbyluminance;
    int                          m_screensave;
    int                          m_screensavehold;

    bool                         m_autotest;
    int                          m_testfreq;
    bool                         m_safOpenningMessage;
    int                          m_layout;
};

#endif // WIDGET_H
