#ifndef CARDITEM_H
#define CARDITEM_H

#include "define.h"

namespace Ui {
class CardItem;
}

class CardItem : public QWidget
{
    Q_OBJECT

public:
    explicit CardItem(QWidget *parent = 0);
    ~CardItem();

    void SetCardState(QString pState);

signals:
    void sCardLog();
    void sCardRepair();
    void sCardLoss();
    void sCardLogOut();

private slots:
    void on_btn_log_clicked();
    void on_btn_act_clicked();

    void on_btn_LogOut_clicked();

private:
    Ui::CardItem *ui;
};

#endif // CARDITEM_H
