#ifndef QUERY_H
#define QUERY_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class query;
}

class query : public QDialog
{
    Q_OBJECT

public:
    explicit query(QWidget *parent = 0);
    ~query();

private:
    Ui::query *ui;
    int readFromFile();
     QList<QString> win_lines;
     QStandardItemModel *win_model;
     void doQuery();
     void sdispaly(QStringList *line, int no);
};

#endif // QUERY_H
