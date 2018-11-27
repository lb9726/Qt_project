#include "physicaldial.h"
#include "deviceinc.h"

PhysicalDial::PhysicalDial(QObject *parent) :
    PhysicalDevice(parent)
{
    qRegisterMetaType<DIAL_TYPE>();
#ifdef N329
    m_File = 0;
    m_File = new QFile;
    m_File->setFileName(QString("/dev/dial"));
#endif
#ifdef IMX6X
    system("mkdir /debug");
    system("mount -t debugfs nodev /debug");
#endif

    mMonitorWait.mFreq = 1500;
}

PhysicalDial::~PhysicalDial()
{
#ifdef N329
    if(m_File)
        m_File->close();
#endif
}

quint32 PhysicalDial::Monitor(quint32 pCount)
{
    if(!mEnable || mElevator.isNull() || !mElevator.data()->mAutoTestEnable)
        return 0;
    if(!mMonitorWait.isValid(pCount, m_DevManager->m_DevMonitorFreq))
        return 2;
    ExecDial(1);
}

void PhysicalDial::slot_Initialize()
{
    PhysicalDevice::slot_Initialize();
#ifdef DISPLAY
    mElevator = VAR_CAST<PhysicalElevator*>(m_DevManager->m_PHDevList.value(P_COM));
    if(mElevator.isNull())
        mElevator = VAR_CAST<PhysicalElevator*>(m_DevManager->m_PHDevList.value(P_SAF));
#endif
#ifdef IMX6X
    system("echo 55  > /sys/class/gpio/export");   //释放使用 echo 36  > /sys/class/gpio/unexport
    system("echo in > /sys/devices/virtual/gpio/gpio55/direction");
    system("echo 87  > /sys/class/gpio/export");
    system("echo in > /sys/devices/virtual/gpio/gpio87/direction");
    system("echo 106  > /sys/class/gpio/export");
    system("echo in > /sys/devices/virtual/gpio/gpio106/direction");
    system("echo 107  > /sys/class/gpio/export");
    system("echo in > /sys/devices/virtual/gpio/gpio107/direction");
#endif
}

quint32 PhysicalDial::ExecDial(int pMode)
{
#if 0
    char *tmpDial = (char*)malloc(D_DIALCOUNT);
    tmpDial[0] = 0;  //>@需要取反
    tmpDial[1] = 1;
    tmpDial[2] = 0;
    tmpDial[3] = 1;
#else
    char *tmpDial = (char*)malloc(D_DIALCOUNT);
#ifdef N329
    if(!m_File->isOpen())
    {
        if(!m_File->open(QIODevice::ReadWrite|QIODevice::Unbuffered))
        {
            free(tmpDial);
            IDE_TRACE();
            return 0;
        }
    }
    qint64 num = m_File->read(tmpDial, D_DIALCOUNT);
    //m_File->close(); //>@如果程序突然死掉，会出现此驱动文件无法打开情况。
    if(num != D_DIALCOUNT)
    {
        IDE_TRACE();
        free(tmpDial);
        return 0;
    }
#endif
#ifdef IMX6X
    QProcess tmpProc;
    tmpProc.start("cat /sys/devices/virtual/gpio/gpio55/value");
    tmpProc.waitForFinished(1000);
    QString tmpStr = tmpProc.readAllStandardOutput();
    tmpDial[0] = tmpStr.toInt();

    tmpProc.start("cat /sys/devices/virtual/gpio/gpio87/value");
    tmpProc.waitForFinished(1000);
    tmpStr = tmpProc.readAllStandardOutput();
    tmpDial[1] = tmpStr.toInt();

    tmpProc.start("cat /sys/devices/virtual/gpio/gpio106/value");
    tmpProc.waitForFinished(1000);
    tmpStr = tmpProc.readAllStandardOutput();
    tmpDial[2] = tmpStr.toInt();

    tmpProc.start("cat /sys/devices/virtual/gpio/gpio107/value");
    tmpProc.waitForFinished(1000);
    tmpStr = tmpProc.readAllStandardOutput();
    tmpDial[3] = tmpStr.toInt();
#endif
#endif
    //>@开机立即读取一次DIAL的值，与上次保存的值进行比较，如果不一样，则表示DIAL已经被修改过了。
    //>@需要保存此次DIAL的值。
    //>@如果上次WIFI修改了，则以软设置为主，否则只有当DIAL修改后才以DIAL为主
    bool flag = false;  //>@是否有改变
    for(int i=0;i<D_DIALCOUNT;i++)
    {
        DIAL_INFO *tmpInfo = m_DialNums.value(i);
        if(!tmpInfo)
            continue;
#ifdef D_DIALFLIP
        if(tmpDial[i])
            tmpDial[i] = 0;
        else
            tmpDial[i] = 1;
#endif
        IDE_DEBUG(QString("Dial %1 is %2").arg(i).arg((int)tmpDial[i]));
        if(tmpInfo->mValue == tmpDial[i])
            continue;
        flag = true;
        tmpInfo->mValue = tmpDial[i];
    }
    if(pMode == 1)
    {
        if(flag)
        {
            emit sInfo(QString("Dial 1[%1] 2[%2] 3[%3] 4[%4]")
                       .arg(tmpDial[0]?QString("ON"):QString("OFF"))
                       .arg(tmpDial[1]?QString("ON"):QString("OFF"))
                       .arg(tmpDial[2]?QString("ON"):QString("OFF"))
                       .arg(tmpDial[3]?QString("ON"):QString("OFF")), 2000, DM_NORMAL);
        }
    }
    else if(pMode == 2)
    {
        ProcessDial();
    }
#if 0
    m_DevManager->mMdpManager->SaveMDP();
#endif
    free(tmpDial);
    return 1;
}

void PhysicalDial::ProcessDial()
{
#ifdef DISPLAY
    QHash<DIAL_TYPE, DIAL_INFO*> tmpDialHash;
    QList<DIAL_TYPE> tmpDialTypeList;
    QList<DIAL_INFO*> tmpDialList = m_DialNums.values();
    for(int i=0;i<tmpDialList.count();i++)
    {
        DIAL_INFO *tmpDial = tmpDialList.at(i);
        if(!tmpDial)
            continue;
        tmpDialTypeList.append(tmpDial->mType);
        tmpDialHash.insert(tmpDial->mType, tmpDial);
    }
    while(!tmpDialTypeList.isEmpty())
    {
        DIAL_TYPE tmpType = tmpDialTypeList.takeFirst();
        switch(tmpType)
        {
#ifdef D_ROTEN
            case DIAL_ROTATION:
            case DIAL_ROTATIONEN:
            {
                tmpDialTypeList.removeOne(DIAL_ROTATION);
                tmpDialTypeList.removeOne(DIAL_ROTATIONEN);
                DIAL_INFO *tmpDialRotate = tmpDialHash.take(DIAL_ROTATION);
                DIAL_INFO *tmpDialRotateEn = tmpDialHash.take(DIAL_ROTATIONEN);
                if(!tmpDialRotate)
                    continue;
                if(!tmpDialRotateEn || (tmpDialRotateEn && tmpDialRotateEn->mValue == 1))  //>@如果没有en拨码或者有并且为使能状态，则处理Roate脚位
                {
                    PhysicalLCD *tmpLCD = VAR_CAST<PhysicalLCD*>(m_DevManager->m_PHDevList.value(P_LCD));
                    if(tmpLCD)
                    {
                        if(tmpDialRotate->mValue == 0)
                            tmpLCD->SetPara("Rotation", "180");
                        else
                            tmpLCD->SetPara("Rotation", "90");
                    }
                }
                break;
            }
#endif
#ifdef D_ROTSEL
            case DIAL_ROTSEL1:
            case DIAL_ROTSEL0:
            {
                tmpDialTypeList.removeOne(DIAL_ROTSEL1);
                tmpDialTypeList.removeOne(DIAL_ROTSEL0);
                DIAL_INFO *tmpDial1 = tmpDialHash.take(DIAL_ROTSEL1);
                DIAL_INFO *tmpDial0 = tmpDialHash.take(DIAL_ROTSEL0);
                if(!(tmpDial0 && tmpDial1))
                    continue;
                PhysicalLCD *tmpLCD = VAR_CAST<PhysicalLCD*>(m_DevManager->m_PHDevList.value(P_LCD));
                if(tmpLCD)
                {
                    IDE_TRACE_INT(tmpDial0->mValue);
                    IDE_TRACE_INT(tmpDial1->mValue);
                    int tmpValue = tmpDial0->mValue + tmpDial1->mValue*2;
                    //RotSel1 RotSel0
                    //   0      0       270
                    //   0      1     Vertical (90°)
                    //   1      0       180°
                    //   1      1       APP
                    if(tmpValue == 0)
                        tmpLCD->SetPara("Rotation", "270");
                    else if(tmpValue == 1)
                        tmpLCD->SetPara("Rotation", "90");
                    else if(tmpValue == 2)
                        tmpLCD->SetPara("Rotation", "180");
                }
                break;
            }
#endif
            case DIAL_STANDBY:
            {
                DIAL_INFO *tmpDial = tmpDialHash.take(DIAL_STANDBY);
                if(!tmpDial)
                    continue;
                PhysicalLCD *tmpLCD = VAR_CAST<PhysicalLCD*>(m_DevManager->m_PHDevList.value(P_LCD));
                if(tmpLCD)
                {
                    if(tmpDial->mValue == 0)
                        tmpLCD->SetPara("StandBy", "0");
                    else
                    {
                        int tmpValue = tmpLCD->mStandBy.mNum;
                        if(tmpValue <= 0)
                            tmpValue = 60000;
                        tmpLCD->SetPara("StandBy", QString("%1").arg(tmpValue));
                    }
                }
                break;
            }
            case DIAL_AUDIO:
            {
                DIAL_INFO *tmpDial = tmpDialHash.take(DIAL_AUDIO);
                if(!tmpDial)
                    continue;
                PhysicalPlayer *tmpPlayer = VAR_CAST<PhysicalPlayer*>(m_DevManager->m_PHDevList.value(P_Player));
                if(tmpPlayer)
                {
                    if(tmpDial->mValue == 0)
                        tmpPlayer->mSoundOn = false;
                    else
                        tmpPlayer->mSoundOn = true;
                }
                break;
            }
            case DIAL_AUTOTEST:  //>@对于Autotest，需要单独处理为只要Dial或者MDP文件中有一处为true，则Autotest模式就有效
            {
                DIAL_INFO *tmpDial = tmpDialHash.take(DIAL_AUTOTEST);
                if(!tmpDial)
                    continue;
                PhysicalSAF *tmpSAF = VAR_CAST<PhysicalSAF*>(m_DevManager->m_PHDevList.value(P_SAF));
                if(tmpSAF)
                {
                    if((tmpDial->mValue == 1) || (tmpSAF->mAutoTestEnable == true))
                        tmpSAF->SetPara("AutoTest", "true");
                    else
                        tmpSAF->SetPara("AutoTest", "false");
                    IDE_TRACE_INT(tmpSAF->mAutoTestEnable);
                }
                break;
            }
            case DIAL_AUDVOL1:
            case DIAL_AUDVOL0:
            {
                tmpDialTypeList.removeOne(DIAL_AUDVOL1);
                tmpDialTypeList.removeOne(DIAL_AUDVOL0);
                DIAL_INFO *tmpDial1 = tmpDialHash.take(DIAL_AUDVOL1);
                DIAL_INFO *tmpDial0 = tmpDialHash.take(DIAL_AUDVOL0);
                if(!(tmpDial0 && tmpDial1))
                    continue;
                PhysicalPlayer *tmpPlayer = VAR_CAST<PhysicalPlayer*>(m_DevManager->m_PHDevList.value(P_Player));
                if(tmpPlayer)
                {
                    IDE_TRACE_INT(tmpDial0->mValue);
                    IDE_TRACE_INT(tmpDial1->mValue);
                    int tmpValue = tmpDial0->mValue + tmpDial1->mValue*2;
                    qreal tmpDiv;
                    if(tmpValue == 1)
                        tmpDiv = 0.33;
                    else if(tmpValue == 2)
                        tmpDiv = 0.5;
                    else if(tmpValue == 3)
                        tmpDiv = 1;
                    else
                        tmpDiv = 0;
                    tmpPlayer->SetScaler(tmpDiv);
                }
                break;
            }
            default:
                break;
        }
    }
#endif
}

quint32 PhysicalDial::InitPara()
{
    QDomNodeList tmpParaNodeList = m_ParaElement.childNodes();
    if(tmpParaNodeList.count() < 1)
        return 0;
    quint32 validcount = 0;
    for(int i=0;i<tmpParaNodeList.count();i++)
    {
        QHBoxLayout* tmpHBoxLayout = 0;
        QDomElement tmpElement = tmpParaNodeList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpName = tmpElement.tagName();
        if(tmpName.startsWith("Switch"))
        {
            bool ok = false;
            int tmpNum = tmpName.mid(6).toInt(&ok, 10);
            if(!ok)
                continue;
            QString tmpTypeName = tmpElement.text();
            DIAL_TYPE tmpType = GetDialType(tmpTypeName);
            if(tmpType == DIAL_NONE)
                continue;
            DIAL_INFO *tmpDialInfo = new DIAL_INFO;
            tmpDialInfo->mType = tmpType;
            tmpDialInfo->mName = tmpTypeName;
            m_DialNums.insert(tmpNum, tmpDialInfo);
            tmpHBoxLayout = CreateTextEdit(tmpName, tmpTypeName);
        }
        else
        {
            continue;
        }
        if(!tmpHBoxLayout)
            continue;
        validcount++;
        m_SettingGroup->m_VLayout->addWidget(CreateLine());
        m_SettingGroup->m_VLayout->addLayout(tmpHBoxLayout);
    }
    ExecDial();
    return validcount;
}

void PhysicalDial::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(tmpName.startsWith("Switch"))
    {
        bool ok = false;
        int tmpNum = tmpName.mid(6).toInt(&ok, 10);
        if(!ok)
            return;
        DIAL_INFO *tmpDialInfo = m_DialNums.value(tmpNum);
        if(!tmpDialInfo)
            return;
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            QString tmpName = tmpDialInfo->mName;
            ModifyTextEdit(tmpComponent, tmpElement, tmpName, tmpComponent->text());
            tmpDialInfo->setName(tmpName);
        }
    }
}

