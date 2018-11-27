#include "threaddlg.h"
#include <QHBoxLayout>
#include <QDebug>
ThreadDlg::ThreadDlg(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("线程"));

    startBtn = new QPushButton(tr("start"));
    stopBtn = new QPushButton(tr("stop"));
    quitBtn = new QPushButton(tr("exit"));

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(startBtn);
    mainLayout->addWidget(stopBtn);
    mainLayout->addWidget(quitBtn);

    connect(startBtn,SIGNAL(clicked()),this,SLOT(slotStart()));
    connect(stopBtn,SIGNAL(clicked()),this,SLOT(slotStop()));
    connect(quitBtn,SIGNAL(clicked()),this,SLOT(close()));
}

ThreadDlg::~ThreadDlg()
{
//    qDebug()<<"is xigou function";
//    delete []workThread;
}

void ThreadDlg::slotStart()
{
    for(int i = 0; i < MAXSIZE; i++)
    {
        workThread[i] = new WorkThread();
        workThread[i]->id = i;//qDebug()<<"id = "<<workThread[i]->id<<'\n';
    }
    for(int i = 0; i < MAXSIZE; i++)
    {
        workThread[i]->start();
    }
    startBtn->setEnabled(false);
    stopBtn->setEnabled(true);
}

void ThreadDlg::slotStop()
{
    for(int i = 0; i < MAXSIZE; i++)
    {
        workThread[i]->terminate();
        workThread[i]->wait();
    }
    startBtn->setEnabled(true);
    stopBtn->setEnabled(false);
}
