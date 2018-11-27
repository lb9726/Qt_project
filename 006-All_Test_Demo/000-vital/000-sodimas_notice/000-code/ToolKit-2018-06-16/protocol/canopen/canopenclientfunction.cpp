#include "canopenclientfunction.h"
#include "define.h"

CanOpenClientFunction::Garbo CanOpenClientFunction::garbo;
CanOpenClientFunction *CanOpenClientFunction::m_pInstance = NULL;
const QString MediaPath = QString("/usr/bst/usrfs/Theme/sodimas/media/");
CanOpenClientFunction::CanOpenClientFunction(QObject *parent) : QObject(parent)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"start";
    m_canopenProtocolThread = NULL;
    m_canopenClient = NULL;
    m_gpio = NULL;
    m_extensionbtn = NULL;
    m_maintenance = 0;
    m_bigText = false;
    m_Mplayer = NULL;
    m_beepvol = 20;
    init();
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"end";
}

CanOpenClientFunction *CanOpenClientFunction::getInstace()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new CanOpenClientFunction();
    }
    return m_pInstance;
}

quint8 CanOpenClientFunction::getDailSwitchNumber()
{
    quint8 tmpValue = 0;
    if (m_gpio)
    {
        tmpValue = m_gpio->oneGetDialSwitchValue();
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"m_gpio->oneGetDialSwitchValue() = "<<tmpValue;
    }
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"tmpValue = "<<tmpValue;
    return tmpValue;
}

void CanOpenClientFunction::setMplayer(Mplayer *pPlayer)
{
    m_Mplayer = pPlayer;
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<m_Mplayer;
}

void CanOpenClientFunction::setBeepVolume(int pVolume)
{
    if (pVolume >= 100)
    {
        pVolume = 99;
    }
    m_beepvol = pVolume;
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<pVolume;
}

void CanOpenClientFunction::setBigOrSmallTextEmpty()
{
    if (m_canopenClient)
    {
        m_canopenClient->setAllTextIsEmpty();
    }
}

void CanOpenClientFunction::dailNumberInitCanClient()
{
    quint8 tmpNodeId = 0;
    tmpNodeId = getDailSwitchNumber();
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<" getDailSwitchNumber tmpNodeId = "<<tmpNodeId;
    int tmpCanBuad = 250;
    if (tmpNodeId <= 1)
    {
        tmpCanBuad = 250;
        tmpNodeId = tmpNodeId + 60;
    }
    else if (tmpNodeId >= 2 && tmpNodeId <= 31)
    {
        tmpCanBuad = 250;
        tmpNodeId = tmpNodeId - 1;
    }
    else if (tmpNodeId >= 32 && tmpNodeId <= 63)
    {
        tmpCanBuad = 50;
        tmpNodeId = tmpNodeId - 31;
    }
    else if (255 == tmpNodeId) // AutoTest
    {
        tmpCanBuad = 0;
    }
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<" tmpCanBuad = "<<tmpCanBuad<<" tmpNodeId = "<<tmpNodeId;
    if (m_canopenClient)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<" tmpCanBuad = "<<tmpCanBuad<<" tmpNodeId = "<<tmpNodeId;
        m_canopenClient->initCanopenClient(tmpNodeId, tmpCanBuad, "0");
    }
}

void CanOpenClientFunction::init()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"start init";
    m_canopenProtocolThread = new QThread;
    if (NULL == m_canopenProtocolThread)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"allocate m_canopenThread memeroy is failed";
        return;
    }
    m_canopenClient = OpenClient::getInstace();
    if (NULL == m_canopenClient)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"allocate m_canopenClient memeroy is failed";
        return;
    }

    m_gpio = new Gpios();
    if (NULL == m_gpio)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"new Gpios() error!!";
        return;
    }
    m_extensionbtn = new ExtensionButtons();
    if (NULL == m_extensionbtn)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"new m_extensionbtn() error!!";
        return;
    }
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"ended init";
}

void CanOpenClientFunction::initCanOpenFunction()
{
    dailNumberInitCanClient();
    if(m_canopenClient)
    {
        if (m_gpio)
        {
            m_gpio->setElibLed(true);
        }
    }
    else
    {
        if (m_gpio)
        {
            m_gpio->setElibLed(false);
        }
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"m_canopenClient is null set can_failed light";
        return;
    }

    if (m_canopenClient)
    {
        m_canopenClient->moveToThread(m_canopenProtocolThread);

        connect(m_canopenClient, &OpenClient::sigNodeState, this, &CanOpenClientFunction::onSigNodeState, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigAFF_EN, this, &CanOpenClientFunction::onSigAFF_EN, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigFCT_AFF, this, &CanOpenClientFunction::onSigFCT_AFF, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigArrowAction, this, &CanOpenClientFunction::onSigArrowAction, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigMainVolumeChange, this, &CanOpenClientFunction::onSigMainVolumeChange, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigMusicVolumeChange, this, &CanOpenClientFunction::onSigMusicVolumeChange, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigButtonLightOn, this, &CanOpenClientFunction::onSigButtonLightOn, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigButtonLightOff, this, &CanOpenClientFunction::onSigButtonLightOff, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigFloorArrive, this, &CanOpenClientFunction::onSigFloorArrive, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigShowFuncLogo, this, &CanOpenClientFunction::onSigShowFuncLogo, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigShowFloorOrLogo, this, &CanOpenClientFunction::onSigShowFloorOrLog, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigMessage, this, &CanOpenClientFunction::onSigMessage, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigRealMessage, this, &CanOpenClientFunction::onSigRealMessage, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigSDO1, this, &CanOpenClientFunction::onSigSDO1, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigOrientation, this, &CanOpenClientFunction::onSigOrientation, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigContraste, this, &CanOpenClientFunction::onSigContraste, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigBacklight, this, &CanOpenClientFunction::onSigBacklight, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigVit_scrol, this, &CanOpenClientFunction::onSigVit_scrol, Qt::QueuedConnection);
        connect(m_canopenClient, &OpenClient::sigSDO2, this, &CanOpenClientFunction::onSigSDO2, Qt::QueuedConnection);
        connect(m_canopenClient, SIGNAL(sigBuzz(int)), this, SLOT(onSigBuzz(int)), Qt::QueuedConnection);
        connect(m_canopenClient, SIGNAL(sigBuzz()), this, SLOT(onSigBuzz()), Qt::QueuedConnection);
        if (m_canopenProtocolThread)
        {
            m_canopenProtocolThread->start();
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"m_canopenProtocolThread start successed!!";
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"m_canopenProtocolThread start error!!";
        }
    }

    this->moveToThread(&m_canopenFunctionThread);
    m_canopenFunctionThread.start();

    if (m_gpio)
    {
        connect(m_gpio, &Gpios::sigBtnPressed, this, &CanOpenClientFunction::reportButtonPressed, Qt::QueuedConnection);
        connect(m_gpio, &Gpios::sigBtnCanceld, this, &CanOpenClientFunction::reportButtonPressed, Qt::QueuedConnection);
        connect(m_gpio, &Gpios::sigSpecialBtnPressed, this, &CanOpenClientFunction::reportButtonPressed, Qt::QueuedConnection);
        connect(m_gpio, &Gpios::sigDailChanged, this, &CanOpenClientFunction::onDailSwitchChanged, Qt::QueuedConnection);
        connect(m_gpio, &Gpios::sigBtnReleased, this, &CanOpenClientFunction::reportButtonReleased, Qt::QueuedConnection);
        m_gpio->onSigBtnPressedLightOn(18);
        m_gpio->start();
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"m_gpio start successed!";
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"m_gpio start failed!";
    }
    if (m_extensionbtn)
    {
        connect(m_extensionbtn, &ExtensionButtons::sigExtensionBtnPressed, this, &CanOpenClientFunction::reportButtonPressed, Qt::QueuedConnection);
        connect(m_extensionbtn, &ExtensionButtons::sigExtensionBtnCanceled, this, &CanOpenClientFunction::reportButtonPressed, Qt::QueuedConnection);
        connect(m_extensionbtn, &ExtensionButtons::sigExtensionBtnReleased, this, &CanOpenClientFunction::reportButtonReleased, Qt::QueuedConnection);
        if (m_extensionbtn->initExtension())
        {
            m_extensionbtn->start();
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"m_extensionbtn start successed!";
        }
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"m_extensionbtn start failed!";
    }
    IDE_TRACE_STR(QString("init will ended!"));
}

//NMT 完成NMT信号发送 true表示为operation 模式 false 表示非operation 模式
void CanOpenClientFunction::onSigNodeState(int state)
{
    IDE_TRACE();
    emit sigNodeState(state);
    //NMT
    switch (state)
    {
    case 1:
        IDE_TRACE_STR(QString("enter [Initialisation] mode"));        
        break;
    case 2:
        IDE_TRACE_STR(QString("enter [PreOperational] mode"));
        break;
    case 3:
        IDE_TRACE_STR(QString("enter [Operational] mode"));
        break;
    case 4:
        IDE_TRACE_STR(QString("enter [Stopped] mode"));
        break;
    default:
        break;
    }
}

void CanOpenClientFunction::onSigAFF_EN(bool isAFF)
{
    IDE_TRACE_INT(isAFF);
    emit sigAFF_EN(isAFF); // backlight control bit
    if (isAFF)
    {
        if (m_gpio)
        {
            m_gpio->onSigBtnPressedLightOn(18);
        }
    }
    else
    {
        if (m_gpio)
        {
            m_gpio->onSigBtnPressedLightOff(18);
        }
    }
}

/*
 * 完成NMT信号发送
 * 完成音频信号发送
 * 完成功能图标和楼层信号发送
 * 完成箭头显示信号发送
 */
void CanOpenClientFunction::onSigFCT_AFF(bool isFCT)
{
    IDE_TRACE_INT(isFCT);
    m_bigText = isFCT; // bigOrsmall text control bit
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"isFCT = "<<m_bigText;
    emit sigFCT_AFF(m_bigText);
}

//完成箭头显示信号发送
void CanOpenClientFunction::onSigArrowAction(bool arrows_FLM, bool arrows_FLD, bool arrows_SCROL)
{
    IDE_DEBUG(QString("%1 %2 %3").arg(Arrows_FLM).arg(arrows_FLD).arg(arrows_SCROL));
    emit sigArrowAction(arrows_FLM, arrows_FLD, arrows_SCROL);
    if (arrows_FLM) // deal with the light FLM and FLD led on or off
    {
        if (m_gpio)
        {
            m_gpio->onSigBtnPressedLightOn(20);
        }
    }
    else
    {
        if (m_gpio)
        {
            m_gpio->onSigBtnPressedLightOff(20);
        }
    }
    if (arrows_FLD)
    {
        if (m_gpio)
        {
            m_gpio->onSigBtnPressedLightOn(19);
        }
    }
    else
    {
        if (m_gpio)
        {
            m_gpio->onSigBtnPressedLightOff(19);
        }
    }
}

// 完成音量大小信号发送
void CanOpenClientFunction::onSigMainVolumeChange(int divGong, int divBip)
{
    IDE_DEBUG(QString("%1 %2").arg(divGong).arg(divBip));
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"isFCT = "<<m_bigText;
    emit sigMainVolumeChange(divGong, divBip);
}

// 完成音频信号发送
void CanOpenClientFunction::onSigMusicVolumeChange(int div)
{
    IDE_TRACE_INT(div);
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"isFCT = "<<m_bigText;
    emit sigMusicVolumeChange(div);
}

//
void CanOpenClientFunction::onSigButtonLightOn(int btIndex, bool isBuzz)
{
    IDE_DEBUG(QString("%1 %2").arg(btIndex).arg(isBuzz));
    if (btIndex >= 1 && btIndex <= 12)
    {
        if (m_gpio)
        {
            m_gpio->onSigBtnPressedLightOn(btIndex - 1);
        }
    }
    else if (btIndex > 12 && btIndex <= 36)
    {
        if (m_extensionbtn)
        {
            m_extensionbtn->setLightOn(btIndex);
        }
    }
}

void CanOpenClientFunction::onSigButtonLightOff(int btIndex)
{
    IDE_TRACE_INT(btIndex);   
    if (btIndex >= 1 && btIndex <= 12)
    {       
        if (m_gpio)
        {
            m_gpio->onSigBtnPressedLightOff(btIndex - 1);
            m_gpio->setLedLightOffFlag(btIndex, 1);
        }
    }
    else if (btIndex > 12 && btIndex <= 36)
    {        
        int pressedFlag = m_extensionbtn->getPressedStatus(btIndex);
        qDebug()<<__PRETTY_FUNCTION__<<"btIndex = "<<btIndex<<"pressedFlag = "<<pressedFlag;
        if (1 == pressedFlag)
        {
            if (m_extensionbtn)
            {
                m_extensionbtn->setCtrBoxLightOffFlag(btIndex, 1);
            }
        }
        else if (0 == pressedFlag)
        {
            if (m_extensionbtn)
            {
                m_extensionbtn->setLightOff(btIndex);
            }
        }
    }
}

void CanOpenClientFunction::onSigBuzz(int index)
{
    IDE_TRACE_INT(index);
    emit sigBuzz(index);
}

void CanOpenClientFunction::onSigBuzz()
{
    if (!m_bigText)
    {
        if (m_Mplayer)
        {
            QString beepMusic = MediaPath + "beep.wav";
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"add beep";
            m_Mplayer->addMedia(AO_BEEP, beepMusic, 1, false, m_beepvol) ;
        }
    }
}

void CanOpenClientFunction::onSigFloorArrive(int left, int center, int right, int soundNumber)
{
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<"left = "<<left<<"center = "<<center << "right = "<<right;
    if ((0 == left || 32 == left) && (0 == center || 32 == center)
        && (0 == right || 32 == right)) // 32 == 空格 0为0 空
    {
        emit sigFloorArrive(0 - 48, 0 - 48, 0 - 48, soundNumber);
        return;
    }

    if (!((right >= 65 && right <= 90)
        || (right >= 43 && right <= 57)
        || right == 92))  // 如果是不合法的数值输入
    {
        right = 0;
    }
    if (!((center >= 65 && center <= 90)
        || (center >= 43 && center <= 57)
        || center == 92)) // 如果是不合法的数值输入
    {
        center = 0;
    }
    if (!((left >= 65 && left <= 90)
        || (left >= 43 && left <= 57)
        || left == 92)) // 如果是不合法的数值输入
    {
        left = 0;
    }

//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<right <<", "<<center<<", "<<left;
    if (0 == left)
    {
        if (0 == center)
        {
            if (0 == right)
            {
                emit sigFloorArrive(0 - 48, 0 - 48, 0 - 48, soundNumber);
            }
            else
            {
                emit sigFloorArrive(right - 48, 0, 0, soundNumber);
            }
        }
        else
        {
            if (0 == right)
            {
                emit sigFloorArrive(center - 48, 0, 0, soundNumber);
            }
            else
            {
                emit sigFloorArrive(right - 48, center - 48, 0, soundNumber);
            }
        }
    }
    else
    {
        if (0 == center)
        {
            if (0 == right)
            {
                emit sigFloorArrive(left - 48, 0, 0, soundNumber);
            }
            else
            {
                emit sigFloorArrive(right - 48, left - 48, 0, soundNumber);
            }
        }
        else
        {
            if (0 == right)
            {
                emit sigFloorArrive(center - 48, left - 48, 0, soundNumber);
            }
            else
            {
                emit sigFloorArrive(right - 48, center - 48, left - 48, soundNumber);
            }
        }
    }
}

// 完成功能图标和楼层信号发送
void CanOpenClientFunction::onSigShowFuncLogo(int funcLogoIndex, int soundNumber)
{
    IDE_DEBUG(QString("%1 %2").arg(funcLogoIndex).arg(soundNumber));
    m_maintenance = funcLogoIndex;
    emit sigShowFuncLogo(funcLogoIndex, soundNumber);
}

void CanOpenClientFunction::onSigShowFloorOrLog(bool isShowLogo)
{
    IDE_TRACE_INT(isShowLogo);
    emit sigShowFloorOrLogo(isShowLogo);
}

void CanOpenClientFunction::onSigMessage(QString m1, QString m2, QString m3, QString m4)
{
    IDE_DEBUG(QString("%1 %2 %3 %4").arg(m1).arg(m2).arg(m3).arg(m4));
    emit sigMessage(m1, m2, m3, m4);
}

void CanOpenClientFunction::onSigRealMessage(QString m1, QString m2)
{
//    IDE_DEBUG(QString("%1 %2").arg(m1).arg(m2));
    emit sigRealMessage(m1, m2);
}

void CanOpenClientFunction::onSigSDO1(int orientation, int contraste, int backlight, int vit_scrol)
{
    IDE_DEBUG(QString("%1 %2 %3 %4").arg(orientation).arg(contraste).arg(backlight).arg(vit_scrol));
    emit sigSDO1(orientation, contraste, backlight, vit_scrol);
}

// 完成横竖显信号发送
void CanOpenClientFunction::onSigOrientation(int orientation)
{
    IDE_TRACE_INT(orientation);
    emit sigOrientation(orientation);
}

void CanOpenClientFunction::onSigContraste(int contraste)
{
    IDE_TRACE_INT(contraste);
    emit sigContraste(contraste);
}

void CanOpenClientFunction::onSigBacklight(int backlight)
{
    IDE_TRACE_INT(backlight);
    emit sigBacklight(backlight);
}

void CanOpenClientFunction::onSigVit_scrol(int vit_scrol)
{
    IDE_TRACE_STR(vit_scrol);
    emit sigVit_scrol(vit_scrol);
}

void CanOpenClientFunction::onSigSDO2(int logo, int periode, int declenchement)
{
    IDE_DEBUG(QString("%1 %2 %3").arg(logo).arg(periode).arg(declenchement));
    emit sigSDO2(logo, periode, declenchement);
}

void CanOpenClientFunction::onDailSwitchChanged(quint8 pNum)
{
    IDE_DEBUG(QString("pNum = %1").arg(pNum));
}

void CanOpenClientFunction::reportButtonPressed(int btIndex)
{
    if (NULL != m_canopenClient)
    {
        if (btIndex <= 36)
        {
            m_canopenClient->reportButtonPressed(btIndex);
        }
        else if (btIndex <= 40)  // 处理四个特殊按钮按下时点亮
        {
            if (m_gpio)
            {
                m_gpio->onSigBtnPressedLightOn(btIndex - 25);
            }
            m_canopenClient->specialButtonPressed(btIndex);
        }
    }
}

void CanOpenClientFunction::reportButtonReleased(int btIndex)
{
    if (NULL != m_canopenClient)
    {
        IDE_TRACE_INT(btIndex);
        if (btIndex <= 36)
        {
            m_canopenClient->reportButtonReleased(btIndex);
            if (btIndex <= 12)
            { // 由于12个板载按钮在控制柜不要求按钮点亮时，只要按下也会点亮，所以需要在松开时熄灭板载按钮，扩展板则不要处理
                if (m_bigText)  //  bigText can not call floor, if calling needs record
                {
                    if (m_gpio)
                    {
                        m_gpio->onSigBtnPressedLightOff(btIndex - 1);
//                         qDebug()<<"is bigtext if call floor no response should be turn off light";
                    }
                }
                else if (40 == m_maintenance)
                {
                    if (m_gpio)
                    {
                        m_gpio->onSigBtnPressedLightOff(btIndex - 1);
//                         qDebug()<<"is m_maintenance if call floor no response should be turn off light";
                    }
                }
            }
            else if (btIndex > 12)
            {
                if (m_extensionbtn)
                {
                    int flag = m_extensionbtn->getCrtBoxLightOffFlag(btIndex);
                    qDebug()<<__PRETTY_FUNCTION__<<"flag = "<<flag<<"btIndex = "<<btIndex;
                    if (1 == flag)
                    {
                        m_extensionbtn->setLightOff(btIndex);
                        m_extensionbtn->setCtrBoxLightOffFlag(btIndex, 0);
                    }
                    else
                    {
                        // do nothing
                    }
                }
            }
        }
        else if (btIndex <= 40) // 4个特殊按钮释放时需要熄灭
        {
            if (m_canopenClient)
            {
                m_canopenClient->specialButtonReleased();
            }
            if (m_gpio)
            {
                m_gpio->onSigBtnPressedLightOff(btIndex - 25);
            }            
        }
    }
}
