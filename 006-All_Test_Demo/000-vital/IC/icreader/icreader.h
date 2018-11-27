#ifndef ICREADER_H
#define ICREADER_H

#include "define.h"
#include "MUR200.h"

#define D_BEEP          (1)
#define D_LED_RED       (1<<1)
#define D_LED_GREEN     (1<<2)
#define D_LED_BLUE      (1<<3)

#define D_CONTROL       (1<<6)

#define D_RET_NONE          0
#define D_RET_NEWCARD       1
#define D_RET_NOCARD       -1

class IcReader : public QIODevice
{
    Q_OBJECT
public:
    IcReader(QObject *parent = 0);
    ~IcReader();

    bool Open(OpenMode mode);
    void Close();
    bool IsOpen();

    bool SetLedBeep(int pLedBeep, int pOpenTime, int pCloseTime, int pLoop);

    bool        HasCard();
    int         DetectCard();  //1:检测到新卡，0：卡片状态没有变化，-1：检测到卡离开

    QString     GetCardNum();

    qint32      WriteCard(quint32 pBlock, QByteArray pData);
    QByteArray  ReadCard(quint32 pBlock, qint32 pLen);

    qint64 readData(char *data, qint64 maxlen){}
    qint64 writeData(const char *data, qint64 len){}

private:
    ACTIVEPARAA         mTypeAActiveInfo;
    QByteArray          mReadBuffer;
    bool                mCardExist;  //是否有卡
};

#endif // ICREADER_H
