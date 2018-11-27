#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnOpen_clicked();
    //读取串口数据
    void readComDataSlot();

    void on_btnSend_clicked();

private:
    Ui::MainWindow *ui;
    //实例化串口指针，用来对串口进行操作
    QSerialPort * my_serialport;
    QTimer *timer;

    //初始化串口信息
    void initPort();

    //设置控件的状态
    void setSelectable();
    void setNonSelectable();
};

#endif // MAINWINDOW_H
