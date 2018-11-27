#ifndef MYSELFDEFINEGAME_H
#define MYSELFDEFINEGAME_H

#include <QDialog>

namespace Ui {
class MySelfDefineGame;
}

class MySelfDefineGame : public QDialog
{
    Q_OBJECT

public:
    explicit MySelfDefineGame(QWidget *parent = 0);
    ~MySelfDefineGame();
    int getRows() const;
    int getColumns() const;
    int getMines() const;

signals:
    void sigNewGameEnableChanged();
private:
    void initWindow();

private slots:
    void sltMineCountsChanged();
    void sltRowsChanged();
    void sltColumnChanged();
    void sltBtnCancled();
    void sltBtnNewGame();
    void sltBtnNewGameEnableChanged();

private:
    Ui::MySelfDefineGame *ui;
    int mRows;
    int mColumns;
    int mMines;
};

#endif // MYSELFDEFINEGAME_H
