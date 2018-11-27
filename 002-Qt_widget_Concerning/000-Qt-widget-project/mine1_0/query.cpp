#include "query.h"
#include "ui_query.h"
#include <QFile>
#include <QMessageBox>

query::query(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::query)
{
    ui->setupUi(this);
    if(-1 == readFromFile())
        QMessageBox::warning(this,"erro","open file fail !!");
    win_model = new QStandardItemModel;
    ui->tableView->setModel(win_model);
    doQuery();
}

query::~query()
{
    delete ui;
}

int query::readFromFile()
{
    QFile file("gamewin.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return -1;
    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        win_lines.append(line);
    }
    return 0;
}

void query::doQuery()
{
    win_model->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("姓名 ")));
    win_model->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("雷区大小")));
    win_model->setHorizontalHeaderItem(2,new QStandardItem(QObject::tr("雷数")));
    win_model->setHorizontalHeaderItem(3,new QStandardItem(QObject::tr("用时")));

    ui->tableView->setColumnWidth(0,100);
    ui->tableView->setColumnWidth(1,200);
    ui->tableView->setColumnWidth(2,100);
    ui->tableView->setColumnWidth(3,100);
    int no = 0;
    for(int i = 0; i< win_lines.size(); ++i)
    {
        QString line = win_lines.at(i);
        QStringList subs = line.split(" ");
        sdispaly(&subs,no++);
    }
}

void query::sdispaly(QStringList *line, int no)
{
    for(int i = 0; i<4 ;i++)
    {
        win_model->setItem(no,i,new QStandardItem(line->at(i)));
    }
}
