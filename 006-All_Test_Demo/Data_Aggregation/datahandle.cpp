#include "datahandle.h"
#include <QtMath>
#include <QDebug>

DataHandle::DataHandle(QObject *parent) : QObject(parent)
{

}

/*
 * 首先看一个关于质数分布的规律：大于等于5的质数一定和6的倍数相邻。例如5和7，11和13,17和19等等；
 证明：令x≥1，将大于等于5的自然数表示如下：
··· 6x-1，6x，6x+1，6x+2，6x+3，6x+4，6x+5，6(x+1），6(x+1)+1 ···
可以看到，不在6的倍数两侧，即6x两侧的数为6x+2，6x+3，6x+4，由于2(3x+1)，3(2x+1)，2(3x+2)，
所以它们一定不是素数，再除去6x本身，显然，素数要出现只可能出现在6x的相邻两侧。
因此在5到sqrt(n)中每6个数只判断2个，时间复杂度O(sqrt(n)/3)。
*/
int DataHandle::isPrime(int num)
{
    //返回1表示判断为质数，0为非质数，-1 在此没有进行输入异常检测
    if (0 >= num)
    {
        return -1; // 表示输入的num error;
    }
    else if (2 == num || 3 == num)
    {
        return 1;   // 素数
    }
    else if (num % 6 != 1 && num % 6 != 5)
    {
        return 0;   // 不是素数
    }
    float nSqrt;
    nSqrt = qFloor(qSqrt((float)num));
    qDebug()<<" nSqrt = "<<nSqrt;
    for (int i = 5; i <= nSqrt; i += 6)
    {
        if ( 0 == (num % (i)) || 0 == num % (i+2))
        {
            return 0; // 不是素数
        }
    }
    return 1;  // 素数
}

void DataHandle::generateRandom(int a[], int num, int mode)
{
    qsrand((unsigned)time(NULL));
    for (int i = 0; i < num; ++i)
    {
        int temp = qrand() % mode;
        qDebug()<<"tmp = "<<temp;
        a[i] = temp;
    }
}

qint64 DataHandle::powValue(qreal bottom, qreal index)
{
    int pow = qPow(bottom, index);
    qDebug()<<"pow = "<<pow;
    return pow;
}
