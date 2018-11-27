#include "openclient.h"
#include <QDebug>
#include "../common/define.h"
///在opencallback中引用
///
OpenClient* canOpenCallbackClient;
s_BOARD CopSlaveBoard; // = {"0", "250K"};
UNS8 CopNodeID;
OpenClient::Garbo OpenClient::garbo;
OpenClient *OpenClient::m_pInstance = NULL;
static char sRealMes1[25] = {(UNS8)0x00};
static char sRealMes2[25] = {(UNS8)0x00};

static char sFullMes1[17] = {(UNS8)0x00};
static char sFullMes2[17] = {(UNS8)0x00};
static char sFullMes3[17] = {(UNS8)0x00};
static char sFullMes4[17] = {(UNS8)0x00};

OpenClient::OpenClient(QObject *parent) : QObject(parent)
{
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 24; ++j)
        {
            realtimeMsg[i][j] = (UNS8)0x00;
            preRealtimeMsg[i][j] = (UNS8)0x00;
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 16; ++j)
        {
            fullscrennMsg[i][j] = (UNS8)0x00;
            preFullscrennMsg[i][j] = (UNS8)0x00;
        }
    }

    m_Display_Mode_FCT_AFF = Display_Mode_FCT_AFF;

    m_Sound_VOLUME_VOLUME_BIP = Sound_VOLUME_VOLUME_BIP;
    m_Sound_VOLUME_MUSIQUE = Sound_VOLUME_MUSIQUE;
    m_Display_parameters_BACKLIGHT = (UNS8)0xec;
    m_Display_parameters_ORIENTATION = (UNS8)0x03;
    m_Display_parameters_CONTRASTE = (UNS8)0x08;
    m_Display_parameters_VIT_SCROL = (UNS8)0x08;
    m_Arrows_FLM = Arrows_FLM;
    m_Arrows_FLD = Arrows_FLD;
    m_Arrows_SCROL = Arrows_SCROL;
    m_Sound_BUZ = Sound_BUZ;
    m_Display_Mode_AFF_EN = Display_Mode_AFF_EN;
    m_Level_text_Logo_LOGO = 0;
    m_Panel_output_Level_S_BC0 = Panel_output_Level_S_BC0;
    m_Panel_output_Level_S_BC1 = Panel_output_Level_S_BC1;
    m_Panel_output_Level_S_BC2 = Panel_output_Level_S_BC2;
    m_Panel_output_Level_S_BC3 = Panel_output_Level_S_BC3;
    m_Panel_output_Level_S_BC4 = Panel_output_Level_S_BC4;
    m_Panel_output_Level_S_BC5 = Panel_output_Level_S_BC5;
    m_Panel_output_Level_S_BC6 = Panel_output_Level_S_BC6;
    m_Panel_output_Level_S_BC7 = Panel_output_Level_S_BC7;
    m_Panel_output_Level_S_BC8 = Panel_output_Level_S_BC8;
    m_Panel_output_Level_S_BC9 = Panel_output_Level_S_BC9;
    m_Panel_output_Level_S_BC10 = Panel_output_Level_S_BC10;
    m_Panel_output_Level_S_BC11 = Panel_output_Level_S_BC11;

    m_Extension_output_S_EXT1 = Extension_output_S_EXT1;
    m_Extension_output_S_EXT2 = Extension_output_S_EXT2;
    m_Extension_output_S_EXT3 = Extension_output_S_EXT3;
    m_Extension_output_S_EXT4 = Extension_output_S_EXT4;
    m_Extension_output_S_EXT5 = Extension_output_S_EXT5;
    m_Extension_output_S_EXT6 = Extension_output_S_EXT6;
    m_Extension_output_S_EXT7 = Extension_output_S_EXT7;
    m_Extension_output_S_EXT8 = Extension_output_S_EXT8;
    m_Extension_output_S_EXT9 = Extension_output_S_EXT9;
    m_Extension_output_S_EXT10 = Extension_output_S_EXT10;
    m_Extension_output_S_EXT11 = Extension_output_S_EXT11;
    m_Extension_output_S_EXT12 = Extension_output_S_EXT12;
    m_Extension_output_S_EXT13 = Extension_output_S_EXT13;
    m_Extension_output_S_EXT14 = Extension_output_S_EXT14;
    m_Extension_output_S_EXT15 = Extension_output_S_EXT15;
    m_Extension_output_S_EXT16 = Extension_output_S_EXT16;
    m_Extension_output_S_EXT17 = Extension_output_S_EXT17;
    m_Extension_output_S_EXT18 = Extension_output_S_EXT18;
    m_Extension_output_S_EXT19 = Extension_output_S_EXT19;
    m_Extension_output_S_EXT20 = Extension_output_S_EXT20;
    m_Extension_output_S_EXT21 = Extension_output_S_EXT21;
    m_Extension_output_S_EXT22 = Extension_output_S_EXT22;
    m_Extension_output_S_EXT23 = Extension_output_S_EXT23;
    m_Extension_output_S_EXT24 = Extension_output_S_EXT24;
    m_beepCount = 0;
}

OpenClient *OpenClient::getInstace()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new OpenClient();
    }
    return m_pInstance;
}

void OpenClient::initCanopenClient(int nodeID, int canBaud, QString canPort)
{
    if(nodeID < 1 || nodeID > 61)
    {
        IDE_TRACE();
        return;
    }
    IDE_TRACE_INT(nodeID);
    IDE_TRACE_INT(canBaud);
    CopNodeID = nodeID; //0x3C;
    QByteArray bu;
    bu.append(QString("%1K").arg(canBaud));
    QByteArray busname;
    busname.append(canPort);

    canOpenCallbackClient = this;
    CopSlaveBoard.busname = busname.data(); //socket-can0
    CopSlaveBoard.baudrate = bu.data();
    initCANDevice();
}

void OpenClient::nodeStateChanged(int state)
{
    IDE_TRACE_INT(state);
    emit sigNodeState(state);
}

void OpenClient::rPDOArrived(int pdoIndex)
{
    ///IDE_TRACE_INT(pdoIndex);
    switch (pdoIndex)
    {
    case 1:
        rPDO1Arrived();
        break;
    case 2:
        rPDO2Arrived();
        break;
    case 3:
        rPDO3Arrived();
        break;
    case 4:
        rPDO4Arrived();
        break;
    default:
        break;
    }
}

void OpenClient::sSDOArrived(int sdoIndex, int subindex)
{
    //IDE_TRACE_INT(sdoIndex);
    if(sdoIndex == 1)
    {
        sSDO1Arrived(subindex);
    }
    else if(sdoIndex == 2)
    {
        sSDO2Arrived(subindex);
    }
    else
    {

    }
}

void OpenClient::rPDO1Arrived()
{
    bool isBuzz = (Sound_BUZ == 1 ? true : false);
    if(isBuzz)
    {
        ++m_beepCount;
        if(m_beepCount == 1)
        {
            emit sigBuzz();
        }
        else if(m_beepCount == 3)
        {
            m_beepCount = 0;
        }
    }

    //处理顺序：显示模式，箭头，音量，点灯+Bip
    if(m_Display_Mode_AFF_EN != Display_Mode_AFF_EN) // 背光亮度
    {
        m_Display_Mode_AFF_EN = Display_Mode_AFF_EN;
        emit sigAFF_EN(m_Display_Mode_AFF_EN == 1 ? true:false);
    }

    if(m_Display_Mode_FCT_AFF != Display_Mode_FCT_AFF) // 大小屏
    {
        IDE_TRACE_INT(m_Display_Mode_FCT_AFF);
        m_Display_Mode_FCT_AFF = Display_Mode_FCT_AFF;
        emit sigFCT_AFF(m_Display_Mode_FCT_AFF == 1 ? true:false);
        //buildMessage();
    }

    if((Arrows_FLM != m_Arrows_FLM) || (Arrows_FLD != m_Arrows_FLD) || (Arrows_SCROL != m_Arrows_SCROL))
    {
        m_Arrows_FLM = Arrows_FLM;
        m_Arrows_FLD = Arrows_FLD;
        m_Arrows_SCROL = Arrows_SCROL;
        emit sigArrowAction(Arrows_FLM, Arrows_FLD, Arrows_SCROL);
    }
    //IDE_TRACE_INT(Sound_VOLUME_VOLUME_BIP);
    if(m_Sound_VOLUME_VOLUME_BIP != Sound_VOLUME_VOLUME_BIP)
    {
        m_Sound_VOLUME_VOLUME_BIP = Sound_VOLUME_VOLUME_BIP;
        emit sigMainVolumeChange(m_Sound_VOLUME_VOLUME_BIP&0x0F, ((m_Sound_VOLUME_VOLUME_BIP>>4)&0x0F));
    }
    // IDE_TRACE_INT(Sound_VOLUME_MUSIQUE);
    if(m_Sound_VOLUME_MUSIQUE != Sound_VOLUME_MUSIQUE)
    {
        m_Sound_VOLUME_MUSIQUE = Sound_VOLUME_MUSIQUE;
        emit sigMusicVolumeChange(m_Sound_VOLUME_MUSIQUE);
    }    

    if(m_Panel_output_Level_S_BC0 != Panel_output_Level_S_BC0)
    {
        m_Panel_output_Level_S_BC0 = Panel_output_Level_S_BC0;
        if(m_Panel_output_Level_S_BC0)
        {
            emit sigButtonLightOn(1, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(1);
        }
    }
    if(m_Panel_output_Level_S_BC1 != Panel_output_Level_S_BC1)
    {
        m_Panel_output_Level_S_BC1 = Panel_output_Level_S_BC1;
        if(m_Panel_output_Level_S_BC1)
        {
            emit sigButtonLightOn(2, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(2);
        }
    }
    if(m_Panel_output_Level_S_BC2 != Panel_output_Level_S_BC2)
    {
        m_Panel_output_Level_S_BC2 = Panel_output_Level_S_BC2;
        if(m_Panel_output_Level_S_BC2)
        {
            emit sigButtonLightOn(3, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(3);
        }
    }
    if(m_Panel_output_Level_S_BC3 != Panel_output_Level_S_BC3)
    {
        m_Panel_output_Level_S_BC3 = Panel_output_Level_S_BC3;
        if(m_Panel_output_Level_S_BC3)
        {
            emit sigButtonLightOn(4, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(4);
        }
    }
    if(m_Panel_output_Level_S_BC4 != Panel_output_Level_S_BC4)
    {
        m_Panel_output_Level_S_BC4 = Panel_output_Level_S_BC4;
        if(m_Panel_output_Level_S_BC4)
        {
            emit sigButtonLightOn(5, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(5);
        }
    }
    if(m_Panel_output_Level_S_BC5 != Panel_output_Level_S_BC5)
    {
        m_Panel_output_Level_S_BC5 = Panel_output_Level_S_BC5;
        if(m_Panel_output_Level_S_BC5)
        {
            emit sigButtonLightOn(6, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(6);
        }
    }
    if(m_Panel_output_Level_S_BC6 != Panel_output_Level_S_BC6)
    {
        m_Panel_output_Level_S_BC6 = Panel_output_Level_S_BC6;
        if(m_Panel_output_Level_S_BC6)
        {
            emit sigButtonLightOn(7, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(7);
        }
    }
    if(m_Panel_output_Level_S_BC7 != Panel_output_Level_S_BC7)
    {
        m_Panel_output_Level_S_BC7 = Panel_output_Level_S_BC7;
        if(m_Panel_output_Level_S_BC7)
        {
            emit sigButtonLightOn(8, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(8);
        }
    }
    if(m_Panel_output_Level_S_BC8 != Panel_output_Level_S_BC8)
    {
        m_Panel_output_Level_S_BC8 = Panel_output_Level_S_BC8;
        if(m_Panel_output_Level_S_BC8)
        {
            emit sigButtonLightOn(9, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(9);
        }
    }
    if(m_Panel_output_Level_S_BC9 != Panel_output_Level_S_BC9)
    {
        m_Panel_output_Level_S_BC9 = Panel_output_Level_S_BC9;
        if(m_Panel_output_Level_S_BC9)
        {
            emit sigButtonLightOn(10, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(10);
        }
    }
    if(m_Panel_output_Level_S_BC10 != Panel_output_Level_S_BC10)
    {
        m_Panel_output_Level_S_BC10 = Panel_output_Level_S_BC10;
        if(m_Panel_output_Level_S_BC10)
        {
            emit sigButtonLightOn(11, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(11);
        }

    }
    if(m_Panel_output_Level_S_BC11 != Panel_output_Level_S_BC11)
    {
        m_Panel_output_Level_S_BC11 = Panel_output_Level_S_BC11;
        if(m_Panel_output_Level_S_BC11)
        {
            emit sigButtonLightOn(12, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(12);
        }
    }

    //Extend Board Output
    if(m_Extension_output_S_EXT1 != Extension_output_S_EXT1)
    {
        m_Extension_output_S_EXT1 = Extension_output_S_EXT1;
        if(m_Extension_output_S_EXT1)
        {
            emit sigButtonLightOn(13, isBuzz);
        }
        else
        {
            IDE_TRACE();
            emit sigButtonLightOff(13);
        }
    }
    if(m_Extension_output_S_EXT2 != Extension_output_S_EXT2)
    {
        m_Extension_output_S_EXT2 = Extension_output_S_EXT2;
        if(m_Extension_output_S_EXT2)
        {
            emit sigButtonLightOn(14, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(14);
        }
    }
    if(m_Extension_output_S_EXT3 != Extension_output_S_EXT3)
    {
        m_Extension_output_S_EXT3 = Extension_output_S_EXT3;
        if(m_Extension_output_S_EXT3)
        {
            emit sigButtonLightOn(15, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(15);
        }
    }
    if(m_Extension_output_S_EXT4 != Extension_output_S_EXT4)
    {
        m_Extension_output_S_EXT4 = Extension_output_S_EXT4;
        if(m_Extension_output_S_EXT4)
        {
            emit sigButtonLightOn(16, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(16);
        }
    }
    if(m_Extension_output_S_EXT5 != Extension_output_S_EXT5)
    {
        m_Extension_output_S_EXT5 = Extension_output_S_EXT5;
        if(m_Extension_output_S_EXT5)
        {
            emit sigButtonLightOn(17, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(17);
        }
    }
    if(m_Extension_output_S_EXT6 != Extension_output_S_EXT6)
    {
        m_Extension_output_S_EXT6 = Extension_output_S_EXT6;
        if(m_Extension_output_S_EXT6)
        {
            emit sigButtonLightOn(18, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(18);
        }
    }
    if(m_Extension_output_S_EXT7 != Extension_output_S_EXT7)
    {
        m_Extension_output_S_EXT7 = Extension_output_S_EXT7;
        if(m_Extension_output_S_EXT7)
        {
            emit sigButtonLightOn(19, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(19);
        }
    }
    if(m_Extension_output_S_EXT8 != Extension_output_S_EXT8)
    {
        m_Extension_output_S_EXT8 = Extension_output_S_EXT8;
        if(m_Extension_output_S_EXT8)
        {
            emit sigButtonLightOn(20, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(20);
        }
    }
    if(m_Extension_output_S_EXT9 != Extension_output_S_EXT9)
    {
        m_Extension_output_S_EXT9 = Extension_output_S_EXT9;
        if(m_Extension_output_S_EXT9)
        {
            emit sigButtonLightOn(21, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(21);
        }
    }
    if(m_Extension_output_S_EXT10 != Extension_output_S_EXT10)
    {
        m_Extension_output_S_EXT10 = Extension_output_S_EXT10;
        if(m_Extension_output_S_EXT10)
        {
            emit sigButtonLightOn(22, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(22);
        }
    }
    if(m_Extension_output_S_EXT11 != Extension_output_S_EXT11)
    {
        m_Extension_output_S_EXT11 = Extension_output_S_EXT11;
        if(m_Extension_output_S_EXT11)
        {
            emit sigButtonLightOn(23, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(23);
        }
    }
    if(m_Extension_output_S_EXT12 != Extension_output_S_EXT12)
    {
        m_Extension_output_S_EXT12 = Extension_output_S_EXT12;
        if(m_Extension_output_S_EXT12)
        {
            emit sigButtonLightOn(24, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(24);
        }
    }
    if(m_Extension_output_S_EXT13 != Extension_output_S_EXT13)
    {
        m_Extension_output_S_EXT13 = Extension_output_S_EXT13;
        if(m_Extension_output_S_EXT13)
        {
            emit sigButtonLightOn(25, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(25);
        }
    }
    if(m_Extension_output_S_EXT14 != Extension_output_S_EXT14)
    {
        m_Extension_output_S_EXT14 = Extension_output_S_EXT14;
        if(m_Extension_output_S_EXT14)
        {
            emit sigButtonLightOn(26, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(26);
        }
    }
    if(m_Extension_output_S_EXT15 != Extension_output_S_EXT15)
    {
        m_Extension_output_S_EXT15 = Extension_output_S_EXT15;
        if(m_Extension_output_S_EXT15)
        {
            emit sigButtonLightOn(27, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(27);
        }
    }
    if(m_Extension_output_S_EXT16 != Extension_output_S_EXT16)
    {
        m_Extension_output_S_EXT16 = Extension_output_S_EXT16;
        if(m_Extension_output_S_EXT16)
        {
            emit sigButtonLightOn(28, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(28);
        }
    }
    if(m_Extension_output_S_EXT17 != Extension_output_S_EXT17)
    {
        m_Extension_output_S_EXT17 = Extension_output_S_EXT17;
        if(m_Extension_output_S_EXT17)
        {
            emit sigButtonLightOn(29, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(29);
        }
    }
    if(m_Extension_output_S_EXT18 != Extension_output_S_EXT18)
    {
        m_Extension_output_S_EXT18 = Extension_output_S_EXT18;
        if(m_Extension_output_S_EXT18)
        {
            emit sigButtonLightOn(30, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(30);
        }
    }
    if(m_Extension_output_S_EXT19 != Extension_output_S_EXT19)
    {
        m_Extension_output_S_EXT19 = Extension_output_S_EXT19;
        if(m_Extension_output_S_EXT19)
        {
            emit sigButtonLightOn(31, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(31);
        }
    }
    if(m_Extension_output_S_EXT20 != Extension_output_S_EXT20)
    {
        m_Extension_output_S_EXT20 = Extension_output_S_EXT20;
        if(m_Extension_output_S_EXT20)
        {
            emit sigButtonLightOn(32, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(32);
        }
    }
    if(m_Extension_output_S_EXT21 != Extension_output_S_EXT21)
    {
        m_Extension_output_S_EXT21 = Extension_output_S_EXT21;
        if(m_Extension_output_S_EXT21)
        {
            emit sigButtonLightOn(33, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(33);
        }
    }
    if(m_Extension_output_S_EXT22 != Extension_output_S_EXT22)
    {
        m_Extension_output_S_EXT22 = Extension_output_S_EXT22;
        if(m_Extension_output_S_EXT22)
        {
            emit sigButtonLightOn(34, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(34);
        }
    }
    if(m_Extension_output_S_EXT23 != Extension_output_S_EXT23)
    {
        m_Extension_output_S_EXT23 = Extension_output_S_EXT23;
        if(m_Extension_output_S_EXT23)
        {
            emit sigButtonLightOn(35, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(35);
        }
    }
    if(m_Extension_output_S_EXT24 != Extension_output_S_EXT24)
    {
        m_Extension_output_S_EXT24 = Extension_output_S_EXT24;
        if(m_Extension_output_S_EXT24)
        {
            emit sigButtonLightOn(36, isBuzz);
        }
        else
        {
            emit sigButtonLightOff(36);
        }
    }
}

void OpenClient::rPDO2Arrived()
{
    if(m_Level_text_Logo_LOGO != Level_text_Logo_LOGO)
    {
        m_Level_text_Logo_LOGO = Level_text_Logo_LOGO;
        emit sigShowFloorOrLogo(m_Level_text_Logo_LOGO);
    }

    if(m_Level_text_Logo_LOGO)
    {
        m_Level_text_Logo_NIV_ALPHA_1 = Level_text_Logo_NIV_ALPHA_1;
        m_Sound_NR_SON = Sound_NR_SON;
        emit sigShowFuncLogo(m_Level_text_Logo_NIV_ALPHA_1, m_Sound_NR_SON);
    }
    else
    {
        if(!((m_Level_text_Logo_NIV_ALPHA_1 == Level_text_Logo_NIV_ALPHA_1)
             && (m_Level_text_Logo_NIV_ALPHA_2 == Level_text_Logo_NIV_ALPHA_2)
             && (m_Level_text_Logo_NIV_ALPHA_3 == Level_text_Logo_NIV_ALPHA_3)
             && (m_Sound_NR_SON == Sound_NR_SON)))
        {
            m_Sound_NR_SON = Sound_NR_SON;
            m_Level_text_Logo_NIV_ALPHA_1 = Level_text_Logo_NIV_ALPHA_1;
            m_Level_text_Logo_NIV_ALPHA_2 = Level_text_Logo_NIV_ALPHA_2;
            m_Level_text_Logo_NIV_ALPHA_3 = Level_text_Logo_NIV_ALPHA_3;

            emit sigFloorArrive(m_Level_text_Logo_NIV_ALPHA_1, m_Level_text_Logo_NIV_ALPHA_2, m_Level_text_Logo_NIV_ALPHA_3, m_Sound_NR_SON);
        }
    }
}

void OpenClient::linkCharacterForRpdo3(int curLine, int curClume)
{
    if (0 == m_Display_Mode_FCT_AFF) // small text
    {
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 24; ++j)
            {
                preRealtimeMsg[i][j] = realtimeMsg[i][j];
            }
        }
        if (curLine > 1 || curClume > 24)
        {
            return;
        }
        if (curClume <= 18)
        {
            realtimeMsg[curLine][curClume] = text_segmented_1_MSG1_CHAR1;
            realtimeMsg[curLine][curClume+1] = text_segmented_1_MSG1_CHAR2;
            realtimeMsg[curLine][curClume+2] = text_segmented_1_MSG1_CHAR3;
            realtimeMsg[curLine][curClume+3] = text_segmented_1_MSG1_CHAR4;
            realtimeMsg[curLine][curClume+4] = text_segmented_1_MSG1_CHAR5;
            realtimeMsg[curLine][curClume+5] = text_segmented_1_MSG1_CHAR6;
        }
        else if (19 == curClume)
        {
            realtimeMsg[curLine][curClume] = text_segmented_1_MSG1_CHAR1;
            realtimeMsg[curLine][curClume+1] = text_segmented_1_MSG1_CHAR2;
            realtimeMsg[curLine][curClume+2] = text_segmented_1_MSG1_CHAR3;
            realtimeMsg[curLine][curClume+3] = text_segmented_1_MSG1_CHAR4;
            realtimeMsg[curLine][curClume+4] = text_segmented_1_MSG1_CHAR5;
        }
        else if (20 == curClume)
        {
            realtimeMsg[curLine][curClume] = text_segmented_1_MSG1_CHAR1;
            realtimeMsg[curLine][curClume+1] = text_segmented_1_MSG1_CHAR2;
            realtimeMsg[curLine][curClume+2] = text_segmented_1_MSG1_CHAR3;
            realtimeMsg[curLine][curClume+3] = text_segmented_1_MSG1_CHAR4;
        }
        else if (21 == curClume)
        {
            realtimeMsg[curLine][curClume] = text_segmented_1_MSG1_CHAR1;
            realtimeMsg[curLine][curClume+1] = text_segmented_1_MSG1_CHAR2;
            realtimeMsg[curLine][curClume+2] = text_segmented_1_MSG1_CHAR3;
        }
        else if (22 == curClume)
        {
            realtimeMsg[curLine][curClume] = text_segmented_1_MSG1_CHAR1;
            realtimeMsg[curLine][curClume+1] = text_segmented_1_MSG1_CHAR2;
        }
        else if (23 == curClume)
        {
            realtimeMsg[curLine][curClume] = text_segmented_1_MSG1_CHAR1;
        }
        judgeSmallOrBigTextChang();
    }
    else  // big text
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 16; ++j)
            {
                preFullscrennMsg[i][j] = fullscrennMsg[i][j];
            }
        }
        if (curLine > 3 || curClume > 15)
        {
            return;
        }
        if (curClume <= 10)
        {
            fullscrennMsg[curLine][curClume] = text_segmented_1_MSG1_CHAR1;
            fullscrennMsg[curLine][curClume+1] = text_segmented_1_MSG1_CHAR2;
            fullscrennMsg[curLine][curClume+2] = text_segmented_1_MSG1_CHAR3;
            fullscrennMsg[curLine][curClume+3] = text_segmented_1_MSG1_CHAR4;
            fullscrennMsg[curLine][curClume+4] = text_segmented_1_MSG1_CHAR5;
            fullscrennMsg[curLine][curClume+5] = text_segmented_1_MSG1_CHAR6;
        }
        else if (11 == curClume)
        {
            fullscrennMsg[curLine][curClume] = text_segmented_1_MSG1_CHAR1;
            fullscrennMsg[curLine][curClume+1] = text_segmented_1_MSG1_CHAR2;
            fullscrennMsg[curLine][curClume+2] = text_segmented_1_MSG1_CHAR3;
            fullscrennMsg[curLine][curClume+3] = text_segmented_1_MSG1_CHAR4;
            fullscrennMsg[curLine][curClume+4] = text_segmented_1_MSG1_CHAR5;
        }
        else if (12 == curClume)
        {
            fullscrennMsg[curLine][curClume] = text_segmented_1_MSG1_CHAR1;
            fullscrennMsg[curLine][curClume+1] = text_segmented_1_MSG1_CHAR2;
            fullscrennMsg[curLine][curClume+2] = text_segmented_1_MSG1_CHAR3;
            fullscrennMsg[curLine][curClume+3] = text_segmented_1_MSG1_CHAR4;
        }
        else if (13 == curClume)
        {
            fullscrennMsg[curLine][curClume] = text_segmented_1_MSG1_CHAR1;
            fullscrennMsg[curLine][curClume+1] = text_segmented_1_MSG1_CHAR2;
            fullscrennMsg[curLine][curClume+2] = text_segmented_1_MSG1_CHAR3;
        }
        else if (14 == curClume)
        {
            fullscrennMsg[curLine][curClume] = text_segmented_1_MSG1_CHAR1;
            fullscrennMsg[curLine][curClume+1] = text_segmented_1_MSG1_CHAR2;
        }
        else if (15 == curClume)
        {
            fullscrennMsg[curLine][curClume] = text_segmented_1_MSG1_CHAR1;
        }
        judgeSmallOrBigTextChang();
    }
}

void OpenClient::linkCharacterForRpdo4(int curLine, int curClume)
{
    if (curClume < 0 || curLine < 0)
    {
        return;
    }
    if (0 == m_Display_Mode_FCT_AFF)
    {
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 24; ++j)
            {
                preRealtimeMsg[i][j] = realtimeMsg[i][j];
            }
        }
        if (curLine > 1 || curClume > 24)
        {
            return;
        }
        if (curClume <= 18)
        {
            realtimeMsg[curLine][curClume] = texte_segmented_2_MSG2_CHAR1;
            realtimeMsg[curLine][curClume+1] = texte_segmented_2_MSG2_CHAR2;
            realtimeMsg[curLine][curClume+2] = texte_segmented_2_MSG2_CHAR3;
            realtimeMsg[curLine][curClume+3] = texte_segmented_2_MSG2_CHAR4;
            realtimeMsg[curLine][curClume+4] = texte_segmented_2_MSG2_CHAR5;
            realtimeMsg[curLine][curClume+5] = texte_segmented_2_MSG2_CHAR6;
        }
        else if (19 == curClume)
        {
            realtimeMsg[curLine][curClume] = texte_segmented_2_MSG2_CHAR1;
            realtimeMsg[curLine][curClume+1] = texte_segmented_2_MSG2_CHAR2;
            realtimeMsg[curLine][curClume+2] = texte_segmented_2_MSG2_CHAR3;
            realtimeMsg[curLine][curClume+3] = texte_segmented_2_MSG2_CHAR4;
            realtimeMsg[curLine][curClume+4] = texte_segmented_2_MSG2_CHAR5;
        }
        else if (20 == curClume)
        {
            realtimeMsg[curLine][curClume] = texte_segmented_2_MSG2_CHAR1;
            realtimeMsg[curLine][curClume+1] = texte_segmented_2_MSG2_CHAR2;
            realtimeMsg[curLine][curClume+2] = texte_segmented_2_MSG2_CHAR3;
            realtimeMsg[curLine][curClume+3] = texte_segmented_2_MSG2_CHAR4;
        }
        else if (21 == curClume)
        {
            realtimeMsg[curLine][curClume] = texte_segmented_2_MSG2_CHAR1;
            realtimeMsg[curLine][curClume+1] = texte_segmented_2_MSG2_CHAR2;
            realtimeMsg[curLine][curClume+2] = texte_segmented_2_MSG2_CHAR3;
        }
        else if (22 == curClume)
        {
            realtimeMsg[curLine][curClume] = texte_segmented_2_MSG2_CHAR1;
            realtimeMsg[curLine][curClume+1] = texte_segmented_2_MSG2_CHAR2;
        }
        else if (23 == curClume)
        {
            realtimeMsg[curLine][curClume] = texte_segmented_2_MSG2_CHAR1;
        }
        judgeSmallOrBigTextChang();
    }
    else
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 16; ++j)
            {
                preFullscrennMsg[i][j] = fullscrennMsg[i][j];
            }
        }
        if (curLine > 3 || curClume > 15)
        {
            return;
        }
        if (curClume <= 10)
        {
            fullscrennMsg[curLine][curClume] = texte_segmented_2_MSG2_CHAR1;
            fullscrennMsg[curLine][curClume+1] = texte_segmented_2_MSG2_CHAR2;
            fullscrennMsg[curLine][curClume+2] = texte_segmented_2_MSG2_CHAR3;
            fullscrennMsg[curLine][curClume+3] = texte_segmented_2_MSG2_CHAR4;
            fullscrennMsg[curLine][curClume+4] = texte_segmented_2_MSG2_CHAR5;
            fullscrennMsg[curLine][curClume+5] = texte_segmented_2_MSG2_CHAR6;
        }
        else if (11 == curClume)
        {
            fullscrennMsg[curLine][curClume] = texte_segmented_2_MSG2_CHAR1;
            fullscrennMsg[curLine][curClume+1] = texte_segmented_2_MSG2_CHAR2;
            fullscrennMsg[curLine][curClume+2] = texte_segmented_2_MSG2_CHAR3;
            fullscrennMsg[curLine][curClume+3] = texte_segmented_2_MSG2_CHAR4;
            fullscrennMsg[curLine][curClume+4] = texte_segmented_2_MSG2_CHAR5;
        }
        else if (12 == curClume)
        {
            fullscrennMsg[curLine][curClume] = texte_segmented_2_MSG2_CHAR1;
            fullscrennMsg[curLine][curClume+1] = texte_segmented_2_MSG2_CHAR2;
            fullscrennMsg[curLine][curClume+2] = texte_segmented_2_MSG2_CHAR3;
            fullscrennMsg[curLine][curClume+3] = texte_segmented_2_MSG2_CHAR4;
        }
        else if (13 == curClume)
        {
            fullscrennMsg[curLine][curClume] = texte_segmented_2_MSG2_CHAR1;
            fullscrennMsg[curLine][curClume+1] = texte_segmented_2_MSG2_CHAR2;
            fullscrennMsg[curLine][curClume+2] = texte_segmented_2_MSG2_CHAR3;
        }
        else if (14 == curClume)
        {
            fullscrennMsg[curLine][curClume] = texte_segmented_2_MSG2_CHAR1;
            fullscrennMsg[curLine][curClume+1] = texte_segmented_2_MSG2_CHAR2;
        }
        else if (15 == curClume)
        {
            fullscrennMsg[curLine][curClume] = texte_segmented_2_MSG2_CHAR1;
        }
        judgeSmallOrBigTextChang();
    }
}

void OpenClient::buildMessageAndEmitSignal(bool pflag)
{
    QString ba1;
    QString ba2;

    if (0 == m_Display_Mode_FCT_AFF)
    {
        memset(sRealMes1, 0, 25);
        memset(sRealMes2, 0, 25);
        memcpy(sRealMes1, &realtimeMsg[0][0], 24);
        ba1.append(sRealMes1);
        memcpy(sRealMes2, &realtimeMsg[1][0], 24);
        ba2.append(sRealMes2);
        if (pflag)
        {
            emit sigRealMessage(ba1, ba2);
        }
        resetAfterEmptyCharacter(m_Display_Mode_FCT_AFF);
    }
    else
    {
        QString ba3;
        QString ba4;
        memset(sFullMes1, 0, 17);
        memset(sFullMes2, 0, 17);
        memset(sFullMes3, 0, 17);
        memset(sFullMes4, 0, 17);
        memcpy(sFullMes1, &fullscrennMsg[0][0], 16);
        ba1.append(sFullMes1);
        memcpy(sFullMes2, &fullscrennMsg[1][0], 16);
        ba2.append(sFullMes2);
        memcpy(sFullMes3, &fullscrennMsg[2][0], 16);
        ba3.append(sFullMes3);
        memcpy(sFullMes4, &fullscrennMsg[3][0], 16);
        ba4.append(sFullMes4);
        if (pflag)
        {
            emit sigMessage(ba1, ba2, ba3, ba4);
        }
        resetAfterEmptyCharacter(m_Display_Mode_FCT_AFF);
    }
}

void OpenClient::rPDO3Arrived()
{
    UNS8 curLine = text_segmented_1_MSG1_LIGNE;     // 哪一行
    UNS8 curClume = text_segmented_1_MSG1_COLONNE;  // 哪一列
    if (0 == m_Display_Mode_FCT_AFF)    // 小屏文本
    {
        if (curLine > 1 || curClume > 23) // 如果小屏文本超过2行或者列数超过24列，不做处理
        {
            return;
        }
        else
        {
            linkCharacterForRpdo3(curLine, curClume);
        }
    }
    else  // 大屏文本
    {
        if (curLine > 3 || curClume > 15)  // 如果大屏文本超过4行或者列数超过16列，不做处理
        {
            return;
        }
        else
        {
            linkCharacterForRpdo3(curLine, curClume);
        }
    }
}

void OpenClient::rPDO4Arrived()
{
    UNS8 curLine = texte_segmented_2_MSG2_LIGNE;     // 哪一行
    UNS8 curClume = texte_segmented_2_MSG2_COLONNE;  // 哪一列
    if (0 == m_Display_Mode_FCT_AFF)    // 小屏文本
    {
        if (curLine > 1 || curClume > 23) // 如果小屏文本超过2行或者列数超过24列，不做处理
        {
            return;
        }
        else
        {
            linkCharacterForRpdo4(curLine, curClume);
        }
    }
    else  // 大屏文本
    {
        if (curLine > 3 || curClume > 15)  // 如果大屏文本超过4行或者列数超过16列，不做处理
        {
            return;
        }
        else
        {
            linkCharacterForRpdo4(curLine, curClume);
        }
    }
}

void OpenClient::sSDO1Arrived(int subIndex)
{
    switch (subIndex)
    {
    case 0x01:
        if(m_Display_parameters_ORIENTATION != Display_parameters_ORIENTATION)
        {
            m_Display_parameters_ORIENTATION = Display_parameters_ORIENTATION;
            emit sigOrientation(m_Display_parameters_ORIENTATION);
        }
        break;
    case 0x02:
        if(m_Display_parameters_CONTRASTE != Display_parameters_CONTRASTE)
        {
            m_Display_parameters_CONTRASTE = Display_parameters_CONTRASTE;
            emit sigContraste(m_Display_parameters_CONTRASTE);
        }
        break;
    case 0x03:
        if(m_Display_parameters_BACKLIGHT != Display_parameters_BACKLIGHT)
        {
            m_Display_parameters_BACKLIGHT = Display_parameters_BACKLIGHT;
            emit sigBacklight(m_Display_parameters_BACKLIGHT);
        }
        break;
    case 0x04:
        if(m_Display_parameters_VIT_SCROL != Display_parameters_VIT_SCROL)
        {
            m_Display_parameters_VIT_SCROL = Display_parameters_VIT_SCROL;
            emit sigVit_scrol(m_Display_parameters_VIT_SCROL);
        }
        break;
    default:
        break;
    }
    ///SDO1
    ///emit sigSDO1(Display_parameters_ORIENTATION, Display_parameters_CONTRASTE, Display_parameters_BACKLIGHT, Display_parameters_VIT_SCROL);
}

void OpenClient::sSDO2Arrived(int subIndex)
{
    switch (subIndex)
    {
    case 0x01:
        if(m_Customer_Logo_LOGO_CLIENT != Customer_Logo_LOGO_CLIENT)
        {
            m_Customer_Logo_LOGO_CLIENT = Customer_Logo_LOGO_CLIENT;
            ///TO-DO
        }
        break;
    case 0x02:
        if(m_Customer_Logo_LOGO_PERIODE != Customer_Logo_LOGO_PERIODE)
        {
            m_Customer_Logo_LOGO_PERIODE = Customer_Logo_LOGO_PERIODE;
        }
        break;
    case 0x03:
        if(m_Customer_Logo_LOGO_DECLENCHEMENT != Customer_Logo_LOGO_DECLENCHEMENT)
        {
            m_Customer_Logo_LOGO_DECLENCHEMENT = Customer_Logo_LOGO_DECLENCHEMENT;
        }
        break;
    default:
        break;
    }
    emit sigSDO2(m_Customer_Logo_LOGO_CLIENT, m_Customer_Logo_LOGO_PERIODE, m_Customer_Logo_LOGO_DECLENCHEMENT);
}

void OpenClient::reportButtonPressed(int btIndex)
{
    //IDE_TRACE_INT(btIndex);
    buildButtonPressed(btIndex);
    sendPDOevent(&display_Data);
}

void OpenClient::reportButtonReleased(int btIndex)
{
    //IDE_TRACE_INT(btIndex);
    buildButtonReleased(btIndex);
    sendPDOevent(&display_Data);
}

void OpenClient::specialButtonPressed(int btIndex)
{
    Panel_input_Other_E_LIB1  =   (btIndex == 37 ? 1:0);
    Panel_input_Other_E_REOUA =   (btIndex == 38 ? 1:0);
    Panel_input_Other_E_BFR   =   (btIndex == 39 ? 1:0);
    Panel_input_Other_E_PCA   =   (btIndex == 40 ? 1:0);
    sendPDOevent(&display_Data);
}

void OpenClient::specialButtonReleased()
{
    Panel_input_Other_E_LIB1  =   0;
    Panel_input_Other_E_REOUA =   0;
    Panel_input_Other_E_BFR   =   0;
    Panel_input_Other_E_PCA   =   0;
    sendPDOevent(&display_Data);
}

void OpenClient::buildButtonPressed(int btIndex)
{
    Panel_input_Level_E_BC0 = (btIndex == 1 ?1:0);
    Panel_input_Level_E_BC1 = (btIndex == 2 ?1:0);
    Panel_input_Level_E_BC2 = (btIndex == 3 ?1:0);
    Panel_input_Level_E_BC3 = (btIndex == 4 ?1:0);
    Panel_input_Level_E_BC4 = (btIndex == 5 ?1:0);
    Panel_input_Level_E_BC5 = (btIndex == 6 ?1:0);
    Panel_input_Level_E_BC6 = (btIndex == 7 ?1:0);
    Panel_input_Level_E_BC7 = (btIndex == 8 ?1:0);
    Panel_input_Level_E_BC8 = (btIndex == 9 ?1:0);
    Panel_input_Level_E_BC9 = (btIndex == 10 ?1:0);
    Panel_input_Level_E_BC10 = (btIndex == 11 ?1:0);
    Panel_input_Level_E_BC11 = (btIndex == 12 ?1:0);

    ///3 Extend Board Var
    if(btIndex >= 13 && btIndex <= 20)
    {
        Display_state_EXT1 = 1;
    }
    else
    {
        Display_state_EXT1 = 0;
    }

    if(btIndex >= 21 && btIndex <= 28)
    {
        Display_state_EXT2 = 1;
    }
    else
    {
        Display_state_EXT2 = 0;
    }

    if(btIndex >= 29 && btIndex <= 36)
    {
        Display_state_EXT3 = 1;
    }
    else
    {
        Display_state_EXT3 = 0;
    }

    ///24 Extend Board Input Var
    Extension_input_E_EXT1 = (btIndex == 13 ?1:0);
    Extension_input_E_EXT2 = (btIndex == 14 ?1:0);
    Extension_input_E_EXT3 = (btIndex == 15 ?1:0);
    Extension_input_E_EXT4 = (btIndex == 16 ?1:0);
    Extension_input_E_EXT5 = (btIndex == 17 ?1:0);
    Extension_input_E_EXT6 = (btIndex == 18 ?1:0);
    Extension_input_E_EXT7 = (btIndex == 19 ?1:0);
    Extension_input_E_EXT8 = (btIndex == 20 ?1:0);

    Extension_input_E_EXT9 = (btIndex == 21 ?1:0);
    Extension_input_E_EXT10 = (btIndex == 22 ?1:0);
    Extension_input_E_EXT11 = (btIndex == 23 ?1:0);
    Extension_input_E_EXT12 = (btIndex == 24 ?1:0);
    Extension_input_E_EXT13 = (btIndex == 25 ?1:0);
    Extension_input_E_EXT14 = (btIndex == 26 ?1:0);
    Extension_input_E_EXT15 = (btIndex == 27 ?1:0);
    Extension_input_E_EXT16 = (btIndex == 28 ?1:0);

    Extension_input_E_EXT17 = (btIndex == 29 ?1:0);
    Extension_input_E_EXT18 = (btIndex == 30 ?1:0);
    Extension_input_E_EXT19 = (btIndex == 31 ?1:0);
    Extension_input_E_EXT20 = (btIndex == 32 ?1:0);
    Extension_input_E_EXT21 = (btIndex == 33 ?1:0);
    Extension_input_E_EXT22 = (btIndex == 34 ?1:0);
    Extension_input_E_EXT23 = (btIndex == 35 ?1:0);
    Extension_input_E_EXT24 = (btIndex == 36 ?1:0);
}

void OpenClient::buildButtonReleased(int btIndex)
{
    Panel_input_Level_E_BC0 = 0; //(btIndex == 1 ?0:1);
    Panel_input_Level_E_BC1 = 0; //(btIndex == 2 ?0:1);
    Panel_input_Level_E_BC2 = 0; //(btIndex == 3 ?0:1);
    Panel_input_Level_E_BC3 = 0; //(btIndex == 4 ?0:1);
    Panel_input_Level_E_BC4 = 0; //(btIndex == 5 ?0:1);
    Panel_input_Level_E_BC5 = 0; //(btIndex == 6 ?0:1);
    Panel_input_Level_E_BC6 = 0; //(btIndex == 7 ?0:1);
    Panel_input_Level_E_BC7 = 0; //(btIndex == 8 ?0:1);
    Panel_input_Level_E_BC8 = 0; //(btIndex == 9 ?0:1);
    Panel_input_Level_E_BC9 = 0; //(btIndex == 10 ?0:1);
    Panel_input_Level_E_BC10 = 0; //(btIndex == 11 ?0:1);
    Panel_input_Level_E_BC11 = 0; //(btIndex == 12 ?0:1);

    ///3 Extend Board Var
    if(btIndex >= 13 && btIndex <= 20)
    {
        Display_state_EXT1 = 1;
    }
    else
    {
        Display_state_EXT1 = 0;
    }

    if(btIndex >= 21 && btIndex <= 28)
    {
        Display_state_EXT2 = 1;
    }
    else
    {
        Display_state_EXT2 = 0;
    }

    if(btIndex >= 29 && btIndex <= 36)
    {
        Display_state_EXT3 = 1;
    }
    else
    {
        Display_state_EXT3 = 0;
    }

    ///24 Extend Board Input Var
    Extension_input_E_EXT1 = 0; //(btIndex == 13 ?0:1);
    Extension_input_E_EXT2 = 0; //(btIndex == 14 ?0:1);
    Extension_input_E_EXT3 = 0; //(btIndex == 15 ?0:1);
    Extension_input_E_EXT4 = 0; //(btIndex == 16 ?0:1);
    Extension_input_E_EXT5 = 0; //(btIndex == 17 ?0:1);
    Extension_input_E_EXT6 = 0; //(btIndex == 18 ?0:1);
    Extension_input_E_EXT7 = 0; //(btIndex == 19 ?0:1);
    Extension_input_E_EXT8 = 0; //(btIndex == 20 ?0:1);

    Extension_input_E_EXT9 = 0; //(btIndex == 21 ?0:1);
    Extension_input_E_EXT10 = 0; //(btIndex == 22 ?0:1);
    Extension_input_E_EXT11 = 0; //(btIndex == 23 ?0:1);
    Extension_input_E_EXT12 = 0; //(btIndex == 24 ?0:1);
    Extension_input_E_EXT13 = 0; //(btIndex == 25 ?0:1);
    Extension_input_E_EXT14 = 0; //(btIndex == 26 ?0:1);
    Extension_input_E_EXT15 = 0; //(btIndex == 27 ?0:1);
    Extension_input_E_EXT16 = 0; //(btIndex == 28 ?0:1);

    Extension_input_E_EXT17 = 0; //(btIndex == 29 ?0:1);
    Extension_input_E_EXT18 = 0; //(btIndex == 30 ?0:1);
    Extension_input_E_EXT19 = 0; //(btIndex == 31 ?0:1);
    Extension_input_E_EXT20 = 0; //(btIndex == 32 ?0:1);
    Extension_input_E_EXT21 = 0; //(btIndex == 33 ?0:1);
    Extension_input_E_EXT22 = 0; //(btIndex == 34 ?0:1);
    Extension_input_E_EXT23 = 0; //(btIndex == 35 ?0:1);
    Extension_input_E_EXT24 = 0; //(btIndex == 36 ?0:1);
}

void OpenClient::setAllTextIsEmpty()
{
    for (int row = 0; row < 2; ++row)
    {
        for (int column = 0; column < 24; ++column)
        {
            realtimeMsg[row][column] = 0x00;
        }
    }

    for (int row1 = 0; row1 < 4; ++row1)
    {
        for (int column1 = 0; column1 < 16; ++column1)
        {
            fullscrennMsg[row1][column1] = 0x00;
        }
    }
}

void OpenClient::judgeSmallOrBigTextChang()
{
    if (0 == m_Display_Mode_FCT_AFF)
    {
        bool tmpSmallTextChanged = false;
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 24; ++j)
            {
                if (preRealtimeMsg[i][j] != realtimeMsg[i][j])
                {
                    tmpSmallTextChanged = true;
                    break;
                }
            }
        }
        if (tmpSmallTextChanged)
        {
            buildMessageAndEmitSignal(true);
        }
    }
    else
    {
        bool tmpBigTextChanged = false;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 16; ++j)
            {
                if (preFullscrennMsg[i][j] != fullscrennMsg[i][j])
                {
                    tmpBigTextChanged = true;
                    break;
                }
            }
        }
        if (tmpBigTextChanged)
        {
            buildMessageAndEmitSignal(true);
        }
    }
}

void OpenClient::resetAfterEmptyCharacter(int isSmallText)
{
    if (0 == isSmallText)
    {
        for (int row = 0; row < 2; ++row)
        {
            for (int column = 0; column < 24; ++column)
            {
                if (0x00 == realtimeMsg[row][column])
                {
                    for (int index = column; index < 24; ++index)
                    {
                        realtimeMsg[row][index] = 0x00;
                    }
                    break;
                }
            }
        }
    }
    else
    {
        for (int row = 0; row < 4; ++row)
        {
            for (int column = 0; column < 16; ++column)
            {
                if (0x00 == fullscrennMsg[row][column])
                {
                    for (int index = column; index < 16; ++index)
                    {
                        fullscrennMsg[row][index] = 0x00;
                    }
                    break;
                }
            }
        }
    }
}
