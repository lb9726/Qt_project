#include "saf.h"

safLine::safLine(QObject *parent) :
    QThread(parent)
{
    mSafFrameNum = 0;
    mSafChanged = false;
}

safLine::~safLine()
{

}

void safLine::run()
{
    QFile tmpExportFile(GPIO_EXPORT);
    if(tmpExportFile.open(QFile::WriteOnly))
    {
        tmpExportFile.write(QByteArray::number(D_PIN_SAF));
        tmpExportFile.close();
    }
    //Direction
    QFile tmpDirFile(GPIO_DIRECT(D_PIN_SAF));
    if(tmpDirFile.open(QFile::WriteOnly))
    {
        tmpDirFile.write("in");
        tmpDirFile.close();
    }

    quint8 tmpOldPinValue = 0;
    quint8 tmpFrameNumber = 0;
    quint8 tmpInvalidCount = 0;
    while(1)
    {
        //Value
        QFile tmpValueFile(GPIO_VALUE(D_PIN_SAF));
        if(tmpValueFile.open(QFile::ReadOnly))
        {
            QByteArray tmpArray = tmpValueFile.read(10);
            if(tmpArray[0] == D_PIN_SAF_LEVAL)  //有效电平
            {
                tmpInvalidCount = 0;

                if(tmpOldPinValue == 0)
                {
                    //逻辑层面上升沿
                    tmpFrameNumber++;
                }
                tmpOldPinValue = 1;
            }
            else
            {
                tmpOldPinValue = 0;

                tmpInvalidCount++;
                if(tmpInvalidCount >= D_SAF_INVALID_COUNT)
                {
                    //帧结束
                    if((tmpFrameNumber >0) && (tmpFrameNumber <70))
                    {
                        IDE_TRACE_INT(tmpFrameNumber);
                        if(mSafFrameNum != tmpFrameNumber)
                        {
                            mSafFrameNum = tmpFrameNumber;
                            mSafChanged = true;                            
                            emit sSafNum(mSafFrameNum);
                        }
                    }
                    tmpFrameNumber = 0;
                    tmpInvalidCount = 0;
                }
           }
           tmpValueFile.close();
        }
        msleep(1);
    }
}

safBus::safBus(QObject *parent) : QThread(parent)
{
    mScroll = false;
    mOldFrameNum = 0;
    mSAFLine = 0;
    mSAFLine = new safLine;
    mSAFLine->start(QThread::HighestPriority);
    //mSAFLine->start();
    //start();
}

safBus::~safBus()
{

}

void safBus::run()
{
    QList<int> tmpList;
    tmpList << D_PIN_FLM << D_PIN_FLD << D_PIN_SGO << D_PIN_TALK << D_PIN_SF;

    QFile tmpValueFile[5];

    for(int i=0;i<tmpList.count();i++)
    {
        int tmpPinNum = tmpList.at(i);

        QFile tmpExportFile(GPIO_EXPORT);
        if(tmpExportFile.open(QFile::WriteOnly))
        {
            tmpExportFile.write(QByteArray::number(tmpPinNum));
            tmpExportFile.close();
        }
        //Direction
        QFile tmpDirFile(GPIO_DIRECT(tmpPinNum));
        if(tmpDirFile.open(QFile::WriteOnly))
        {
            tmpDirFile.write("in");
            tmpDirFile.close();
        }
    }

    quint8 tmpSGOOld = 0;
    quint8 tmpTalkOld = 0;
    QFile  tmpFile;
    QByteArray tmpArray;
    while(1)
    {
        //打开gpio value管脚文件
        //读值
        //关闭

        //FLM
        tmpFile.setFileName(GPIO_VALUE(D_PIN_FLM));
        if(tmpFile.open(QFile::ReadOnly))
        {
            tmpArray = tmpFile.read(10);
            if(tmpArray[0] == D_PIN_OTHERS_LEVAL)  //有效电平
            {
                mFLMLevel = 1;
            }
            else
            {
                mFLMLevel = 0;
            }
            tmpFile.close();
        }

        // FLD
        tmpFile.setFileName(GPIO_VALUE(D_PIN_FLM));
        if(tmpFile.open(QFile::ReadOnly))
        {
            tmpArray = tmpFile.read(10);
            if(tmpArray[0] == D_PIN_OTHERS_LEVAL)  //有效电平
            {
                mFLDLevel = 1;
            }
            else
            {
                mFLDLevel = 0;
            }
            tmpFile.close();
        }

        // SGO
        tmpFile.setFileName(GPIO_VALUE(D_PIN_SGO));
        if(tmpFile.open(QFile::ReadOnly))
        {
            tmpArray = tmpFile.read(10);
            if(tmpArray[0] == D_PIN_OTHERS_LEVAL)  //有效电平
            {
                if(tmpSGOOld == 0)
                {
                    mSGOUpEdge = 1;
                }
                else
                {
                    mSGOUpEdge = 0;
                }
                tmpSGOOld = 1;
            }
            else
            {
                mSGOUpEdge = 0;
                tmpSGOOld =0;
            }
            tmpFile.close();
        }

        // TALK
        tmpFile.setFileName(GPIO_VALUE(D_PIN_TALK));
        if(tmpFile.open(QFile::ReadOnly))
        {
            tmpArray = tmpValueFile[3].read(10);
            if(tmpArray[0] == D_PIN_OTHERS_LEVAL)  //有效电平
            {
                if(tmpTalkOld == 0)
                {
                    mTALKUpEdge = 1;
                }
                else
                {
                    mTALKUpEdge = 0;
                }
                tmpTalkOld = 1;
            }
            else
            {
                mTALKUpEdge = 0;
                tmpTalkOld =0;
            }
            tmpFile.close();
        }


        // SF
        tmpFile.setFileName(GPIO_VALUE(D_PIN_SF));
        if(tmpFile.open(QFile::ReadOnly))
        {
            tmpArray = tmpFile.read(10);
            if(tmpArray[0] == D_PIN_OTHERS_LEVAL)  //有效电平
            {
                mSFLevel = 1;
            }
            else
            {
                mSFLevel = 0;
            }
            tmpFile.close();
        }

        parseFrame();

        msleep(50);
    }
}

void safBus::parseFrame()
{ //箭头和到站钟
    if(mSFLevel)
    {

    }
    else
    {
        if(mSAFLine->mSafFrameNum == 52)
        {
            mScroll = true;
        }
        else if(mSAFLine->mSafFrameNum == 53)
        {
            mScroll = false;
        }
        emit sigArrowAction((bool)mFLMLevel,(bool)mFLDLevel,mScroll);
        if(mSGOUpEdge)
        {
            if(mFLMLevel == 1 && mFLDLevel ==0)
            {
                emit sMessageAudio(40);
            }
            else if(mFLMLevel == 0 && mFLDLevel ==1)
            {
                emit sMessageAudio(41);
            }
            else if(mFLMLevel == 1 && mFLDLevel ==1)
            {
                emit sMessageAudio(42);
            }
        }
    }
    //楼层和楼层报站

    if((mSAFLine->mSafFrameNum>0 && mSAFLine->mSafFrameNum<39) || mSAFLine->mSafFrameNum== 70)
    {
        if(mSAFLine->mSafFrameNum != mOldFrameNum)
        {
            mOldFrameNum  = mSAFLine->mSafFrameNum;
            if(mTALKUpEdge)
            {
                emit sMessageAudio(mSAFLine->mSafFrameNum);
            }
        }
    }
}
