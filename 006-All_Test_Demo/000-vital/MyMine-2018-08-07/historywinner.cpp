#include "historywinner.h"
#include "ui_historywinner.h"

HistoryWinner::HistoryWinner(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistoryWinner)
{
    ui->setupUi(this);
}

HistoryWinner::~HistoryWinner()
{
    delete ui;
}
