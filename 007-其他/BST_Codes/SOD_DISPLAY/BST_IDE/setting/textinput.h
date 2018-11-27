#ifndef TEXTINPUT_H
#define TEXTINPUT_H

#include "global.h"

namespace Ui {
class TextInput;
}

class TextInput : public QDialog
{
    Q_OBJECT
    
public:
    explicit TextInput(QWidget *parent = 0);
    ~TextInput();

    QString GetInput();

public:
    Ui::TextInput *ui;
};

#endif // TEXTINPUT_H

