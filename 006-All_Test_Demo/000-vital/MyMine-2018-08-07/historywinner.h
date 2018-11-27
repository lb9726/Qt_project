#ifndef HISTORYWINNER_H
#define HISTORYWINNER_H

#include <QDialog>

namespace Ui {
class HistoryWinner;
}

class HistoryWinner : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryWinner(QWidget *parent = 0);
    ~HistoryWinner();

private:
    Ui::HistoryWinner *ui;
};

#endif // HISTORYWINNER_H
