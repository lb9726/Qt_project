#ifndef USRITEM_H
#define USRITEM_H

#include <QWidget>

namespace Ui {
class UsrItem;
}

class UsrItem : public QWidget
{
    Q_OBJECT

public:
    explicit UsrItem(QWidget *parent = 0);
    ~UsrItem();

signals:
    void sCardRead();
    void sMakeUsrCard();
    void sDestoryCard();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_btn_destory_clicked();

private:
    Ui::UsrItem *ui;
};

#endif // USRITEM_H
