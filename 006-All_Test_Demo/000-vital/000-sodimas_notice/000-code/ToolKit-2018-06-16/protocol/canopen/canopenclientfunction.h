#ifndef CANOPENCLIENTFUNCTION_H
#define CANOPENCLIENTFUNCTION_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include "../protocol/canopen/openclient.h"
#include "driver/gpios.h"
#include "driver/extensionbuttons.h"
#include "media/mplayer.h"

class CanOpenClientFunction : public QObject
{
    Q_OBJECT
private:
    explicit CanOpenClientFunction(QObject *parent = 0);
    static CanOpenClientFunction *m_pInstance;
    class Garbo
    {
    public:
        ~Garbo()
        {
            if (NULL != CanOpenClientFunction::m_pInstance)
            {
                delete CanOpenClientFunction::m_pInstance;
            }
        }
    };
    static Garbo garbo;

public:
    void init();
    void initCanOpenFunction();
    static CanOpenClientFunction* getInstace();
    quint8 getDailSwitchNumber();
    void setMplayer(Mplayer *pPlayer);
    void setBeepVolume(int pVolume);
    void setBigOrSmallTextEmpty();

signals:
    ///NMT
    void sigNodeState(int state);
//    void nodeStateChanged(QString state);
    ///RPDO1
    void sigAFF_EN(bool isAFF);
    void sigFCT_AFF(bool isFCT);
    void sigArrowAction(bool arrows_FLM, bool arrows_FLD, bool arrows_SCROL);
    void sigMainVolumeChange(int divGong, int divBip);
    void sigMusicVolumeChange(int div);

    void sigBuzz(int index);
    void sigBuzz();
    ///RPDO2
    void sigFloorArrive(int left, int center, int right, int soundNumber);
    void sigShowFuncLogo(int funcLogoIndex, int soundNumber);
    void sigShowFloorOrLogo(bool isShowLogo);
    ///RPDO3&RPDO4
    void sigMessage(QString m1, QString m2, QString m3, QString m4);
    void sigRealMessage(QString m1, QString m2);
    ///SDO1
    void sigSDO1(int orientation, int contraste, int backlight, int vit_scrol);
    void sigOrientation(int orientation);
    void sigContraste(int contraste);
    void sigBacklight(int backlight);
    void sigVit_scrol(int vit_scrol);
    ///SDO2
    void sigSDO2(int logo, int periode, int declenchement);

private :
    void dailNumberInitCanClient();

private slots:
    ///NMT
    void onSigNodeState(int state);
    ///RPDO1
    void onSigAFF_EN(bool isAFF);
//    void onSigEncharChanged(bool flag);
    void onSigFCT_AFF(bool isFCT);
    void onSigArrowAction(bool arrows_FLM, bool arrows_FLD, bool arrows_SCROL);
    void onSigMainVolumeChange(int divGong, int divBip);
    void onSigMusicVolumeChange(int div);
    void onSigButtonLightOn(int btIndex, bool isBuzz);
    void onSigButtonLightOff(int btIndex);
    void onSigBuzz(int index);
    void onSigBuzz();
    ///RPDO2
    void onSigFloorArrive(int left, int center, int right, int soundNumber);
    void onSigShowFuncLogo(int funcLogoIndex, int soundNumber);
    void onSigShowFloorOrLog(bool isShowLogo);
    ///RPDO3&RPDO4
    void onSigMessage(QString m1, QString m2, QString m3, QString m4);
    void onSigRealMessage(QString m1, QString m2);
    ///SDO1
    void onSigSDO1(int orientation, int contraste, int backlight, int vit_scrol);
    void onSigOrientation(int orientation);
    void onSigContraste(int contraste);
    void onSigBacklight(int backlight);
    void onSigVit_scrol(int vit_scrol);
    ///SDO2
    void onSigSDO2(int logo, int periode, int declenchement);
    void onDailSwitchChanged(quint8 pNum);
    /// buttons
    void reportButtonPressed(int btIndex);
    void reportButtonReleased(int btIndex);

private:
    QThread             *m_canopenProtocolThread;
    QThread             m_canopenFunctionThread;
    OpenClient          *m_canopenClient;
    Gpios               *m_gpio;
    ExtensionButtons    *m_extensionbtn;
    bool                m_bigText;
    int                 m_maintenance;
    Mplayer             *m_Mplayer;
    int                 m_beepvol;
};

#endif // CANOPENCLIENTFUNCTION_H
