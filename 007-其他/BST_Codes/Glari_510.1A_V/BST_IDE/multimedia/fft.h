#ifndef FFT_H
#define FFT_H

#include "global.h"
#include <iostream>
#include <fstream>
#include "math.h"
#include "string.h"
#include <QObject>

// 复数结构体
typedef struct
{
    double Real;
    double Imag;
} Complex_Array;

class FFT : public QObject
{
    Q_OBJECT
public:
    FFT(QObject *parent = 0);
    ~FFT();
signals:
    void sFDComplexArray(Complex_Array *pFD, quint32 pNum);
    void sTDComplexArray(Complex_Array *pTD, quint32 pNum);
public slots:
    // 一维 FFT 变换
    void slot_FFT1D(Complex_Array* pTD, Complex_Array* pFD, quint32 nGrade);
    // 一维 IFFT 变换
    void slot_IFFT1D(Complex_Array* pFD, Complex_Array* pTD, quint32 nGrade);
};

#endif // FFT_H
