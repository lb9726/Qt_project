#include "fft.h"

//const double PI = 3.14159265358979323846;

FFT::FFT(QObject *parent)
    :   QObject(parent)
{

}

FFT::~FFT()
{

}

// 一维 FFT 变换
void FFT::slot_FFT1D(Complex_Array* pTD, Complex_Array* pFD, quint32 nGrade)
{
    Complex_Array *W, *X1, *X2, *X;
    Complex_Array TempComplex_Array;
    quint32 i, j, k;
    quint32 bfsize, p;
    quint32 nCount = 1 << nGrade;
    double dAngle;
    // 分配内存空间
    W = new Complex_Array[nCount >> 1];
    X1 = new Complex_Array[nCount];
    X2 = new Complex_Array[nCount];
    // 计算加权系数
    for(i = 0; i < (nCount >> 1); i++)
    {
        dAngle = PI * 2.0 * i / nCount;
        W[i].Real = cos(dAngle);
        W[i].Imag = - sin(dAngle);
    }
    // 内存拷贝
    memcpy(X1, pTD, sizeof(Complex_Array) * nCount);
    // 采用蝶形算法进行快速傅立叶变换
    for(k = 0; k < nGrade; k++)
    {
        for(j = 0; j < (quint32) (1 << k); j++)
        {
            bfsize = 1 << (nGrade - k);
            for(i = 0; i < bfsize / 2; i++)
            {
                p = j * bfsize;
                X2[i + p].Real = X1[i + p].Real + X1[i + p + bfsize / 2].Real;
                X2[i + p].Imag = X1[i + p].Imag + X1[i + p + bfsize / 2].Imag;
                TempComplex_Array.Real = X1[i + p].Real - X1[i + p + bfsize / 2].Real;
                TempComplex_Array.Imag = X1[i + p].Imag - X1[i + p + bfsize / 2].Imag;
                X2[i + p + bfsize / 2].Real = TempComplex_Array.Real * W[i * (1 << k)].Real - TempComplex_Array.Imag * W[i * (1 << k)].Imag;
                X2[i + p + bfsize / 2].Imag = TempComplex_Array.Real * W[i * (1 << k)].Imag + TempComplex_Array.Imag * W[i * (1 << k)].Real;
            }
        }
        X = X1;
        X1 = X2;
        X2 = X;
    }
    // 重新排序
    for(j = 0; j < nCount; j++)
    {
        p = 0;
        for(i = 0; i < nGrade; i++)
        {
            if (j & (1 << i))
            {
                p += 1 << (nGrade - i - 1);
            }
        }
        pFD[j].Real = X1[p].Real;
        pFD[j].Imag = X1[p].Imag;
    }
    delete[] W;
//    delete[] X;
    delete[] X1;
    delete[] X2;
    emit sFDComplexArray(pFD, nCount);
}

// 一维 IFFT 变换
void FFT::slot_IFFT1D(Complex_Array* pFD, Complex_Array* pTD, quint32 nGrade)
{
    Complex_Array *W, *X1, *X2, *X;
    Complex_Array TempComplex_Array;
    quint32 i, j, k;
    quint32 bfsize, p;
    quint32 nCount = 1 << nGrade;
    double dAngle;
    // 分配内存空间
    W = new Complex_Array[nCount >> 1];
    X1 = new Complex_Array[nCount];
    X2 = new Complex_Array[nCount];
    // 计算加权系数
    for(i = 0; i < (nCount >> 1); i++)
    {
    dAngle = PI * 2.0 * i / nCount;
    W[i].Real = cos(dAngle);
    W[i].Imag = sin(dAngle);
    }
    // 内存拷贝
    memcpy(X1, pFD, sizeof(Complex_Array) * nCount);
    // 采用蝶形算法进行快速傅立叶变换
    for(k = 0; k < nGrade; k++)
    {
        for(j = 0; j < (quint32) (1 << k); j++)
        {
            bfsize = 1 << (nGrade - k);
            for(i = 0; i < bfsize / 2; i++)
            {
                p = j * bfsize;
                X2[i + p].Real = X1[i + p].Real + X1[i + p + bfsize / 2].Real;
                X2[i + p].Imag = X1[i + p].Imag + X1[i + p + bfsize / 2].Imag;
                TempComplex_Array.Real = X1[i + p].Real - X1[i + p + bfsize / 2].Real;
                TempComplex_Array.Imag = X1[i + p].Imag - X1[i + p + bfsize / 2].Imag;
                X2[i + p + bfsize / 2].Real = TempComplex_Array.Real * W[i * (1 << k)].Real - TempComplex_Array.Imag * W[i * (1 << k)].Imag;
                X2[i + p + bfsize / 2].Imag = TempComplex_Array.Real * W[i * (1 << k)].Imag + TempComplex_Array.Imag * W[i * (1 << k)].Real;
            }
        }
        X = X1;
        X1 = X2;
        X2 = X;
    }
    // 重新排序
    for(j = 0; j < nCount; j++)
    {
        p = 0;
        for(i = 0; i < nGrade; i++)
        {
            if (j & (1 << i))
            {
                p += 1 << (nGrade - i - 1);
            }
        }
        pTD[j].Real = X1[p].Real / (double) nCount;
        pTD[j].Imag = X1[p].Imag / (double) nCount;
    }
    delete[] W;
//    delete[] X;
    delete[] X1;
    delete[] X2;
    emit sTDComplexArray(pFD, nCount);
}
