#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    double a = -19.3,b = 9.7;
    double c = qAbs(a);
    double max = qMax(b,c);
    int bn = qRound(b);
    int cn = qRound(c);
    qDebug()<<"a = "<<a;
    qDebug()<<"b = "<<b;
    qDebug()<<"c = qAbs(a)= "<<c;
    qDebug()<<"qMax(b,c)= "<<max;
    qDebug()<<"bn = qRound(b)= "<<bn;
    qDebug()<<"cn = qRound(c)= "<<cn;
    qSwap(bn,cn);
    //
    qDebug()<<"qSwap(bn,cn):"<<"bn ="<<bn<<" cn ="<<cn;
    return 0;
}
