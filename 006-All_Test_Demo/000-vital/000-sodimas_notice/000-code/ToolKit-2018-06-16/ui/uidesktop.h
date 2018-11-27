#ifndef UIDESKTOP_H
#define UIDESKTOP_H
#include <QWidget>
#include <QDomElement>
#include <QLabel>
#include "splashform.h"
#include "messagebar.h"

class UiDesktop : public QWidget
{
    Q_OBJECT
public:
    UiDesktop(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    void Init(QDomElement pElement);

    void paintEvent(QPaintEvent *event);
    void setSplashForm(SplashForm *pSplash);
    void setScreenText(QLabel *mlabel);
    void setMessageBar(MessageBar *pMessageBar);

public:
    QPixmap  mHorizontalPixmap;
    QPixmap  mVerticalPixmap;

private:
    SplashForm      *mSplashForm;
    MessageBar   *m_messageBar;
};

#endif // UIDESKTOP_H
