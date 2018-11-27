#include "bcbuttons.h"

BCButton::BCButton(QObject *parent) :
    QThread(parent)
{
    mLedOutState = 0;
    mKeyInState = 0;
}

SpecialBtns::SpecialBtns(QObject *parent):
    QThread(parent)
{
    mKeyInState = 0;
    mLedoutState = 0;

}

void SpecialBtns::setLedState(quint16 pValue)
{
   mLedoutState = pValue & 0xf000;
}

void SpecialBtns::run()
{
    QList<int> tmpList;
    tmpList<< 1 <<2 <<3 <<4 << 5 <<6 <<7 <<8;
    QFile tmpFile(GPIO_EXPORT);
    if(tmpFile.open(QFile::WriteOnly))
    {
        for(int i=0;i<tmpList.count();i++)
        {
            int tmpGpio = tmpList.at(i);
            tmpFile.write(QByteArray::number(tmpGpio));
        }
        tmpFile.close();
    }

    QList<int> tmpInList;
    tmpInList<< 1 <<2 <<3 <<4;
    for(int i=0;i<tmpInList.count();i++)
    {
        QFile tmpFile(GPIO_DIRECT(tmpInList.at(i)));
        if(tmpFile.open(QFile::WriteOnly))
        {
            tmpFile.write("in");
            tmpFile.close();
        }
    }

    QList<int> tmpOutList;
    tmpOutList<< 5 <<6 <<7 <<8;
    for(int i=0;i<tmpOutList.count();i++)
    {
        QFile tmpFile(GPIO_DIRECT(tmpOutList.at(i)));
        if(tmpFile.open(QFile::WriteOnly))
        {
            tmpFile.write("out");
            tmpFile.close();
        }
    }

    while(1)
    {
        for(int i=0;i<tmpInList.count();i++)
        {
            QFile tmpFile(GPIO_VALUE(tmpInList.at(i)));
            if(tmpFile.open(QFile::ReadOnly))
            {
                QByteArray tmpArray = tmpFile.read(10);
                if(tmpArray[0] == BUTTON_RELEASEED)
                {
                    mKeyInState = 0;
                }
                else
                {
                    if(mKeyInState == 0)
                    {
                        emit sClicked(i);
                    }
                    mKeyInState = 1;
                }
                tmpFile.close();
            }
        }

        for(int i=0;i<tmpOutList.count();i++)
        {
            QFile tmpFile(GPIO_VALUE(tmpOutList.at(i)));
            if(tmpFile.open(QFile::WriteOnly))
            {
                if(mLedoutState&(1<<(i+12)))
                {
                    tmpFile.write(QByteArray(BUTTON_LIGHT));
                }
                else
                {
                    tmpFile.write(QByteArray(BUTTON_NONLIGHT));
                }
                tmpFile.close();
            }
        }
        msleep(100);
    }
}

void BCButton::setGpio(quint8 pKeyNum, quint8 pKeyInNum, quint8 pLedOutNum)
{
    mKeyNum = pKeyNum;
    mKeyInGpioNum = pKeyInNum;
    mLedOutGpioNum = pLedOutNum;
    start();
}

void BCButton::run()
{
    QFile tmpFile;
    tmpFile.setFileName(GPIO_EXPORT);
    if(tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write(QByteArray::number(mLedOutGpioNum));
        tmpFile.close();
    }
    tmpFile.setFileName(GPIO_DIRECT(mLedOutGpioNum));
    if(tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write("out");
        tmpFile.close();
    }

    tmpFile.setFileName(GPIO_EXPORT);
    if(tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write(QByteArray::number(mKeyInGpioNum));
        tmpFile.close();
    }
    tmpFile.setFileName(GPIO_DIRECT(mKeyInGpioNum));
    if(tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write("in");
        tmpFile.close();
    }

    while(1)
    {
        bool tmpLedLightFlag = false;
        tmpFile.setFileName(GPIO_VALUE(mLedOutGpioNum));
        tmpFile.open(QFile::WriteOnly);
        if(mLedOutState == 0)
        {
            tmpLedLightFlag = false;
            tmpFile.write(QByteArray(BUTTON_NONLIGHT));
        }
        else
        {
            tmpLedLightFlag = true;
            tmpFile.write(QByteArray(BUTTON_LIGHT));
        }
        tmpFile.close();
        msleep(150);

        if(tmpLedLightFlag)
        {
            tmpFile.setFileName(GPIO_VALUE(mLedOutGpioNum));
            tmpFile.open(QFile::WriteOnly);
            tmpFile.write(QByteArray(BUTTON_NONLIGHT));
            tmpFile.close();
        }
        //msleep(1);

        tmpFile.setFileName(GPIO_VALUE(mKeyInGpioNum));
        tmpFile.open(QFile::ReadOnly);
        QByteArray tmpRead = tmpFile.read(10);
        tmpFile.close();
        if(tmpRead[0] == BUTTON_RELEASEED)   //按钮没有被按下
        {
            mKeyInState = 0;
        }
        else     //按钮按下
        {
            if(mKeyInState == 0)        //上一次按钮是否被按下
            {
                emit sClicked(mKeyNum);
                IDE_TRACE_INT(mKeyNum);
            }
            mKeyInState = 1;      //记录这次值
        }
    }
}

BCButtons::BCButtons(QObject *parent) :
    QThread(parent)
{
    mButtonState = 0;

    QList<int> tmpOutputList;
    for(int i=0;i<12;i++)
    {
        tmpOutputList << (32+i);
    }

    QList<int> tmpInputList;
    for(int i=0;i<10;i++)
    {
        tmpInputList << 128+i;
    }
    tmpInputList << 48;
    tmpInputList << 49;

    for(int i=0;i<12;i++)
    {
        connect(&mButtons[i], SIGNAL(sClicked(quint8)), this, SLOT(slot_Clicked(quint8)), Qt::QueuedConnection);
        mButtons[i].setGpio(i, tmpInputList.at(i), tmpOutputList.at(i));
    }

    setLedState(0x0fff);

//    start();
}

void BCButtons::setLedState(quint16 pValue)
{
    for(int i=0;i<12;i++)
    {
        mButtons[i].mLedOutState = (pValue >> i) & 0x01; //取出pValue的第i位
    }
}

//void BCButtons::run()
//{
//    QFile tmpFile;
//    tmpFile.setFileName("/dev/spidev3.0");
//    if(!tmpFile.open(QFile::ReadWrite))
//        return;

//    /*这里 mode 的值为 0，这是 SPI 总线的 SPI_CLK 在上升沿阶段， SPI_DIN 的信号有效*/
//    quint8 mode = 0;
//    int ret = ioctl(tmpFile.handle(), SPI_IOC_WR_MODE, &mode);
//    if (ret == -1)
//    {
//        IDE_DEBUG("can't set spi mode to WR");
//        return -1;
//    }

//    /* 设置 SPI 的数据位 */
//    quint8 bits = 8;
//    ret = ioctl(tmpFile.handle(), SPI_IOC_WR_BITS_PER_WORD, &bits);
//    if (ret == -1)
//    {
//        IDE_DEBUG("can't set bits per word");
//        return -1;
//    }

//    /* 设置 SPI 的最大总线频率 */
//    quint32 speed = 5000;
//    ret = ioctl(tmpFile.handle(), SPI_IOC_WR_MAX_SPEED_HZ, &speed);
//    if (ret == -1)
//    {
//        IDE_DEBUG("can't set max speed hz");
//        return -1;
//    }

//    quint16 delay = 0;
//    quint8 tmpSendBuffer[128];
//    quint8 tmpRecvBuffer[128];
//    struct spi_ioc_transfer txrxBuf[] = {
//        {
//            .tx_buf = tmpSendBuffer,
//            .rx_buf = 0,
//            .len = 2,
//            .delay_usecs = delay,
//            .speed_hz = speed,
//            .bits_per_word = bits
//        },
//        {
//            .tx_buf = 0,
//            .rx_buf = tmpRecvBuffer,
//            .len = 2,
//            .delay_usecs = delay,
//            .speed_hz = speed,
//            .bits_per_word = bits
//        }
//    };
//    while(1)
//    {
//        ret = ioctl(fd_spi, SPI_IOC_MESSAGE(1), &txrxBuf[0]);
//        if (ret == 0)
//        {
//            //发送成功
//        }

//        ret = ioctl(fd_spi, SPI_IOC_MESSAGE(1), &txrxBuf[1]);
//        if (ret == 0)
//        {
//            //接收成功
//        }
//    }
//}

void BCButtons::slot_Clicked(quint8 pKeyNum)
{
    mButtonState |= (1<<pKeyNum);
    emit sNewPressed(mButtonState);

    mButtonState = 0;
}

