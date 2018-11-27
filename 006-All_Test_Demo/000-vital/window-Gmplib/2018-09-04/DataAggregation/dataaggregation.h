#ifndef DATAAGGREGATION_H
#define DATAAGGREGATION_H

#include <QObject>
#include <QList>
#include <QElapsedTimer>

#include "HeaderFile/gmp.h"

class DataAggregation : public QObject
{
    Q_OBJECT
private:
    typedef struct __TEXTDATA
    {
        int id;
        QString content;
        mpz_t pubKey;           // 公钥
        mpz_t privateKey;       // 私钥
        mpz_t encryptionKey;    // 加密秘钥
//        mpz_t fakeData;
    }TEXTDATA;

    typedef struct __MESSDATA {
        int id;
//        int index;
        int len;
//        int curPos;
        char *strPoint;
    }MESSDATA;

    void initAllParameters();
    void generateG(mpz_t g, mpz_t pBigPrimep);
    void generateRandom(mpz_t pRandom, unsigned int pNumLen);
    void generatePublicKey(mpz_t pPubKey, mpz_t pG, mpz_t pPriKey, mpz_t pBigP);
    void getXor(mpz_t skij, mpz_t pPubKey, mpz_t pPriKey);
    void getCutStr(char *str, int pLen);

public:
    explicit DataAggregation(QObject *parent = nullptr);
    ~DataAggregation();
    void readDataFile(QString fileName); /**/
    void printQList();
    int getUserNumbers() const;
    void generateBigPrime();
    void checkBigPrime();
    void generatePublicKeyAndPrivateKeyAndEncryptionKey();
    void getHashNum(char *pStr, char *pDest);
    void testCmp(mpz_t g, mpz_t p);
    void checkPrivateKey();
    void checkPubKey();
    void encryptionFileData();

signals:

public slots:

private:
    QList<TEXTDATA*> mTextData;
    QList<MESSDATA*> mEncryptionStr;
    int mUserNumbers;
    QElapsedTimer mCountTime;
    QString mRunPath;
    mpz_t g;                // 生成元,大于等于2，小于等于p
    mpz_t mBigPrimeP;
    unsigned int mLastRandom;
    int mObserveObjNum;
    QList<int> mResultString;
};

#endif // DATAAGGREGATION_H
