#include "bcbuttons.h"

BCButton::BCButton(QObject *parent) :
    QThread(parent)
{
    mLedOutState = 0;
    mKeyInState = 0;
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
    QObject(parent)
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
}

void BCButtons::setLedState(quint16 pValue)
{
    for(int i=0;i<12;i++)
    {
        mButtons[i].mLedOutState = (pValue >> i) & 0x01; //取出pValue的第i位
    }
}

void BCButtons::slot_Clicked(quint8 pKeyNum)
{
    mButtonState |= (1<<pKeyNum);
    emit sNewPressed(mButtonState);

    mButtonState = 0;
}

