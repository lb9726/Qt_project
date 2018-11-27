#include "dataaggregation.h"
#include <QFile>
#include <QCoreApplication>
#include <QThread>
#include "logcat.h"
#include <QStringList>
#include <QByteArray>
#include <QRegExp>

extern "C"
{
#include <time.h>
#include <stdio.h>
#include <string.h>
}

#include "sha1.hpp"
#include <string>
#include <iostream>
using std::string;
using std::nothrow;

const QRegExp splitStr = QRegExp(", ");
const unsigned int dataBitLen = 512;
const int hashBitlen = 40; /*40位16进制*/

DataAggregation::DataAggregation(QObject *parent) : QObject(parent)
{
    mUserNumbers = 0;
    mRunPath = QCoreApplication::applicationDirPath();
    if (!mRunPath.endsWith("/"))
    {
        mRunPath.append("/");
    }
    initAllParameters();
}

DataAggregation::~DataAggregation()
{
    for (int i = 0; i < mTextData.length(); ++i)
    {
        TEXTDATA *tmpData = mTextData.at(i);
//        MYDebug();
        if (tmpData)
        {
            mpz_clear(tmpData->pubKey);
            mpz_clear(tmpData->privateKey);
            mpz_clear(tmpData->encryptionKey);
//            mpz_clear(tmpData->fakeData);
            delete tmpData;
            tmpData = nullptr;
//            MYDebug();
        }
    }
    mpz_clear(g);          // 回收分配的空间
    mpz_clear(mBigPrimeP); // 回收分配的空间

    for (int i = 0; i < mEncryptionStr.length(); ++i)
    {
        MESSDATA * tmpMessage = mEncryptionStr.at(i);
        if (tmpMessage)
        {
            if (tmpMessage->strPoint)
            {
                delete tmpMessage->strPoint;
                tmpMessage->strPoint = nullptr;
            }
            delete tmpMessage;
            tmpMessage = nullptr;
//            MYDebug("delete success");
        }
    }
}

void DataAggregation::initAllParameters()
{    
    MYDebug();
    mLastRandom = 68; // 初始化一个数值，可以任意设置一个不超过100的正整数
    mpz_init(g);
    mpz_init(mBigPrimeP);
    generateBigPrime();
    generateG(g, mBigPrimeP);
    mCountTime.start();
}

void DataAggregation::generateG(mpz_t g, mpz_t pBigPrimep)
{
    mpz_t tmpN;
    mpz_init(tmpN);

    gmp_randstate_t grt;
    gmp_randinit_default(grt); //设置随机数生成算法为默认
    gmp_randseed_ui(grt, time(NULL)); //设置随机化种子为当前时间
    char buffer[1000];
    memset(buffer, 0, sizeof(buffer));
    int res = 0;
    do {
        mpz_urandomb(tmpN, grt, 200); // 随机生成一个0 ~ 2^200的一个数
        mpz_mod(g, tmpN, pBigPrimep); // 对大素数p进行取模
        res = mpz_cmp_ui(g, 2);       // 大于2
        MYDebug("res = %d", res);
        mpz_get_str(buffer, 2, g);
        MYDebug("buffer g = %s", buffer);
    } while (1 != res);
    mpz_get_str(buffer, 2, g);
    MYDebug("buffer g = %s", buffer);
    mpz_clear(tmpN);
}

void DataAggregation::generateRandom(mpz_t pRandom, unsigned int pNumLen)
{
    gmp_randstate_t grt;
    gmp_randinit_default(grt);           // 设置随机数生成算法为默认
    gmp_randseed_ui(grt, time(NULL));    // 设置随机化种子为当前时间
    mpz_urandomb(pRandom, grt, pNumLen); // 随机生成一个0 ~ 2^pNumLen的一个数

    char buffer[1000];
    memset(buffer, 0, sizeof(buffer));
    qsrand((unsigned)time(NULL));
    unsigned int x;
    while (1)
    {
        x = qrand() % 1000;
        if (x != mLastRandom)
        {
            mLastRandom = x;
            break;
        }
    }
    static int base = 8;
    int y = qrand() % 20;
    mpz_sub_ui(pRandom, pRandom, mLastRandom * y *(++base));
    if (base > 1000)
    {
        base = 1;
    }
    mpz_get_str(buffer, 16, pRandom);
    qDebug("bufferpNumLen is %s", buffer);
}

void DataAggregation::generatePublicKey(mpz_t pPubKey, mpz_t pG, mpz_t pPriKey, mpz_t pBigP)
{
    mpz_powm(pPubKey, pG, pPriKey, pBigP);
}

void DataAggregation::getXor(mpz_t skij, mpz_t pPubKey, mpz_t pPriKey)
{
    mpz_xor(skij, pPubKey, pPriKey);
}

/*
 * 对最终按列异或出来的一串字符串进行切割
 * 理论是以sha1的hash为40位16进制的位长进行切割
 * 但实际上的如果开头是0的话是不会显示的，从第一个非0的数值开始显示
 * 所以字符串的长度一般是小于等于40乘以用户个数的，4个用户，则字符串小于等于160位
 * 依此类推，但中间的不管是不是0，都会显示40位连续的
*/

void DataAggregation::getCutStr(char *str, int pLen)
{
    int modNum = hashBitlen - (mUserNumbers * hashBitlen - pLen) % hashBitlen;
    QString tmpArray(str);
    if (hashBitlen != modNum)
    {
        for (int i = 0; i < mUserNumbers; ++i)
        {
            if (0 == i)
            {
                QString s0 = tmpArray.mid(0, modNum);
                int t = s0.toInt(0, 16);
                mResultString.append(t);
            }
            else
            {
                QString si = tmpArray.mid(modNum + (i-1)*hashBitlen, hashBitlen);
                int t = si.toInt(0, 16);
                mResultString.append(t);
            }
        }
    }
    else if (hashBitlen == modNum)
    {
        for (int i = 0; i < mUserNumbers; ++i)
        {
            QString si = tmpArray.mid(i*hashBitlen, hashBitlen);
            int t = si.toInt(0, 16);
            mResultString.append(t);
        }
    }
}

/*
实现 按行读取文件的每一行数据存起来存到了QList的一个list表中，设置id和文本内容
*/
void DataAggregation::readDataFile(QString fileName)
{
    mObserveObjNum = 0;
    QFile tmpFile(fileName);
    if (!tmpFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        MYDebug("open file %s is error!", fileName.toLatin1().data());
    }

    int lines = 0;
    while (!tmpFile.atEnd())
    {        
        QByteArray textLine = tmpFile.readLine();
        if (textLine.endsWith("\n")) // 如果读取的文本以\n结束，则去掉它
        {
            textLine = textLine.remove(textLine.length()-1, 1);
        }
        if (textLine.isEmpty()) // 遇到空行跳过
        {
            MYDebug("encounter an empty line will continue");
            continue;
        }
        TEXTDATA *tmpData = new TEXTDATA;
        ++lines;
        if (tmpData)
        {
            tmpData->id = lines;
            tmpData->content = QString(textLine);
            mTextData.append(tmpData);
            QStringList tmpList = tmpData->content.split(splitStr);
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"tmpList = "<<tmpList;
            MYDebug("mObserveObjNum = %d", mObserveObjNum);
            MYDebug("tmpList.length() = %d", tmpList.length());
            if (mObserveObjNum < tmpList.length())
            {
                mObserveObjNum = tmpList.length(); // 求最大被观测对象的个数
                MYDebug("mObserveObjNum = %d", mObserveObjNum);
            }
        }
    }
    mUserNumbers = lines;
}

void DataAggregation::printQList()
{
    for (int i = 0; i < mTextData.length(); ++i)
    {
        TEXTDATA *tmpData = mTextData.at(i);
        MYDebug("the str is %s", tmpData->content.toLatin1().data());
        MYDebug("the id is %d", tmpData->id);
    }
}

int DataAggregation::getUserNumbers() const
{
    MYDebug("mUserNumbers is %d", mUserNumbers);
    return mUserNumbers;
}

/*生成一个是512位2进制的大素数*/
void DataAggregation::generateBigPrime()
{
    gmp_randstate_t grt;
    gmp_randinit_default(grt); //设置随机数生成算法为默认
    gmp_randseed_ui(grt, time(NULL)); //设置随机化种子为当前时间

    char buffer[1000];
    mpz_init(mBigPrimeP);

    do {
        memset(buffer, 0, sizeof(buffer));
        mpz_urandomb(mBigPrimeP, grt, dataBitLen);//随机生成一个0 ~ 2^512的一个数
        mpz_nextprime(mBigPrimeP, mBigPrimeP);  //使用GMP自带的素数生成函数,生成一个在mBigPrimeP后面的大素数
        mpz_get_str(buffer, 2, mBigPrimeP);
    } while(dataBitLen != strlen(buffer));
    qDebug("buffer512 is %s", buffer);    
}

void DataAggregation::checkBigPrime()
{
    char buffer[1000];
    memset(buffer, 0, sizeof(buffer));
    mpz_get_str(buffer, 10, mBigPrimeP);
    qDebug("buffer512 check is %s", buffer);
}


void DataAggregation::generatePublicKeyAndPrivateKeyAndEncryptionKey()
{
    MYDebug("mUserNumbers = %d", mUserNumbers);
    /*初始化公钥和私钥，并且生成公钥和私钥*/
    for (int i = 0; i < mUserNumbers; ++i)
    {
        TEXTDATA *tmpData = mTextData.at(i);
        if (tmpData)
        {
            mpz_init(tmpData->pubKey);
            mpz_init(tmpData->privateKey);
            mpz_init(tmpData->encryptionKey);
            generateRandom(tmpData->privateKey, 160); // 私钥随机生成160位
            generatePublicKey(tmpData->pubKey, g, tmpData->privateKey, mBigPrimeP); // 生成公钥=g^pri%p
        }
    }

    /*计算skij，计算hash值*/
    for (int j = 0; j < mUserNumbers; ++j)
    {
        mpz_t tmpMpzSkij[mUserNumbers];
        TEXTDATA *tmpData1 = mTextData.at(j);
        for (int k = 0; k < mUserNumbers; ++k)
        {
            mpz_init(tmpMpzSkij[k]);
            if (k != j) // 自己的不用计算
            {
                TEXTDATA *tmpData2 = mTextData.at(k);
                if (tmpData1 && tmpData2)
                {/*skij其实就是别人公钥的我的私钥次方，取余p */
                    mpz_mul(tmpMpzSkij[k], tmpData1->pubKey, tmpData2->pubKey); // 公钥乘以公钥
                    mpz_mod(tmpMpzSkij[k], tmpMpzSkij[k], mBigPrimeP); // mod big P
                    mpz_powm(tmpMpzSkij[k], g, tmpMpzSkij[k], mBigPrimeP);
                }
            }
            if (k == mUserNumbers - 1) // k-1个skij计算完成时，计算hash值
            {
                char buf[800];
                char dest[800];
                mpz_t tmpHash[mUserNumbers];

                for (int i = 0; i < mUserNumbers; ++i)
                {
                    mpz_init(tmpHash[i]);
                    memset(buf, 0, sizeof(buf));
                    memset(dest, 0, sizeof(dest));

                    if (i != j) // 排除自己的，因为自己的那个没有数值
                    {
                        MYDebug("j = %d i = %d", j, i);
                        mpz_get_str(buf, 16, tmpMpzSkij[i]);
                        getHashNum(buf, dest);
                        mpz_set_str(tmpHash[i], dest, 16); // 得到hash值的字符串
                    }
                    mpz_clear(tmpMpzSkij[i]);// 回收分配的空间
                }

                for (int i = 0; i < mUserNumbers; ++i) // 计算skij的k-1个hash值的异或
                {
                    if (i != j) // 排除自己的，因为自己的那个没有数值
                    {
                        if (tmpData1)
                        {
                            MYDebug("count siyao");
                            mpz_xor(tmpData1->encryptionKey, tmpData1->encryptionKey, tmpHash[i]);
                        }
                    }
                    mpz_clear(tmpHash[i]); // 回收分配的空间
                }
            }
        }
    }

//    for (int i = 0; i < mUserNumbers; ++i)
//    {
//        TEXTDATA *tmpData = mTextData.at(i);
//        if (tmpData)
//        {
//            char buf[800];
//            mpz_get_str(buf, 16, tmpData->encryptionKey);
//            MYDebug("the jiami16 = %s", buf);
//            /* 每一个用户的虚假数据是一样的，为0bit与加密秘钥的异或 */
//            mpz_xor(tmpData->fakeData, tmpData->fakeData, tmpData->encryptionKey);
////            char buf2[800];
////            mpz_get_str(buf2, 2, tmpData->encryptionKey);
////            MYDebug("the jiami2 = %s", buf2);
//        }
//    }
    qint64 time0 = mCountTime.nsecsElapsed();
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"time0 = "<<time0;
}

void DataAggregation::getHashNum(char *pStr, char *pDest)
{
    SHA1 checksum;
    const string input = pStr;
    checksum.update(input);
    const string hash = checksum.final();
    memcpy(pDest, hash.c_str(), strlen(hash.c_str()));
    qDebug("pDest = %s", pDest);
    qDebug()<<QString::fromStdString(hash);
}

void DataAggregation::testCmp(mpz_t g, mpz_t p)
{
    int res = mpz_cmp(g, p);
    MYDebug("res = %d", res);
}

void DataAggregation::checkPrivateKey()
{
    char buffer[1000];
    for (int i = 0; i < mUserNumbers; ++i)
    {
        memset(buffer, 0, sizeof(buffer));
        TEXTDATA *tmpData = mTextData.at(i);
        if (tmpData)
        {
            mpz_get_str(buffer, 10, tmpData->privateKey);
            MYDebug("bufferpNumLen is %s", buffer);
        }
    }
}

void DataAggregation::checkPubKey()
{
    char buffer[1000];
    for (int i = 0; i < mUserNumbers; ++i)
    {
        memset(buffer, 0, sizeof(buffer));
        TEXTDATA *tmpData = mTextData.at(i);
        if (tmpData)
        {
            mpz_get_str(buffer, 10, tmpData->pubKey);
            MYDebug("bufferPubKey is %s", buffer);
        }
    }
}

void DataAggregation::encryptionFileData()
{
    MYDebug("mUserNumbers = %d, mObserveObjNum = %d", mUserNumbers, mObserveObjNum);
    mpz_t tmpMpz[mUserNumbers][mObserveObjNum]; // 密文数据
    mpz_t tmpFakeMpz[mUserNumbers]; // 虚假数据
    for (int i = 0; i < mUserNumbers; ++i)
    {
        mpz_init(tmpFakeMpz[i]);
        TEXTDATA *tmpData = mTextData.at(i);

        if (tmpData)
        {
            char Fakebuf[800] = {0};
            mpz_get_str(Fakebuf, 16, tmpData->encryptionKey);
            MYDebug("the Fakebuf16 = %s", Fakebuf);
            /* 每一个用户的虚假数据是一样的，为0bit与加密秘钥的异或 */
            mpz_xor(tmpFakeMpz[i], tmpFakeMpz[i], tmpData->encryptionKey);

            if (!tmpData->content.isEmpty()) // 内容不为空
            {
                QString tmpStr = tmpData->content;
                QStringList tmpList = tmpStr.split(splitStr);
                MYDebug("tmpList len = %d", tmpList.length());
                qDebug()<<"tmpList is "<<tmpList;
                int len = tmpList.length();
                for (int j = 0; j < len; ++j)
                {
                    mpz_init(tmpMpz[i][j]);
                    QString tmpStr = tmpList.at(j);
                    char *tmpPoint = tmpStr.toLatin1().data();
                    mpz_set_str(tmpMpz[i][j], tmpPoint, 10);
                    /* 对明文数据和用户i的秘钥进行异或 得到所有的密文数据 */
                    mpz_xor(tmpMpz[i][j], tmpMpz[i][j], tmpData->encryptionKey);
                    char tmps[1000];
                    mpz_get_str(tmps, 16, tmpMpz[i][j]);
                    MYDebug("tmps = %s", tmps);
                }
            }
        }
    }
    /*
     * 对用户的每一个观测数据对应的虚假串进行初始化，m个观测数据，k个用户
     * 形成一个k*40+1的字符串(16进制，等于2进制的160位，16进制可以节省空间)
     * 初始化id, index, len, curpos,将new出来的messageData 加入QList中
     * 在析构函数中进行回收资源
    */
    for (int i = 0; i < mUserNumbers; ++i)
    {
        TEXTDATA *tmpData = mTextData.at(i);
        for (int j = 0; j < mObserveObjNum; ++j)
        {
            MESSDATA * tmpMessData = new MESSDATA;
            if (tmpMessData)
            {
                tmpMessData->id = tmpData->id;
//                tmpMessData->index = j;
                tmpMessData->len = hashBitlen * mUserNumbers + 1;
//                tmpMessData->curPos = 0;
                char *p = new(nothrow) char[tmpMessData->len]; // 多一位存\0的结束符
                if (p)
                {
                   tmpMessData->strPoint = p;
                   memset(tmpMessData->strPoint, 0, tmpMessData->len);
                   MYDebug("new char is success");
                }
                else
                {
                    tmpMessData->strPoint = nullptr;
                    MYDebug("new char is failure");
                }
                mEncryptionStr.append(tmpMessData);
            }
        }
    }
    /*
     * 组虚假的数据串 以16进制的来表示，40位为一个
     * 每一个观测数据有k-1个虚假数据，其加密值的位置以id作为标志，1就为首位,...
    */

    int totalLen = mEncryptionStr.length();
    int row;
    int col;
//    int flag[mUserNumbers] = {0};
    for (int i = 0; i < totalLen; ++i)
    {
        row = i / mObserveObjNum;     // 取到行
        col = i % mObserveObjNum;     // 取到列
        MYDebug("row = %d column = %d", row, col);
        MESSDATA * tmpMessData = mEncryptionStr.at(i);
        if (tmpMessData)
        {
            char jiamiBuf[200] = {0};
            mpz_get_str(jiamiBuf, 16, tmpMpz[row][col]); // 加密的密文数据转成16进制的字符串
            int jiamiLen = strlen(jiamiBuf);
            MYDebug("jiamiBuf = %s jiamiLen = %d", jiamiBuf, jiamiLen);
            if (jiamiLen < hashBitlen)
            {
                char tmpJiaMi[100] = {0};
                memcpy(tmpJiaMi, jiamiBuf, jiamiLen);
                for (int m = 0; m < hashBitlen-jiamiLen; ++m)
                {
                    jiamiBuf[m] = '0';
                }
                memcpy(jiamiBuf + hashBitlen - jiamiLen, tmpJiaMi, jiamiLen);
            }
            MYDebug("jiamiBuf = %s jiamiLen = %d", jiamiBuf, jiamiLen);

            char fakeBuf[200] = {0};
            mpz_get_str(fakeBuf, 16, tmpFakeMpz[row]);
            int fakeLen = strlen(fakeBuf);
            MYDebug("fakeBuf = %s fakeLen = %d", fakeBuf, fakeLen);

            if (fakeLen < hashBitlen)
            {
                char tmpFake[100] = {0};
                memcpy(tmpFake, fakeBuf, fakeLen);
                for (int m = 0; m < hashBitlen-fakeLen; ++m)
                {
                    fakeBuf[m] = '0';
                }
                memcpy(fakeBuf + hashBitlen-fakeLen, tmpFake, fakeLen);
            }
            MYDebug("fakeBuf = %s fakeLen = %d", fakeBuf, fakeLen);

            for (int j = 0; j < mUserNumbers; ++j)
            {
                strncat(tmpMessData->strPoint, fakeBuf, hashBitlen); // 全部填充为虚假数据
            }
            memcpy(tmpMessData->strPoint + (tmpMessData->id - 1)*hashBitlen,
                    jiamiBuf, hashBitlen); // 将对应位的真实加密数据补上去
            MYDebug("i = %d pinjiestr = %s", i, tmpMessData->strPoint);
        }
    }

    /*
    将生成的字符串转成gmp大数表示
    */
    int count = 0;
    mpz_t tmpMixed[mObserveObjNum]; // 混合的数据
    for (int i = 0; i < mUserNumbers; ++i)
    {
        for (int j = 0; j < mObserveObjNum; ++j)
        {
            if (0 == i)
            {
                mpz_init(tmpMixed[j]);
            }
            MESSDATA * tmpMessData = mEncryptionStr.at(count);
            if (tmpMessData)
            {
                mpz_set_str(tmpMpz[i][j], tmpMessData->strPoint, 16);
            }
            ++count;
        }
    }

    /*
     * 将对同一个观测数据的混合的数据，进行异或，可以看做是按列来异或
     * 互换行和列
    */
    for (int i = 0; i < mObserveObjNum; ++i)
    {
        for (int j = 0; j < mUserNumbers; ++j)
        {
            mpz_xor(tmpMixed[i], tmpMixed[i], tmpMpz[j][i]); /*将对应的列与tmpMixed进行异或*/
        }
    }

    for (int i = 0; i < mObserveObjNum; ++i)
    {
        char mixedBuf[800] = {0};
        mpz_get_str(mixedBuf, 16, tmpMixed[i]);
        MYDebug("mixedBuf = %s", mixedBuf);
        getCutStr(mixedBuf, strlen(mixedBuf));
    }

    for (int j = 0; j < mResultString.length(); ++j)
    {
        MYDebug(" the result %d = %d", j, mResultString.at(j));
    }

    for (int i = 0; i < mUserNumbers; ++i)
    {
        mpz_clear(tmpFakeMpz[i]);
        for (int j = 0; j < mObserveObjNum; ++j)
        {
//            MYDebug("i = %d j = %d", i, j);
            mpz_clear(tmpMpz[i][j]);
            if (0 == i)
            {
                mpz_clear(tmpMixed[j]);
            }
        }
    }
}
