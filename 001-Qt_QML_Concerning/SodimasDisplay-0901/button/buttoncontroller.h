#ifndef BUTTONCONTROLLER_H
#define BUTTONCONTROLLER_H

#include <QThread>
#include <QMutex>
#define ONBOARD_BUTTON_NUMBER 12

#define EXTEND_BUTTON_NUMBER 24

#define BUTTON_NUMBER 36

class ButtonController : public QThread
{
    Q_OBJECT
public:
    explicit ButtonController(QThread *parent = 0);
    void init();
    bool startWork();
    bool initButtonStatu();
    void run();
    ///board input button
    bool openInputBoardbuttonFiles();
    bool loopBoardbutton(int index);
    ///board output button
    bool openoutputBoardbuttonFiles();
    bool onboardlightCtrl(int index,bool isLighton);
    ///Extend board
    bool initExtendboard();
    void parseSpiData();
    bool isExtendButtonPressed(quint8 index);
    void setExtendButtonPressed(int index, bool pressed);    
    void buildTxBuffer(int index, bool isOn);
    unsigned char bitMaskCaq(unsigned char value,int index, bool isSet);
signals:
    ///board input button
    void sigBoardButtonPressed(int index);
    void sigBoardButtonReleased(int index);
    ///Extend board
    void sigExtendButtonPressed(int index);
    void sigButtonBeep();
public slots:
    ///board output button
    void sltLightOnBoardbutton(int index,bool isBuzz);
    void sltLightOffBoardbutton(int index);
public:
    ///board input button
    QList<QString> m_boardInputbuttonGpioNumber;
    int m_boardinputButtonFilefd[ONBOARD_BUTTON_NUMBER];
    int m_buttoninputStateBuffer[ONBOARD_BUTTON_NUMBER];
    ///board output button
    QList<QString> m_buttonOutputGpioNumber;
    int m_outputButtonFilefd[ONBOARD_BUTTON_NUMBER];
    int  m_outputButtonStateBuffer[ONBOARD_BUTTON_NUMBER];//0 _ light off, 1 _ light on
    ///Extend board
    quint8 tx_buf_extend[3];//1 _ light off, 0 _ light on
    quint8 rx_buf_extend[3];
    int    m_extendfd ;
    bool   extendButtonStatu[EXTEND_BUTTON_NUMBER];

    int   extendSendByteCount;

    QMutex m_mutex;
};

#endif // BUTTONCONTROLLER_H
