#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include "qtuart.h"
#include <QButtonGroup>
#include <menuitem.h>
#include <QFile>
#include <QTimer>

#define GBKToUnicode(x)                         QTextCodec::codecForName("GBK")->toUnicode(x)
#define GBKToUTF8(x)                            QTextCodec::codecForName("UTF-8")->fromUnicode(GBKToUnicode(x)).data()

namespace Ui {
class MainWindow;
}

void QtSleep(unsigned int msec, QEventLoop::ProcessEventsFlags flags = QEventLoop::AllEvents);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QByteArray      mOldRead;

    QSignalMapper   mMapper;
    QSignalMapper   mMapperPassWd;
    SerialPort*     mPort;

    QTimer*         timer;
    QTimer*         timer1;
    QTimer*         timer2;
    QTimer*         timer3;
    QTimer*         timer4;
    QTimer*         timer5;

    QFile*          mWDFile;  //watchdog
    QTimer*         timer_Wd; //watchdog

    QButtonGroup   mMenuGroup;

    QString        mSetFile;
    QStringList    mEnterList;
    QStringList    mDelList;
    QStringList    m4KeyList;

    int setValue(int minValue, int maxValue, int pValue);
    int SetLuminance(int pValue);

public slots:
    void slot_Read();
    void slot_BtnClicked(int id);
    void slot_BtnPassWdClicked(int id);

    void slot_CallEleTimeout();
    void slot_AutoCall();
    void slot_inputTips();
    void slot_ClearOldArray();
    void slot_MenuPage(int id);

    void slot_ArrowSelected();
    void slot_KeySelected();

    void slot_GotoPage();
    void slot_BackHome();
    void slot_KeyChanged(QString pPic);
    void slot_DirArrChanged(QString pDirArrPic);
    void slot_wd();  //watchdog

private slots:
    void on_btn_0_released();

    void on_btn_back_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
