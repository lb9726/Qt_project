#include "extensionbuttons.h"
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

ExtensionButtons::ExtensionButtons(QThread *parent) : QThread(parent)
{
    initStatus();
    if(initExtension()) {
        IDE_TRACE();
        start();
    }
}

// initialize Extend board
void ExtensionButtons::initStatus()
{
    IDE_TRACE_STR(QString("void ExtensionButtons::initStatus()"));
    tx_buf_extend[0] = 0x00;
    tx_buf_extend[1] = 0x00;
    tx_buf_extend[2] = 0x00;

    rx_buf_extend[0] = 0x00;
    rx_buf_extend[1] = 0x00;
    rx_buf_extend[2] = 0x00;

    for(int i = 0; i < EXTEND_BUTTON_NUMBER; ++i )
    {
        m_BtnPressed[i] = 0 ;
        m_isLightOn[i] = 0;
    }
}

bool ExtensionButtons::initExtension()
{
    IDE_TRACE();
    uint8_t mode = 0;
    uint8_t bits = 8;
    uint32_t speed = 50000;
    int res;
    m_extendfd = open(SPI_PATH, O_RDWR);  // open SPI device
    if(m_extendfd < 0)
    {
        IDE_TRACE_STR(QString("can not open SPI device"));
        return false;
    }
    res = ioctl(m_extendfd, SPI_IOC_WR_MODE, &mode);
    if(-1 == res)
    {
        IDE_TRACE_STR(QString("Cant not set spi mode to WR"));
        close(m_extendfd);
        return false;
    }
    res = ioctl(m_extendfd, SPI_IOC_WR_BITS_PER_WORD, &bits); /*设置SPI数据位*/
    if(-1 == res)
    {
        IDE_TRACE_STR(QString("Cant not set bits per word"));
        close(m_extendfd);
        return false;
    }
    res = ioctl(m_extendfd, SPI_IOC_WR_MAX_SPEED_HZ, &speed); /*设置SPI最大总线频率*/
    if(-1 == res)
    {
        IDE_TRACE_STR(QString("Can not set spi speed"));
        close(m_extendfd);
        return false ;
    }
    return true;
}

void ExtensionButtons::run()
{
    while(1)
    {
        msleep(10);
        read(m_extendfd, rx_buf_extend, sizeof(rx_buf_extend));
        parseSpiData();
        write(m_extendfd, tx_buf_extend, sizeof(tx_buf_extend));
        printf("%x %x %x\n", rx_buf_extend[0], rx_buf_extend[1], rx_buf_extend[2]);
    }
}

void ExtensionButtons::setLightOn(int index)
{
    int pos = index / 8;
    int pos2 = index % 8;
    IDE_TRACE_INT(pos);
    IDE_TRACE_INT(pos2);
    tx_buf_extend[2-pos] = tx_buf_extend[2-pos] | (0x01<<pos2); // SPI的write的时候，是反的，0,1,2 变为 2,1,0
    printf("tx_buf_extend |= %x %x %x\n", tx_buf_extend[0], tx_buf_extend[1], tx_buf_extend[2]);
    write(m_extendfd, tx_buf_extend, 3);
}

void ExtensionButtons::setLightOff(int index)
{
    int pos = index / 8;
    int pos2 = index % 8;
    IDE_TRACE_INT(pos);
    IDE_TRACE_INT(pos2);
    tx_buf_extend[2-pos]  &= (~(0x01 << pos2)); // SPI的write的时候，是反的，0,1,2 变为 2,1,0
    printf("tx_buf_extend |= %x %x %x\n", tx_buf_extend[0], tx_buf_extend[1], tx_buf_extend[2]);
    write(m_extendfd, tx_buf_extend, 3);
}

void ExtensionButtons::parseSpiData()
{
    quint8 va;
    quint8 buttonstatu;
    quint8 btNumber;

    for (int i = 0; i < 3; ++i)
    {
        va = rx_buf_extend[i];
        for (int j = 0; j < 8; ++j)
        {
            buttonstatu = (va >> j) & 0x01;  // 右移取值
            btNumber = i*8 + j;
            if (1 == m_isLightOn[btNumber])    // 灯亮的情况下
            {
                if (0x00 == buttonstatu)     // 如果灯亮的时候又被按下
                {
                    m_BtnPressed[btNumber] = 1;
                }
                else    // 如果灯亮的时候松开,取消亮灯
                {
                    if (1 == m_BtnPressed[btNumber])
                    {
                        m_BtnPressed[btNumber] = 0;
                        setLightOff(btNumber);
                        m_isLightOn[btNumber] = 0;
                        printf("should be cancled\n");
                    }
                }
            }
            else  // 灯不亮的情况下
            {
                if (1 == m_BtnPressed[btNumber])
                {
                    if (0x01 == buttonstatu)  // 如果松开
                    {
                        m_BtnPressed[btNumber] = 0;
                        m_isLightOn[btNumber] = 1;
                    }
                }
                else
                {
                    if (0x00 == buttonstatu)  // 如果按下
                    {
                        m_BtnPressed[btNumber] = 1;
                        setLightOn(btNumber);
                    }
                }
            }
        }
    }
}
