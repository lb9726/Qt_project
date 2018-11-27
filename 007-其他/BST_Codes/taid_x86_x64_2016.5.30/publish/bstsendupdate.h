#ifndef BSTSENDUPDATE_H
#define BSTSENDUPDATE_H
#include <QObject>
#include "client.h"
#include <QTimer>
#include "../uieditor.h"

class BstSendUpdate : public QObject
{
        Q_OBJECT
    public:
        explicit BstSendUpdate( QObject* parent = 0 );
        ~BstSendUpdate();
    signals:
        void currentProgressNum( QString terminalId, int progressNum );
        void updateError( QString terminalId, QString err );
        void updateOk( QString terminalId );
        void updateCurDevice(int curDeviceNumber);
        void updateFinish();
    public slots:
        void putUpdateFile( QString filePath, QString terminalId );
        void batchUpdate( QStringList deviceList);
        QStringList getTerminalList();
        bool zipResource();
    private:
        QMap<QString, Client*>* m_clientMap;
        QUdpSocket* m_terListSocket;
        QTimer* testTimer;
    private slots:
        void dataReceived();
        void testCurrentProgressNum(QString terminalId,int progressNum);
        void testUpdateError(QString terminalId,QString err);
        void testUpdateOk(QString terminalId);
        void testGetTerminalList();
public:
        UiEditor* m_editor;
        int m_allUpdateCount;
        int m_curUpdateCount;
        int m_curprogress;
};

#endif // BSTSENDUPDATE_H
