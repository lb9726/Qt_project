#include "dipsio.h"

DipsIO::DipsIO(QObject *parent) : QThread(parent)
{
    mDipsExport.setFileName(GPIO_EXPORT); //把gpio放入导入文件夹
    if(mDipsExport.open(QFile::WriteOnly))
    {
       for(int i=0;i<8;i++)
       {
           int tmpGpio = i+18;
           mDipsExport.write(QByteArray::number(tmpGpio));
       }
       mDipsExport.close();
    }


    for(int i=0;i<8;i++)  //设gpio口为输入
    {
        int tmpGpio = i+18;
        mDips8[i].setFileName(GPIO_DIRECT(tmpGpio));
        if(mDips8[i].open(QFile::WriteOnly))
        {
            mDips8[i].write("in");
            mDips8[i].close();
        }
    }
    start();

}

void DipsIO::run()
{
    QFile tmpFile;
    while(1)
    {
        quint8 tmpValue = 0;
        for(int i=0;i<8;i++)
        {
            int tmpGpio = i+18;
            tmpFile.setFileName(GPIO_VALUE(tmpGpio));
            tmpFile.open(QFile::ReadOnly);
            QByteArray tmpArray = tmpFile.read(10);
//            IDE_TRACE_STR(QString(tmpArray));
//            IDE_TRACE_INT(tmpArray.count());
            tmpFile.close();

            if(tmpArray[0] == '0')
            {
                tmpValue |= (1<<i);
            }
            else
            {
                tmpValue &= ~(1<<i);
            }
        }
        if(tmpValue != mDipsValue)
        {
            mDipsValue = tmpValue;
            emit sValueChanged(mDipsValue);
//            IDE_DEBUG(QString::number(tmpValue, 16));
        }
//        IDE_DEBUG("1");
        msleep(500);
    }
}
