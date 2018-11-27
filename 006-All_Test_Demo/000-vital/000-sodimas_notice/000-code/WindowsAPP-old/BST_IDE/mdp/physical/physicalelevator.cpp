#include "physicalelevator.h"
#include "deviceinc.h"
#include "physicallcd.h"
#include "physicalnet.h"
#include "logicthememanager.h"
#ifdef SUPPORT_SCENE
#include "graphicsmedia.h"
#elif SUPPORT_WIDGET
#include "widgetmedia.h"
#endif

ElevatorTestcaseParse::ElevatorTestcaseParse(QString pFile) :
    QObject()
{
    mInvalid = false;
    SetTestFile(pFile);
}

ElevatorTestcaseParse::~ElevatorTestcaseParse()
{
    ClearJsTestcaseList();
    ClearIniTestcaseList();
}

void ElevatorTestcaseParse::ClearJsTestcaseList()
{
    QStringList tmpKeyList = mJsTestcaseList.keys();
    if(tmpKeyList.isEmpty())
        return;
    for(int i=tmpKeyList.count()-1;i>=0;i--)
    {
        JS_TEST_CASE *tmpTestcase = mJsTestcaseList.take(tmpKeyList.at(i));
        if(tmpTestcase)
            delete tmpTestcase;
    }
}

void ElevatorTestcaseParse::ClearIniTestcaseList()
{
    QStringList tmpKeyList = mIniTestcaseList.keys();
    if(tmpKeyList.isEmpty())
        return;
    for(int i=tmpKeyList.count()-1;i>=0;i--)
    {
        INI_TEST_CASE *tmpTestcase = mIniTestcaseList.take(tmpKeyList.at(i));
        if(tmpTestcase)
        {
            tmpTestcase->mFrameList.clear();
            delete tmpTestcase;
        }
    }
}

bool ElevatorTestcaseParse::SetTestFile(QString pFile)
{
    mInvalid = false;
    QFileInfo tmpFile(pFile);
    if(tmpFile == mTestCaseFile)
        return false;
    if(!tmpFile.exists())
        return false;
    mTestCaseFile = tmpFile;
    if(!mTestCaseFile.suffix().compare("js", Qt::CaseInsensitive))
    {
        QFile tmpFile(pFile);
        if(!tmpFile.exists())
            return false;
        if(!tmpFile.open(QFile::ReadOnly))
            return false;
        ClearJsTestcaseList();
        while(!tmpFile.atEnd())
        {
            QByteArray tmpLine = tmpFile.readLine();
            QString tmpFunName = GetFuncName(tmpLine);
            if(tmpFunName.isEmpty())
                continue;
            if(tmpFunName.endsWith(RESULTSUFFIX))
                continue;
            QString tmpFuncScript;
            QString tmpProcScript;
            tmpFuncScript.append(tmpLine);
            while(!tmpFile.atEnd())
            {
                QByteArray tmpFuncLine = tmpFile.readLine();
                tmpFuncScript.append(tmpFuncLine);
                if(tmpFuncLine.startsWith("}"))
                    break;
            }
            QByteArray tmpProcLine;
            QString tmpProcName;
            while(!tmpFile.atEnd())  //>@寻找result函数
            {
                tmpProcLine = tmpFile.readLine();
                tmpProcName = GetFuncName(tmpProcLine);
                if(!tmpProcName.isEmpty())
                    break;
            }
            if(!tmpProcName.endsWith(RESULTSUFFIX))  //>@无效的Case，丢弃
                continue;
            tmpProcScript.append(tmpProcLine);
            while(!tmpFile.atEnd())
            {
                tmpProcLine = tmpFile.readLine();
                tmpProcScript.append(tmpProcLine);
                if(tmpProcLine.startsWith("}"))
                    break;
            }
            JS_TEST_CASE *tmpTestcase = new JS_TEST_CASE;
            tmpTestcase->mFuncScript = tmpFuncScript;
            tmpTestcase->mProcScript = tmpProcScript;
            mJsTestcaseList.insert(tmpFunName, tmpTestcase);
        }
    }
    else if(!mTestCaseFile.suffix().compare("ini", Qt::CaseInsensitive))
    {
        iniParse *tmpIniParse = new iniParse(pFile);
        if(tmpIniParse->status() != QSettings::NoError)
        {
            tmpIniParse->deleteLater();
            return false;
        }
        ClearIniTestcaseList();
        QStringList tmpGroupList = tmpIniParse->childGroups();
        QString tmpGroup;
        foreach(tmpGroup, tmpGroupList)
        {
            if(tmpGroup.isEmpty())
                continue;
            INI_TEST_CASE *tmpTestcase = new INI_TEST_CASE;
            mIniTestcaseList.insert(tmpGroup, tmpTestcase);
            //>@
            tmpIniParse->beginGroup(tmpGroup);
            int tmpNotation = 10;//>@默认10进制
            QStringList tmpParaList = tmpGroup.split('-');
            if(tmpParaList.count()>1)
            {
                if(!tmpParaList.at(1).compare("hex"))
                    tmpNotation = 16;
                else if(!tmpParaList.at(1).compare("dec"))
                    tmpNotation = 10;
                else if(!tmpParaList.at(1).compare("oct"))
                    tmpNotation = 8;
                else if(!tmpParaList.at(1).compare("bin"))
                    tmpNotation = 2;
            }
            QStringList tmpKeyList = tmpIniParse->childKeys();
            QString tmpKey;
            foreach(tmpKey, tmpKeyList)
            {
                if(tmpKey.isEmpty())
                    continue;
                QStringList tmpFrameList = tmpIniParse->value(tmpKey).toString().split(' ', QString::SkipEmptyParts);
                QString tmpFrame;
                foreach(tmpFrame, tmpFrameList)
                {
                    if(tmpFrame.isEmpty())
                        continue;
                    QStringList tmpNumList = tmpFrame.split('.');
                    QString tmpNum;
                    QByteArray tmpArray;
                    bool success = true;
                    foreach(tmpNum, tmpNumList)
                    {
                        bool ok = false;
                        char tmpValue = (char)(tmpNum.toInt(&ok, tmpNotation));
                        if(!ok)
                        {
                            IDE_TRACE();
                            success = false;
                            break;
                        }
                        tmpArray.append(tmpValue);
                    }
                    if(success)
                        tmpTestcase->mFrameList.append(tmpArray);
                }
            }
        }
    }
    mInvalid = true;
    return true;
}

ElevatorFrameErrorParse::ElevatorFrameErrorParse(QString pFile) :
    QObject()
{
    mInvalid = false;
    SetFrameErrorFile(pFile);
}

ElevatorFrameErrorParse::~ElevatorFrameErrorParse()
{
    ClearIniFrameErrorList();
}

void ElevatorFrameErrorParse::ClearIniFrameErrorList()
{
    QStringList tmpKeyList = mIniFrameErrorList.keys();
    if(tmpKeyList.isEmpty())
        return;
    for(int i=tmpKeyList.count()-1;i>=0;i--)
    {
        INI_FRAME_ERROR *tmpFrameError = mIniFrameErrorList.take(tmpKeyList.at(i));
        if(tmpFrameError)
        {
            tmpFrameError->mOriginFrameList.clear();
            tmpFrameError->mParsedFrameList.clear();
            delete tmpFrameError;
        }
    }
}

bool ElevatorFrameErrorParse::SetFrameErrorFile(QString pFile)
{
    mInvalid = false;
    QFileInfo tmpFile(pFile);
    if(tmpFile == mFrameErrorFile)
        return false;
    if(!tmpFile.exists())
        return false;
    mFrameErrorFile = tmpFile;
    if(!mFrameErrorFile.suffix().compare("ini", Qt::CaseInsensitive))
    {
        iniParse *tmpIniParse = new iniParse(pFile);
        if(tmpIniParse->status() != QSettings::NoError)
        {
            tmpIniParse->deleteLater();
            return false;
        }
        ClearIniFrameErrorList();
        QStringList tmpGroupList = tmpIniParse->childGroups();
        QString tmpGroup;
        foreach(tmpGroup, tmpGroupList)
        {
            if(tmpGroup.isEmpty())
                continue;
            INI_FRAME_ERROR *tmpFrameError = new INI_FRAME_ERROR;
            mIniFrameErrorList.insert(tmpGroup, tmpFrameError);
            //>@
            tmpIniParse->beginGroup(tmpGroup);
            QStringList tmpFrameList = tmpIniParse->value(D_FRAMEERROR_ORIGIN).toString().split(' ', QString::SkipEmptyParts);
            QString tmpFrame;
            foreach(tmpFrame, tmpFrameList)
            {
                if(tmpFrame.isEmpty())
                    continue;
                QStringList tmpNumList = tmpFrame.split('.');
                QString tmpNum;
                QByteArray tmpArray;
                foreach(tmpNum, tmpNumList)
                    tmpArray.append((char)(tmpNum.toInt()));
                tmpFrameError->mOriginFrameList.append(tmpArray);
            }
            tmpFrameList = tmpIniParse->value(D_FRAMEERROR_PARSED).toString().split(' ', QString::SkipEmptyParts);
            foreach(tmpFrame, tmpFrameList)
            {
                if(tmpFrame.isEmpty())
                    continue;
                QStringList tmpNumList = tmpFrame.split('.');
                QString tmpNum;
                QByteArray tmpArray;
                foreach(tmpNum, tmpNumList)
                    tmpArray.append((char)(tmpNum.toInt()));
                tmpFrameError->mParsedFrameList.append(tmpArray);
            }
        }
    }
    mInvalid = true;
    return true;
}

PhysicalElevator::PhysicalElevator(QObject *parent) :
    PhysicalProtocol(parent)
{
    mDebugTimeoutCount = 0;
    mDebugMonitorEn = true;

    Connected();

    mHasArrow = false;

    mEnvSaved = false;
    mTestcaseParse = 0;
    mFrameErrorParse = 0;
    mTemporaryTest = false;
    mSimulateMode = false;
    mInitialized = false;
    baiwei = shiwei = gewei = checkfun = raidofunc = audionum = extendfunc =0;
}

PhysicalElevator::~PhysicalElevator()
{
    if(!mTestcaseParse)
        mTestcaseParse->deleteLater();
    if(!mFrameErrorParse)
        mFrameErrorParse->deleteLater();
}

bool PhysicalElevator::InitTestcase(QString pFile)
{
    if(!mTestcaseParse)
        mTestcaseParse = new ElevatorTestcaseParse(pFile);
    if(!mTestcaseParse->mInvalid)
    {
        mTestcaseParse->deleteLater();
        mTestcaseParse = 0;
        return false;
    }
    return true;
}

bool PhysicalElevator::InitFrameError(QString pFile)
{
    if(!mFrameErrorParse)
        mFrameErrorParse = new ElevatorFrameErrorParse(pFile);
    if(!mFrameErrorParse->mInvalid)
    {
        mFrameErrorParse->deleteLater();
        mFrameErrorParse = 0;
        return false;
    }
    return true;
}

void PhysicalElevator::slot_Initialize()
{
    PhysicalProtocol::slot_Initialize();
#ifdef IDE
    mLANC = VAR_CAST<PhysicalLANC*>(m_DevManager->m_PHDevList.value(P_LANC));
#elif DISPLAY
    mLANC = VAR_CAST<PhysicalLANC*>(m_DevManager->m_PHDevList.value(P_LANC));
#endif
    if(!mLANC.isNull())
        connect(mLANC, SIGNAL(sNetState(bool)), this, SLOT(slot_NetState(bool)));
}

//>@不论WIFI建立和退出连接，都要重新刷新超时值。
void PhysicalElevator::slot_NetState(bool pFlag)
{
    IDE_TRACE_INT(pFlag);
    bool tmpEnable = !pFlag;
    mDebugMonitorEn = tmpEnable;
    if(pFlag)
    {
        mDebugTimeoutCount = (mMonitorCount + mDebugTimeout.mNum/100) % D_MAXTIMEOUT;
    }
    else
    {
        mDebugTimeoutCount = (mMonitorCount + mDebugTimeout.mNum/100) % D_MAXTIMEOUT;
        SetTemporaryTest(false, false); //>@连接断线后，退出临时测试模式
    }
}

void PhysicalElevator::Clear()
{
    baiwei = 0;
    shiwei = 0;
    gewei = 0;
    checkfun = 0;
    raidofunc = 0;
    audionum = 0;
    extendfunc = 0x0f;
}

void PhysicalElevator::GotoTest(bool pEnable, bool pNormal)
{
    if(pEnable == true)
    {
        //>@如果音频禁能掉了，则使能音频
        emit sInfo(QString(), MSG_TEMPORARY, DM_NET);
        emit sInfo(QString("Autotest started!"), 1000);
        PhysicalPlayer *tmpPlayer = mPlayer.data();
        if(tmpPlayer)
        {
            tmpPlayer->SetEnable(true, DM_NET);
        }
        LogicThemeManager *tmpManager = mThemeManager.data();
        if(tmpManager)
        {
            IDE_TRACE();
            tmpManager->SetAutoTest(true);
        }
    }
    else
    {
        emit sInfo(QString("Autotest finished!"), MSG_HOLD);
        if(pNormal)
        {
            //>如果当前player依旧是禁能状态，则关闭音乐
            PhysicalPlayer *tmpPlayer = mPlayer.data();
            if(tmpPlayer)
            {
                tmpPlayer->SetEnable(false, DM_NORMAL, true);
            }
        }
        LogicThemeManager *tmpManager = mThemeManager.data();
        if(tmpManager)
        {
            IDE_TRACE();
            tmpManager->SetAutoTest(false);
        }
    }
}

void PhysicalElevator::OpenWIFI()
{
    if(!mLANC.isNull())
    {
        mLANC.data()->OpenWIFI();
        mDebugTimeoutCount = (mMonitorCount + mDebugTimeout.mNum/100) % D_MAXTIMEOUT;
    }
}

void PhysicalElevator::CloseWIFI()
{
    if(!mLANC.isNull())
        mLANC.data()->CloseWIFI();
}

void PhysicalElevator::slot_DebugTimeout()
{
    CloseWIFI();
#ifdef SODIMAS
    if(!mPlayer.isNull())
    {
        IDE_TRACE();
        mPlayer.data()->Restore(AO_ADMUSIC, 1);
    }
#endif
}

void PhysicalElevator::SetAutoTest(bool pEnable, bool pNormal)
{
    mAutoTestEnable = pEnable;
    IDE_TRACE_INT(mAutoTestEnable);
    GotoTest(pEnable, pNormal);
}

//>@pNormal为true时表示正常关闭，为false表示因为网络断线强制关闭
void PhysicalElevator::SetTemporaryTest(bool pEnable, bool pNormal)
{
    //>@清空状态，比如箭头滚动等
    Clear();

    mTemporaryTest = pEnable;
    IDE_TRACE_INT(mTemporaryTest);
    GotoTest(pEnable, pNormal);
}

QByteArray PhysicalElevator::GetSavedFrame()
{
    quint8 tmpEnable = 0;
    QByteArray tmpArray;
    tmpArray.append((char)0x80);
    tmpArray.append((char)0x01);       //data 1
    if(mFlrLogoInfoSaved.isFloor())
    {
        tmpEnable |= 0x01;
        tmpArray.append((char)((mFlrLogoInfoSaved.mFloor>>16)&0xff));     //data 2
        tmpArray.append((char)((mFlrLogoInfoSaved.mFloor>>8)&0xff));     //data 3
        tmpArray.append((char)((mFlrLogoInfoSaved.mFloor)&0xff));      //data 4
    }
    tmpArray.append((char)mCheckFunSaved);   //data 5
    tmpEnable |= 0x02;
    if(mFlrLogoInfoSaved.isLogo())
    {
        tmpEnable |= 0x04;
        tmpArray.append((char)mFlrLogoInfoSaved.mLogo);      //data 6
    }
    tmpArray.append((char)0);      //data 7
    tmpArray.append((char)tmpEnable);      //data 8
    tmpArray.append((char)0);
    tmpArray.append((char)0x81);
    return tmpArray;
}

PhysicalCOMHost::PhysicalCOMHost(QString pAddr, QString pBufferPath, QObject *parent) :
    PhysicalHost(pAddr, pBufferPath, parent)
{
    mDevType = TYPE_COM;
}

PhysicalCOMHost::~PhysicalCOMHost()
{

}

bool PhysicalCOMHost::Scan()
{
    return false;
}

bool PhysicalCOMHost::isSync()
{
    return false;
}

bool PhysicalCOMHost::Sync()
{

}

void PhysicalCOMHost::Async()
{

}

bool PhysicalCOMHost::Download(QString pThemeName, QString pThemePath)
{
    return false;
}

bool PhysicalCOMHost::Upload(QString pThemeName)
{
    return false;
}

quint32 MonachThread::ParseFrameBuffer(PH_Type pType, QIODevice *pDev, QByteArray pFrameBuffer)
{
    //>@记录时间
    mBuffer.append(pFrameBuffer);

}

PhysicalCOM::PhysicalCOM(QObject *parent) :
    PhysicalElevator(parent)
{
    isBusy = false;

    mDebugTimeoutCount = D_MAXTIMEOUT+1;
    mNoArrowMode = 1; //>@默认认为开启后没有信号。

    mNoArrowFreqChange = false;
    mTestcase = 0;

    mCanopenMode = 0;
    mMonitorCount = 0;

    m_IBMonitorEnable = false;
    m_IBMonitorWait.SetTimeout(20000);
    m_IBMonitorWait.update(0, m_DevManager->m_DevMonitorFreq);

#ifndef IOS
    mSerialPort = new SerialPort;
#endif

    m_Bootup = false;
    mDataEnable = false;

    Connected();
}

PhysicalCOM::~PhysicalCOM()
{
#ifndef IOS
    if(!mSerialPort)
        return;
    mSerialPort->close();
    mSerialPort->deleteLater();
    mSerialPort = 0;
#endif
}

bool PhysicalCOM::PrepareRelease()
{
#ifndef IOS
    if(mSerialPort)
    {
        m_CurDevType = P_COM;
        m_CurTransDev = mSerialPort;
        int ret = SendConfFrame(QString("cmd#reboot"));
        emit sInfo(QString("reboot extend chip - %1").arg(ret));
        QtSleep(100);
    }
#endif
}

bool PhysicalCOM::SetEnable(bool pFlag, E_DEVMODE pDevMode, bool pForce)
{
    if(pFlag)
    {
#ifndef IOS
        if(!mSerialPort->isOpen())   //>@必须加上，否则每次使能设备时都会将串口重新打开一次，这样会把串口缓冲区的数据清空的。
            mSerialPort->open(QIODevice::ReadWrite, false);
#endif
    }
    return IODeviceBase::SetEnable(pFlag, pDevMode, pForce);
}

qint64 PhysicalCOM::write(QByteArray data)
{
#ifndef IOS
    if(mSerialPort)
        return mSerialPort->write(data);
#endif
    return -1;
}

qint64 PhysicalCOM::reply(QByteArray data)
{
    return write(data);
}

bool PhysicalCOM::SetValue(QString pValue)
{
    if(pValue.isEmpty())
        return true;
    QStringList tmpList = pValue.split(';');
    for(int i=0;i<tmpList.count();i++)
    {
        QStringList tmpCmdList = tmpList.at(i).split(':');
        if(tmpCmdList.count() < 2)
            continue;
        QString tmpName = tmpCmdList.at(0);
        QString tmpValue = tmpCmdList.at(1);
        if(!tmpName.compare("Autotest", Qt::CaseInsensitive))
        {
            if(!tmpValue.compare("on", Qt::CaseInsensitive))
            {
                SetTemporaryTest(true, true);
            }
            else
            {
                SetTemporaryTest(false, true);
            }
        }
        else if(!tmpName.compare("Powerup", Qt::CaseInsensitive))
        {
            mCanopenMode = 0;
            emit sInfo(tmpName, tmpValue.toInt(), DM_NORMAL);
            emit sOperateTheme(OPERATE_CLEAR, QVariant());
            mFloorLogoInfo.clear();
        }
        else if(!tmpName.compare("Initializing", Qt::CaseInsensitive))
        {
            mCanopenMode = 2;
            emit sInfo(tmpName, tmpValue.toInt(), DM_NORMAL);
            emit sOperateTheme(OPERATE_CLEAR, QVariant());
            mFloorLogoInfo.clear();
        }
        else if(!tmpName.compare("Pre-Operational", Qt::CaseInsensitive))
        {
            mCanopenMode = 3;
            emit sInfo(tmpName, tmpValue.toInt(), DM_NORMAL);
            emit sOperateTheme(OPERATE_CLEAR, QVariant());
            mFloorLogoInfo.clear();
        }
        else if(!tmpName.compare("Operational", Qt::CaseInsensitive))
        {
            mCanopenMode = 4;
            emit sInfo(tmpName, MSG_TEMPORARY, DM_NET);
            emit sInfo(tmpName, tmpValue.toInt(), DM_NORMAL);
        }
        else if(!tmpName.compare("Stopping", Qt::CaseInsensitive))
        {
            mCanopenMode = 5;
            emit sInfo(tmpName, tmpValue.toInt(), DM_NORMAL);
            emit sOperateTheme(OPERATE_CLEAR, QVariant());
            mFloorLogoInfo.clear();
        }
        else if(!tmpName.compare("Rebooting", Qt::CaseInsensitive))
        {
            mCanopenMode = 6;
            emit sInfo(tmpName, tmpValue.toInt(), DM_NORMAL);
            emit sOperateTheme(OPERATE_CLEAR, QVariant());
            mFloorLogoInfo.clear();
        }
        else if(!tmpName.compare("Dial", Qt::CaseInsensitive))
        {
            int tmpDial = tmpValue.toInt();
            QString tmpString = QString("[%1][%2][%3][%4][%5][%6][%7][%8]")
                                        .arg((tmpDial&0x00000001)?QString("ON"):QString("OFF"))
                                        .arg(((tmpDial>>1)&0x00000001)?QString("ON"):QString("OFF"))
                                        .arg(((tmpDial>>2)&0x00000001)?QString("ON"):QString("OFF"))
                                        .arg(((tmpDial>>3)&0x00000001)?QString("ON"):QString("OFF"))
                                        .arg(((tmpDial>>4)&0x00000001)?QString("ON"):QString("OFF"))
                                        .arg(((tmpDial>>5)&0x00000001)?QString("ON"):QString("OFF"))
                                        .arg(((tmpDial>>6)&0x00000001)?QString("ON"):QString("OFF"))
                                        .arg(((tmpDial>>7)&0x00000001)?QString("ON"):QString("OFF"));
            emit sInfo(tmpString, 10000, DM_NORMAL);
            return true;  //>@必须写，否则每次都会再执行一次下面的代码
        }
    }
    if(mCanopenMode == 4)
    {
        if(!mPlayer.isNull())
        {
            mPlayer.data()->Restore(AO_ADMUSIC, 2);
        }
    }
    else
    {
        if(!mPlayer.isNull())
        {
            mPlayer.data()->Abort(AO_ADMUSIC, 2);
        }
    }
    return true;
}

void PhysicalCOM::SetCanopenMode(int pMode)  //>@仅同步msgbox即可
{
    mCanopenMode = pMode;
    if(mCanopenMode == 0)
    {
        emit sInfo("Powerup", MSG_PERMANENT, DM_NORMAL);
        emit sOperateTheme(OPERATE_CLEAR, QVariant());
        mFloorLogoInfo.clear();
    }
    else if(mCanopenMode == 2)
    {
        emit sInfo("Initializing", MSG_PERMANENT, DM_NORMAL);
        emit sOperateTheme(OPERATE_CLEAR, QVariant());
        mFloorLogoInfo.clear();
    }
    else if(mCanopenMode == 3)
    {
        emit sInfo("Pre-Operational", MSG_PERMANENT, DM_NORMAL);
        emit sOperateTheme(OPERATE_CLEAR, QVariant());
        mFloorLogoInfo.clear();
    }
    else if(mCanopenMode == 4)
    {
        //>@但在第一次初始化时，不能使用MSG_HOLD
        emit sInfo("Operational", MSG_HOLD, DM_NORMAL);  //>@不修改之前的MSG显示时间
    }
    else if(mCanopenMode == 5)
    {
        emit sInfo("Stopping", MSG_PERMANENT, DM_NORMAL);
        emit sOperateTheme(OPERATE_CLEAR, QVariant());
        mFloorLogoInfo.clear();
    }
    else if(mCanopenMode == 6)
    {
        emit sInfo("Rebooting", MSG_PERMANENT, DM_NORMAL);
        emit sOperateTheme(OPERATE_CLEAR, QVariant());
        mFloorLogoInfo.clear();
    }
}

void PhysicalCOM::slot_Initialize()
{
    PhysicalElevator::slot_Initialize();
    QTimer::singleShot(1000, this, SLOT(slot_DataEnable()));
}

void PhysicalCOM::slot_DataEnable()
{
    mDataEnable = true;
}

quint32 PhysicalCOM::Monitor(quint32 pCount)
{
    if(isBusy)
        return 0;
    isBusy = false;
    mMonitorCount = pCount;
    if(!mInitialized)
        return 0;
    if(!mDataEnable)
        return 0;
    QByteArray tmpByteArray;
    PH_Type tmpType = P_INVALID;
    QIODevice *tmpDevice = 0;
    if(mAutoTestEnable || mSimulateMode || mTemporaryTest)
    {
        if(mAutoTestFreq.mNum < 100)
            return 0;

        //>@根据测试与正常模式切换保护现场
        if(mTemporaryTest)
        {
            if(!mEnvSaved)
            {
                mEnvSaved = true;
                mFlrLogoInfoSaved = mFloorLogoInfo;
                if(extendfunc & 0x02)
                {
                    mCheckFunSaved = checkfun;
                }
                else
                {
                    mCheckFunSaved = INVALIDRC;
                }
            }
        }

        if((mMonitorCount*100)%mAutoTestFreq.mNum == 0)  //<自动测试周期
        {
            if(!mTestcase)
            {
                if(!mTestcaseParse)
                {
                    if(mSimulateMode)
#ifdef SETTING
                        if(!InitTestcase(m_DevManager->mBufferPath+D_SYSTEMDIR+D_PHYSICALDIR+mDevName+QString("/") + "test.ini"))
#endif
                            return 0;
                    else
                        if(!InitTestcase(GetSystemPath() + "test.ini"))
                            return 0;
                }
                mTestcase = mTestcaseParse->mIniTestcaseList.value(mTestcaseParse->mIniTestcaseList.keys().first());
            }
            if(mTestcase->mFrameList.isEmpty())
                return 0;
            if(mTestFrameNum >= mTestcase->mFrameList.count())
                mTestFrameNum = 0;
            tmpByteArray = mTestcase->mFrameList.at(mTestFrameNum);
            mTestFrameNum++;
        }
        tmpType = P_TEST;
    }
    else
    {
        //>@根据测试与正常模式切换回复现场
        if(!mTemporaryTest)
        {
            if(mEnvSaved)
            {
                mEnvSaved = false;
                QByteArray tmpArray = GetSavedFrame();
                mFrameList.append(tmpArray);
                emit readyRead();
            }
        }

#ifndef IOS
        if(mSerialPort && mSerialPort->isOpen())
        {
            tmpByteArray = mSerialPort->readAll();
            m_CurTransDev = mSerialPort;
            tmpDevice = m_CurTransDev.data();
            tmpType = m_CurDevType = P_COM;

            if(m_Bootup == false)
            {
                m_Bootup = true;
                Bootup();
            }

            if(m_IBMonitorEnable && m_IBMonitorWait.isValid(pCount, m_DevManager->m_DevMonitorFreq))
            {
                m_IBMonitorEnable = false;
                QString tmpSrc = D_FSRCPATH + QString("EXE/CANOPENBK.bin");
                QString tmpDst = D_FSRCPATH + QString("EXE/CANOPEN.bin");
                CutFile(tmpSrc, tmpDst);
                emit sInfo("CANOPEN error, Please reboot!", MSG_PERMANENT, DM_NORMAL);
            }
        }
#endif
    }

    if(!tmpByteArray.isEmpty())
    {
        //IDE_TRACE_INT(tmpByteArray.count());
        ParseFrameBuffer(tmpType, tmpDevice, tmpByteArray);
    }

    if(mEnable)
    {
        if((mNoArrowMode == 1))
        {
            if(mMonitorCount >= mNoArrowTimeoutCount)
            {
                mNoArrowMode = 2;
                mNoArrowFreqCount = mNoArrowFreq.mNum/100;
            }
        }
        if((mNoArrowMode == 2) && (mMonitorCount % mNoArrowFreqCount == 0))
        {
            ProcNoArrow();
        }
    }

    if(!mTemporaryTest && (mDebugTimeout.mNum > 0) && mDebugMonitorEn && mMonitorCount == mDebugTimeoutCount)
    {
        slot_DebugTimeout();
    }
}

void PhysicalCOM::Bootup()
{
#ifdef DISPLAY
    SendConfFrame(QString("init#bootup"));
#endif
}

quint32 PhysicalCOM::ParseScandevFrame(FRAME_STRUCT *pFrame)
{
    if(pFrame->mFlag == Flag_Normal)  //>@主机发来的扫描帧
    {
#ifndef SERVER
        //>@获取扫描帧中的信息
        if(!pFrame->mData.isEmpty())
        {
            QStringList tmpParaList = QString(pFrame->mData).split(";");
            QString tmpPara;
            foreach(tmpPara, tmpParaList)
            {
                QStringList tmpSubParaList = tmpPara.split(":");
                if(tmpSubParaList.count() != 2)
                    continue;
                if(!tmpSubParaList.at(0).compare("APP", Qt::CaseInsensitive))
                {
                    m_IBMonitorEnable = false;
                    mInterBoardVersion = tmpSubParaList.at(1);
                }
                else if(!tmpSubParaList.at(0).compare("IAP", Qt::CaseInsensitive))
                {
                    m_IBMonitorEnable = true;
                }
                emit sInfo(QString("%1").arg(tmpPara), MSG_PERMANENT, DM_NORMAL);
            }
        }
        //>@根据扫描帧中的指令返回响应数据
#ifdef DISPLAY
        QString tmpMsg = QString("Code[%1]").arg(m_DevManager->mDevCode);
        emit sInfo(tmpMsg, MSG_PERMANENT, DM_NORMAL);
#endif
        pFrame->mData.clear();
#ifdef DISPLAY
        pFrame->mData.append(m_DevManager->mDevCode.toLatin1());
#elif IDE
        pFrame->mData.append(m_DevManager->mDevCode.toLatin1());
#endif
#endif
        return ReplyFrame(pFrame);
    }
    pFrame->deleteLater();
    return 1;
}

bool PhysicalCOM::WriteReg(quint16 pRegAddr, QByteArray pContent)
{
    switch(pRegAddr)
    {
        case 80:  //>@呼梯
        {
#ifdef DISPLAY
#ifdef SUPPORT_WIDGET
            if(pContent.count() >= 3)
            {
                FLOOR_INFO tmpFloor((quint8)pContent.at(0), (quint8)pContent.at(1), (quint8)pContent.at(2));
                if(!mThemeManager.isNull() && mThemeManager.data()->m_UiContainer)
                {
                    if(mThemeManager.data()->m_UiContainer->m_ComDiagramGroup.contains(COM_FLRBTN))
                    {
                        WidgetFlrBtn *tmpBtn = VAR_CAST<WidgetFlrBtn*>(mThemeManager.data()->m_UiContainer->m_ComDiagramGroup.value(COM_FLRBTN));
                        if(tmpBtn)
                        {
                            tmpBtn->slot_ExecOperate(OPERATE_KEYPRESS, tmpFloor.toIndex());
                        }
                    }
                    else if(mThemeManager.data()->m_UiContainer->m_ComDiagramGroup.contains(COM_KEYBOARDBTN))
                    {
                        WidgetKeyboardBtn *tmpBtn = VAR_CAST<WidgetKeyboardBtn*>(mThemeManager.data()->m_UiContainer->m_ComDiagramGroup.value(COM_KEYBOARDBTN));
                        if(tmpBtn)
                        {
                            tmpBtn->m_KeyBuf = QString::number(tmpFloor.toIndex());
                            tmpBtn->slot_ExecOperate(OPERATE_KEYPRESS, 13);
                        }
                    }
                }
            }
#endif
#endif
            break;
        }
        default:
            break;
    }
}

QByteArray PhysicalCOM::ReadReg(quint16 pRegAddr, quint32 pTimeout)
{
    switch(pRegAddr)
    {
        case 80:  //>@获取协议
        {
            FLOOR_INFO tmpFloor(mFloorLogoInfo.mFloor);
            int index = tmpFloor.toIndex();
            QByteArray tmpArray;
            tmpArray.append((quint8)(index/100+48));
            tmpArray.append((quint8)(index%100/10+48));
            tmpArray.append((quint8)(index%10+48));
            return tmpArray;
        }
        default:
            break;
    }
    return QByteArray();
}

quint32 PhysicalCOM::ParseFloorFrame(FRAME_STRUCT *pFrame)
{
    if(pFrame->mFlag == Flag_Normal)   //>@解析主机发来的数据
    {
        //IDE_TRACE_INT(pFrame->mType);
        if(pFrame->mType == Type_FloorStatus)
        {
            IDE_DEBUG(QString("baiwei:%1 shiwei:%2 gewei:%3 checkfun:%4 raidofunc:%5 audionum:%6 extendfunc:%7")
                      .arg((quint8)pFrame->mData.at(0)).arg((quint8)pFrame->mData.at(1)).arg((quint8)pFrame->mData.at(2))
                      .arg((quint8)pFrame->mData.at(3)).arg((quint8)pFrame->mData.at(4)).arg((quint8)pFrame->mData.at(5)).arg((quint8)pFrame->mData.at(6)));

            mHasArrow = false;
            bool diff_flag = false;  //>@standby 处理：查看是否有楼层或者箭头改变

            extendfunc = (quint8)pFrame->mData.at(6);
            quint8 tmpArrowFresh = extendfunc & D_ArrowFresh;
            quint8 tmpFloorFresh = extendfunc & D_FloorFresh;
            quint8 tmpFuncFresh = extendfunc & D_FunctionFresh;
            quint8 tmpAudioFresh = extendfunc & D_AudioFresh;

            if(tmpFloorFresh && mFloorLogoInfo.isLogo())  //>@如果楼层显示，并且上次为logo，则需要唤醒。否则先发123楼层，再发40logo，进入standby，再发123楼层会无法唤醒。
                diff_flag = true;

            //>@Floor与Logo相比，floor的优先级比较高，因此，需要先处理logo，这样floor有变化时才可以覆盖logo操作
            if(tmpFuncFresh)
            {
                raidofunc = (quint8)pFrame->mData.at(4);
                //>@Floor logo检测
                mFloorLogoInfo.SetLogo(raidofunc);
                diff_flag = true;

#ifdef SODIMAS
                //>@遇到40帧，则开启WIFI
                if(!mLANC.isNull() && raidofunc == mDebugFrame.mNum) //>@Debug开关，40帧开启，半小时后依旧没有连接APP，则自动关闭Debug
                {
                    OpenWIFI();
                    if(!mPlayer.isNull())
                    {
                        mPlayer.data()->Abort(AO_ADMUSIC, 1);
                    }
                }
#endif
            }
            if(tmpFloorFresh)
            {
                quint8 tmpBaiwei, tmpShiwei, tmpGewei;
                tmpBaiwei = (quint8)pFrame->mData.at(0);
                tmpShiwei = (quint8)pFrame->mData.at(1);
                tmpGewei = (quint8)pFrame->mData.at(2);
                if(tmpBaiwei != baiwei)
                {
                    baiwei = tmpBaiwei;
                    diff_flag = true;
                }
                if(tmpShiwei != shiwei)
                {
                    shiwei = tmpShiwei;
                    diff_flag = true;
                }
                if(tmpGewei != gewei)
                {
                    gewei = tmpGewei;
                    diff_flag = true;
                }
                //>@Floor logo检测
                FLOOR_INFO tmpFloor(baiwei, shiwei, gewei);
                mFloorLogoInfo.SetFloor(tmpFloor.toInt());
            }
            if(tmpArrowFresh)
            {
                quint8 tmpArrow = (quint8)pFrame->mData.at(3);
                if(tmpArrow != checkfun)
                {
                    checkfun = tmpArrow;
                    diff_flag = true;
                }
            }
            mHasArrow = (checkfun & 0x03) ? true : false;

            //>@判断是否要关闭当前正在播放的警报beep声
            if((tmpFloorFresh || tmpFuncFresh) && mFloorLogoInfo.mFloorOrLogo > 0)
            {
#ifdef DISPLAY
                PhysicalPlayer *tmpPlayer = mPlayer.data();
                MEDIA_PLAYER *tmpMediaPlayer = 0;
                if(tmpPlayer && (tmpMediaPlayer = tmpPlayer->GetMediaPlayer(AO_LIFTFLR)))
                {
                    if(!mMedia.isNull())
                    {
                        QString tmpMediaFile = mMedia.data()->GetPixmapPath(43);
                        tmpMediaPlayer->StopOne(tmpMediaFile);
                    }
                }
#endif
            }

            //>@只有当帧有变化时，才从standby中唤醒
            if(!m_LCD.isNull())
            {
                PhysicalLCD *tmpLcd = m_LCD.data();
                if(tmpLcd)
                {
                    tmpLcd->SetSrnSaveEn(!mHasArrow);
                    //IDE_TRACE_INT(!mHasArrow);
                    tmpLcd->SetStandbyEn(!mHasArrow);  //>@-> 只要有箭头，就不会进入standby模式
                    if(mHasArrow || diff_flag)
                    {
#ifdef DISPLAY  //>@退出standby模式时需要执行将设备恢复操作
                        emit sSetDevices(true, DM_STANDBY);
#endif
                        if(diff_flag) //>@包括了箭头从有变为没有时需要开启standby模式的考虑
                        {
                            emit sFeedLCD();  //>@发送喂狗命令，防止进入standby模式
                        }
                    }
                }
            }

            //>@改变界面显示
            //>@箭头
            if(tmpArrowFresh)
                emit sArrowEvent((quint32)checkfun);
            //>@楼层/单选功能
            if(m_ReuseType == RU_FLRFUNC)
            {
                if(mFloorLogoInfo.isFloor())
                {
                    emit sRadioFunEvent(HIDERC);
                    emit sFloorEvent(mFloorLogoInfo.mFloor);
                }
                else if(mFloorLogoInfo.isLogo())
                {
                    emit sFloorEvent(HIDERC);
                    emit sRadioFunEvent(mFloorLogoInfo.mLogo);
                }
            }
            else
            {
                emit sFloorEvent(mFloorLogoInfo.mFloor);
                emit sRadioFunEvent(mFloorLogoInfo.mLogo);
            }
            //>@先显示图片，再播放声音
            if(tmpAudioFresh)
            {
                audionum = (quint8)pFrame->mData.at(5);
                if(audionum == 43)
                {
                    PhysicalPlayer *tmpPlayer = mPlayer.data();
                    MEDIA_PLAYER *tmpMediaPlayer = 0;
                    if(tmpPlayer && (tmpMediaPlayer = tmpPlayer->GetMediaPlayer(AO_LIFTFLR)))
                    {
                        tmpMediaPlayer->Clear();
                        tmpMediaPlayer->Abort();
                    }
                }
                emit sMediaEvent(audionum);
            }

            if(mEnable)
            {
                //>@Comstomer模式切换
                if(mNoArrowMode == 0)
                {
                    if(mNoArrowTimeout.mNum >= 100 && mNoArrowFreq.mNum >= 100)
                    {
                        if(!mHasArrow)
                        {
                            if(mNoArrowType.compare("None", Qt::CaseInsensitive))  //>@楼层与检修切换：每秒切换、固定为某种图片不变
                            {
                                mNoArrowMode = 1;
                                mNoArrowTimeoutCount = (mMonitorCount + mNoArrowTimeout.mNum/100) % D_MAXTIMEOUT;
                            }
                            else
                            {
                                mNoArrowMode = 0;
                            }
                        }
                    }
                }
                else
                {
                    if(mHasArrow)
                    {
                        mNoArrowMode = 0;
                        ProcNoArrow(true);
                    }
                }
            }
        }
        else if(pFrame->mType == Type_FloorStatus2)
        {
            FLOOR_INFO tmpFloor(pFrame->mData.at(0), pFrame->mData.at(1), pFrame->mData.at(2));
            ARROWSTATE tmpArrow = (ARROWSTATE)((pFrame->mData.at(3)) & D_ArrowArea);
            quint8 tmpArrive = (pFrame->mData.at(3)) & D_SignalArrive;
            quint8 tmpVoice = (pFrame->mData.at(3)) & D_SignalVoice;
            quint8 tmpLowpower = (pFrame->mData.at(3)) & D_SignalLowPower;
            quint32 tmpResvFunc = ((quint32)pFrame->mData.at(6)<<14) + ((quint32)pFrame->mData.at(5)<<7) + (quint32)pFrame->mData.at(4);
            emit sFloorEvent(tmpFloor.toInt());
            emit sArrowEvent((quint32)tmpArrow);
            if(tmpResvFunc)
            {
                for(int i=0;i<21;i++)
                {
                    if((tmpResvFunc>>i) & 0x00000001)
                    {
                        emit sRadioFunEvent(i+1);
                        break;
                    }
                }
            }
            else
            {
                emit sRadioFunEvent(0);
            }
            if(tmpArrive)
            {
                if(tmpArrow & D_ArrowUpDown == D_ArrowUpDown)
                    emit sMediaEvent(100+D_ArrowUpDown);
                else if(tmpArrow & D_ArrowUp)
                    emit sMediaEvent(100+D_ArrowUp);
                else if(tmpArrow & D_ArrowDown)
                    emit sMediaEvent(100+D_ArrowDown);
            }
            if(tmpVoice)
            {
                emit sMediaEvent(tmpFloor.toIndex());
            }
        }
    }
    pFrame->deleteLater();
    return 1;
}

//>@pForce表示强制设置为Floor或者LOGO
bool PhysicalCOM::ProcNoArrow(bool pForce)
{
    if(!mNoArrowType.compare("Logo", Qt::CaseInsensitive))
    {
        //>@新版规格书中更改：只有在当前显示Floor时才会进入Logo模式
        if(!mFloorLogoInfo.isFloor())
            return false;

        if(m_ReuseType == RU_FLRFUNC)
        {
            if(pForce)
            {
                emit sRadioFunEvent(HIDERC);
                emit sFloorEvent(mFloorLogoInfo.mFloor);
            }
            else
            {
                emit sFloorEvent(HIDERC);
                emit sRadioFunEvent(51);
            }
        }
        else
        {
            emit sFloorEvent(mFloorLogoInfo.mFloor);
            emit sRadioFunEvent(51);
        }
    }
    else if(!mNoArrowType.compare("Flr_Logo", Qt::CaseInsensitive))
    {
        static int sFlrLogoFlag = 0;  //>1表示检修图片

        //>@新版规格书中更改：只有在当前显示Floor时才会进入Floor与Logo切换模式
        if(!mFloorLogoInfo.isFloor())
            return false;

        if(m_ReuseType == RU_FLRFUNC)
        {
            if(!pForce && sFlrLogoFlag)
            {
                sFlrLogoFlag = 0;
                emit sFloorEvent(HIDERC);
                emit sRadioFunEvent(51);
            }
            else
            {
                sFlrLogoFlag = 1;
                emit sRadioFunEvent(HIDERC);
                emit sFloorEvent(mFloorLogoInfo.mFloor);
            }
        }
        else
        {
            emit sFloorEvent(mFloorLogoInfo.mFloor);
            emit sRadioFunEvent(51);
        }
    }
    else if(!mNoArrowType.compare("Mte_Logo", Qt::CaseInsensitive))
    {
        static int sFlrLogoFlag = 0;  //>1表示检修图片

        //>@新版规格书中更改：只有在当前显示检修时才会进入Mte与Logo切换模式
        if(!mFloorLogoInfo.isLogo() || (mFloorLogoInfo.mLogo != 40))
            return false;

        if(m_ReuseType == RU_FLRFUNC)
        {
            if(!pForce && sFlrLogoFlag)
            {
                sFlrLogoFlag = 0;
                emit sRadioFunEvent(51);
            }
            else
            {
                sFlrLogoFlag = 1;
                emit sRadioFunEvent(40);
            }
        }
        else
        {
            if(sFlrLogoFlag)
            {
                sFlrLogoFlag = 0;
                emit sRadioFunEvent(51);
            }
            else
            {
                sFlrLogoFlag = 1;
                emit sRadioFunEvent(40);
            }
        }
    }
    else
    {
        return false;  //>@如果不添加此行，每次显示完功能后都自动切换到Floor去。

        //>@优先显示楼层
//        if(isVisibleAscii(baiwei) || isVisibleAscii(shiwei) || isVisibleAscii(gewei))
//            tmpExtendfunc |= 0x01;
//        else
//            tmpExtendfunc |= 0x04;
    }
    return true;
}

QHBoxLayout* PhysicalCOM::InitSubPara(QString pName, QDomElement pElement)
{
    QHBoxLayout* tmpHBoxLayout = 0;
    if(!pName.compare("Port"))
    {
        QString tmpValue = pElement.text();
#ifndef IOS
        mPortName = GetSerialPort(tmpValue);
        mSerialPort->setPortName(mPortName);
#endif
        tmpHBoxLayout = CreateEnum(pName, pElement.attribute("List").split(';'), tmpValue);
    }
    else if(!pName.compare("Buad"))
    {
        QString tmpValue = pElement.text();
        mPortBaud = (BaudRateType)(tmpValue.toInt());
#ifndef IOS
        IDE_TRACE_INT(mPortBaud);
        mSerialPort->Settings.BaudRate = mPortBaud;
#endif
        tmpHBoxLayout = CreateEnum(pName, pElement.attribute("List").split(';'), tmpValue);
    }
    else if(!pName.compare("Parity"))
    {
        QString tmpValue = pElement.text();
        if(!tmpValue.compare("ODD", Qt::CaseInsensitive))
            mPortParity = PAR_ODD;
        else if(!tmpValue.compare("EVEN", Qt::CaseInsensitive))
            mPortParity = PAR_EVEN;
        else
            mPortParity = PAR_NONE;
#ifndef IOS
        IDE_TRACE_INT(mPortParity);
        mSerialPort->Settings.Parity = mPortParity;
#endif
        tmpHBoxLayout = CreateEnum(pName, pElement.attribute("List").split(';'), tmpValue);
    }
    else if(!pName.compare("NoArrow"))
    {
        mNoArrowType = pElement.text();
        tmpHBoxLayout = CreateEnum(pName, pElement.attribute("List").split(';'), mNoArrowType);
    }
    else if(!pName.compare("ArrowTimeout"))
    {
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()==2)
        {
            mNoArrowTimeout.setnum(pElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
        }
        if(mNoArrowTimeout.mNum)
        {
            mNoArrowTimeoutCount = (mMonitorCount + mNoArrowTimeout.mNum/100) % D_MAXTIMEOUT;
        }
        tmpHBoxLayout = CreateNum(pName, mNoArrowTimeout.mNum, mNoArrowTimeout.mMinNum, mNoArrowTimeout.mMaxNum);
    }
    else if(!pName.compare("SwitchFreq"))
    {
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()==2)
        {
            mNoArrowFreq.setnum(pElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
        }
        tmpHBoxLayout = CreateNum(pName, mNoArrowFreq.mNum, mNoArrowFreq.mMinNum, mNoArrowFreq.mMaxNum);
    }
    else if(!pName.compare("DebugFrame"))
    {
        mDebugFrame.mNum = pElement.text().toInt();
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()!=2)
        {
            mDebugFrame.mMinNum = 0;
            mDebugFrame.mMaxNum = 0;
            mDebugFrame.mNum = mDebugFrame.mMinNum;
        }
        else
        {
            mDebugFrame.mMinNum = tmpValueList.at(0).toInt();
            mDebugFrame.mMaxNum = tmpValueList.at(1).toInt();
            if(mDebugFrame.mNum < mDebugFrame.mMinNum || mDebugFrame.mNum > mDebugFrame.mMaxNum)  //>@判断值是否有效
                mDebugFrame.mNum = mDebugFrame.mMinNum;
        }
        tmpHBoxLayout = CreateNum(pName, mDebugFrame.mNum, mDebugFrame.mMinNum, mDebugFrame.mMaxNum);
    }
    else if(!pName.compare("DebugTimeout"))
    {
        mDebugTimeout.mNum = pElement.text().toInt();
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()!=2)
        {
            mDebugTimeout.mMinNum = 0;
            mDebugTimeout.mMaxNum = 0;
            mDebugTimeout.mNum = mDebugTimeout.mMinNum;
        }
        else
        {
            mDebugTimeout.mMinNum = tmpValueList.at(0).toInt();
            mDebugTimeout.mMaxNum = tmpValueList.at(1).toInt();
            if(mDebugTimeout.mNum < mDebugTimeout.mMinNum || mDebugTimeout.mNum > mDebugTimeout.mMaxNum)  //>@判断值是否有效
                mDebugTimeout.mNum = mDebugTimeout.mMinNum;
        }
        tmpHBoxLayout = CreateNum(pName, mDebugTimeout.mNum, mDebugTimeout.mMinNum, mDebugTimeout.mMaxNum);
    }
    else if(!pName.compare("AutoTest"))
    {
        QString tmpValue = pElement.text();
        mAutoTestEnable = (tmpValue.compare("true", Qt::CaseInsensitive))?(false):(true);
        tmpHBoxLayout = CreateBool(pName, tmpValue);
    }
    else if(!pName.compare("TestFreq"))
    {
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()==2)
        {
            mAutoTestFreq.setnum(pElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
        }
        tmpHBoxLayout = CreateNum(pName, mAutoTestFreq.mNum, mAutoTestFreq.mMinNum, mAutoTestFreq.mMaxNum);
    }
    return tmpHBoxLayout;
}

void PhysicalCOM::UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement)
{
    if(!pName.compare("Port"))
    {
        ComboBox *tmpComponent = VAR_CAST<ComboBox *>(pObject);
        if(tmpComponent)
        {
            QString tmpPortNum;
            if(!ModifyEnum(tmpComponent, pElement, tmpPortNum, tmpComponent->currentText()))
            {
            }
            else
            {
#ifndef IOS
                mPortName = GetSerialPort(tmpPortNum);
#endif
            }
        }
    }
    else if(!pName.compare("Buad"))
    {
        ComboBox *tmpComponent = VAR_CAST<ComboBox *>(pObject);
        if(tmpComponent)
        {
            QString tmpBuad;
            if(!ModifyEnum(tmpComponent, pElement, tmpBuad, tmpComponent->currentText()))
            {
            }
            else
            {
                mPortBaud = (BaudRateType)(tmpBuad.toInt());
            }
        }
    }
    else if(!pName.compare("Parity"))
    {
        ComboBox *tmpComponent = VAR_CAST<ComboBox *>(pObject);
        if(tmpComponent)
        {
            QString tmpParity;
            if(!ModifyEnum(tmpComponent, pElement, tmpParity, tmpComponent->currentText()))
            {
            }
            else
            {
                if(!tmpParity.compare("ODD", Qt::CaseInsensitive))
                    mPortParity = PAR_ODD;
                else if(!tmpParity.compare("EVEN", Qt::CaseInsensitive))
                    mPortParity = PAR_EVEN;
                else
                    mPortParity = PAR_NONE;
            }
        }
    }
    else if(!pName.compare("NoArrow"))
    {
        ComboBox *tmpComponent = VAR_CAST<ComboBox *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyEnum(tmpComponent, pElement, mNoArrowType, tmpComponent->currentText()))
            {
            }
        }
    }
    else if(!pName.compare("ArrowTimeout"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, mNoArrowTimeout.mNum, tmpComponent->value(), mNoArrowTimeout.mMinNum, mNoArrowTimeout.mMaxNum))
            {
            }
        }
    }
    else if(!pName.compare("SwitchFreq"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, mNoArrowFreq.mNum, tmpComponent->value(), mNoArrowFreq.mMinNum, mNoArrowFreq.mMaxNum))
            {
            }
        }
    }

    else if(!pName.compare("DebugFrame"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, mDebugFrame.mNum, tmpComponent->value(), mDebugFrame.mMinNum, mDebugFrame.mMaxNum))
            {
            }
        }
    }
    else if(!pName.compare("DebugTimeout"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, mDebugTimeout.mNum, tmpComponent->value(), mDebugTimeout.mMinNum, mDebugTimeout.mMaxNum))
            {
            }
        }
    }
    else if(!pName.compare("AutoTest"))
    {
        Switch *tmpComponent = qobject_cast<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, pElement, mAutoTestEnable, tmpComponent->isChecked()))
            {
            }
            else
            {
                SetAutoTest(mAutoTestEnable, true);
            }
        }
    }
    else if(!pName.compare("TestFreq"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, mAutoTestFreq.mNum, tmpComponent->value(), mAutoTestFreq.mMinNum, mAutoTestFreq.mMaxNum))
            {
            }
        }
    }
}

PhysicalSAF::PhysicalSAF(QObject *parent) :
    PhysicalElevator(parent)
{
    mFrame51Mode = 1; //>@默认认为开启后没有信号。

    indicatecheckfun = 0x00;
    realcheckfun = 0x00;

    mTestcase = 0;
    mFile = 0;
    mAutoGenFrame = 0;
    mTestFrameNum = 0;

    mAutoTestFreq.mNum = 1000;
    mAutoTestFreq.mMinNum = 500;
    mAutoTestFreq.mMaxNum = 10000;

    mOpenMsgEnable = false;
    mAutoTestEnable = false;
    audiolist.clear();
    floortalkcount = 0;

    mDebugTimeoutCount = D_MAXTIMEOUT+1;

    mMonitorCount = 0;

    old_saf = old_flm = old_fld = 0;

    Connected();
}

PhysicalSAF::~PhysicalSAF()
{
    close();
    if(mAutoGenFrame)
        mAutoGenFrame->deleteLater();
}

QHBoxLayout* PhysicalSAF::InitSubPara(QString pName, QDomElement pElement)
{
    QHBoxLayout* tmpHBoxLayout = 0;
    if(!pName.compare("OpenMsg"))
    {
        QString tmpValue = pElement.text();
        mOpenMsgEnable = (tmpValue.compare("true", Qt::CaseInsensitive))?(false):(true);
        tmpHBoxLayout = CreateBool(pName, tmpValue);
    }
    else if(!pName.compare("DebugFrame"))
    {
        mDebugFrame.mNum = pElement.text().toInt();
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()!=2)
        {
            mDebugFrame.mMinNum = 0;
            mDebugFrame.mMaxNum = 0;
            mDebugFrame.mNum = mDebugFrame.mMinNum;
        }
        else
        {
            mDebugFrame.mMinNum = tmpValueList.at(0).toInt();
            mDebugFrame.mMaxNum = tmpValueList.at(1).toInt();
            if(mDebugFrame.mNum < mDebugFrame.mMinNum || mDebugFrame.mNum > mDebugFrame.mMaxNum)  //>@判断值是否有效
                mDebugFrame.mNum = mDebugFrame.mMinNum;
        }
        tmpHBoxLayout = CreateNum(pName, mDebugFrame.mNum, mDebugFrame.mMinNum, mDebugFrame.mMaxNum);
    }
    else if(!pName.compare("DebugTimeout"))
    {
        mDebugTimeout.mNum = pElement.text().toInt();
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()!=2)
        {
            mDebugTimeout.mMinNum = 0;
            mDebugTimeout.mMaxNum = 0;
            mDebugTimeout.mNum = mDebugTimeout.mMinNum;
        }
        else
        {
            mDebugTimeout.mMinNum = tmpValueList.at(0).toInt();
            mDebugTimeout.mMaxNum = tmpValueList.at(1).toInt();
            if(mDebugTimeout.mNum < mDebugTimeout.mMinNum || mDebugTimeout.mNum > mDebugTimeout.mMaxNum)  //>@判断值是否有效
                mDebugTimeout.mNum = mDebugTimeout.mMinNum;
        }
        tmpHBoxLayout = CreateNum(pName, mDebugTimeout.mNum, mDebugTimeout.mMinNum, mDebugTimeout.mMaxNum);
    }
    else if(!pName.compare("IdleState"))
    {
        mFrame51Type = pElement.text();
        tmpHBoxLayout = CreateEnum(pName, pElement.attribute("List").split(';'), mFrame51Type);
    }
    else if(!pName.compare("IdleTimeout"))
    {
        mFrame51Timeout.mNum = pElement.text().toInt();
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()!=2)
        {
            mFrame51Timeout.mMinNum = 0;
            mFrame51Timeout.mMaxNum = 0;
            mFrame51Timeout.mNum = mFrame51Timeout.mMinNum;
        }
        else
        {
            mFrame51Timeout.mMinNum = tmpValueList.at(0).toInt();
            mFrame51Timeout.mMaxNum = tmpValueList.at(1).toInt();
            if(mFrame51Timeout.mNum < mFrame51Timeout.mMinNum || mFrame51Timeout.mNum > mFrame51Timeout.mMaxNum)  //>@判断值是否有效
                mFrame51Timeout.mNum = mFrame51Timeout.mMinNum;
        }
        tmpHBoxLayout = CreateNum(pName, mFrame51Timeout.mNum, mFrame51Timeout.mMinNum, mFrame51Timeout.mMaxNum);
    }
    else if(!pName.compare("SwitchFreq"))
    {
        mFrame51Freq.mNum = pElement.text().toInt();
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()!=2)
        {
            mFrame51Freq.mMinNum = 0;
            mFrame51Freq.mMaxNum = 0;
            mFrame51Freq.mNum = mFrame51Freq.mMinNum;
        }
        else
        {
            mFrame51Freq.mMinNum = tmpValueList.at(0).toInt();
            mFrame51Freq.mMaxNum = tmpValueList.at(1).toInt();
            if(mFrame51Freq.mNum < mFrame51Freq.mMinNum || mFrame51Freq.mNum > mFrame51Freq.mMaxNum)  //>@判断值是否有效
                mFrame51Freq.mNum = mFrame51Freq.mMinNum;
        }
        tmpHBoxLayout = CreateNum(pName, mFrame51Freq.mNum, mFrame51Freq.mMinNum, mFrame51Freq.mMaxNum);
    }
    else if(!pName.compare("AutoTest"))
    {
        QString tmpValue = pElement.text();
        mAutoTestEnable = (tmpValue.compare("true", Qt::CaseInsensitive))?(false):(true);
        tmpHBoxLayout = CreateBool(pName, tmpValue);
    }
    else if(!pName.compare("TestFreq"))
    {
        mAutoTestFreq.mNum = pElement.text().toInt();
        QStringList tmpValueList = pElement.attribute("List").split(';');
        if(tmpValueList.count()!=2)
        {
            mAutoTestFreq.mMinNum = 0;
            mAutoTestFreq.mMaxNum = 0;
            mAutoTestFreq.mNum = mAutoTestFreq.mMinNum;
        }
        else
        {
            mAutoTestFreq.mMinNum = tmpValueList.at(0).toInt();
            mAutoTestFreq.mMaxNum = tmpValueList.at(1).toInt();
            if(mAutoTestFreq.mNum < mAutoTestFreq.mMinNum || mAutoTestFreq.mNum > mAutoTestFreq.mMaxNum)  //>@判断值是否有效
                mAutoTestFreq.mNum = mAutoTestFreq.mMinNum;
        }
        tmpHBoxLayout = CreateNum(pName, mAutoTestFreq.mNum, mAutoTestFreq.mMinNum, mAutoTestFreq.mMaxNum);
    }
    return tmpHBoxLayout;
}

void PhysicalSAF::UpdateSubPara(QString pName, QObject* pObject, QDomElement pElement)
{
    if(!pName.compare("OpenMsg"))
    {
        Switch *tmpComponent = qobject_cast<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, pElement, mOpenMsgEnable, tmpComponent->isChecked()))
            {
            }
        }
    }
    else if(!pName.compare("DebugFrame"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, mDebugFrame.mNum, tmpComponent->value(), mDebugFrame.mMinNum, mDebugFrame.mMaxNum))
            {
            }
        }
    }
    else if(!pName.compare("DebugTimeout"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, mDebugTimeout.mNum, tmpComponent->value(), mDebugTimeout.mMinNum, mDebugTimeout.mMaxNum))
            {
            }
        }
    }
    else if(!pName.compare("IdleState"))
    {
        ComboBox *tmpComponent = VAR_CAST<ComboBox *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyEnum(tmpComponent, pElement, mFrame51Type, tmpComponent->currentText()))
            {
            }
        }
    }
    else if(!pName.compare("IdleTimeout"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, mFrame51Timeout.mNum, tmpComponent->value(), mFrame51Timeout.mMinNum, mFrame51Timeout.mMaxNum))
            {
            }
        }
    }
    else if(!pName.compare("SwitchFreq"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, mFrame51Freq.mNum, tmpComponent->value(), mFrame51Freq.mMinNum, mFrame51Freq.mMaxNum))
            {
            }
        }
    }
    else if(!pName.compare("AutoTest"))
    {
        Switch *tmpComponent = qobject_cast<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, pElement, mAutoTestEnable, tmpComponent->isChecked()))
            {
            }
        }
    }
    else if(!pName.compare("TestFreq", Qt::CaseInsensitive))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, pElement, mAutoTestFreq.mNum, tmpComponent->value(), mAutoTestFreq.mMinNum, mAutoTestFreq.mMaxNum))
            {
            }
        }
    }
}

bool PhysicalSAF::open(OpenMode mode)
{
    if(!mFile)
        mFile = new QFile;
    mFile->setFileName(QString("/dev/hmi"));
    if (mFile->open(QIODevice::ReadWrite|QIODevice::Unbuffered))
    {
        QIODevice::open(mode);
        return true;
    }
    return false;
}

void PhysicalSAF::close()
{
    if(mFile)
        mFile->close();
    QIODevice::close();
}

void PhysicalSAF::slot_Initialize()
{
    PhysicalElevator::slot_Initialize();

    PhysicalPlayer *tmpPlayer = mPlayer.data();
    if(tmpPlayer)
    {
        IDE_TRACE();
        tmpPlayer->Restore();
    }
}

quint32 PhysicalSAF::Monitor(quint32 pCount)
{
    mMonitorCount = pCount;
    if(!mInitialized)
        return 0;
    if(mEnable)
    {
        if((mFrame51Mode == 0) && (mMonitorCount % 10 == 0)) //>@2s查看一次
        {
            if(mFrame51Timeout.mNum >= 100 && mFrame51Freq.mNum >= 100)
            {
                mFrame51Mode = 1;
                mFrame51TimeoutCount = (mMonitorCount + mFrame51Timeout.mNum/100) % D_MAXTIMEOUT;
            }
        }
        if((mFrame51Mode == 1))
        {
            if(mMonitorCount >= mFrame51TimeoutCount)
            {
                mFrame51Mode = 2;
                mFrame51FreqCount = mFrame51Freq.mNum/100;
            }
        }
        if((mFrame51Mode == 2) && (mMonitorCount % mFrame51FreqCount == 0))
        {
            if(!Frame51())
                mFrame51Mode = 0;
        }
    }

    QByteArray tmpByteArray;
    if(mAutoTestEnable || mSimulateMode || mTemporaryTest)
    {
        if(mAutoTestFreq.mNum < 100)
            return 0;

        //>@根据测试与正常模式切换保护现场
        if(mTemporaryTest)
        {
            if(!mEnvSaved)
            {
                mEnvSaved = true;
                if(extendfunc & 0x01)
                {
                    mFlrLogoInfoSaved.mFloorOrLogo = 1;
                    mFlrLogoInfoSaved.mFloor = ((quint32)baiwei << 16) + ((quint32)shiwei << 8) + gewei;
                }
                else if(extendfunc & 0x04)
                {
                    mFlrLogoInfoSaved.mFloorOrLogo = 0;
                    mFlrLogoInfoSaved.mLogo = raidofunc;
                }
                if(extendfunc & 0x02)
                {
                    mCheckFunSaved = checkfun;
                }
                else
                {
                    mCheckFunSaved = INVALIDRC;
                }
            }
        }

        if((mMonitorCount*100)%mAutoTestFreq.mNum == 0)  //<自动测试周期
        {
            if(!mTestcase)
            {
                if(!mTestcaseParse)
                {
                    if(mSimulateMode)
                    {
#ifdef SETTING
                        if(!InitTestcase(m_DevManager->mBufferPath+D_SYSTEMDIR+D_PHYSICALDIR+mDevName+QString("/") + "test.ini"))
#endif
                            return 0;
                    }
                    else
                    {
                        if(!InitTestcase(GetSystemPath() + "test.ini"))
                            return 0;
                    }
                }
                mTestcase = mTestcaseParse->mIniTestcaseList.value(mTestcaseParse->mIniTestcaseList.keys().first());
            }
            if(mTestcase->mFrameList.isEmpty())
                return 0;
            if(mTestFrameNum >= mTestcase->mFrameList.count())
                mTestFrameNum = 0;
            tmpByteArray = mTestcase->mFrameList.at(mTestFrameNum);
            mTestFrameNum++;
        }
    }
    else
    {
        //>@根据测试与正常模式切换回复现场
        if(!mTemporaryTest)
        {
            if(mEnvSaved)
            {
                mEnvSaved = false;
                mReadByteArray = GetSavedFrame();
                emit readyRead();
            }
        }

        if(!(mFile && mFile->isOpen()))
        {
            if(!open(ReadWrite))
            {
                return 0;
            }
        }
        tmpByteArray = mFile->read(6);
    }

    ParseFrameBuffer(P_SAF, 0, tmpByteArray);  //>@表示不需要返回

    if(!mTemporaryTest && (mDebugTimeout.mNum > 0) && mDebugMonitorEn && mMonitorCount == mDebugTimeoutCount)
    //if(!mTemporaryTest && mDebugMonitorEn && mMonitorCount == mDebugTimeoutCount)
    {
        slot_DebugTimeout();
    }
    return 1;
}

bool PhysicalSAF::SetValue(QString pValue)
{
    if(pValue.isEmpty())
        return true;
    QStringList tmpList = pValue.split(';');
    for(int i=0;i<tmpList.count();i++)
    {
        QStringList tmpCmdList = tmpList.at(i).split(':');
        if(tmpCmdList.count() < 2)
            continue;
        QString tmpName = tmpCmdList.at(0);
        QString tmpValue = tmpCmdList.at(1);
        if(!tmpName.compare("autotest", Qt::CaseInsensitive))
        {
            if(!tmpValue.compare("on", Qt::CaseInsensitive))
            {
                SetTemporaryTest(true, true);
            }
            else
            {
                SetTemporaryTest(false, true);
            }
        }
    }
    return true;
}

void PhysicalSAF::Clear()
{
    PhysicalElevator::Clear();
}

//>@楼层不变的情况下，只响应第一次talk
//>@只要设置了楼层，都会显示楼层
void PhysicalSAF::SetFloor(quint8 pSAF, quint8 pTalk)
{
    volatile bool change = false;
    volatile quint8 tmpValue = 0;
#if 0
    tmpValue == pSAF / 100 + 0x30;
    if(tmpValue == 0x30)
        tmpValue = 0x20;
#else
    tmpValue = 0x20;
#endif
    if(baiwei != tmpValue)
    {
        change = true;
        baiwei = tmpValue;
    }
    tmpValue = pSAF % 100 / 10 + 0x30;
    if((tmpValue == 0x30) && (baiwei == 0x20))
        tmpValue = 0x20;
    if(shiwei != tmpValue)
    {
        change = true;
        shiwei = tmpValue;
    }
    tmpValue = pSAF % 10 + 0x30;
    if(gewei != tmpValue)
    {
        change = true;
        gewei = tmpValue;
    }
    if(change)
        floortalkcount = 0;
    if(pTalk)
    {
        if(!floortalkcount)
            SetAudio(pSAF);
        floortalkcount++;
    }
    if(!change && (mFrame51Mode == 2))
        extendfunc &= 0xfe;
    else
        extendfunc |= 0x01;
}

void PhysicalSAF::SetFloorEmpty()
{
    volatile bool change = false;
    volatile quint8 tmpValue = 0;
    tmpValue = 0x20;
    if(baiwei != tmpValue)
    {
        change = true;
        baiwei = tmpValue;
    }
    tmpValue = 0x20;
    if(shiwei != tmpValue)
    {
        change = true;
        shiwei = tmpValue;
    }
    tmpValue = 0x20;
    if(gewei != tmpValue)
    {
        change = true;
        gewei = tmpValue;
    }
    if(!change && (mFrame51Mode == 2))
        extendfunc &= 0xfe;
    else
        extendfunc |= 0x01;
}

//>@只要设置了音频都进行播放
//>@如果设置了多个音频标号，则发送多个音频帧
void PhysicalSAF::SetAudio(quint8 pNum)
{
    audiolist.append(pNum);
    extendfunc |= 0x08;
}

void PhysicalSAF::SetRadioFunc(quint8 pNum)
{
    bool change = false;
    if(raidofunc != pNum)
    {
        change = true;
        raidofunc = pNum;
    }
    if(!change && (mFrame51Mode == 2))
        extendfunc &= 0xfb;
    else
        extendfunc |= 0x04;
}

void PhysicalSAF::SetArrow(quint8 pEnable, quint8 pUp, quint8 pDown)
{
    volatile bool change = false;
    volatile quint8 tmpValue = realcheckfun & 0x01;
    if(tmpValue)
        tmpValue = 1;
    if(pUp != tmpValue)
    {
        change = true;
        if(pUp)
            realcheckfun |= 0x01;
        else
            realcheckfun &= 0xfe;
    }
    tmpValue = realcheckfun & 0x02;
    if(tmpValue)
        tmpValue = 1;
    if(pDown != tmpValue)
    {
        change = true;
        if(pDown)
            realcheckfun |= 0x02;
        else
            realcheckfun &= 0xfd;
    }
    checkfun &= 0xfc;
    if(!pEnable)
    {
        indicatecheckfun = 0;
        checkfun = 0;
    }
    else/* if(change)*/
    {
        indicatecheckfun = realcheckfun;
        checkfun |= realcheckfun;
    }
    extendfunc |= 0x02;
}

void PhysicalSAF::RollArrow(bool pEnable)
{
    if(pEnable) //arrow scrolling on
        checkfun |= 0x04;
    else //arrow scrolling off
        checkfun &= 0x0fb;
    extendfunc |= 0x02;
}

bool PhysicalSAF::Frame51()
{
    IDE_TRACE_STR(mFrame51Type);
    if(!mFrame51Type.compare("Logo", Qt::CaseInsensitive))
    {
        SetRadioFunc(51);
        extendfunc &= 0xfe;
    }
    else if(!mFrame51Type.compare("Flr_Logo", Qt::CaseInsensitive))
    {
        static int sFlrLogoFlag = 0;  //>1表示检修图片
        if(sFlrLogoFlag)
        {
            sFlrLogoFlag = 0;
            raidofunc = 51;
            extendfunc |= 0x04;
            extendfunc &= 0xfe;
        }
        else
        {
            sFlrLogoFlag = 1;
            extendfunc |= 0x01;
            //extendfunc &= 0xfb;
        }
    }
    else if(!mFrame51Type.compare("Mte_Logo", Qt::CaseInsensitive))
    {
        static int sFlrLogoFlag = 0;  //>1表示检修图片
        if(sFlrLogoFlag)
        {
            sFlrLogoFlag = 0;
            raidofunc = 51;
            extendfunc |= 0x04;
            extendfunc &= 0xfe;
        }
        else
        {
            sFlrLogoFlag = 1;
            raidofunc = 40;
            extendfunc |= 0x04;
            extendfunc &= 0xfe;
        }
    }
    else
    {
        return false;
    }

    IDE_DEBUG(QString("baiwei:%1 shiwei:%2 gewei:%3 checkfun:%4 raidofunc:%5 extendfunc:%6")
              .arg(baiwei).arg(shiwei).arg(gewei).arg(checkfun).arg(raidofunc).arg(extendfunc));

    volatile quint8 check /*= 0x01 ^ baiwei ^ shiwei ^ gewei ^ checkfun ^ raidofunc ^ 0 ^ extendfunc & 0x7f*/;
    mReadByteArray.append((char)0x80);
    mReadByteArray.append((char)0x01);       //data 1
    mReadByteArray.append((char)baiwei);     //data 2
    mReadByteArray.append((char)shiwei);     //data 3
    mReadByteArray.append((char)gewei);      //data 4
    mReadByteArray.append((char)checkfun);   //data 5
    mReadByteArray.append((char)raidofunc);      //data 6
    mReadByteArray.append((char)0);      //data 7
    mReadByteArray.append((char)extendfunc);      //data 8
    mReadByteArray.append((char)check);
    mReadByteArray.append((char)0x81);
    emit readyRead();
    return true;
}

bool isFloorFrame(quint8 pSaf)
{
    if((pSaf > 0 && pSaf < 40) || (pSaf > 60 && pSaf < 69) || (pSaf == 70))
        return true;
    return false;
}

bool isLogoFrame(quint8 pSaf)
{
    if((pSaf >= 40 && pSaf <= 41) || (pSaf >= 45 && pSaf <= 48) || (pSaf >= 56 && pSaf <= 58)
             || (pSaf == 51) || (pSaf == 44) || (pSaf == 60))
        return true;
    return false;
}

quint32 PhysicalSAF::ParseFrameBuffer(PH_Type pType, QIODevice *pDev, QByteArray pByteArray)
{
    static volatile quint8 oldsgo = 0;
    static volatile quint8 oldtalk = 0;
    static volatile quint8 oldsaf = 255;

    int tmpCount = pByteArray.count();
    if(tmpCount < 6)
    {
        pByteArray.clear();
        return 0;
    }

    volatile quint8 saf   = (quint8)pByteArray.at(0) & 0x7f;  //>@HMI_ED01	0
    volatile quint8 flm   = (quint8)pByteArray.at(1) & 0x01;  //>@HMI_EFLM	1
    volatile quint8 sgo   = (quint8)pByteArray.at(2) & 0x01;  //>@HMI_ESGO	2  上升沿有效
    volatile quint8 fld   = (quint8)pByteArray.at(3) & 0x01;  //>@HMI_EFLD	3
    volatile quint8 esf   = (quint8)pByteArray.at(4) & 0x01;  //>@HMI_ESF	4
    volatile quint8 talk  = (quint8)pByteArray.at(5) & 0x01;  //>@HMI_ETALK	5  上升沿有效

    if(!saf && !flm && !sgo && !fld && !esf && !talk)
    {
        //IDE_DEBUG("This frame is broken, it's all zero!");
        return 0;
    }

    volatile quint8 oldextendfunc = extendfunc;
    extendfunc = 0;

    bool diff_flag = false;
    if((saf != 53) && (old_saf != saf))
    {
        old_saf = saf;
        diff_flag = true;
    }
    if(old_flm != flm)
    {
        old_flm = flm;
        diff_flag = true;
    }
    if(old_fld != fld)
    {
        old_fld = fld;
        diff_flag = true;
    }

    /*
        add 2015-02-09
        bug:测试机给芯子52帧，芯子进入standby模式下后，再通过测试机给芯子箭头信号，芯子界面上没有出现箭头
        原因：从standby模式退出时，需要经过一定时间才会mEnable变为true，因此唤醒standby模式的指令因为mEnable依旧为false被抛弃
        解决方法：增加一个标志位tmpWakeupLcd，如果是唤醒操作，则直接忽视mEnable为false的情况
    */
    bool tmpWakeupLcd = false;
    if(!mTemporaryTest)
    {
        if(saf == mDebugFrame.mNum) //>@Debug开关，40帧开启，半小时后依旧没有连接APP，则自动关闭Debug
        {
            OpenWIFI();
            if(!mPlayer.isNull())
            {
                mPlayer.data()->Abort(AO_ADMUSIC, 1);
            }
        }

        //>@查看是否为重复帧，要排除53帧以及51帧(此处是客户丢弃了51帧后才更改的。)
        if(!m_LCD.isNull())
        {
            PhysicalLCD *tmpLcd = m_LCD.data();
            if(tmpLcd)
            {
                mHasArrow = fld || flm;
                //>@只有当帧有变化时，才从standby中唤醒,只要有箭头，就不会进入standby模式
                if(mHasArrow)
                {
                    tmpLcd->SetSrnSaveEn(false);
                    tmpLcd->SetStandbyEn(false);
    #ifdef DISPLAY  //>@退出standby模式时需要执行将设备恢复操作
                    emit sSetDevices(true, DM_STANDBY);
    #endif
                    tmpWakeupLcd = true;
                }
                else
                {
                    tmpLcd->SetSrnSaveEn(true);
                    tmpLcd->SetStandbyEn(true);
                    if((saf != 51) && diff_flag) //>@包括了箭头从有变为没有时需要开启standby模式的考虑
                    {
    #ifdef DISPLAY  //>@退出standby模式时需要执行将设备恢复操作
                        emit sSetDevices(true, DM_STANDBY);
                        emit sFeedLCD();   //>@发送喂狗命令，防止进入standby模式
    #endif
                        tmpWakeupLcd = true;
                    }
                }
            }
        }
    }

    IDE_DEBUG(QString("Driver -- saf:%1 flm:%2 sgo:%3 fld:%4 esf:%5 talk:%6").arg(saf).arg(flm).arg(sgo).arg(fld).arg(esf).arg(talk));

    if(!mTemporaryTest)
    {
        if(!mEnable && !tmpWakeupLcd)
        {
            IDE_TRACE();
            return 0;
        }
    }

    if(diff_flag)  //>@只有当帧有变化时，才从customer logo切换状态中退出
    {
        mFrame51Mode = 0;
    }

    if((oldsgo == 0) && (sgo == 1))
    {
        oldsgo = sgo;
    }
    else
    {
        oldsgo = sgo;
        sgo = 0;
    }
    //>@当楼层帧改变时，不论之前talk是否为高，都认为是上升沿
    if(talk == 1)
    {
        if(isFloorFrame(saf))
        {
            volatile quint8 oldfloor = baiwei*100+shiwei*10+gewei;
            if(saf != oldfloor)
            {
                oldtalk = 0;
                //IDE_TRACE();
            }
        }
        else if(saf == 59)
        {
            oldtalk = 0;
            //IDE_TRACE();
        }
    }
    if((oldtalk == 0) && (talk == 1))
    {
        oldtalk = talk;
    }
    else
    {
        oldtalk = talk;
        talk = 0;
    }

    IDE_DEBUG(QString("Edge -- saf:%1 flm:%2 sgo:%3 fld:%4 esf:%5 talk:%6").arg(saf).arg(flm).arg(sgo).arg(fld).arg(esf).arg(talk));

    SetArrow(esf, flm, fld);
    //>@根据协议图中Autotest1中52帧发生时，如果来talk信号，依然播报上一个楼层帧的音频，因此53/52帧要独立于其它saf帧处理，并且处理完后将saf转换为上一个帧。
    if(saf == 52) //arrow scrolling on
    {
        RollArrow(true);
        saf = oldsaf;
    }
    else if(saf == 53) //arrow scrolling off
    {
        RollArrow(false);
        saf = oldsaf;
    }

    switch(saf)
    {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
        {
            SetFloor(saf, talk);
            break;
        }
        case 70:
        {
            SetFloor(0, talk);
            break;
        }
        case 61:
        case 62:
        case 63:
        case 64:
        case 65:
        case 66:
        case 67:
        case 68:
        case 69:
        {
            SetFloor(saf, talk);
            break;
        }
        case 40://检修 Inspection
        case 41://禁入 Access forbidden
        case 46://消防 fire mode
        case 47://Liftier
        case 48://cell CLEEULE
        {
            SetRadioFunc(saf);
            break;
        }
        case 45://满载 car full
        {
            SetRadioFunc(saf);
            SetAudio(48);  //>@SAF新增加M48音频
            break;
        }
        case 51://logo
        {
            SetRadioFunc(saf);
            break;
        }
        case 44://超载 overload
        case 56://强关 forced closeing，超载和强关音频的优先级最高，需要先关闭其它音频。
        {
            SetRadioFunc(saf);

            PhysicalPlayer *tmpPlayer = mPlayer.data();
            MEDIA_PLAYER *tmpMediaPlayer = 0;
            if(tmpPlayer && (tmpMediaPlayer = tmpPlayer->GetMediaPlayer(AO_LIFTFLR)))
            {
                tmpMediaPlayer->Clear();
                tmpMediaPlayer->Abort();
            }

            SetAudio(D_AO_BEEP3);  //>@协议上是循环播放，直至帧改变(需要修改)
            break;
        }
        case 57://rescue(emergency) mode
        case 58://digicode(password)
        {
            SetRadioFunc(saf);
            if(talk)
                SetAudio(saf);
            break;
        }
        case 59://door close
        {
            extendfunc |= 0x01; //>@注意：显示之前的楼层图片
            SetRadioFunc(saf);
            if(talk)
                SetAudio(D_AO_DOORCLOSE);
            break;
        }
        case 60://warning car roof
        {
            SetRadioFunc(saf);
            SetAudio(saf);
            break;
        }
        default:
        {
            extendfunc = oldextendfunc & (~(1<<4));
            break;
        }
    }
    oldsaf = saf;  //>@保存SAF的值

    //>@如果是其它信号，则关闭强关与超载的音频循环，还要去除53、54帧。
    if((saf != 44) && (saf != 56) && (saf != 53) && (saf != 54))
    {
        if(mPlayer.isNull())
        {
#ifdef IDE
            mPlayer = VAR_CAST<PhysicalPlayer*>(m_DevManager->m_PHDevList.value(P_Player));
#elif DISPLAY
            mPlayer = VAR_CAST<PhysicalPlayer*>(m_DevManager->m_PHDevList.value(P_Player));
#endif
        }
#ifdef DISPLAY
        PhysicalPlayer *tmpPlayer = mPlayer.data();
        MEDIA_PLAYER *tmpMediaPlayer = 0;
        if(tmpPlayer && (tmpMediaPlayer = tmpPlayer->GetMediaPlayer(AO_LIFTFLR)))
        {
            if(!mMedia.isNull())
            {
                QString tmpMediaFile = mMedia.data()->GetPixmapPath(43);
                IDE_TRACE_STR(tmpMediaFile);
                tmpMediaPlayer->StopOne(tmpMediaFile);
            }
        }
#endif
    }

    //>@SGO信号产生dooropen
    if(mOpenMsgEnable && sgo)  //>@door open
        SetAudio(D_AO_DOOROPEN);
    if(esf && sgo)
    {
        quint32 tmpAudioNum = INVALIDRC;
        if(flm && fld)  //>@双箭头
            tmpAudioNum = D_AO_GONG3;
        else if(flm)
            tmpAudioNum = D_AO_GONG1;
        else if(fld)
            tmpAudioNum = D_AO_GONG2;
        if(tmpAudioNum != INVALIDRC)
        {
            bool flag = false;
#ifdef DISPLAY
            if(mPlayer.isNull())
                mPlayer = VAR_CAST<PhysicalPlayer*>(m_DevManager->m_PHDevList.value(P_Player));
            PhysicalPlayer *tmpPlayer = mPlayer.data();
            MEDIA_PLAYER *tmpMediaPlayer = 0;
            if(tmpPlayer && (tmpMediaPlayer = tmpPlayer->GetMediaPlayer(AO_LIFTFLR)))
            {
                if(!mMedia.isNull())
                {
                    QString tmpMediaFile = mMedia.data()->GetPixmapPath(tmpAudioNum);
                    IDE_TRACE_STR(tmpMediaFile);
                    flag = tmpMediaPlayer->CanPlay(tmpMediaFile);
                }
            }
#endif
            if(flag)
            {
                IDE_TRACE_INT(tmpAudioNum);
                SetAudio(tmpAudioNum);
            }
        }
    }

    //>@界面显示
    volatile quint8 extendFun = extendfunc & 0x7f;
    volatile quint8 tmpArrowFresh = extendFun & D_ArrowFresh;
    volatile quint8 tmpFloorFresh = extendFun & D_FloorFresh;
    volatile quint8 tmpFuncFresh = extendFun & D_FunctionFresh;
    if(m_ReuseType == RU_FLRFUNC)
    {
        if(tmpFloorFresh)
        {
            tmpFuncFresh = 0;
            emit sRadioFunEvent(HIDERC);
        }
        else if(tmpFuncFresh)
        {
            tmpFloorFresh = 0;
            emit sFloorEvent(HIDERC);
        }
    }
    //>@箭头
    if(tmpArrowFresh)
        emit sArrowEvent((quint32)checkfun);
    //>@楼层
    if(tmpFloorFresh)
        emit sFloorEvent(((quint32)baiwei<<16)+((quint32)shiwei<<8)+(quint32)gewei);
    //>@单选功能
    if(tmpFuncFresh)
        emit sRadioFunEvent(raidofunc);
    //>@先显示图片，再播放声音
    for(int i=0;i<audiolist.count();i++)
        emit sMediaEvent(audiolist.at(i));
    audiolist.clear();
    return 1;
}
