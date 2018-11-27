#include "tcpclient.h"
#include <QCoreApplication>

#define BVT_STX (0x80)        /* 帧起始字符 */
#define BVT_ETX (0x81)        /* 帧结束字符 */
#define BVT_ESC (0x1B)        /* 转换字符 */

#define BVT_STX_AF (0xE7)     /* 帧起始转换后增加字符 */
#define BVT_ETX_AF (0xE8)     /* 帧结束转换后增加字符 */
#define BVT_ESC_AF (0x00)     /* 转换字符转换后增加字符 */

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    mPort = 60001;
    mIpString = QString("192.168.1.10");
}

TcpClient::~TcpClient()
{
    mTcpSocket.disconnectFromHost();
    mTcpSocket.close();
}

int TcpClient::writeMsgToServer(QByteArray msg, int length)
{
    int tmplen = mTcpSocket.write(msg.data(), length);
    return tmplen;
}

void TcpClient::connectToServer()
{
    mTcpSocket.connectToHost(mServerIP, mPort);
}

QByteArray &TcpClient::generateBuffer(QByteArray &buffer)
{
    mSendBufferFrame.clear();
    int j = 0;
    if (judgeArrayIsEmpty(buffer))
    {
        return mSendBufferFrame;
    }
    if (j < buffer.length()) // 满足条件先加入帧头
    {
        mSendBufferFrame.append(BVT_STX);
    }

    // 校验位是本数据中不包含帧头和帧尾 但包含ID 的时候计算出来的
    char crc = getCrcVerify(buffer, buffer.length());
    QByteArray addCrcBuffer = buffer.append(crc);
    int i = addCrcBuffer.length();
    char tmp;
    while (j < i)
    {
        tmp = addCrcBuffer.at(j);
        if ((quint8)tmp == BVT_STX)
        {
            mSendBufferFrame.append((char)BVT_ESC);
            mSendBufferFrame.append((char)BVT_STX_AF);
        }
        else if ((quint8)tmp == BVT_ETX)
        {
            mSendBufferFrame.append((char)BVT_ESC);
            mSendBufferFrame.append((char)BVT_ETX_AF);
        }
        else if ((quint8)tmp == BVT_ESC)
        {
            mSendBufferFrame.append((char)BVT_ESC);
            mSendBufferFrame.append((char)BVT_ESC_AF);
        }
        else
        {
            mSendBufferFrame.append(tmp);
        }
        ++j;
    }
    mSendBufferFrame.append((char)BVT_ETX); // 加入帧尾
    return mSendBufferFrame;
}

char TcpClient::getCrcVerify(QByteArray msg, int length)
{
    char crc = 0x00;
    for (int i = 0; i < length; ++i)
    {
        crc ^= msg.at(i);
    }
    return crc & 0x7f;
}

void TcpClient::setIpString(const QString &ipString)
{
    mIpString = ipString;
}

void TcpClient::setPort(int port)
{
    mPort = port;
}

bool TcpClient::judgeArrayIsEmpty(const QByteArray &buffer)
{
    bool flag = true;
    if (buffer.isNull() || buffer.isEmpty())
    {
        flag = true;
    }
    else
    {
        flag = false;
    }
    return flag;
}
