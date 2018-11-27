#include "mygraphicspixmapitem.h"
#include "logcat.h"
#include <QString>
#include <QMessageBox>

MyGraphicsPixmapItem::MyGraphicsPixmapItem()
{
    mIsMineflag = 0;                    // 标识是否为雷
    mButtonStatus = NONE_PRESSED;       // 标识按钮状态 0未按  1按下右键  2 按下左键
    mMineSNumber = 0;                   // 标识周围雷数
    mRowPos = 0;
    mRowPos = 0;
    mAlreadyOpenFlag = 0;
    setImage(":/images/FirstInit.png");
}

void MyGraphicsPixmapItem::showMine()
{
    if (getIsMineflag())
    {
        setImage(":/images/foundMine.png");
    }
    else
    {
        QString tmpPath = QString(":/images/%1.png").arg(mMineSNumber);
        setImage(tmpPath);
    }
}

void MyGraphicsPixmapItem::setImage(QString pPicName)
{
    setPixmap(pPicName);
}

int MyGraphicsPixmapItem::getAlreadyOpenFlag() const
{
    return mAlreadyOpenFlag;
}

void MyGraphicsPixmapItem::setAlreadyOpenFlag(int alreadyOpenFlag)
{
    mAlreadyOpenFlag = alreadyOpenFlag;
}

int MyGraphicsPixmapItem::getMinesNumber() const
{
    return mMineSNumber;
}

void MyGraphicsPixmapItem::setMinesNumber(int mineSNumber)
{
    if (mineSNumber < 0 || mineSNumber > 8)
    {
        MYDebug("mineSNumber is out of range is %d", mineSNumber);
        mMineSNumber = -1;
        return;
    }
    mMineSNumber = mineSNumber;
}

void MyGraphicsPixmapItem::setRowAndColPos(int pRow, int pCol)
{
    mRowPos = pRow;
    mColPos = pCol;
//    MYDebug("mRowPos = %d mColPos = %d", mRowPos, mColPos);
}

MyGraphicsPixmapItem::~MyGraphicsPixmapItem()
{
    MYDebug("is call destroyFunction");
}

int MyGraphicsPixmapItem::getButtonStatus() const
{
    return mButtonStatus;
}

void MyGraphicsPixmapItem::setButtonStatus(int buttonStatus)
{
    if (buttonStatus > LEFT_PRESSED || buttonStatus < NONE_PRESSED)
    {
        MYDebug("buttonStatus is out of range is %d", buttonStatus);
        mButtonStatus = NONE_PRESSED;
        return;
    }
    mButtonStatus = buttonStatus;
}

int MyGraphicsPixmapItem::getIsMineflag() const
{
    return mIsMineflag;
}

void MyGraphicsPixmapItem::setIsMineflag(int isMineflag)
{
    mIsMineflag = isMineflag;
}

void MyGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (Qt::RightButton == event->button())
    {
        MYDebug("is rightBtn Clicked mButtonStatus = %d", mButtonStatus);
        if (NONE_PRESSED == mButtonStatus) // 右键点击1次
        {
            setImage(":/images/flag.png"); // 右键点击1次, 标记为旗子图标，认为是雷
            mButtonStatus = RIGHT_PRESSED;
        }
        else if (RIGHT_PRESSED == mButtonStatus) // 右键点击2次, 取消认为是雷标记的旗子图标,标记为正常的图标
        {
            setImage(":/images/0.png");
            mButtonStatus = NONE_PRESSED;
        }
        else
        {
            // do nothing
        }
    }
    else if (Qt::LeftButton == event->button())
    {
        mAlreadyOpenFlag = 1;
        MYDebug("is leftBtn Clicked mButtonStatus = %d", mButtonStatus);
        /*没有按下过，并且本身是雷，显示雷的图片，游戏失败了！*/
        if (1 == mIsMineflag && NONE_PRESSED == mButtonStatus)
        {
            setImage(":/images/foundMine.png");
        }
        /*本身不是雷，并且没有被按下过，左键按下后显示对应的数字,*/
        /* mMineSNumber 是每一个图元初始化之后根据排布后设置的*/
        else if (0 == mIsMineflag && NONE_PRESSED == mButtonStatus)
        {
            QString tmpPath =QString(":/images/%1.png").arg(mMineSNumber);
            setImage(tmpPath);
            if (0 == mMineSNumber) // 只有是空白时，发送信号触发递归函数
            {
                MYDebug("is blank block send signal");
                emit sigLeftBtnClicked(mRowPos, mColPos);
            }
        }
        mButtonStatus = LEFT_PRESSED; // 左键按下标记以后，点击右键也不会起到作用
    }
}
