#include "openclient.h"
#include <QDebug>
#include "../common/define.h"
///在opencallback中引用
///
OpenClient* canOpenCallbackClient;
s_BOARD CopSlaveBoard;// = {"0", "250K"};
UNS8 CopNodeID;

OpenClient::OpenClient(QObject *parent) : QObject(parent)
{
    connect(&mTestTimer, SIGNAL(timeout()), this, SLOT(slot_Test()));
    mTestTimer.setInterval(1000);
    mTestTimer.start();

    for(int i=0;i<48;i++){
        *((UNS8*)realtimeMsg+i) = (u_int8_t)0;
    }
    for(int i=0;i<64;i++){
        *((UNS8*)fullscrennMsg+i) = (u_int8_t)0;
    }

    m_Display_Mode_FCT_AFF = Display_Mode_FCT_AFF;

    m_Sound_VOLUME_VOLUME_BIP = Sound_VOLUME_VOLUME_BIP;
    m_Sound_VOLUME_MUSIQUE = Sound_VOLUME_MUSIQUE;

    m_Arrows_FLM = Arrows_FLM;
    m_Arrows_FLD = Arrows_FLD;
    m_Arrows_SCROL = Arrows_SCROL;
    m_Sound_BUZ = Sound_BUZ;

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

//void OpenClient::slot_Test()
//{
//    IDE_TRACE();
//    unsigned char data[8] = {0x2f,0x00,0x20,0x01,0x00,0x00,0x00,0x00};
//    int tmp = sendSDO(&display_Data, SDO_CLIENT, 1, data);
//    IDE_TRACE_INT(tmp);  //
//}

void OpenClient::initCanopenClient(int nodeID,int canBaud, QString canPort){
    if(nodeID<1||nodeID>61){
        IDE_TRACE();
        return;
    }
    IDE_TRACE_INT(nodeID);
    IDE_TRACE_INT(canBaud);
    CopNodeID = nodeID;//0x3C;
    QByteArray bu;
    bu.append(QString("%1K").arg(canBaud));
    QByteArray busname;
    busname.append(canPort);

    canOpenCallbackClient = this;
    CopSlaveBoard.busname = busname.data();//socket-can0
    CopSlaveBoard.baudrate = bu.data();
    initCANDevice();
}

void OpenClient::nodeStateChanged(int state){
    IDE_TRACE_INT(state);
    emit sigNodeState(state);
}

void OpenClient::sendUserPdo(){
    if(Panel_input_Level_E_BC0 == 0x00 ){
        Panel_input_Level_E_BC0 = 0x01;
    }else{
        Panel_input_Level_E_BC0 = 0x00;
    }
    sendPDOevent(&display_Data);
}

void OpenClient::rPDOArrived(int pdoIndex)
{
    ///IDE_TRACE_INT(pdoIndex);
    switch (pdoIndex) {
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

void OpenClient::sSDOArrived(int sdoIndex,int subindex)
{
    //IDE_TRACE_INT(sdoIndex);
    if(sdoIndex == 1){
        sSDO1Arrived(subindex);
    }else if(sdoIndex == 2){
        sSDO2Arrived(subindex);
    }else{

    }
}

void OpenClient::rPDO1Arrived()
{
    //处理顺序：显示模式，箭头，音量，点灯+Bip
    if(m_Display_Mode_AFF_EN != Display_Mode_AFF_EN){
        m_Display_Mode_AFF_EN = Display_Mode_AFF_EN;
        ///Unused for now.
    }

    if(m_Display_Mode_FCT_AFF != Display_Mode_FCT_AFF){
        IDE_TRACE_INT(m_Display_Mode_FCT_AFF);
        m_Display_Mode_FCT_AFF = Display_Mode_FCT_AFF;
        emit sigFCT_AFF(m_Display_Mode_FCT_AFF == 1 ? true:false);
        buildMessage();
    }

    if((Arrows_FLM != m_Arrows_FLM) || (Arrows_FLD != m_Arrows_FLD) || (Arrows_SCROL != m_Arrows_SCROL)){
        m_Arrows_FLM = Arrows_FLM;
        m_Arrows_FLD = Arrows_FLD;
        m_Arrows_SCROL = Arrows_SCROL;
        emit sigArrowAction(Arrows_FLM,Arrows_FLD,Arrows_SCROL);
    }
    //IDE_TRACE_INT(Sound_VOLUME_VOLUME_BIP);
    if(m_Sound_VOLUME_VOLUME_BIP != Sound_VOLUME_VOLUME_BIP){
        m_Sound_VOLUME_VOLUME_BIP = Sound_VOLUME_VOLUME_BIP;
        emit sigMainVolumeChange(m_Sound_VOLUME_VOLUME_BIP&0x0F,((m_Sound_VOLUME_VOLUME_BIP>>4)&0x0F));
    }
    // IDE_TRACE_INT(Sound_VOLUME_MUSIQUE);
    if(m_Sound_VOLUME_MUSIQUE != Sound_VOLUME_MUSIQUE){
        m_Sound_VOLUME_MUSIQUE = Sound_VOLUME_MUSIQUE;
        emit sigMusicVolumeChange(m_Sound_VOLUME_MUSIQUE);
    }

    bool isBuzz = (Sound_BUZ == 1 ? true : false);

    if(isBuzz){
        m_beepCount++;
        if(m_beepCount ==1 ){
            emit sigBuzz();
        }else if(m_beepCount ==3){
            m_beepCount = 0;
        }
    }

    if(m_Panel_output_Level_S_BC0 != Panel_output_Level_S_BC0){
        m_Panel_output_Level_S_BC0 = Panel_output_Level_S_BC0;
        if(m_Panel_output_Level_S_BC0){
            //            if(Sound_BUZ){
            //                emit sigBuzz(1);
            //            }
            emit sigButtonLightOn(1,isBuzz);
        }else{
            IDE_TRACE();
            emit sigButtonLightOff(1);
        }
    }
    if(m_Panel_output_Level_S_BC1 != Panel_output_Level_S_BC1){
        m_Panel_output_Level_S_BC1 = Panel_output_Level_S_BC1;

        if(m_Panel_output_Level_S_BC1){
            //            if(Sound_BUZ){
            //                emit sigBuzz(2);
            //            }
            emit sigButtonLightOn(2,isBuzz);

        }else{
            emit sigButtonLightOff(2);
        }
    }
    if(m_Panel_output_Level_S_BC2 != Panel_output_Level_S_BC2){
        m_Panel_output_Level_S_BC2 = Panel_output_Level_S_BC2;

        if(m_Panel_output_Level_S_BC2){
            //            if(Sound_BUZ){
            //                emit sigBuzz(3);
            //            }
            emit sigButtonLightOn(3,isBuzz);

        }else{
            emit sigButtonLightOff(3);
        }
    }
    if(m_Panel_output_Level_S_BC3 != Panel_output_Level_S_BC3){
        m_Panel_output_Level_S_BC3 = Panel_output_Level_S_BC3;

        if(m_Panel_output_Level_S_BC3){
            //            if(Sound_BUZ){
            //                emit sigBuzz(4);
            //            }
            emit sigButtonLightOn(4,isBuzz);
        }else{
            emit sigButtonLightOff(4);
        }
    }
    if(m_Panel_output_Level_S_BC4 != Panel_output_Level_S_BC4){
        m_Panel_output_Level_S_BC4 = Panel_output_Level_S_BC4;

        if(m_Panel_output_Level_S_BC4){
            //            if(Sound_BUZ){
            //                emit sigBuzz(5);
            //            }
            emit sigButtonLightOn(5,isBuzz);
        }else{
            emit sigButtonLightOff(5);
        }
    }
    if(m_Panel_output_Level_S_BC5 != Panel_output_Level_S_BC5){
        m_Panel_output_Level_S_BC5 = Panel_output_Level_S_BC5;

        if(m_Panel_output_Level_S_BC5){
            //            if(Sound_BUZ){
            //                emit sigBuzz(6);
            //            }
            emit sigButtonLightOn(6,isBuzz);
        }else{
            emit sigButtonLightOff(6);
        }
    }
    if(m_Panel_output_Level_S_BC6 != Panel_output_Level_S_BC6){
        m_Panel_output_Level_S_BC6 = Panel_output_Level_S_BC6;

        if(m_Panel_output_Level_S_BC6){
            //            if(Sound_BUZ){
            //                emit sigBuzz(7);
            //            }
            emit sigButtonLightOn(7,isBuzz);
        }else{
            emit sigButtonLightOff(7);
        }
    }
    if(m_Panel_output_Level_S_BC7 != Panel_output_Level_S_BC7){
        m_Panel_output_Level_S_BC7 = Panel_output_Level_S_BC7;

        if(m_Panel_output_Level_S_BC7){
            //            if(Sound_BUZ){
            //                emit sigBuzz(8);
            //            }
            emit sigButtonLightOn(8,isBuzz);
        }else{
            emit sigButtonLightOff(8);
        }
    }
    if(m_Panel_output_Level_S_BC8 != Panel_output_Level_S_BC8){
        m_Panel_output_Level_S_BC8 = Panel_output_Level_S_BC8;

        if(m_Panel_output_Level_S_BC8){
            //            if(Sound_BUZ){
            //                emit sigBuzz(9);
            //            }
            emit sigButtonLightOn(9,isBuzz);
        }else{
            emit sigButtonLightOff(9);
        }
    }
    if(m_Panel_output_Level_S_BC9 != Panel_output_Level_S_BC9){
        m_Panel_output_Level_S_BC9 = Panel_output_Level_S_BC9;
        if(m_Panel_output_Level_S_BC9){
            //            if(Sound_BUZ){
            //                emit sigBuzz(10);
            //            }
            emit sigButtonLightOn(10,isBuzz);
        }else{
            emit sigButtonLightOff(10);
        }
    }
    if(m_Panel_output_Level_S_BC10 != Panel_output_Level_S_BC10){
        m_Panel_output_Level_S_BC10 = Panel_output_Level_S_BC10;
        if(m_Panel_output_Level_S_BC10){
            //            if(Sound_BUZ){
            //                emit sigBuzz(11);
            //            }
            emit sigButtonLightOn(11,isBuzz);
        }else{
            emit sigButtonLightOff(11);
        }

    }
    if(m_Panel_output_Level_S_BC11 != Panel_output_Level_S_BC11){
        m_Panel_output_Level_S_BC11 = Panel_output_Level_S_BC11;
        if(m_Panel_output_Level_S_BC11){
            //            if(Sound_BUZ){
            //                emit sigBuzz(12);
            //            }
            emit sigButtonLightOn(12,isBuzz);
        }else{
            emit sigButtonLightOff(12);
        }
    }

    //Extend Board Output
    if(m_Extension_output_S_EXT1 != Extension_output_S_EXT1){
        m_Extension_output_S_EXT1 = Extension_output_S_EXT1;

        if(m_Extension_output_S_EXT1){
            //            if(Sound_BUZ){
            //                emit sigBuzz(13);
            //            }
            emit sigButtonLightOn(13,isBuzz);
        }else{
            IDE_TRACE();
            emit sigButtonLightOff(13);
        }
    }
    if(m_Extension_output_S_EXT2 != Extension_output_S_EXT2){
        m_Extension_output_S_EXT2 = Extension_output_S_EXT2;
        if(m_Extension_output_S_EXT2){
            //            if(Sound_BUZ){
            //                emit sigBuzz(14);
            //            }
            emit sigButtonLightOn(14,isBuzz);
        }else{
            emit sigButtonLightOff(14);
        }
    }
    if(m_Extension_output_S_EXT3 != Extension_output_S_EXT3){
        m_Extension_output_S_EXT3 = Extension_output_S_EXT3;
        if(m_Extension_output_S_EXT3){
            //            if(Sound_BUZ){
            //                emit sigBuzz(15);
            //            }
            emit sigButtonLightOn(15,isBuzz);
        }else{
            emit sigButtonLightOff(15);
        }
    }
    if(m_Extension_output_S_EXT4 != Extension_output_S_EXT4){
        m_Extension_output_S_EXT4 = Extension_output_S_EXT4;
        if(m_Extension_output_S_EXT4){
            //            if(Sound_BUZ){
            //                emit sigBuzz(16);
            //            }
            emit sigButtonLightOn(16,isBuzz);
        }else{
            emit sigButtonLightOff(16);
        }
    }
    if(m_Extension_output_S_EXT5 != Extension_output_S_EXT5){
        m_Extension_output_S_EXT5 = Extension_output_S_EXT5;
        if(m_Extension_output_S_EXT5){
            //            if(Sound_BUZ){
            //                emit sigBuzz(17);
            //            }
            emit sigButtonLightOn(17,isBuzz);
        }else{
            emit sigButtonLightOff(17);
        }
    }
    if(m_Extension_output_S_EXT6 != Extension_output_S_EXT6){
        m_Extension_output_S_EXT6 = Extension_output_S_EXT6;
        if(m_Extension_output_S_EXT6){
            //            if(Sound_BUZ){
            //                emit sigBuzz(18);
            //            }
            emit sigButtonLightOn(18,isBuzz);
        }else{
            emit sigButtonLightOff(18);
        }
    }
    if(m_Extension_output_S_EXT7 != Extension_output_S_EXT7){
        m_Extension_output_S_EXT7 = Extension_output_S_EXT7;
        if(m_Extension_output_S_EXT7){
            //            if(Sound_BUZ){
            //                emit sigBuzz(19);
            //            }
            emit sigButtonLightOn(19,isBuzz);
        }else{
            emit sigButtonLightOff(19);
        }
    }
    if(m_Extension_output_S_EXT8 != Extension_output_S_EXT8){
        m_Extension_output_S_EXT8 = Extension_output_S_EXT8;
        if(m_Extension_output_S_EXT8){
            //            if(Sound_BUZ){
            //                emit sigBuzz(20);
            //            }
            emit sigButtonLightOn(20,isBuzz);
        }else{
            emit sigButtonLightOff(20);
        }
    }
    if(m_Extension_output_S_EXT9 != Extension_output_S_EXT9){
        m_Extension_output_S_EXT9 = Extension_output_S_EXT9;
        if(m_Extension_output_S_EXT9){
            //            if(Sound_BUZ){
            //                emit sigBuzz(21);
            //            }
            emit sigButtonLightOn(21,isBuzz);
        }else{
            emit sigButtonLightOff(21);
        }
    }
    if(m_Extension_output_S_EXT10 != Extension_output_S_EXT10){
        m_Extension_output_S_EXT10 = Extension_output_S_EXT10;
        if(m_Extension_output_S_EXT10){
            //            if(Sound_BUZ){
            //                emit sigBuzz(22);
            //            }
            emit sigButtonLightOn(22,isBuzz);
        }else{
            emit sigButtonLightOff(22);
        }
    }
    if(m_Extension_output_S_EXT11 != Extension_output_S_EXT11){
        m_Extension_output_S_EXT11 = Extension_output_S_EXT11;
        if(m_Extension_output_S_EXT11){
            //            if(Sound_BUZ){
            //                emit sigBuzz(23);
            //            }
            emit sigButtonLightOn(23,isBuzz);
        }else{
            emit sigButtonLightOff(23);
        }
    }
    if(m_Extension_output_S_EXT12 != Extension_output_S_EXT12){
        m_Extension_output_S_EXT12 = Extension_output_S_EXT12;
        if(m_Extension_output_S_EXT12){
            //            if(Sound_BUZ){
            //                emit sigBuzz(24);
            //            }
            emit sigButtonLightOn(24,isBuzz);
        }else{
            emit sigButtonLightOff(24);
        }
    }
    if(m_Extension_output_S_EXT13 != Extension_output_S_EXT13){
        m_Extension_output_S_EXT13 = Extension_output_S_EXT13;
        if(m_Extension_output_S_EXT13){
            //            if(Sound_BUZ){
            //                emit sigBuzz(25);
            //            }
            emit sigButtonLightOn(25,isBuzz);
        }else{
            emit sigButtonLightOff(25);
        }
    }
    if(m_Extension_output_S_EXT14 != Extension_output_S_EXT14){
        m_Extension_output_S_EXT14 = Extension_output_S_EXT14;
        if(m_Extension_output_S_EXT14){
            //            if(Sound_BUZ){
            //                emit sigBuzz(26);
            //            }
            emit sigButtonLightOn(26,isBuzz);
        }else{
            emit sigButtonLightOff(26);
        }
    }
    if(m_Extension_output_S_EXT15 != Extension_output_S_EXT15){
        m_Extension_output_S_EXT15 = Extension_output_S_EXT15;
        if(m_Extension_output_S_EXT15){
            //            if(Sound_BUZ){
            //                emit sigBuzz(27);
            //            }
            emit sigButtonLightOn(27,isBuzz);
        }else{
            emit sigButtonLightOff(27);
        }
    }
    if(m_Extension_output_S_EXT16 != Extension_output_S_EXT16){
        m_Extension_output_S_EXT16 = Extension_output_S_EXT16;
        if(m_Extension_output_S_EXT16){
            //            if(Sound_BUZ){
            //                emit sigBuzz(28);
            //            }
            emit sigButtonLightOn(28,isBuzz);
        }else{
            emit sigButtonLightOff(28);
        }
    }
    if(m_Extension_output_S_EXT17 != Extension_output_S_EXT17){
        m_Extension_output_S_EXT17 = Extension_output_S_EXT17;
        if(m_Extension_output_S_EXT17){
            //            if(Sound_BUZ){
            //                emit sigBuzz(29);
            //            }
            emit sigButtonLightOn(29,isBuzz);
        }else{
            emit sigButtonLightOff(29);
        }
    }
    if(m_Extension_output_S_EXT18 != Extension_output_S_EXT18){
        m_Extension_output_S_EXT18 = Extension_output_S_EXT18;
        if(m_Extension_output_S_EXT18){
            //            if(Sound_BUZ){
            //                emit sigBuzz(30);
            //            }
            emit sigButtonLightOn(30,isBuzz);
        }else{
            emit sigButtonLightOff(30);
        }
    }
    if(m_Extension_output_S_EXT19 != Extension_output_S_EXT19){
        m_Extension_output_S_EXT19 = Extension_output_S_EXT19;
        if(m_Extension_output_S_EXT19){
            //            if(Sound_BUZ){
            //                emit sigBuzz(31);
            //            }
            emit sigButtonLightOn(31,isBuzz);
        }else{
            emit sigButtonLightOff(31);
        }
    }
    if(m_Extension_output_S_EXT20 != Extension_output_S_EXT20){
        m_Extension_output_S_EXT20 = Extension_output_S_EXT20;
        if(m_Extension_output_S_EXT20){
            //            if(Sound_BUZ){
            //                emit sigBuzz(32);
            //            }
            emit sigButtonLightOn(32,isBuzz);
        }else{
            emit sigButtonLightOff(32);
        }
    }
    if(m_Extension_output_S_EXT21 != Extension_output_S_EXT21){
        m_Extension_output_S_EXT21 = Extension_output_S_EXT21;
        if(m_Extension_output_S_EXT21){
            //            if(Sound_BUZ){
            //                emit sigBuzz(33);
            //            }
            emit sigButtonLightOn(33,isBuzz);
        }else{
            emit sigButtonLightOff(33);
        }
    }
    if(m_Extension_output_S_EXT22 != Extension_output_S_EXT22){
        m_Extension_output_S_EXT22 = Extension_output_S_EXT22;
        if(m_Extension_output_S_EXT22){
            //            if(Sound_BUZ){
            //                emit sigBuzz(34);
            //            }
            emit sigButtonLightOn(34,isBuzz);
        }else{
            emit sigButtonLightOff(34);
        }
    }
    if(m_Extension_output_S_EXT23 != Extension_output_S_EXT23){
        m_Extension_output_S_EXT23 = Extension_output_S_EXT23;
        if(m_Extension_output_S_EXT23){
            //            if(Sound_BUZ){
            //                emit sigBuzz(35);
            //            }
            emit sigButtonLightOn(35,isBuzz);
        }else{
            emit sigButtonLightOff(35);
        }
    }
    if(m_Extension_output_S_EXT24 != Extension_output_S_EXT24){
        m_Extension_output_S_EXT24 = Extension_output_S_EXT24;
        if(m_Extension_output_S_EXT24){
            //            if(Sound_BUZ){
            //                emit sigBuzz(36);
            //            }
            emit sigButtonLightOn(36,isBuzz);
        }else{
            emit sigButtonLightOff(36);
        }
    }
}

void OpenClient::rPDO2Arrived()
{
    if(m_Level_text_Logo_LOGO != Level_text_Logo_LOGO){
        m_Level_text_Logo_LOGO = Level_text_Logo_LOGO;
        emit sigShowFloorOrLogo(m_Level_text_Logo_LOGO);
    }

    if(m_Level_text_Logo_LOGO){
        m_Level_text_Logo_NIV_ALPHA_1 = Level_text_Logo_NIV_ALPHA_1;
        m_Sound_NR_SON = Sound_NR_SON;
        emit sigShowFuncLogo(m_Level_text_Logo_NIV_ALPHA_1,m_Sound_NR_SON);
    }else{
        if(!((m_Level_text_Logo_NIV_ALPHA_1 == Level_text_Logo_NIV_ALPHA_1)
             &&(m_Level_text_Logo_NIV_ALPHA_2 == Level_text_Logo_NIV_ALPHA_2)
             &&(m_Level_text_Logo_NIV_ALPHA_3 == Level_text_Logo_NIV_ALPHA_3)&&(m_Sound_NR_SON == Sound_NR_SON))){
            m_Sound_NR_SON = Sound_NR_SON;
            m_Level_text_Logo_NIV_ALPHA_1 = Level_text_Logo_NIV_ALPHA_1;
            m_Level_text_Logo_NIV_ALPHA_2 = Level_text_Logo_NIV_ALPHA_2;
            m_Level_text_Logo_NIV_ALPHA_3 = Level_text_Logo_NIV_ALPHA_3;
            //            IDE_TRACE_INT(m_Level_text_Logo_NIV_ALPHA_1);
            //            IDE_TRACE_INT(m_Level_text_Logo_NIV_ALPHA_2);
            //            IDE_TRACE_INT(m_Level_text_Logo_NIV_ALPHA_3);
            emit sigFloorArrive(m_Level_text_Logo_NIV_ALPHA_3,m_Level_text_Logo_NIV_ALPHA_2,m_Level_text_Logo_NIV_ALPHA_1,m_Sound_NR_SON);
        }
    }
}

void OpenClient::rPDO3Arrived()
{
    UNS8 curLine = text_segmented_1_MSG1_LIGNE;
    UNS8 curClume = text_segmented_1_MSG1_COLONNE;
    if(!text_segmented_1_MSG1_CHAR1 && !text_segmented_1_MSG1_CHAR2&&\
            !text_segmented_1_MSG1_CHAR3 && !text_segmented_1_MSG1_CHAR4&&\
            !text_segmented_1_MSG1_CHAR5 &&!text_segmented_1_MSG1_CHAR6){
        //IDE_TRACE();
        buildMessage(true);
    }else
    {
        if(m_Display_Mode_FCT_AFF == 0 ){
            if(curLine != 0 && curLine != 1){///LINE offset is : 1
                IDE_TRACE();
                return;
            }
            if(curClume>23){///COLONNE offset is :0
                IDE_TRACE();
                return;
            }
            if(text_segmented_1_MSG1_CHAR1 == 0x00){
                for(int i = 0; i < 24; i++){
                    realtimeMsg[curLine][i] = 0x00;
                }
                ///IDE_TRACE();
                buildMessage();
                return;
            }
            if(curClume <= 18){
                realtimeMsg[curLine][curClume] = text_segmented_1_MSG1_CHAR1;
                realtimeMsg[curLine][curClume+1] = text_segmented_1_MSG1_CHAR2;
                realtimeMsg[curLine][curClume+2] = text_segmented_1_MSG1_CHAR3;
                realtimeMsg[curLine][curClume+3] = text_segmented_1_MSG1_CHAR4;
                realtimeMsg[curLine][curClume+4] = text_segmented_1_MSG1_CHAR5;
                realtimeMsg[curLine][curClume+5] = text_segmented_1_MSG1_CHAR6;
            }else if(curClume == 19){
                realtimeMsg[curLine][19] = text_segmented_1_MSG1_CHAR1;
                realtimeMsg[curLine][20] = text_segmented_1_MSG1_CHAR2;
                realtimeMsg[curLine][21] = text_segmented_1_MSG1_CHAR3;
                realtimeMsg[curLine][22] = text_segmented_1_MSG1_CHAR4;
                realtimeMsg[curLine][23] = text_segmented_1_MSG1_CHAR5;
            }else if(curClume == 20){
                realtimeMsg[curLine][20] = text_segmented_1_MSG1_CHAR1;
                realtimeMsg[curLine][21] = text_segmented_1_MSG1_CHAR2;
                realtimeMsg[curLine][22] = text_segmented_1_MSG1_CHAR3;
                realtimeMsg[curLine][23] = text_segmented_1_MSG1_CHAR4;
            }else if(curClume == 21){
                realtimeMsg[curLine][21] = text_segmented_1_MSG1_CHAR1;
                realtimeMsg[curLine][22] = text_segmented_1_MSG1_CHAR2;
                realtimeMsg[curLine][23] = text_segmented_1_MSG1_CHAR3;
            }else if(curClume == 22){
                realtimeMsg[curLine][22] = text_segmented_1_MSG1_CHAR1;
                realtimeMsg[curLine][23] = text_segmented_1_MSG1_CHAR2;
            }else if(curClume == 23){
                realtimeMsg[curLine][23] = text_segmented_1_MSG1_CHAR1;
            }
        }else{
            if(curLine > 3){///LINE offset is : 1
                IDE_TRACE();
                return;
            }
            if(curClume>15){///COLONNE offset is :0
                IDE_TRACE();
                return;
            }
            if(text_segmented_1_MSG1_CHAR1 == 0x00){
                for(int i = 0; i < 16; i++){
                    fullscrennMsg[curLine][i] = 0x00;
                }
                IDE_TRACE();
                buildMessage();
                return;
            }
            if(curClume <= 10){
                fullscrennMsg[curLine][curClume] = text_segmented_1_MSG1_CHAR1;
                fullscrennMsg[curLine][curClume+1] = text_segmented_1_MSG1_CHAR2;
                fullscrennMsg[curLine][curClume+2] = text_segmented_1_MSG1_CHAR3;
                fullscrennMsg[curLine][curClume+3] = text_segmented_1_MSG1_CHAR4;
                fullscrennMsg[curLine][curClume+4] = text_segmented_1_MSG1_CHAR5;
                fullscrennMsg[curLine][curClume+5] = text_segmented_1_MSG1_CHAR6;
            }else if(curClume == 11){
                fullscrennMsg[curLine][11] = text_segmented_1_MSG1_CHAR1;
                fullscrennMsg[curLine][12] = text_segmented_1_MSG1_CHAR2;
                fullscrennMsg[curLine][13] = text_segmented_1_MSG1_CHAR3;
                fullscrennMsg[curLine][14] = text_segmented_1_MSG1_CHAR4;
                fullscrennMsg[curLine][15] = text_segmented_1_MSG1_CHAR5;
            }else if(curClume == 12){
                fullscrennMsg[curLine][12] = text_segmented_1_MSG1_CHAR1;
                fullscrennMsg[curLine][13] = text_segmented_1_MSG1_CHAR2;
                fullscrennMsg[curLine][14] = text_segmented_1_MSG1_CHAR3;
                fullscrennMsg[curLine][15] = text_segmented_1_MSG1_CHAR4;
            }else if(curClume == 13){
                fullscrennMsg[curLine][13] = text_segmented_1_MSG1_CHAR1;
                fullscrennMsg[curLine][14] = text_segmented_1_MSG1_CHAR2;
                fullscrennMsg[curLine][15] = text_segmented_1_MSG1_CHAR3;
            }else if(curClume == 14){
                fullscrennMsg[curLine][14] = text_segmented_1_MSG1_CHAR1;
                fullscrennMsg[curLine][15] = text_segmented_1_MSG1_CHAR2;
            }else if(curClume == 15){
                fullscrennMsg[curLine][15] = text_segmented_1_MSG1_CHAR1;
            }
            buildMessage(true);
        }
    }
}

void OpenClient::rPDO4Arrived()
{
    UNS8 curLine = texte_segmented_2_MSG2_LIGNE;
    UNS8 curClume = texte_segmented_2_MSG2_COLONNE;

    if(!texte_segmented_2_MSG2_CHAR1 && !texte_segmented_2_MSG2_CHAR2&&\
            !texte_segmented_2_MSG2_CHAR3 && !texte_segmented_2_MSG2_CHAR4&&\
            !texte_segmented_2_MSG2_CHAR5 &&!texte_segmented_2_MSG2_CHAR6){
        //IDE_TRACE();
        buildMessage(true);
    }else{
        if(m_Display_Mode_FCT_AFF == 0 ){
            if(curLine != 0 && curLine != 1){///LINE offset is : 1
                IDE_TRACE();
                return;
            }
            if(curClume>23){///COLONNE offset is :0
                IDE_TRACE();
                return;
            }

            if(texte_segmented_2_MSG2_CHAR1 == 0x00){
                for(int i = 0; i < 24; i++){
                    realtimeMsg[curLine][i] = 0x00;
                }
                ///IDE_TRACE();
                return;
            }

            if(curClume <= 18){
                realtimeMsg[curLine][curClume] = texte_segmented_2_MSG2_CHAR1;
                realtimeMsg[curLine][curClume+1] = texte_segmented_2_MSG2_CHAR2;
                realtimeMsg[curLine][curClume+2] = texte_segmented_2_MSG2_CHAR3;
                realtimeMsg[curLine][curClume+3] = texte_segmented_2_MSG2_CHAR4;
                realtimeMsg[curLine][curClume+4] = texte_segmented_2_MSG2_CHAR5;
                realtimeMsg[curLine][curClume+5] = texte_segmented_2_MSG2_CHAR6;
            }else if(curClume == 19){
                realtimeMsg[curLine][19] = texte_segmented_2_MSG2_CHAR1;
                realtimeMsg[curLine][20] = texte_segmented_2_MSG2_CHAR2;
                realtimeMsg[curLine][21] = texte_segmented_2_MSG2_CHAR3;
                realtimeMsg[curLine][22] = texte_segmented_2_MSG2_CHAR4;
                realtimeMsg[curLine][23] = texte_segmented_2_MSG2_CHAR5;
            }else if(curClume == 20){
                realtimeMsg[curLine][20] = texte_segmented_2_MSG2_CHAR1;
                realtimeMsg[curLine][21] = texte_segmented_2_MSG2_CHAR2;
                realtimeMsg[curLine][22] = texte_segmented_2_MSG2_CHAR3;
                realtimeMsg[curLine][23] = texte_segmented_2_MSG2_CHAR4;
            }else if(curClume == 21){
                realtimeMsg[curLine][21] = texte_segmented_2_MSG2_CHAR1;
                realtimeMsg[curLine][22] = texte_segmented_2_MSG2_CHAR2;
                realtimeMsg[curLine][23] = texte_segmented_2_MSG2_CHAR3;
            }else if(curClume == 22){
                realtimeMsg[curLine][22] = texte_segmented_2_MSG2_CHAR1;
                realtimeMsg[curLine][23] = texte_segmented_2_MSG2_CHAR2;
            }else if(curClume == 23){
                realtimeMsg[curLine][23] = texte_segmented_2_MSG2_CHAR1;
            }
        }else{
            if(curLine > 3){///LINE offset is : 1
                IDE_TRACE();
                return;
            }
            if(curClume> 15){///COLONNE offset is :0
                IDE_TRACE();
                return;
            }

            if(texte_segmented_2_MSG2_CHAR1 == 0x00){
                for(int i = 0; i < 16; i++){
                    fullscrennMsg[curLine][i] = 0x00;
                }
                IDE_TRACE();
                //buildMessage();
                return;
            }

            if(curClume <= 10){
                fullscrennMsg[curLine][curClume] = texte_segmented_2_MSG2_CHAR1;
                fullscrennMsg[curLine][curClume+1] = texte_segmented_2_MSG2_CHAR2;
                fullscrennMsg[curLine][curClume+2] = texte_segmented_2_MSG2_CHAR3;
                fullscrennMsg[curLine][curClume+3] = texte_segmented_2_MSG2_CHAR4;
                fullscrennMsg[curLine][curClume+4] = texte_segmented_2_MSG2_CHAR5;
                fullscrennMsg[curLine][curClume+5] = texte_segmented_2_MSG2_CHAR6;
            }else if(curClume == 11){
                fullscrennMsg[curLine][11] = texte_segmented_2_MSG2_CHAR1;
                fullscrennMsg[curLine][12] = texte_segmented_2_MSG2_CHAR2;
                fullscrennMsg[curLine][13] = texte_segmented_2_MSG2_CHAR3;
                fullscrennMsg[curLine][14] = texte_segmented_2_MSG2_CHAR4;
                fullscrennMsg[curLine][15] = texte_segmented_2_MSG2_CHAR5;
            }else if(curClume == 12){
                fullscrennMsg[curLine][12] = texte_segmented_2_MSG2_CHAR1;
                fullscrennMsg[curLine][13] = texte_segmented_2_MSG2_CHAR2;
                fullscrennMsg[curLine][14] = texte_segmented_2_MSG2_CHAR3;
                fullscrennMsg[curLine][15] = texte_segmented_2_MSG2_CHAR4;
            }else if(curClume == 13){
                fullscrennMsg[curLine][13] = texte_segmented_2_MSG2_CHAR1;
                fullscrennMsg[curLine][14] = texte_segmented_2_MSG2_CHAR2;
                fullscrennMsg[curLine][15] = texte_segmented_2_MSG2_CHAR3;
            }else if(curClume == 14){
                fullscrennMsg[curLine][14] = texte_segmented_2_MSG2_CHAR1;
                fullscrennMsg[curLine][15] = texte_segmented_2_MSG2_CHAR2;
            }else if(curClume == 15){
                fullscrennMsg[curLine][15] = texte_segmented_2_MSG2_CHAR1;
            }
            buildMessage(true);
        }

    }
}

void OpenClient::buildMessage(bool falg)
{
    QString ba1;
    QString ba2;

    if(m_Display_Mode_FCT_AFF == 0 ){
        for(int j=0;j<24;j++){
            if(realtimeMsg[0][j]==0){
                ba1.append(' ');
            }else{
                ba1.append(realtimeMsg[0][j]);
            }
            //IDE_TRACE_INT(realtimeMsg[0][j]);
        }
        for(int k=0;k<24;k++){
            if(realtimeMsg[1][k]==0){
                ba2.append(' ');
            }else{
                ba2.append(realtimeMsg[1][k]);
            }
            //IDE_TRACE_INT(realtimeMsg[1][j]);
        }
        if(falg)
            emit sigRealMessage(ba1,ba2);
    }else{
        //IDE_TRACE();
        for(int j=0;j<16;j++){
            if(fullscrennMsg[0][j]==0){
                ba1.append(' ');
            }else{
                ba1.append(fullscrennMsg[0][j]);
            }
            //IDE_TRACE_INT(realtimeMsg[0][j]);
        }

        for(int k=0;k<16;k++){
            if(fullscrennMsg[1][k]==0){
                ba2.append(' ');
            }else{
                ba2.append(fullscrennMsg[1][k]);
            }
            //IDE_TRACE_INT(realtimeMsg[1][k]);
        }

        QString ba3;
        QString ba4;
        for(int l=0;l<16;l++){
            if(fullscrennMsg[2][l]==0){
                ba3.append(' ');
            }else{
                ba3.append(fullscrennMsg[2][l]);
            }

            //IDE_TRACE_INT(realtimeMsg[2][l]);
        }
        for(int m=0;m<16;m++){
            if(fullscrennMsg[3][m]==0){
                ba4.append(' ');
            }else{
                ba4.append(fullscrennMsg[3][m]);
            }
        }
        if(falg)
            emit sigMessage(ba1,ba2,ba3,ba4);
    }
}

void OpenClient::sSDO1Arrived(int subIndex)
{
    switch (subIndex) {
    case 0x01:
        if(m_Display_parameters_ORIENTATION != Display_parameters_ORIENTATION){
            m_Display_parameters_ORIENTATION = Display_parameters_ORIENTATION;
            emit sigOrientation(m_Display_parameters_ORIENTATION);
        }
        break;
    case 0x02:
        if(m_Display_parameters_CONTRASTE != Display_parameters_CONTRASTE){
            m_Display_parameters_CONTRASTE != Display_parameters_CONTRASTE;
            emit sigContraste(m_Display_parameters_CONTRASTE);
        }
        break;
    case 0x03:
        if(m_Display_parameters_BACKLIGHT != Display_parameters_BACKLIGHT){
            m_Display_parameters_BACKLIGHT = Display_parameters_BACKLIGHT;
            emit sigBacklight(m_Display_parameters_BACKLIGHT);
        }
        break;
    case 0x04:
        if(m_Display_parameters_VIT_SCROL != Display_parameters_VIT_SCROL){
            m_Display_parameters_VIT_SCROL = Display_parameters_VIT_SCROL;
            emit sigVit_scrol(m_Display_parameters_VIT_SCROL);
        }
        break;
    default:
        break;
    }
    ///SDO1
    ///emit sigSDO1(Display_parameters_ORIENTATION,Display_parameters_CONTRASTE,Display_parameters_BACKLIGHT,Display_parameters_VIT_SCROL);
}

void OpenClient::sSDO2Arrived(int subIndex)
{
    switch (subIndex) {
    case 0x01:
        if(m_Customer_Logo_LOGO_CLIENT != Customer_Logo_LOGO_CLIENT){
            m_Customer_Logo_LOGO_CLIENT = Customer_Logo_LOGO_CLIENT;
            ///TO-DO
        }
        break;
    case 0x02:
        if(m_Customer_Logo_LOGO_PERIODE != Customer_Logo_LOGO_PERIODE){
            m_Customer_Logo_LOGO_PERIODE = Customer_Logo_LOGO_PERIODE;
        }
        break;
    case 0x03:
        if(m_Customer_Logo_LOGO_DECLENCHEMENT != Customer_Logo_LOGO_DECLENCHEMENT){
            m_Customer_Logo_LOGO_DECLENCHEMENT = Customer_Logo_LOGO_DECLENCHEMENT;
        }
        break;
    default:
        break;
    }
    emit sigSDO2(m_Customer_Logo_LOGO_CLIENT,m_Customer_Logo_LOGO_PERIODE,m_Customer_Logo_LOGO_DECLENCHEMENT);
}

void OpenClient::reportButtonPressed(int btIndex){
    //IDE_TRACE_INT(btIndex);
    buildButtonPressed(btIndex);
    sendPDOevent(&display_Data);
}

void OpenClient::reportButtonReleased(int btIndex){
    //IDE_TRACE_INT(btIndex);
    buildButtonReleased(btIndex);
    sendPDOevent(&display_Data);
}

void OpenClient::buildButtonPressed(int btIndex){
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
    if(btIndex >= 13 && btIndex <= 20){
        Display_state_EXT1 = 1;
    }else{
        Display_state_EXT1 = 0;
    }

    if(btIndex >= 21 && btIndex <= 28){
        Display_state_EXT2 = 1;
    }else{
        Display_state_EXT2 = 0;
    }

    if(btIndex >= 29 && btIndex <= 36){
        Display_state_EXT3 = 1;
    }else{
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

void OpenClient::buildButtonReleased(int btIndex){
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
    if(btIndex >= 13 && btIndex <= 20){
        Display_state_EXT1 = 1;
    }else{
        Display_state_EXT1 = 0;
    }

    if(btIndex >= 21 && btIndex <= 28){
        Display_state_EXT2 = 1;
    }else{
        Display_state_EXT2 = 0;
    }

    if(btIndex >= 29 && btIndex <= 36){
        Display_state_EXT3 = 1;
    }else{
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
