#ifndef SRNSAVEFORM_H
#define SRNSAVEFORM_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class SrnSaveForm;
}

class SrnSaveForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit SrnSaveForm(QWidget *parent = 0);
    ~SrnSaveForm();
    
    QLabel *GetLogoLabel();

private:
    Ui::SrnSaveForm *ui;
};

#endif // SRNSAVEFORM_H
