//#include "bcbutton.h"
//#include "define.h"
//BCButton::BCButton(QObject *parent) :
//    QThread(parent)
//{
//    IDE_TRACE();
//    mLedOutState = 0;
//    mKeyInState = 0;
//}

//SpecialBtns::SpecialBtns(QObject *parent):
//    QThread(parent)
//{
//    IDE_TRACE();
//    mKeyInState = 0;
//    mLedoutState = 0;
//}

//void SpecialBtns::setLedState(quint16 pValue)
//{
//    IDE_TRACE();
//   mLedoutState = pValue & 0xf000;
//}

//void SpecialBtns::run()
//{
//    IDE_TRACE();
//    QList<int> tmpList;
//    tmpList<<50 <<51 <<52 <<53 <<113 <<114 <<115 <<116 ;    // gpio2.IO18 = 50~53-->in  // gpio4.IO17 = 113~116 -->out
//    QFile tmpFile(GPIO_EXPORT);
//    if(tmpFile.open(QFile::WriteOnly))
//    {
//        for(int i=0; i<tmpList.count(); i++)
//        {
//            int tmpGpio = tmpList.at(i);
//            tmpFile.write(QByteArray::number(tmpGpio));
//        }
//        tmpFile.close();
//    }

//    QList<int> tmpInList;
//    tmpInList<<50 <<51 <<52 <<53;  // gpio2.IO18 = 50~53-->in
//    for(int i=0; i<tmpInList.count(); i++)
//    {
//        QFile tmpFile(GPIO_DIRECT(tmpInList.at(i)));
//        if(tmpFile.open(QFile::WriteOnly))
//        {
//            tmpFile.write("in");
//            tmpFile.close();
//        }
//    }

//    QList<int> tmpOutList;
//    tmpOutList<<113 <<114 <<115 <<116 ;  // gpio4.IO17 = 113~116 -->out
//    for(int i=0; i<tmpOutList.count(); i++)
//    {
//        QFile tmpFile(GPIO_DIRECT(tmpOutList.at(i)));
//        if(tmpFile.open(QFile::WriteOnly))
//        {
//            tmpFile.write("out");
//            tmpFile.close();
//        }
//    }

//    while(1)
//    {
//        for(int i=0; i<tmpInList.count(); i++)
//        {
//            QFile tmpFile(GPIO_VALUE(tmpInList.at(i)));
//            if(tmpFile.open(QFile::ReadOnly))
//            {
//                QByteArray tmpArray = tmpFile.read(10);
//                if(tmpArray[0] == BUTTON_RELEASEED)  // 按钮没有被按
//                {
//                    mKeyInState = 0;
//                }
//                else
//                {
//                    if(0 == mKeyInState)  // 一直按下则为1，避免按下不松手时多次发送信号
//                    {
//                        emit sClicked(i);
//                    }
//                    mKeyInState = 1;
//                }
//                tmpFile.close();
//            }
//        }

//        for(int i=0;  i<tmpOutList.count();  ++i)
//        {
//            QFile tmpFile(GPIO_VALUE(tmpOutList.at(i)));
//            if(tmpFile.open(QFile::WriteOnly))
//            {
//                if(mLedoutState & (1<<(i+12))) // out LED 特殊按钮按位与运算，取得确定位的数值
//                {
//                    tmpFile.write(QByteArray(BUTTON_LIGHT));
//                }
//                else
//                {
//                    tmpFile.write(QByteArray(BUTTON_NONLIGHT));
//                }
//                tmpFile.close();
//            }
//        }
//        msleep(100);
//    }
//}

//void BCButton::setGpio(quint8 pKeyNum, quint8 pKeyInNum, quint8 pLedOutNum)
//{
//    IDE_TRACE();
//    mKeyNum = pKeyNum;              //  0～11 表示哪一个按钮
//    mKeyInGpioNum = pKeyInNum;      //  按钮的Gpio的数值
//    mLedOutGpioNum = pLedOutNum;    //  LED的Gpio的数值
//    start();                        //  开启对应的按钮的线程
//}

//void BCButton::run()
//{
//    IDE_TRACE();
//    QFile tmpFile;
//    QFile tmpKeyIn;
//    tmpFile.setFileName(GPIO_EXPORT);
//    if(tmpFile.open(QFile::WriteOnly))
//    {
//        tmpFile.write(QByteArray::number(mLedOutGpioNum));
//        tmpFile.close();
//    }
//    tmpFile.setFileName(GPIO_DIRECT(mLedOutGpioNum));
//    if(tmpFile.open(QFile::WriteOnly))
//    {
//        tmpFile.write("out");
//        tmpFile.close();
//    }

//    tmpFile.setFileName(GPIO_EXPORT);
//    if(tmpFile.open(QFile::WriteOnly))
//    {
//        tmpFile.write(QByteArray::number(mKeyInGpioNum));
//        tmpFile.close();
//    }
//    tmpFile.setFileName(GPIO_DIRECT(mKeyInGpioNum));
//    if(tmpFile.open(QFile::WriteOnly))
//    {
//        tmpFile.write("in");
//        tmpFile.close();
//    }
//    static int tflg = 0;
//    while(1)
//    {
////        bool tmpLedLightFlag = false;
//        tmpFile.setFileName(GPIO_VALUE(mLedOutGpioNum));
//        tmpFile.open(QFile::WriteOnly);
////        if(0 == mLedOutState)
////        {
////            tmpLedLightFlag = false;
////            tmpFile.write(QByteArray(BUTTON_NONLIGHT));
////        }
////        else
////        {
////            tmpLedLightFlag = true;
////            tmpFile.write(QByteArray(BUTTON_LIGHT));
////        }
//        tmpFile.write(QByteArray(BUTTON_NONLIGHT));  // off

////        if(tmpLedLightFlag)
////        {
////            tmpFile.setFileName(GPIO_VALUE(mLedOutGpioNum));
////            tmpFile.open(QFile::WriteOnly);
////            tmpFile.write(QByteArray(BUTTON_NONLIGHT));
////            tmpFile.close();
////        }
//        //msleep(1);

//        tmpKeyIn.setFileName(GPIO_VALUE(mKeyInGpioNum));
//        tmpKeyIn.open(QFile::ReadOnly);
//        QByteArray tmpRead = tmpKeyIn.read(10);
//        tmpKeyIn.close();
//        tflg++;
//        if (1300 == tflg)
//        {
//            qDebug()<<"tflg tmpRead = "<<tmpRead;
////            tflg = 0;
//        }

//        if(tmpRead[0] == BUTTON_PRESSED)   //按钮没有被按下
//        {
//            mKeyInState = 0;
//            if (1300 == tflg)
//            {
//                qDebug()<<"tmpRead[0] = "<<tmpRead[0]<<"BUTTON_PRESSED = "<< BUTTON_PRESSED;
//                tflg = 0;
//            }
//        }
//        else     //按钮按下
//        {
//            if (1300 == tflg)
//            {
//                qDebug()<<"else tmpRead[0] = "<<tmpRead[0]<<"BUTTON_PRESSED = "<< BUTTON_PRESSED;
//                tflg = 0;
//            }
//            if(0 == mKeyInState)        // 一直按下则为1，避免按下不松手时多次发送信号
//            {
//                emit sClicked(mKeyNum);
//                IDE_TRACE_INT(mKeyNum);
//            }
//            mKeyInState = 1;       //记录这次值
//        }
//        tmpFile.write(QByteArray(BUTTON_LIGHT));  // on
//        tmpFile.close();
//        msleep(5);
//    }
//}

//BCButtons::BCButtons(QObject *parent) :
//    QThread(parent)
//{
//    IDE_TRACE();
//    mButtonState = 0;

//    QList<int> tmpOutputList;
//    for(int i=0; i<12; i++)  // led1 - led12 32 ~ 43
//    {
//        tmpOutputList << (32+i);
//    }

//    QList<int> tmpInputList;
//    for(int i=0; i<10; i++)  //  button1 ~ button10 128 ~ 137
//    {
//        tmpInputList << 128+i;
//    }
//    tmpInputList << 48;     // button11 button12  48 ~ 49
//    tmpInputList << 49;

//    for(int i=0; i<12; i++)
//    {
//        mButtons[i] = new BCButton();
//        connect(mButtons[i], &BCButton::sClicked, this, &BCButtons::slot_Clicked, Qt::QueuedConnection);
////        connect(&mButtons[i], SIGNAL(sClicked(quint8)), this, SLOT(slot_Clicked(quint8)), Qt::QueuedConnection);
//        mButtons[i]->setGpio(i, tmpInputList.at(i), tmpOutputList.at(i));  // 将会开启12个线程
//    }

//    setLedState(0x0fff);

//    //    start();
//}

//BCButtons::~BCButtons()
//{
////    for(int i = 0; i < 12; i++)
////    {
////        delete mButtons[i];
////        mButtons[i] = NULL;
////    }
//}

//void BCButtons::setLedState(quint16 pValue)
//{
//    for(int i=0; i<12; i++)
//    {
//        mButtons[i]->mLedOutState = (pValue >> i) & 0x01;  //取出pValue的第i位
//    }
//}

////void BCButtons::run()
////{
////    QFile tmpFile;
////    tmpFile.setFileName("/dev/spidev3.0");
////    if(!tmpFile.open(QFile::ReadWrite))
////        return;

////    /*这里 mode 的值为 0，这是 SPI 总线的 SPI_CLK 在上升沿阶段， SPI_DIN 的信号有效*/
////    quint8 mode = 0;
////    int ret = ioctl(tmpFile.handle(), SPI_IOC_WR_MODE, &mode);
////    if (ret == -1)
////    {
////        IDE_DEBUG("can't set spi mode to WR");
////        return -1;
////    }

////    /* 设置 SPI 的数据位 */
////    quint8 bits = 8;
////    ret = ioctl(tmpFile.handle(), SPI_IOC_WR_BITS_PER_WORD, &bits);
////    if (ret == -1)
////    {
////        IDE_DEBUG("can't set bits per word");
////        return -1;
////    }

////    /* 设置 SPI 的最大总线频率 */
////    quint32 speed = 5000;
////    ret = ioctl(tmpFile.handle(), SPI_IOC_WR_MAX_SPEED_HZ, &speed);
////    if (ret == -1)
////    {
////        IDE_DEBUG("can't set max speed hz");
////        return -1;
////    }

////    quint16 delay = 0;
////    quint8 tmpSendBuffer[128];
////    quint8 tmpRecvBuffer[128];
////    struct spi_ioc_transfer txrxBuf[] = {
////        {
////            .tx_buf = tmpSendBuffer,
////            .rx_buf = 0,
////            .len = 2,
////            .delay_usecs = delay,
////            .speed_hz = speed,
////            .bits_per_word = bits
////        },
////        {
////            .tx_buf = 0,
////            .rx_buf = tmpRecvBuffer,
////            .len = 2,
////            .delay_usecs = delay,
////            .speed_hz = speed,
////            .bits_per_word = bits
////        }
////    };
////    while(1)
////    {
////        ret = ioctl(fd_spi, SPI_IOC_MESSAGE(1), &txrxBuf[0]);
////        if (ret == 0)
////        {
////            //发送成功
////        }

////        ret = ioctl(fd_spi, SPI_IOC_MESSAGE(1), &txrxBuf[1]);
////        if (ret == 0)
////        {
////            //接收成功
////        }
////    }
////}

//void BCButtons::slot_Clicked(quint8 pKeyNum)
//{
//    mButtonState |= (1<<pKeyNum);
//    emit sNewPressed(mButtonState);

//    mButtonState = 0;
//}
