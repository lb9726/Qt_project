#ifndef CUSTOMLOGO_H
#define CUSTOMLOGO_H

#include <QObject>
#include <QTimer>

class CustomLogo : public QObject
{
    Q_OBJECT
public:
    explicit CustomLogo(QObject *parent = 0);

signals:
    void sigShowCustomLogo();
    void sigShowMintenanceLogo();
    void sigShowNormal();
public slots:    
    void startLogo();
    void stop();
    void logoTrigger(int logo,int periode,int declenchement);
    void logoTimeOut();
    void logoSwitchTimeOut();
public:
    int m_logo;
    int m_periode;
    int m_declenchement;
    QTimer* m_logoTriggerTimer;
    QTimer* m_logoSwitchTimer;

    bool isShowingLogo;
};

#endif // CUSTOMLOGO_H
