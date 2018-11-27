#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QDialog>
#include <QTextBrowser>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>

#include <QHostAddress>
#include <QTcpSocket>
#include <QTextCodec>

class TCPClient : public QDialog
{
    Q_OBJECT

public:
    TCPClient(QWidget *parent = 0);
    ~TCPClient();

private:
    QTextBrowser *textBrowser;
    QLineEdit *sendLineEdit;
    QPushButton *sendBtn;

    QLabel *userNameLabel;
    QLineEdit *userNameLineEdit;

    QLabel *serverIPLabel;
    QLineEdit *serverIPLineEdit;

    QLabel *portLabel;
    QLineEdit *portLineEdit;

    QPushButton *enterBtn;
    QGridLayout *mainLayout;

    bool status;
    int port;
    QHostAddress *serverIP;
    QString userName;
    QTcpSocket *tcpSocket;
    QTextCodec *codec;
public slots:
    void slotEnter();
    void slotConnected();
    void slotDisconnected();
    void dataReceived();
    void slotSend();

    void autoScroll();

};

#endif // TCPCLIENT_H
