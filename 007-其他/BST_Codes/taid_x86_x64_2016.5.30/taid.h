#ifndef TAID_H
#define TAID_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QWidget>
#include "exeupdate.h"
#include "multmedia.h"
#include "netclient.h"
#include "pubulishpackage.h"
#include "templeteload.h"
#include "uieditor.h"
#include "./publish/bstsendupdate.h"

class Taid : public QObject
{
    Q_OBJECT
public:
    explicit Taid(QObject *parent = 0);
    Taid(QQmlApplicationEngine *engine);
    ~Taid();
    void Init();
signals:

public slots:
    void slt_currentProgress( QString terminalId, int progressNum );
    void slt_updateError( QString terminalId, QString err );
    void slt_updateOk( QString terminalId );
    void slt_updateCurDevice(int curDeviceNumber);
    //void slt_updateFinish( QString terminalId );
    void slt_updateFinish();
    void qmlObjectCreated(QObject *object, const QUrl &url);
public:
    ExeUpdate *m_ExeUpdate;
    MultMedia *m_MultMedia;
    NetClient *m_NetClient;
    Pubulishpackage *m_Pubulishpackage;
    TempleteLoad *m_TempleteLoad;
    UiEditor *m_UiEditor;
    QQmlApplicationEngine *engine;
    QObject*  rootObject;
    QUrl      mainQml;
    BstSendUpdate*      m_BstSendUpdate;
};

#endif // TAID_H
