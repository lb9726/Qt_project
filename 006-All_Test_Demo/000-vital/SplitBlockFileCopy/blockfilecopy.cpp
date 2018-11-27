#include "blockfilecopy.h"

BlockFileCopy::BlockFileCopy(QObject *parent) : QObject(parent)
//  , m_IsAverage(false)
{
    qDebug()<< __PRETTY_FUNCTION__;
}

BlockFileCopy::~BlockFileCopy()
{
    qDebug()<< __PRETTY_FUNCTION__;
//    int i = 0;
//    for(i = 0; i < MAXSIZE; i++)
//    {
//        delete wCopyThread[i].wThread;
//    }
}

// 将要拷贝的文件平均分割成不同的段，记录需要拷贝的大小,偏移量,拷贝的起点和终点
void BlockFileCopy::splitFileLength(const char *filename, int Max)
{
//    qDebug()<< __PRETTY_FUNCTION__;
//    QFile file(filename);
//    int flen = file.size();
//    int blocksize = flen / Max;
//    if (0 == flen - blocksize * Max) // 刚好能整除
//    {
//        int i = 0;
//        for(i = 0; i < Max; i++)
//        {
//            wCopyThread[i].id = i;
//            wCopyThread[i].wThread = new WorkThread();
//            wCopyThread[i].size = blocksize;
//            wCopyThread[i].offset_filehead = wCopyThread[i].id * wCopyThread[i].size;
//        }
//        m_IsAverage = true;
//    }
//    else if (0 <= flen - blocksize * Max) // 不能整除，最后一个块将比其他的多一点
//    {
//        int i = 0;
//        for(i = 0; i < Max-1; i++)
//        {
//            wCopyThread[i].id = i;
//            wCopyThread[i].wThread = new WorkThread();
//            wCopyThread[i].size = blocksize;
//            wCopyThread[i].offset_filehead = wCopyThread[i].id * wCopyThread[i].size;
//        }
//        wCopyThread[i].id = i;
//        wCopyThread[i].wThread = new WorkThread();
//        wCopyThread[i].size = flen - blocksize * (Max-1);
//        wCopyThread[i].offset_filehead = flen;
//        m_IsAverage = false;
//    }
}

