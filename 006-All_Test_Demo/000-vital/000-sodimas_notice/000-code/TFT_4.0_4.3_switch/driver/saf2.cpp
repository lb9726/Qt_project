#include "saf.h"
#include "dipsio.h"

safLine::safLine(QObject *parent) : QThread(parent) //D01
{
    mSafNum = 0;
    QFile tmpExportFile(GPIO_EXPORT);
    if(tmpExportFile.open(QFile::WriteOnly))
    {
        tmpExportFile.write(QByteArray::number(D_PIN_SAF));
        tmpExportFile.close();
    }
    QFile tmpDirFile(GPIO_DIRECT(D_PIN_SAF));
    if(tmpDirFile.open(QFile::WriteOnly))
    {
        tmpDirFile.write("in");
        tmpDirFile.close();
    }
    start();
}

void safLine::run()
{
    QFile tmpValueFile(GPIO_VALUE(D_PIN_SAF));
    if(!tmpValueFile.open(QFile::ReadOnly))
    {
        return;
    }

    quint8 tmpSafFlag = 0;
    quint8 tmpSafNum = 0;
    quint8 tmpInvalidNum = 0;

    while(1)
    {
        QByteArray tmpSafArray = tmpValueFile.read(10);
        if(tmpSafArray == D_PIN_SAF_LEVEL)
        {
            tmpInvalidNum = 0;
            if(tmpSafFlag == 0)
            {
                tmpSafNum ++;
                tmpSafFlag = 1;
            }
        }
        else
        {
            tmpInvalidNum ++ ;
            if(tmpInvalidNum >= D_SAF_MAX_TIME)  //帧结束
            {
                if((tmpSafNum >0) && (tmpSafNum <70))
                {
                    if(mSafNum != tmpSafNum)
                    {
                        mSafNum = tmpSafNum;
                    }
                }
                tmpSafNum = 0;
                tmpInvalidNum = 0;
            }
            tmpSafFlag = 0;
        }
        msleep(1);
    }
}

safBus::safBus(QObject *parent): QThread(parent)
{
    start();
}

void safBus::run()
{
    QList<int> tmpPinList;
    tmpPinList << D_PIN_FLM << D_PIN_FLD << D_PIN_SGO << D_PIN_TALK << D_PIN_SF;

    QFile tmpValueFile[5];

    QFile tmpExportFile(GPIO_EXPORT);
    for(int i=0;i<tmpPinList;i++)
    {
        int tmpPin = tmpPinList.at(i);
        if(tmpExportFile.open(QFile::WriteOnly))
        {
            tmpExportFile.write(QByteArray::number(tmpPin));
            tmpExportFile.close();
        }

        QFile tmpDirFile(GPIO_DIRECT(tmpPin));
        if(tmpDirFile.open(QFile::WriteOnly))
        {
            tmpDirFile.write("in");
            tmpDirFile.close();
        }

        tmpValueFile[i](GPIO_VALUE(tmpPin));
        if(!tmpDirFile.open(QFile::ReadOnly))
        {
            return;
        }
    }

    quint8 tmpOldSGO = 0;
    quint8 tmpOldTalk = 0;
    while(1)
    {
        QByteArray tmpArray = tmpValueFile[0].read(10);
        if(tmpArray.startsWith(D_PIN_SAF_OTHERLEVEL))
        {
            mFLMLevel = 1;
        }
        else
        {
            mFLMLevel = 0;
        }

        tmpArray = tmpValueFile[1].read(10);
        if(tmpArray.startsWith(D_PIN_SAF_OTHERLEVEL))
        {
            mFLDLevel = 1;
        }
        else
        {
            mFLDLevel = 0;
        }

        tmpArray = tmpValueFile[2].read(10);
        if(tmpArray.startsWith(D_PIN_SAF_OTHERLEVEL))
        {
            if(tmpOldSGO == 0)
            {
                mSGOUpEdge = 1;
                tmpOldSGO = 1;
            }
            else
            {
                mSGOUpEdge = 0;
            }
        }
        else
        {
            tmpOldSGO = 0;
            mSGOUpEdge = 0;
        }

        tmpArray = tmpValueFile[3].read(10);
        if(tmpArray.startsWith(D_PIN_SAF_OTHERLEVEL))
        {
            if(tmpOldTalk == 0)
            {
                mTalkUpEdge = 1;
                tmpOldTalk = 1;
            }
            else
            {
                mTalkUpEdge = 0;
            }
        }
        else
        {
            mTalkUpEdge = 0;
            tmpOldTalk = 0;
        }

        tmpArray = tmpValueFile[4].read(10);
        if(tmpArray.startsWith(D_PIN_SAF_OTHERLEVEL))
        {
            mSFLevel = 1;
        }
        else
        {
            mSFLevel = 0;
        }

        PraseFrame();
    }
    msleep(50);
}

void safBus::PraseFrame()
{
    if(mSFLevel)
    {

    }
    else
    {
        if(mSafLine->mSafNum == 52) //箭头滚动
        {
            mScroll = true;
        }
        else if(mSafLine->mSafNum == 53) //箭头不滚动
        {
            mScroll = false;
        }
        emit sigArrowAction((bool)mFLMLevel,(bool)mFLDLevel,mScroll);

        if(mSGOUpEdge)  //到站钟，播报叮当咚可选
        {
            if(mFLMLevel == 1 && mFLDLevel == 0)
            {
                emit sMessageAudio(40);
            }
            else if(mFLMLevel == 0 && mFLDLevel == 1)
            {
                emit sMessageAudio(41);
            }
            else if(mFLMLevel == 1 && mFLDLevel == 1)
            {
                emit sMessageAudio(42);
            }
        }
    }

    if((mSafLine->mSafNum>0 && mSafLine->mSafNum<39)||(mSafLine->mSafNum == 70))
    {
        if(mOldSAFNum != mSafLine->mSafNum)
        {
            mOldSAFNum = mSafLine->mSafNum;
            if(mTalkUpEdge)
            {
                emit sMessageAudio(mSafLine->mSafNum);
            }
        }
    }
}
