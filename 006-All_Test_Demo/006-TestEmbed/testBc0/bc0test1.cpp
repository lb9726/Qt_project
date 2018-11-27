#include "bc0test1.h"

Bc0Test1::Bc0Test1()
{
    init();
    this->start();
    qDebug()<<__FUNCTION__;
}

void Bc0Test1::init()
{
    qDebug()<<__FUNCTION__;
    mDipsExport.setFileName(GPIO_EXPORT);

    if (!mDipsExport.open(QIODevice::WriteOnly))
    {
        qDebug()<<__FUNCTION__<<"open error mDipsExport";
        return;
    }
    mDipsExport.write("32");

    mDipsExport.close();

    mDipsExport.setFileName(GPIO_EXPORT);

    if (!mDipsExport.open(QIODevice::WriteOnly))
    {
        qDebug()<<__FUNCTION__<<"open error mDipsExport";
        return;
    }
    mDipsExport.write("128");
    mDipsExport.close();

    mDirection.setFileName(GPIO_DIRECT(32));
    if (!mDirection.open(QIODevice::ReadWrite))
    {
        qDebug()<<__FUNCTION__<<"open error mDirection";
        return;
    }
    mDirection.write("out");
    mDirection.close();

    mDirection.setFileName(GPIO_DIRECT(128));
    if (!mDirection.open(QIODevice::ReadWrite))
    {
        qDebug()<<__FUNCTION__<<"open error mDirection";
        return;
    }
    mDirection.write("in");
    mDirection.close();
}

void Bc0Test1::bc0On(int index)
{
    qDebug()<<__FUNCTION__;
    mDirection[i].setFileName(GPIO_VALUE(32 + index));
    if (!mDirection[i].open(QIODevice::ReadWrite))
    {
        qDebug()<<"open mDirection[i] = "<<i<<"error";
        return;
    }
    mDirection[i].write("1");
    mDirection[i].close();
}

void Bc0Test1::bc0Off(int index)
{
    mDirection[i].setFileName(GPIO_VALUE(32));
    if (!mDirection[i].open(QIODevice::ReadWrite))
    {
        return;
    }
    mDirection[i].write("0");
    mDirection[i].close();
}

void Bc0Test1::bc0Staus()
{
    for(i = 0; i < 10; i++){
        bc0Off(i);
        mDipsExport[i].setFileName(GPIO_VALUE(128 + i));
        if (!mDipsExport[i].open(QIODevice::ReadWrite))
        {
            return;
        }
        QByteArray temp = mDipsExport[i].read(1);
        mDipsExport[i].close();
        qDebug()<<"temp = "<< temp;
        if ("0" == temp)//an
        {
            canpensend();//tong zhi kong zhi you ren quxiao
        }
        bc0On(i);
    }

}
void Bc0Test1::run()
{
    bool LiftCtrlLedOn = false;
    while (1)
    {
        recv = Canopen();
        if(recv){
            LiftCtrlLedOn = true;
        }else{
            LiftCtrlLedOn = false;
            bc0Off();
        }
        if(LiftCtrlLedOn){//kong zhi gui dian deng qing kuang xia pan ding an niu shi fou an xia.
            bc0Staus();
        }
        msleep(5);
    }
}
