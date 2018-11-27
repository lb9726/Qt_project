#include "textinput.h"
#include "ui_textinput.h"

TextInput::TextInput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextInput)
{
    ui->setupUi(this);

    connect(ui->btn_ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->btn_cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

TextInput::~TextInput()
{
    delete ui;
}

QString TextInput::GetInput()
{
    return ui->edit_text->text();
}
