#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QFile>
#include <QTcpSocket>
const QString path = "/home/libo/Desktop/";

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void start();                   //开始监听
    void acceptConnection();        //建立连接
    void updateServerProgress();    //更新进度条，接收数据
    void displayError(QAbstractSocket::SocketError socketError);
    void on_startButton_clicked();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpServer tcpServer;
    QTcpSocket *tcpServerConnection;
    qint64 totalBytes;      // 存放总大小信息
    qint64 bytesReceived;   // 已收到数据的大小
    qint64 fileNameSize;    // 文件名的大小信息
    QString fileName;       // 存放文件名
    QFile localFile;        // 本地文件
    QByteArray inBlock;     // 数据缓冲区
};

#endif // MAINWINDOW_H
