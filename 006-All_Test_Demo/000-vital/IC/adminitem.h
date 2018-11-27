#ifndef ADMINITEM_H
#define ADMINITEM_H

#include <QWidget>

namespace Ui {
class AdminItem;
}

class AdminItem : public QWidget
{
    Q_OBJECT

public:
    explicit AdminItem(QWidget *parent = 0);
    ~AdminItem();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_btn_destory_clicked();

    void on_pushButton_3_clicked();

signals:
    void sMadeCardsNum();
    void sMakeAdminCard();
    void sDestoryCard();
    void setPassword();

private:
    Ui::AdminItem *ui;
};

#endif // ADMINITEM_H
