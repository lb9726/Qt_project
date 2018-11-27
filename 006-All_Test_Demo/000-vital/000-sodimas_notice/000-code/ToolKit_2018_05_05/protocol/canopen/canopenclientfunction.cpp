#include "canopenclientfunction.h"
#include "define.h"

CanOpenClientFunction::Garbo CanOpenClientFunction::garbo;
CanOpenClientFunction *CanOpenClientFunction::m_pInstance = NULL;

CanOpenClientFunction::CanOpenClientFunction(QObject *parent) : QObject(parent)
{
    IDE_TRACE();
    m_bigText = false;
    m_maintenance = 0;
    init();    
}

CanOpenClientFunction *CanOpenClientFunction::getInstace()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new CanOpenClientFunction();
    }
    return m_pInstance;
}

void CanOpenClientFunction::init()
{
    IDE_TRACE();
    m_canopenProtocolThread = new QThread;
    if (NULL == m_canopenProtocolThread)
    {
        IDE_TRACE_STR(QString("allocate m_canopenThread memeroy is failed"));
        return;
    }
    m_canopenClient = OpenClient::getInstace();
    if (NULL == m_canopenClient)
    {
        IDE_TRACE_STR(QString("allocate m_canopenClient memeroy is failed"));       
    }

    m_gpio = new Gpios();
    if (NULL == m_gpio)
    {
        IDE_TRACE_STR(QString("new Gpios() error!!"));
        return;
    }

//    QThread::msleep(30);
    quint8 nodeId = m_gpio->oneGetDialSwitchValue();
    qDebug()<<"nodeId = "<< nodeId;
//    m_canopenClient->initCanopenClient(61, 250, "0");
    if (nodeId <= 1)
    {
        m_canopenClient->initCanopenClient(nodeId+60, 250, "0");
    }

    if(m_canopenClient)
    {
        m_gpio->setElibLed(true);
    }
    else
    {
        m_gpio->setElibLed(false);
        IDE_TRACE_STR(QString("m_canopenClient is null set can_failed light"));
        return;
    }

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

    m_canopenProtocolThread->start();

    this->moveToThread(&m_canopenFunctionThread);
    m_canopenFunctionThread.start();

    connect(m_gpio, &Gpios::sigBtnPressed, this, &CanOpenClientFunction::reportButtonPressed, Qt::QueuedConnection);
    connect(m_gpio, &Gpios::sigBtnCanceld, this, &CanOpenClientFunction::reportButtonPressed, Qt::QueuedConnection);
    connect(m_gpio, &Gpios::sigSpecialBtnPressed, this, &CanOpenClientFunction::reportButtonPressed, Qt::QueuedConnection);
    connect(m_gpio, &Gpios::sigDailChanged, this, &CanOpenClientFunction::onDailSwitchChanged, Qt::QueuedConnection);
    connect(m_gpio, &Gpios::sigBtnReleased, this, &CanOpenClientFunction::reportButtonReleased, Qt::QueuedConnection);
    if (m_gpio)
    {
        m_gpio->onSigBtnPressedLightOn(18);
    }
    m_gpio->start();

    m_extensionbtn = new ExtensionButtons();
    connect(m_extensionbtn, &ExtensionButtons::sigExtensionBtnPressed, this, &CanOpenClientFunction::reportButtonPressed, Qt::QueuedConnection);
    connect(m_extensionbtn, &ExtensionButtons::sigExtensionBtnCanceled, this, &CanOpenClientFunction::reportButtonPressed, Qt::QueuedConnection);
    connect(m_extensionbtn, &ExtensionButtons::sigExtensionBtnReleased, this, &CanOpenClientFunction::reportButtonReleased, Qt::QueuedConnection);
    if (m_extensionbtn->initExtension())
    {
        IDE_TRACE_STR(QString("will start extensionbtn Thread!"));
        m_extensionbtn->start();
    }
    IDE_TRACE_STR(QString("init will ended!"));
}

//NMT 完成NMT信号发送 true表示为operation 模式 false 表示非operation 模式
void CanOpenClientFunction::onSigNodeState(int state)
{
    IDE_TRACE();
    emit sigNodeState(state);
    //NMT
    switch (state) {
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
    emit sigFCT_AFF(isFCT);
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
    emit sigMainVolumeChange(divGong, divBip);
}

// 完成音频信号发送
void CanOpenClientFunction::onSigMusicVolumeChange(int div)
{
    IDE_TRACE_INT(div);
    emit sigMusicVolumeChange(div);
}

//
void CanOpenClientFunction::onSigButtonLightOn(int btIndex, bool isBuzz)
{
    IDE_DEBUG(QString("%1 %2").arg(btIndex).arg(isBuzz));
    if (btIndex >= 1 && btIndex <= 12)
    {
        m_gpio->onSigBtnPressedLightOn(btIndex - 1);
    }
    else if (btIndex > 12 && btIndex <= 36)
    {
        m_extensionbtn->setLightOn(btIndex);
    }
}

void CanOpenClientFunction::onSigButtonLightOff(int btIndex)
{
    IDE_TRACE_INT(btIndex);   
    if (btIndex >= 1 && btIndex <= 12)
    {       
        m_gpio->onSigBtnPressedLightOff(btIndex - 1);
        m_gpio->setLedLightOffFlag(btIndex, 1);
    }
    else if (btIndex > 12 && btIndex <= 36)
    {        
        int pressedFlag = m_extensionbtn->getPressedStatus(btIndex);
        qDebug()<<__PRETTY_FUNCTION__<<"btIndex = "<<btIndex<<"pressedFlag = "<<pressedFlag;
        if (1 == pressedFlag)
        {
            m_extensionbtn->setCtrBoxLightOffFlag(btIndex, 1);
        }
        else if (0 == pressedFlag)
        {
            m_extensionbtn->setLightOff(btIndex);
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
    IDE_TRACE();
    if (!m_bigText)
    {
        emit sigBuzz();
    }
}

void CanOpenClientFunction::onSigFloorArrive(int left, int center, int right, int soundNumber)
{
    IDE_TRACE_INT(left);
    IDE_TRACE_INT(center);
    IDE_TRACE_INT(right);

    if (0 == right || 0 == center || 0 == left)
    {
        if (0 == right && 0 == center)
        {
            if (0 != left)
            {
                left = left - 48;
            }
            emit sigFloorArrive(left, center, right, soundNumber);  //
        }
        else if (0 == right && 0 != center)
        {
            if (0 != left)
            {
                left = left - 48;
            }
            center = center - 48;
            emit sigFloorArrive(center, left, right, soundNumber);  //
        }
        else if (0 != right && 0 == center)
        {
            if (0 != left)
            {
                left = left - 48;
            }
            right = right - 48;
            emit sigFloorArrive(right, center, left, soundNumber);  //
        }
    }
    else
    {
       left = left - 48;
       center = center - 48;
       right = right - 48;
       emit sigFloorArrive(right, center, left, soundNumber);  //
    }
    IDE_TRACE_INT(left);
    IDE_TRACE_INT(center);
    IDE_TRACE_INT(right);
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
        IDE_TRACE_INT(btIndex);
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
        else if (btIndex <= 40) // 4个特殊按钮释放时需要熄灭
        {
            m_canopenClient->specialButtonReleased();
            if (m_gpio)
            {
                m_gpio->onSigBtnPressedLightOff(btIndex - 25);
            }            
        }
    }
}
