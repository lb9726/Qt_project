#ifndef DATAHANDLE_H
#define DATAHANDLE_H

#include <QObject>

class DataHandle : public QObject
{
    Q_OBJECT
public:
    explicit DataHandle(QObject *parent = 0);
    int isPrime(int num);                                // 判别素数的算法
    void generateRandom(int a[], int num, int mode);     // 生成随机数算法
    qint64 powValue(qreal bottom, qreal index);          // 求n次方

signals:

public slots:
private:

};

#endif // DATAHANDLE_H
