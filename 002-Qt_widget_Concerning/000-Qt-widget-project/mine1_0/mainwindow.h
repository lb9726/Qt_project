#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "myscence.h"
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QString>
#include "newwiner.h"
#include "mygame.h"
#include "query.h"
#include <QTextStream>
#include <QProcess>
#include "introduce.h"
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
    void on_pushButton_clicked();

    void on_action_triggered();

    void on_action_2_triggered();
    void uptime();
    void gamevoer();    //游戏结束

    void on_action9_9_10_triggered();

    void on_action16_16_40_triggered();

    void on_action_3_triggered();

    void on_action_4_triggered();

    void on_action_5_triggered();

    void on_action_6_triggered();

    void on_action_7_triggered();

private:
    Ui::MainWindow *ui;
    myScence *sc;
    QTimer *qtimer;
    int gameflag;  //新游戏 标识
    int timeflag;    //初始化计时
    void newgame();    //新游戏
//    void newwiner();    //游戏胜利

protected:
    virtual void closeEvent(QCloseEvent * event);
};

#endif // MAINWINDOW_H
