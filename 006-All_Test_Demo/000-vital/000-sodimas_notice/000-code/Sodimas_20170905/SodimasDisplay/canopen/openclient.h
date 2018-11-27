#ifndef OPENCLIENT_H
#define OPENCLIENT_H

#include <QObject>
#include "opencallback.h"
#include "display.h"

#define MSG_ROW_FCT_OFF  2
#define MSG_ROW_FCT_ON  4

#define MSG_LENGTH_FCT_OFF  24
#define MSG_LENGTH_FCT_ON  16


class OpenClient : public QObject
{
    Q_OBJECT
public:
    explicit OpenClient(QObject *parent = 0);
    void initCanopenClient(int nodeID,int canBaud,QString canPort = "0");

    void rPDOArrived(int pdoIndex);
    void sSDOArrived(int sdoIndex,int subIndex);
    void rPDO1Arrived();
    void rPDO2Arrived();
    void rPDO3Arrived();
    void rPDO4Arrived();

    void sSDO1Arrived(int subIndex);
    void sSDO2Arrived(int subIndex);
    void sendUserPdo();
    void buildButtonPressed(int btIndex);
    void buildButtonReleased(int btIndex);
    void buildMessage(bool falg = false);
signals:
    ///NMT
    void sigNodeState(int state);
    ///RPDO1
    void sigFCT_AFF(bool isFCT);
    void sigArrowAction(bool arrows_FLM,bool arrows_FLD,bool arrows_SCROL);
    void sigMainVolumeChange(int divGong,int divBip);
    void sigMusicVolumeChange(int div);
    void sigButtonLightOn(int btIndex,bool isBuzz);
    void sigButtonLightOff(int btIndex);
    void sigBuzz(int index);
    void sigBuzz();
    ///RPDO2
    void sigFloorArrive(int ge,int shi,int bai,int soundNumber);
    void sigShowFuncLogo(int funcLogoIndex,int soundNumber);
    void sigShowFloorOrLogo(bool isShowLogo);
    ///RPDO3&RPDO4
    void sigMessage(QString m1,QString m2,QString m3,QString m4);
    void sigRealMessage(QString m1,QString m2);
    ///SDO1
    void sigSDO1(int orientation,int contraste,int backlight,int vit_scrol);
    void sigOrientation(int orientation);
    void sigContraste(int contraste);
    void sigBacklight(int backlight);
    void sigVit_scrol(int vit_scrol);
    ///SDO2
    void sigSDO2(int logo,int periode,int declenchement);
public slots:
    void nodeStateChanged(int state);
    void reportButtonPressed(int btIndex);
    void reportButtonReleased(int btIndex);
public:

int m_beepCount;
    /************************************RPDO1 - Start***********************************/
    //按钮点灯信号
    UNS8 m_Panel_output_Level_S_BC0;
    UNS8 m_Panel_output_Level_S_BC1;
    UNS8 m_Panel_output_Level_S_BC2;
    UNS8 m_Panel_output_Level_S_BC3;
    UNS8 m_Panel_output_Level_S_BC4;
    UNS8 m_Panel_output_Level_S_BC5;
    UNS8 m_Panel_output_Level_S_BC6;
    UNS8 m_Panel_output_Level_S_BC7;
    UNS8 m_Panel_output_Level_S_BC8;
    UNS8 m_Panel_output_Level_S_BC9;
    UNS8 m_Panel_output_Level_S_BC10;
    UNS8 m_Panel_output_Level_S_BC11;
    ///Arrow and Buzz
    UNS8 m_Arrows_FLM;
    UNS8 m_Arrows_FLD;
    UNS8 m_Arrows_SCROL;
    UNS8 m_Sound_BUZ;
    UNS8 m_Display_Mode_AFF_EN;
    UNS8 m_Display_Mode_FCT_AFF;
    //Extend Board Output
    UNS8 m_Extension_output_S_EXT1;
    UNS8 m_Extension_output_S_EXT2;
    UNS8 m_Extension_output_S_EXT3;
    UNS8 m_Extension_output_S_EXT4;
    UNS8 m_Extension_output_S_EXT5;
    UNS8 m_Extension_output_S_EXT6;
    UNS8 m_Extension_output_S_EXT7;
    UNS8 m_Extension_output_S_EXT8;
    UNS8 m_Extension_output_S_EXT9;
    UNS8 m_Extension_output_S_EXT10;
    UNS8 m_Extension_output_S_EXT11;
    UNS8 m_Extension_output_S_EXT12;
    UNS8 m_Extension_output_S_EXT13;
    UNS8 m_Extension_output_S_EXT14;
    UNS8 m_Extension_output_S_EXT15;
    UNS8 m_Extension_output_S_EXT16;
    UNS8 m_Extension_output_S_EXT17;
    UNS8 m_Extension_output_S_EXT18;
    UNS8 m_Extension_output_S_EXT19;
    UNS8 m_Extension_output_S_EXT20;
    UNS8 m_Extension_output_S_EXT21;
    UNS8 m_Extension_output_S_EXT22;
    UNS8 m_Extension_output_S_EXT23;
    UNS8 m_Extension_output_S_EXT24;
    //Volume
    UNS8 m_Sound_VOLUME_VOLUME_BIP;
    //    UNS8 m_Sound_VOLUME_GONG;
    //    UNS8 m_Sound_VOLUME_BIP;
    UNS8 m_Sound_VOLUME_MUSIQUE;
    /***********************************RPDO1 - End***********************************/


    /***********************************RPDO2 - Start***********************************/
    UNS8 m_Level_text_Logo_NIV_ALPHA_1;
    UNS8 m_Level_text_Logo_NIV_ALPHA_2;
    UNS8 m_Level_text_Logo_NIV_ALPHA_3;
    UNS8 m_Sound_NR_SON;
    UNS8 m_Level_text_Logo_LOGO;
    /***********************************RPDO2 - End***********************************/

    UNS8 m_Msg_Buffer[MSG_ROW_FCT_ON][MSG_LENGTH_FCT_OFF];

    /***********************************RPDO3 - Start***********************************/
    UNS8 m_text_segmented_1_MSG1_LIGNE;
    UNS8 m_text_segmented_1_MSG1_COLONNE;
    UNS8 m_text_segmented_1_MSG1_CHAR1;
    UNS8 m_text_segmented_1_MSG1_CHAR2;
    UNS8 m_text_segmented_1_MSG1_CHAR3;
    UNS8 m_text_segmented_1_MSG1_CHAR4;
    UNS8 m_text_segmented_1_MSG1_CHAR5;
    UNS8 m_text_segmented_1_MSG1_CHAR6;
    /***********************************RPDO3 - End***********************************/

    /***********************************RPDO4 - Start***********************************/
    UNS8 m_texte_segmented_2_MSG2_LIGNE;
    UNS8 m_texte_segmented_2_MSG2_COLONNE;
    UNS8 m_texte_segmented_2_MSG2_CHAR1;
    UNS8 m_texte_segmented_2_MSG2_CHAR2;
    UNS8 m_texte_segmented_2_MSG2_CHAR3;
    UNS8 m_texte_segmented_2_MSG2_CHAR4;
    UNS8 m_texte_segmented_2_MSG2_CHAR5;
    UNS8 m_texte_segmented_2_MSG2_CHAR6;
    /***********************************RPDO4 - End***********************************/

    /************************************SDO:2000 - Start***********************************/
    UNS8 m_Display_parameters_ORIENTATION;
    UNS8 m_Display_parameters_CONTRASTE;
    UNS8 m_Display_parameters_BACKLIGHT;
    UNS8 m_Display_parameters_VIT_SCROL;
    /************************************SDO:2000 - End***********************************/

    /************************************SDO:2001 - Start***********************************/
    UNS8 m_Customer_Logo_LOGO_CLIENT;
    UNS8 m_Customer_Logo_LOGO_PERIODE;
    UNS8 m_Customer_Logo_LOGO_DECLENCHEMENT;
    /************************************SDO:2001 - End***********************************/


    /***********************************TPDO1 - Start***********************************/
   /*
    //Onboard Button Input
    UNS8 m_Panel_input_Level_E_BC0;
    UNS8 m_Panel_input_Level_E_BC1;
    UNS8 m_Panel_input_Level_E_BC2;
    UNS8 m_Panel_input_Level_E_BC3;
    UNS8 m_Panel_input_Level_E_BC4;
    UNS8 m_Panel_input_Level_E_BC5;
    UNS8 m_Panel_input_Level_E_BC6;
    UNS8 m_Panel_input_Level_E_BC7;
    UNS8 m_Panel_input_Level_E_BC8;
    UNS8 m_Panel_input_Level_E_BC9;
    UNS8 m_Panel_input_Level_E_BC10;
    UNS8 m_Panel_input_Level_E_BC11;
    ///3 Extend Board Var
    UNS8 m_Display_state_EXT1;
    UNS8 m_Display_state_EXT2;
    UNS8 m_Display_state_EXT3;
    //24 Extend Board Input Var
    UNS8 m_Extension_input_E_EXT1;
    UNS8 m_Extension_input_E_EXT2;
    UNS8 m_Extension_input_E_EXT3;
    UNS8 m_Extension_input_E_EXT4;
    UNS8 m_Extension_input_E_EXT5;
    UNS8 m_Extension_input_E_EXT6;
    UNS8 m_Extension_input_E_EXT7;
    UNS8 m_Extension_input_E_EXT8;

    UNS8 m_Extension_input_E_EXT9;
    UNS8 m_Extension_input_E_EXT10;
    UNS8 m_Extension_input_E_EXT11;
    UNS8 m_Extension_input_E_EXT12;
    UNS8 m_Extension_input_E_EXT13;
    UNS8 m_Extension_input_E_EXT14;
    UNS8 m_Extension_input_E_EXT15;
    UNS8 m_Extension_input_E_EXT16;

    UNS8 m_Extension_input_E_EXT17;
    UNS8 m_Extension_input_E_EXT18;
    UNS8 m_Extension_input_E_EXT19;
    UNS8 m_Extension_input_E_EXT20;
    UNS8 m_Extension_input_E_EXT21;
    UNS8 m_Extension_input_E_EXT22;
    UNS8 m_Extension_input_E_EXT23;
    UNS8 m_Extension_input_E_EXT24;
    */
    /***********TPDO1 - End****************/

    UNS8 realtimeMsg[2][24];
    UNS8 fullscrennMsg[4][16];

//    QString m_RealtimeTextLine1;
//    QString m_RealtimeTextLine2;
//    QString m_FullScreenTextLine1;
//    QString m_FullScreenTextLine2;
//    QString m_FullScreenTextLine3;
//    QString m_FullScreenTextLine4;

};

#endif // OPENCLIENT_H
