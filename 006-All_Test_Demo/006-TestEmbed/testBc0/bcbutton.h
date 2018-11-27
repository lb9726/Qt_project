//#ifndef BCBUTTONS_H
//#define BCBUTTONS_H
//#include <QThread>
//#include <QFile>
//#include <QString>

//#define GPIO_VALUE(x)       QString("/sys/class/gpio/gpio%1/value").arg(x)      // 输入输出电平值设备
//#define GPIO_EXPORT         "/sys/class/gpio/export"              // GPIO 设备导出设备
//#define GPIO_DIRECT(x)      QString("/sys/class/gpio/gpio%1/direction").arg(x)  // GPIO 输入输出控制设备

//#define BUTTON_PRESSED      '0'
//#define BUTTON_RELEASEED    '1'

//#define BUTTON_LIGHT        "1"
//#define BUTTON_NONLIGHT     "0"


//class BCButton : public QThread
//{
//    Q_OBJECT
//public:
//    BCButton(QObject *parent = 0);

//    void setGpio(quint8 pKeyNum, quint8 pKeyInNum, quint8 pLedOutNum);
//    void run();

//signals:
//    void sClicked(int pNum);

//public:
//    quint8             mLedOutState;
//    quint8             mKeyInState;

//    quint8             mKeyNum;
//    quint8             mKeyInGpioNum;
//    quint8             mLedOutGpioNum;
//};

//class SpecialBtns : public QThread
//{
//    Q_OBJECT
//public:
//    SpecialBtns(QObject *parent = 0);

//    void run();
//    void setLedState(quint16 pValue);

//signals:
//    void sClicked(int pNum);

//public:
//    quint8  mKeyInState;
//    quint16 mLedoutState;
//};

//class BCButtons : public QThread
//{
//    Q_OBJECT
//public:
//    BCButtons(QObject *parent = 0);
//    ~BCButtons();
//    void setLedState(quint16 pValue);

////    void run();

//public slots:
//    void slot_Clicked(quint8 pKeyNum);

//signals:
//    void sNewPressed(quint16 pNewValue);

//private:
//    BCButton            *mButtons[12];
//    quint16             mButtonState;
//};

//#endif // BCBUTTONS_H
