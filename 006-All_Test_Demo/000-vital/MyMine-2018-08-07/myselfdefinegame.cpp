#include "myselfdefinegame.h"
#include "ui_myselfdefinegame.h"
#include "logcat.h"

MySelfDefineGame::MySelfDefineGame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MySelfDefineGame)
{
    ui->setupUi(this);
    initWindow();
}

void MySelfDefineGame::initWindow()
{
    connect(ui->mineCounts, &QLineEdit::textChanged, this, &MySelfDefineGame::sltMineCountsChanged);
    connect(ui->mineRow, &QLineEdit::textChanged, this, &MySelfDefineGame::sltRowsChanged);
    connect(ui->MineColumn, &QLineEdit::textChanged, this, &MySelfDefineGame::sltColumnChanged);
    connect(ui->btnCancle, &QPushButton::clicked, this, &MySelfDefineGame::sltBtnCancled);
    connect(ui->btnNewGame, &QPushButton::clicked, this, &MySelfDefineGame::sltBtnNewGame);
    connect(this, &MySelfDefineGame::sigNewGameEnableChanged, this, &MySelfDefineGame::sltBtnNewGameEnableChanged);
}

MySelfDefineGame::~MySelfDefineGame()
{
    delete ui;
}

void MySelfDefineGame::sltMineCountsChanged()
{
    mMines = ui->mineCounts->text().toInt();
    emit sigNewGameEnableChanged();
    MYDebug("mMines = %d", mMines);
}

void MySelfDefineGame::sltRowsChanged()
{
    mRows = ui->mineRow->text().toInt();
    emit sigNewGameEnableChanged();
    MYDebug("mRows = %d", mRows);
}

void MySelfDefineGame::sltColumnChanged()
{
    mColumns = ui->MineColumn->text().toInt();
    emit sigNewGameEnableChanged();
    MYDebug("mColumns = %d", mColumns);
}

void MySelfDefineGame::sltBtnCancled()
{

}

void MySelfDefineGame::sltBtnNewGame()
{
    if (0 == mColumns || 0 == mRows || 0 == mMines)
    {

    }
    else
    {
        ui->btnNewGame->setEnabled(false);
    }
}

void MySelfDefineGame::sltBtnNewGameEnableChanged()
{
    if (!ui->mineRow->text().isEmpty()
        && !ui->MineColumn->text().isEmpty()
        && !ui->mineCounts->text().isEmpty())
    {
        ui->btnNewGame->setEnabled(true);
    }
    else
    {
        ui->btnNewGame->setEnabled(false);
    }
}

int MySelfDefineGame::getMines() const
{
    return mMines;
}

int MySelfDefineGame::getColumns() const
{
    return mColumns;
}

int MySelfDefineGame::getRows() const
{
    return mRows;
}
