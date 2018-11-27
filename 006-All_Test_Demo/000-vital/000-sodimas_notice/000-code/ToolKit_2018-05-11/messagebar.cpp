#include "messagebar.h"
#include "ui_messagebar.h"

MessageBar::MessageBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageBar)
{
    ui->setupUi(this);
    this->setWindowOpacity(0.5);
    m_messageLabel = ui->label;
    m_messageLabel->setWindowOpacity(0.5);
    QFont font;
    font.setPointSize(10);
    m_messageLabel->setFont(font);
}

MessageBar::~MessageBar()
{
    delete ui;
}
