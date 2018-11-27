#include "openclient.h"
#include <QDebug>

///在opencallback中引用
OpenClient* canOpenCallbackClient;
s_BOARD CopSlaveBoard;// = {"0", "250K"};
UNS8 CopNodeID;
OpenClient::OpenClient(QObject *parent) : QObject(parent)
{

}

void OpenClient::initCanopenClient(){
    int baud = 250;
    CopNodeID = 0x3C;
    QByteArray bu;
    bu.append(QString("%1K").arg(baud));
    canOpenCallbackClient = this;
    CopSlaveBoard.busname = "0";//socket-can0
    CopSlaveBoard.baudrate = bu.data();
    initCANDevice();
}

void OpenClient::nodeStateChanged(int state){
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

void OpenClient::sSDOArrived(int sdoIndex)
{
    if(sdoIndex == 1){
        sSDO1Arrived();
    }else if(sdoIndex == 2){
        sSDO2Arrived();
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
        m_Display_Mode_FCT_AFF = Display_Mode_FCT_AFF;
        emit sigFCT_AFF(m_Display_Mode_FCT_AFF == 1 ? true:false);
    }

    if((Arrows_FLM != m_Arrows_FLM) || (Arrows_FLD != m_Arrows_FLD) || (Arrows_SCROL != m_Arrows_SCROL)){
        m_Arrows_FLM = Arrows_FLM;
        m_Arrows_FLD = Arrows_FLD;
        m_Arrows_SCROL = Arrows_SCROL;
        sigArrowAction(Arrows_FLM,Arrows_FLD,Arrows_SCROL);
    }

    if(m_Sound_VOLUME_VOLUME_BIP != Sound_VOLUME_VOLUME_BIP){
        m_Sound_VOLUME_VOLUME_BIP = Sound_VOLUME_VOLUME_BIP;
        emit sigMainVolumeChange(m_Sound_VOLUME_VOLUME_BIP&0x0F,((m_Sound_VOLUME_VOLUME_BIP>>4)&0x0F));
    }

    if(m_Sound_VOLUME_MUSIQUE != Sound_VOLUME_MUSIQUE){
        m_Sound_VOLUME_MUSIQUE = Sound_VOLUME_MUSIQUE;
        emit sigMusicVolumeChange(m_Sound_VOLUME_MUSIQUE);
    }

    bool isBuzz = (Sound_BUZ == 1 ? true : false);
    if(m_Panel_output_Level_S_BC0 != Panel_output_Level_S_BC0){
        m_Panel_output_Level_S_BC0 = Panel_output_Level_S_BC0;
        emit sigButtonLightOn(1,isBuzz);
    }
    if(m_Panel_output_Level_S_BC1 != Panel_output_Level_S_BC1){
        m_Panel_output_Level_S_BC1 = Panel_output_Level_S_BC1;
        emit sigButtonLightOn(2,isBuzz);
    }
    if(m_Panel_output_Level_S_BC2 != Panel_output_Level_S_BC2){
        m_Panel_output_Level_S_BC2 = Panel_output_Level_S_BC2;
        emit sigButtonLightOn(3,isBuzz);
    }
    if(m_Panel_output_Level_S_BC3 != Panel_output_Level_S_BC3){
        m_Panel_output_Level_S_BC3 = Panel_output_Level_S_BC3;
        emit sigButtonLightOn(4,isBuzz);
    }
    if(m_Panel_output_Level_S_BC4 != Panel_output_Level_S_BC4){
        m_Panel_output_Level_S_BC4 = Panel_output_Level_S_BC4;
        emit sigButtonLightOn(5,isBuzz);
    }
    if(m_Panel_output_Level_S_BC5 != Panel_output_Level_S_BC5){
        m_Panel_output_Level_S_BC5 = Panel_output_Level_S_BC5;
        emit sigButtonLightOn(6,isBuzz);
    }
    if(m_Panel_output_Level_S_BC6 != Panel_output_Level_S_BC6){
        m_Panel_output_Level_S_BC6 = Panel_output_Level_S_BC6;
        emit sigButtonLightOn(7,isBuzz);
    }
    if(m_Panel_output_Level_S_BC7 != Panel_output_Level_S_BC7){
        m_Panel_output_Level_S_BC7 = Panel_output_Level_S_BC7;
        emit sigButtonLightOn(8,isBuzz);
    }
    if(m_Panel_output_Level_S_BC8 != Panel_output_Level_S_BC8){
        m_Panel_output_Level_S_BC8 = Panel_output_Level_S_BC8;
        emit sigButtonLightOn(9,isBuzz);
    }
    if(m_Panel_output_Level_S_BC9 != Panel_output_Level_S_BC9){
        m_Panel_output_Level_S_BC9 = Panel_output_Level_S_BC9;
        emit sigButtonLightOn(10,isBuzz);
    }
    if(m_Panel_output_Level_S_BC10 != Panel_output_Level_S_BC10){
        m_Panel_output_Level_S_BC10 = Panel_output_Level_S_BC10;
        emit sigButtonLightOn(11,isBuzz);
    }
    if(m_Panel_output_Level_S_BC11 != Panel_output_Level_S_BC11){
        m_Panel_output_Level_S_BC11 = Panel_output_Level_S_BC11;
        emit sigButtonLightOn(12,isBuzz);
    }

    //Extend Board Output
    if(m_Extension_output_S_EXT1 != Extension_output_S_EXT1){
        m_Extension_output_S_EXT1 = Extension_output_S_EXT1;
        emit sigButtonLightOn(13,isBuzz);
    }
    if(m_Extension_output_S_EXT2 != Extension_output_S_EXT2){
        m_Extension_output_S_EXT2 = Extension_output_S_EXT2;
        emit sigButtonLightOn(14,isBuzz);
    }
    if(m_Extension_output_S_EXT3 != Extension_output_S_EXT3){
        m_Extension_output_S_EXT3 = Extension_output_S_EXT3;
        emit sigButtonLightOn(15,isBuzz);
    }
    if(m_Extension_output_S_EXT4 != Extension_output_S_EXT4){
        m_Extension_output_S_EXT4 = Extension_output_S_EXT4;
        emit sigButtonLightOn(16,isBuzz);
    }
    if(m_Extension_output_S_EXT5 != Extension_output_S_EXT5){
        m_Extension_output_S_EXT5 = Extension_output_S_EXT5;
        emit sigButtonLightOn(17,isBuzz);
    }
    if(m_Extension_output_S_EXT6 != Extension_output_S_EXT6){
        m_Extension_output_S_EXT6 = Extension_output_S_EXT6;
        emit sigButtonLightOn(18,isBuzz);
    }
    if(m_Extension_output_S_EXT7 != Extension_output_S_EXT7){
        m_Extension_output_S_EXT7 = Extension_output_S_EXT7;
        emit sigButtonLightOn(19,isBuzz);
    }
    if(m_Extension_output_S_EXT8 != Extension_output_S_EXT8){
        m_Extension_output_S_EXT8 = Extension_output_S_EXT8;
        emit sigButtonLightOn(20,isBuzz);
    }
    if(m_Extension_output_S_EXT9 != Extension_output_S_EXT9){
        m_Extension_output_S_EXT9 = Extension_output_S_EXT9;
        emit sigButtonLightOn(21,isBuzz);
    }
    if(m_Extension_output_S_EXT10 != Extension_output_S_EXT10){
        m_Extension_output_S_EXT10 = Extension_output_S_EXT10;
        emit sigButtonLightOn(22,isBuzz);
    }
    if(m_Extension_output_S_EXT11 != Extension_output_S_EXT11){
        m_Extension_output_S_EXT11 = Extension_output_S_EXT11;
        emit sigButtonLightOn(23,isBuzz);
    }
    if(m_Extension_output_S_EXT12 != Extension_output_S_EXT12){
        m_Extension_output_S_EXT12 = Extension_output_S_EXT12;
        emit sigButtonLightOn(24,isBuzz);
    }
    if(m_Extension_output_S_EXT13 != Extension_output_S_EXT13){
        m_Extension_output_S_EXT13 = Extension_output_S_EXT13;
        emit sigButtonLightOn(25,isBuzz);
    }
    if(m_Extension_output_S_EXT14 != Extension_output_S_EXT14){
        m_Extension_output_S_EXT14 = Extension_output_S_EXT14;
        emit sigButtonLightOn(26,isBuzz);
    }
    if(m_Extension_output_S_EXT15 != Extension_output_S_EXT15){
        m_Extension_output_S_EXT15 = Extension_output_S_EXT15;
        emit sigButtonLightOn(27,isBuzz);
    }
    if(m_Extension_output_S_EXT16 != Extension_output_S_EXT16){
        m_Extension_output_S_EXT16 = Extension_output_S_EXT16;
        emit sigButtonLightOn(28,isBuzz);
    }
    if(m_Extension_output_S_EXT17 != Extension_output_S_EXT17){
        m_Extension_output_S_EXT17 = Extension_output_S_EXT17;
        emit sigButtonLightOn(29,isBuzz);
    }
    if(m_Extension_output_S_EXT18 != Extension_output_S_EXT18){
        m_Extension_output_S_EXT18 = Extension_output_S_EXT18;
        emit sigButtonLightOn(30,isBuzz);
    }
    if(m_Extension_output_S_EXT19 != Extension_output_S_EXT19){
        m_Extension_output_S_EXT19 = Extension_output_S_EXT19;
        emit sigButtonLightOn(31,isBuzz);
    }
    if(m_Extension_output_S_EXT20 != Extension_output_S_EXT20){
        m_Extension_output_S_EXT20 = Extension_output_S_EXT20;
        emit sigButtonLightOn(32,isBuzz);
    }
    if(m_Extension_output_S_EXT21 != Extension_output_S_EXT21){
        m_Extension_output_S_EXT21 = Extension_output_S_EXT21;
        emit sigButtonLightOn(33,isBuzz);
    }
    if(m_Extension_output_S_EXT22 != Extension_output_S_EXT22){
        m_Extension_output_S_EXT22 = Extension_output_S_EXT22;
        emit sigButtonLightOn(34,isBuzz);
    }
    if(m_Extension_output_S_EXT23 != Extension_output_S_EXT23){
        m_Extension_output_S_EXT23 = Extension_output_S_EXT23;
        emit sigButtonLightOn(35,isBuzz);
    }
    if(m_Extension_output_S_EXT24 != Extension_output_S_EXT24){
        m_Extension_output_S_EXT24 = Extension_output_S_EXT24;
        emit sigButtonLightOn(36,isBuzz);
    }
}

void OpenClient::rPDO2Arrived()
{
    //    UNS8 m_Level_text_Logo_NIV_ALPHA_1;
    //    UNS8 m_Level_text_Logo_NIV_ALPHA_2;
    //    UNS8 m_Level_text_Logo_NIV_ALPHA_3;
    //    UNS8 m_Sound_NR_SON;
    //    UNS8 m_Level_text_Logo_LOGO;
    if(m_Level_text_Logo_LOGO != Level_text_Logo_LOGO){
        m_Level_text_Logo_LOGO = Level_text_Logo_LOGO;
        m_Sound_NR_SON = Sound_NR_SON;
        if(m_Level_text_Logo_LOGO){
            emit sigShowFuncLogo(m_Level_text_Logo_NIV_ALPHA_1,m_Sound_NR_SON);
        }else{
            if(!((m_Level_text_Logo_NIV_ALPHA_1 == Level_text_Logo_NIV_ALPHA_1)
                 &&(m_Level_text_Logo_NIV_ALPHA_2 == Level_text_Logo_NIV_ALPHA_2)
                 &&(m_Level_text_Logo_NIV_ALPHA_3 == Level_text_Logo_NIV_ALPHA_3))){

                m_Level_text_Logo_NIV_ALPHA_1 = Level_text_Logo_NIV_ALPHA_1;
                m_Level_text_Logo_NIV_ALPHA_2 = Level_text_Logo_NIV_ALPHA_2;
                m_Level_text_Logo_NIV_ALPHA_3 = Level_text_Logo_NIV_ALPHA_3;

                emit sigFloorArrive(m_Level_text_Logo_NIV_ALPHA_3,m_Level_text_Logo_NIV_ALPHA_2,m_Level_text_Logo_NIV_ALPHA_1,m_Sound_NR_SON);
            }
        }
    }
}

void OpenClient::rPDO3Arrived()
{

}

void OpenClient::rPDO4Arrived()
{

}

void OpenClient::sSDO1Arrived()
{

}

void OpenClient::sSDO2Arrived()
{

}

void OpenClient::reportButton(int btIndex){
    buildTPDO(btIndex);
    sendPDOevent(&display_Data);
}

void OpenClient::buildTPDO(int btIndex){
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
