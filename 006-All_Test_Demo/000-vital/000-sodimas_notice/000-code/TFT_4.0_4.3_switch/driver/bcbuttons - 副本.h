#ifndef BCBUTTONS_H
#define BCBUTTONS_H

#include "dipsio.h"

#define BUTTON_PRESSED      '0'
#define BUTTON_RELEASEED    '1'

#define BUTTON_LIGHT        "1"
#define BUTTON_NONLIGHT     "0"

class BCButton : public QThread
{
    Q_OBJECT
public:
    BCButton(QObject *parent = 0);

    void setGpio(quint8 pKeyNum, quint8 pKeyInNum, quint8 pLedOutNum);
    void run();

signals:
    void sClicked(quint8 pNum);

public:
    quint8             mLedOutState;
    quint8             mKeyInState;

    quint8             mKeyNum;
    quint8             mKeyInGpioNum;
    quint8             mLedOutGpioNum;
};

class BCButtons : public QObject
{
    Q_OBJECT
public:
    BCButtons(QObject *parent = 0);

    void setLedState(quint16 pValue);

    void run();

public slots:
    void slot_Clicked(quint8 pKeyNum);

signals:
    void sNewPressed(quint16 pNewValue);

public:
    BCButton            mButtons[12];
    quint16             mButtonState;
};

#endif // BCBUTTONS_H
