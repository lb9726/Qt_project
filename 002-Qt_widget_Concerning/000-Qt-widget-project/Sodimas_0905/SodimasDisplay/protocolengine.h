#ifndef PROTOCOLENGINE_H
#define PROTOCOLENGINE_H

#include <QObject>

#ifdef CANOPEN
#include "canopen/openclient.h"
#endif

class ProtocolEngine : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolEngine(QObject *parent = 0);
    void initCanopen();
signals:
    void sigInfoProgess(QString msg,int progress);
public slots:
    void nodeStateChanged(int state);
    void fct_affChanged(bool isFCT);
    void arrowAction(bool arrows_FLM,bool arrows_FLD,bool arrows_SCROL);
    void mainVolumeChange(int divGong,int divBip);
    void musicVolumeChange(int div);
    void buttonLightOn(int btIndex,bool isBuzz);
    void buttonLightOff(int btIndex);
    void sigButtonLightOff(int btIndex);
    void floorArrive(int ge,int shi,int bai,int soundNumber);
    void showFuncLogo(int funcLogoIndex,int soundNumber);
    void textMessage(QString m1,QString m2,QString m3,QString m4);
    void sDO1(int orientation,int contraste,int backlight,int vit_scrol);
    void sDO2(int logo,int periode,int declenchement);
public:
#ifdef CANOPEN
    OpenClient *m_canopenClient;
#endif
};

#endif // PROTOCOLENGINE_H
