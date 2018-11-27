#ifndef MYMSGBOX_H
#define MYMSGBOX_H

#include <QDialog>

namespace Ui {
class MyMsgBox;
}

class MyMsgBox : public QDialog
{
    Q_OBJECT

public:
    explicit MyMsgBox(QWidget *parent = 0);
    ~MyMsgBox();
    void initMsgDailog();
    void setTitleName(QString pTitleName);
    void setTextContent(QString pText);
private slots:
    void sltBtnClicked();
signals:
    void sigBtnSelect(QString pYes);

private:
    Ui::MyMsgBox *ui;
    QString mContentText;
    QString mTitleName;
};

#endif // MYMSGBOX_H
