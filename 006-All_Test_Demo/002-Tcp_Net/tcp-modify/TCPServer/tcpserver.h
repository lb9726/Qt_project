#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QDialog>
#include <QTextBrowser>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

#include "server.h"


class TCPServer : public QDialog
{
    Q_OBJECT

public:
    TCPServer(QWidget *parent = 0);
    ~TCPServer();

private:
    QTextBrowser *textBrowser;
    QLabel *portLabel;
    QLineEdit *portLineEdit;
    QPushButton *createBtn;
    QGridLayout *mainLayout;

    QLabel *textLabel;

    int port;
    Server *server;

public slots:
    void slotCreateServer();
    void updataServer(QString,int);

    void autoScroll();
};

#endif // TCPSERVER_H
