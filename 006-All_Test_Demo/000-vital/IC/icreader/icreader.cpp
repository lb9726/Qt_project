#include "icreader.h"

IcReader::IcReader(QObject *parent) : QIODevice(parent)
{
    mCardExist = false;
}

IcReader::~IcReader()
{
    mCardExist = false;
}

bool IcReader::IsOpen()
{
    int tmpReaderCount = MUR200Init();
    if(tmpReaderCount <= 0)
    {
        Close();
        return false;
    }
    return QIODevice::isOpen();
}

bool IcReader::Open(OpenMode mode)
{
    if(!IsOpen())
    {
        mCardExist = false;
        char Status;
        if (Status = MUR200Open(0))
        {
            return false;
        }
        Status = PiccReset(10, 10); //产生RF场间隙
        if(STATUS_SUCCESS != Status)
        {
            return false;
        }
        return QIODevice::open(mode);
    }
    return true;
}

void IcReader::Close()
{
    mCardExist = false;
    if(QIODevice::isOpen())
    {
        MUR200Exit();
        QIODevice::close();
    }
}

bool IcReader::SetLedBeep(int pLedBeep, int pOpenTime, int pCloseTime, int pLoop)
{
    if(!QIODevice::isOpen())
        return false;
    char Status = SetIO(pLedBeep, pOpenTime, pCloseTime, pLoop);
    if(STATUS_SUCCESS != Status)
    {
        return false;
    }
    return true;
}

bool IcReader::HasCard()
{
    return mCardExist;
}

QString IcReader::GetCardNum()
{
    int i = mTypeAActiveInfo.UIDLen-1;
    QString tmpNum;
    while(i >=0 )
    {
        tmpNum.append(QString::number(mTypeAActiveInfo.UID[i], 16));
        tmpNum.append(QString(" "));
        i--;
    }
    return tmpNum;
}

int IcReader::DetectCard()
{
    char Status = MFActivate('0', ISO14443_3A_REQALL, &mTypeAActiveInfo); //激活Mifare卡
    if(STATUS_SUCCESS == Status) //检测到卡
    {
        if(!mCardExist)
        {
            IDE_TRACE_INT(mTypeAActiveInfo.UIDLen);
            IDE_DEBUG(QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10")
                                .arg(mTypeAActiveInfo.UID[0])
                                .arg(mTypeAActiveInfo.UID[1])
                                .arg(mTypeAActiveInfo.UID[2])
                                .arg(mTypeAActiveInfo.UID[3])
                                .arg(mTypeAActiveInfo.UID[4])
                                .arg(mTypeAActiveInfo.UID[5])
                                .arg(mTypeAActiveInfo.UID[6])
                                .arg(mTypeAActiveInfo.UID[7])
                                .arg(mTypeAActiveInfo.UID[8])
                                .arg(mTypeAActiveInfo.UID[9]));
            mCardExist = true;
            return D_RET_NEWCARD;
        }
    }
    else
    {
        if(mCardExist)
        {
            mCardExist = false;
            return D_RET_NOCARD;
        }
    }
    return D_RET_NONE;
}

qint32 IcReader::WriteCard(quint32 pBlock, QByteArray pData)
{
    //验证秘钥keyA和keyB
    unsigned char ucaTempBuf[16];
    ucaTempBuf[0] = pBlock;
    char Status = MFAuth(KEY_SOURCE_E2 | KEY_TYPE_A, pBlock * 4, ucaTempBuf);
    if(STATUS_SUCCESS != Status)
    {
        return -1;
    }
    //写数据
    int count = pData.count();
    int blkNum = count / 16;
    int blkMore = count % 16;
    if(blkMore)
        blkNum++;
    unsigned char *tmpBuffer = (unsigned char *)pData.data();
    Status = MFWrite(pBlock * 4, blkNum, tmpBuffer);
    if(STATUS_SUCCESS != Status)
    {
        return -1;
    }
    return count;
}

QByteArray IcReader::ReadCard(quint32 pBlock, qint32 pLen)
{
    //验证秘钥keyA和keyB
    unsigned char ucaTempBuf[16];
    ucaTempBuf[0] = pBlock;
    char Status = MFAuth(KEY_SOURCE_E2 | KEY_TYPE_A, pBlock * 4, ucaTempBuf);
    if(STATUS_SUCCESS != Status)
    {
        return QByteArray();
    }
    //读数据
    int blkNum = pLen / 16;
    int blkMore = pLen % 16;
    if(blkMore)
        blkNum++;
    mReadBuffer.resize(blkNum * 16);
    mReadBuffer.fill(0);
    unsigned char *tmpBuffer = (unsigned char *)mReadBuffer.data();
    Status = MFRead(pBlock * 4, blkNum, tmpBuffer);
    if(STATUS_SUCCESS != Status)
    {
        return QByteArray();
    }
    return mReadBuffer;
}

