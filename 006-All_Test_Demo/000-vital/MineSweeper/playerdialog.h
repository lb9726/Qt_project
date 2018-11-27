#ifndef PLAYERDIALOG_H
#define PLAYERDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

class playerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit playerDialog(QWidget *parent = 0);//构造函数
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *pushButtonOK;
    QGridLayout *glayout;

signals:
    void sendPlayerName(QString t_playerName);//发送玩家（信号）
    
public slots:
    void BtnOKClicked();//（槽）OK按钮被点击槽
    
};

#endif // PLAYERDIALOG_H
