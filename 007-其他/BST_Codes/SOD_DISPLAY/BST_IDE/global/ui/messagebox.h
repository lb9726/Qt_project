#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include "globalfun.h"
#include <QDialog>

namespace Ui {
class MessageBox;
}

class MsgBox : public QDialog
{
    Q_OBJECT
    
public:
    //>@pCancel表示cancel按钮是否无效
    MsgBox(QString pMsg, bool pCancel=false, MSG_LEVEL pLevel = LEVEL_ERROR, QWidget *parent = 0);
    ~MsgBox();

    void SetText(QString pCancel, QString pOK);

public slots:
    void slot_Reject();
    void slot_Accept();

private:
    Ui::MessageBox *ui;
};

#endif // MESSAGEBOX_H

