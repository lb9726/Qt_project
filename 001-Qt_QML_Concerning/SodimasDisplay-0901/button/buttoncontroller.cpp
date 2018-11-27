#include "buttoncontroller.h"

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/types.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>
#include <linux/spi/spidev.h>

#include "define.h"

#define	BUTTON_PRESSED 1
#define	BUTTON_UNPRESSED 0

static unsigned char lightOn_value = '1';
static unsigned char lightOff_value = '0';

ButtonController::ButtonController(QThread *parent)
{
    //init();
}

void ButtonController::init(){
    ///board input button
    m_boardInputbuttonGpioNumber.clear();
    m_boardInputbuttonGpioNumber.append("/sys/class/gpio/gpio128/value");
    m_boardInputbuttonGpioNumber.append("/sys/class/gpio/gpio129/value");
    m_boardInputbuttonGpioNumber.append("/sys/class/gpio/gpio130/value");
    m_boardInputbuttonGpioNumber.append("/sys/class/gpio/gpio131/value");
    m_boardInputbuttonGpioNumber.append("/sys/class/gpio/gpio132/value");
    m_boardInputbuttonGpioNumber.append("/sys/class/gpio/gpio133/value");
    m_boardInputbuttonGpioNumber.append("/sys/class/gpio/gpio134/value");
    m_boardInputbuttonGpioNumber.append("/sys/class/gpio/gpio135/value");
    m_boardInputbuttonGpioNumber.append("/sys/class/gpio/gpio136/value");
    m_boardInputbuttonGpioNumber.append("/sys/class/gpio/gpio137/value");
    m_boardInputbuttonGpioNumber.append("/sys/class/gpio/gpio48/value");
    m_boardInputbuttonGpioNumber.append("/sys/class/gpio/gpio49/value");
    for(int i=0;i<ONBOARD_BUTTON_NUMBER;i++)
    {
        m_outputButtonFilefd[i] = 0;
        m_buttoninputStateBuffer[i] = BUTTON_UNPRESSED;
    }
    ///board output button ,Pin Nuber: 32 33 34 35 36 37 38 39 40 41 42 43
    m_buttonOutputGpioNumber.clear();
    for(int i=32;i<44;i++)
    {
        m_buttonOutputGpioNumber.append(QString("/sys/class/gpio/gpio%1/value").arg(i));
    }

    for(int i=0;i<ONBOARD_BUTTON_NUMBER;i++)
    {

        m_outputButtonFilefd[i] = 0;
        m_outputButtonStateBuffer[i] = BUTTON_UNPRESSED;
    }
    ///Extend board
    tx_buf_extend[0] = 0x00;
    tx_buf_extend[1] = 0x00;
    tx_buf_extend[2] = 0x00;

    rx_buf_extend[0] = 0xff;
    rx_buf_extend[1] = 0xff;
    rx_buf_extend[2] = 0xff;
    for(int i = 0; i < EXTEND_BUTTON_NUMBER; i++ ){
        extendButtonStatu[i] = false;
    }
    IDE_TRACE();
    extendSendByteCount = 0;
}

bool ButtonController::startWork()
{
    openoutputBoardbuttonFiles();
    if(!initExtendboard()){
        IDE_TRACE();
    }
    start();//QThread::HighestPriority
    initButtonStatu();
}

bool ButtonController::initButtonStatu(){
    //    for(int i = 0; i < EXTEND_BUTTON_NUMBER; i++ ){
    //        m_buttoninputStateBuffer(i);
    //    }
}

void ButtonController::run(){
    int res;
    while(1){
        ///Onboard Input Button
        for(int i = 0; i < ONBOARD_BUTTON_NUMBER; i++){
            loopBoardbutton(i);
//            bool flag = loopBoardbutton(i);
//            if(flag){
//                emit sigBoardButtonPressed(i+1);
//                printf("%d button pressed\n",i);
//            }
        }
        ///Extend Button
        m_mutex.lock();
        write(m_extendfd,tx_buf_extend,sizeof(tx_buf_extend));///sizeof(tx_buf_extend));
        m_mutex.unlock();
        res = read(m_extendfd,rx_buf_extend,sizeof(rx_buf_extend));
        if(res == sizeof(rx_buf_extend)){
            parseSpiData();
        }
        ///msleep(20);
    }
}

///board input button
bool ButtonController::openInputBoardbuttonFiles()
{
    if(m_boardInputbuttonGpioNumber.count() != ONBOARD_BUTTON_NUMBER){
        IDE_TRACE_STR("Onboard button number error.");
        return false;
    }

    for(int i=0;i< ONBOARD_BUTTON_NUMBER ;i++){
        QByteArray fileName;
        fileName.append(m_boardInputbuttonGpioNumber.at(i));
        int fd = open(fileName.data(),O_RDONLY | O_NONBLOCK);
        if(fd == -1)
        {
            IDE_TRACE_STR("Open gpio "+QString(fileName)+" failed");
            return false;
        }
        else
        {
            m_boardinputButtonFilefd[i] = fd;
        }
    }
    return true;
}
bool ButtonController::loopBoardbutton(int index)
{
    int ret;
    char bcValue[2];
    QByteArray ba;
    bool flag = false;
    if(index >= 0 && index < ONBOARD_BUTTON_NUMBER){
        ba.append(m_boardInputbuttonGpioNumber.at(index));
        int fd = open(ba.data(),O_RDONLY);
        if(fd == -1)
        {
            IDE_TRACE_STR("Open gpio  failed");
            flag = false;
        }
        else
        {
            ret = read(fd,bcValue,2);
            ///printf("onboard button :%d,%x,",index,bcValue[0]);
            if(ret>0){
                if(bcValue[0] == '0' && !m_buttoninputStateBuffer[index])
                {
                    if(!m_outputButtonStateBuffer[index]){//Decide by hardware of this problem
                        m_buttoninputStateBuffer[index] = BUTTON_PRESSED;
                        emit sigBoardButtonPressed(index+1);
                        flag = true;
                    }
                }else if(bcValue[0] == '1' && m_buttoninputStateBuffer[index]){
                    m_buttoninputStateBuffer[index] = BUTTON_UNPRESSED;
                    emit sigBoardButtonReleased(index+1);
                    flag = false;
                }else{
                    flag = false;
                }
            }else{
                flag = false;
            }
            close(fd);
        }
    }else{
        flag = false;
    }
    return flag;
}

///board output button
bool ButtonController::openoutputBoardbuttonFiles()
{
    if(m_buttonOutputGpioNumber.count() != ONBOARD_BUTTON_NUMBER){
        IDE_TRACE_STR("Onboard button number error.");
        return false;
    }

    for(int i=0;i< ONBOARD_BUTTON_NUMBER ;i++){
        QByteArray fileName;
        fileName.append(m_buttonOutputGpioNumber.at(i));
        int fd = open(fileName.data(),O_WRONLY | O_NONBLOCK);
        if(fd == -1)
        {
            IDE_TRACE_STR("Open gpio "+QString(fileName)+" failed");
            return false;
        }
        else
        {
            m_outputButtonFilefd[i] = fd;
        }
    }

    return true;
}

void ButtonController::sltLightOnBoardbutton(int index,bool isBuzz){
    //IDE_TRACE_INT(isBuzz);
    index = index -1;//According to canopenclient
    //IDE_TRACE_INT(index);
    if(index >= 0 && index < ONBOARD_BUTTON_NUMBER){
        if(!m_outputButtonStateBuffer[index]){///light still off
//            if(isBuzz){
//                emit sigButtonBeep();
//                IDE_TRACE();
//            }else{
//                IDE_TRACE();
//            }
            onboardlightCtrl(index,true);
            m_outputButtonStateBuffer[index] = 1;
        }
    }else if(index > (ONBOARD_BUTTON_NUMBER-1) && index < BUTTON_NUMBER){
        if(!isExtendButtonPressed(index-ONBOARD_BUTTON_NUMBER)){

            setExtendButtonPressed(index-ONBOARD_BUTTON_NUMBER,true);
            buildTxBuffer(index-ONBOARD_BUTTON_NUMBER,true);
        }
    }else{
        ///IDE_TRACE();
    }
}

void ButtonController::sltLightOffBoardbutton(int index){

    index = index -1;//According to canopenclient
    IDE_TRACE_INT(index);
    if(index >= 0 && index < ONBOARD_BUTTON_NUMBER){
        if(m_outputButtonStateBuffer[index]){///light still on
            onboardlightCtrl(index,false);
            m_outputButtonStateBuffer[index] = 0;
        }
    }else if(index > (ONBOARD_BUTTON_NUMBER-1) && index < BUTTON_NUMBER){
        setExtendButtonPressed(index-ONBOARD_BUTTON_NUMBER,false);
        buildTxBuffer(index-ONBOARD_BUTTON_NUMBER,false);
    }else{
        IDE_TRACE();
    }
}

bool ButtonController::onboardlightCtrl(int index,bool isLighton){
    if(index <0||index > (ONBOARD_BUTTON_NUMBER-1)){
        IDE_TRACE();
        return false;
    }
    if(isLighton){
        write(m_outputButtonFilefd[index],&lightOn_value,sizeof(lightOn_value));
    }else{
        write(m_outputButtonFilefd[index],&lightOff_value,sizeof(lightOff_value));
    }

    return true;
}

///Extend board
bool ButtonController::initExtendboard(){
    uint8_t mode = 0;
    uint8_t bits = 8;
    uint32_t speed = 50000;
    int res;
    m_extendfd = open("/dev/spidev3.0",O_RDWR);
    if(m_extendfd < 0){
        printf("can not open SPI device\n");
        return false;
    }
    res = ioctl(m_extendfd,SPI_IOC_WR_MODE,&mode);
    if(res == -1){
        printf("Cant not set spi mode to WR\n");
        close(m_extendfd);
        return false;
    }
    res = ioctl(m_extendfd,SPI_IOC_WR_BITS_PER_WORD,&bits);/*设置SPI数据位*/
    if(res == -1){
        printf("Cant not set bits per word\n");
        close(m_extendfd);
        return false;
    }
    res = ioctl(m_extendfd,SPI_IOC_WR_MAX_SPEED_HZ,&speed);/*设置SPI最大总线频率*/
    if(res == -1){
        printf("Can not set spi speed\n");
        close(m_extendfd);
        return false ;
    }
    return true;
}
void ButtonController::parseSpiData(){
    quint8 va,buttonstatu,btNumber;
    for(int i=0;i<3;i++){
        va = rx_buf_extend[i];
        for(int j=0;j<8;j++){
            buttonstatu = (va>>j)&0x01;
            btNumber = i*8+j;
            if(buttonstatu == 0x00){
                if(!isExtendButtonPressed(btNumber)){
                    emit sigBoardButtonPressed(btNumber + ONBOARD_BUTTON_NUMBER+1);
                    //emit sigExtendButtonPressed(btNumber + ONBOARD_BUTTON_NUMBER+1);
                    //setExtendButtonPressed(btNumber,true);
                    //IDE_TRACE_STR(QString("Button %1 pressed").arg(btNumber));
                }else{

                }
            }else if(buttonstatu == 0x01){
                if(isExtendButtonPressed(btNumber)){
                    //setExtendButtonPressed(btNumber,false);
                    emit sigBoardButtonReleased(btNumber + ONBOARD_BUTTON_NUMBER+1);
                    ///IDE_TRACE_STR(QString("Button %1 released").arg(btNumber));
                }
            }
        }
    }
}

bool ButtonController::isExtendButtonPressed(quint8 index){
    if(index < 0 ||index > (EXTEND_BUTTON_NUMBER -1)){
        return false;
    }
    return extendButtonStatu[index];
}

void ButtonController::setExtendButtonPressed(int index, bool pressed){
    if(index <0||index > (EXTEND_BUTTON_NUMBER -1)){
        return;
    }
    extendButtonStatu[index] = pressed;
}

void ButtonController::buildTxBuffer(int index, bool isOn){
    //tx_buf_extend
    //IDE_TRACE();
    m_mutex.lock();
    if(index >= 0 && index < 8){
        tx_buf_extend[2] = bitMaskCaq(tx_buf_extend[2],index,isOn);
    }else if(index > 7 && index < 16){
        tx_buf_extend[1] = bitMaskCaq(tx_buf_extend[1],index-8,isOn);
    }else if(index > 15 && index < 24){
        tx_buf_extend[0]  = bitMaskCaq(tx_buf_extend[0],index-16,isOn);
    }
    m_mutex.unlock();
    ///printf("tx_buf_extend : %x,%x,%x \n",tx_buf_extend[0],tx_buf_extend[1],tx_buf_extend[2]);
}

unsigned char ButtonController::bitMaskCaq(unsigned char value,int index, bool linghtON){
    unsigned char va = value;
    switch (index) {
    case 0:
        if(linghtON){//light on
            va = value | 0x01;
        }else{
            va = value & 0xfe;
        }
        break;
    case 1:
        if(linghtON){//light on
            va = value | 0x02;
        }else{
            va = value & 0xfd;
        }
        break;
    case 2:
        if(linghtON){//light on
            va = value | 0x04;
        }else{
            va = value & 0xfb;
        }
        break;
    case 3:
        if(linghtON){//light on
            va = value | 0x08;
        }else{
            va = value & 0xf7;
        }
        break;
    case 4:
        if(linghtON){//light on
            va = value | 0x10;
        }else{
            va = value & 0xef;
        }
        break;
    case 5:
        if(linghtON){//light on
            va = value | 0x20;
        }else{
            va = value & 0xdf;
        }
        break;
    case 6:
        if(linghtON){//light on
            va = value | 0x40;
        }else{
            va = value & 0xbf;
        }
        break;
    case 7:
        if(linghtON){//light on
            va = value | 0x80;
        }else{
            va = value & 0x7f;
        }
        break;
    default:
        break;
    }
    //IDE_TRACE();
    return va;
}
