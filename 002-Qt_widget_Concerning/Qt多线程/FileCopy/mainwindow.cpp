#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include "copythread.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    copyThread=new CopyThread;
    connect(copyThread,SIGNAL(copyStationSig(int,QString)),this,SLOT(copyStation(int,QString)));
}

MainWindow::~MainWindow()
{
    copyThread->copyStop();
    copyThread->wait();

    delete ui;
}

void MainWindow::on_selectSrcBtn_clicked()
{
    QString dirPath;

    dirPath = QFileDialog::getExistingDirectory(this,
                 tr("Open Directory"),QDir::currentPath(),
                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dirPath.isEmpty()) return;
    ui->srcLineEdit->setText(dirPath.toUtf8());
}

void MainWindow::on_selectDesBtn_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this,
                  tr("Open Directory"),QDir::currentPath(),
                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dirPath.isEmpty()) return;

    ui->desLineEdit->setText(dirPath.toUtf8());
}

void MainWindow::on_startBtn_clicked()
{
    if(ui->srcLineEdit->text().isEmpty())
    {
        ui->staLab->setText("请选择文件夹路径！");
        return;
    }
    else if(ui->desLineEdit->text().isEmpty())
    {
        ui->staLab->setText("请选择目标文件夹路径！");
        return;
    }
    else if(ui->srcLineEdit->text()==ui->desLineEdit->text())
    {
        ui->staLab->setText("路径不能相同！");
        return;
    }

    int nRet;
    QString qsTest;

    copyThread->setSrcDirPath(ui->srcLineEdit->text());
    copyThread->setDesDirPath(ui->desLineEdit->text());

    nRet=copyThread->copyStart();

    switch(nRet)
    {
        case ERROR_SRC_PATH_NULL: qsTest="文件夹路径不存在！"; break;
        case ERROR_DES_PATH_NULL: qsTest="目标文件夹路径不存在！"; break;
        case ERROR_NO_FILES: qsTest="文件夹下没有jpg、avi文件！"; break;
    }

    if(nRet != SUCCESS) ui->staLab->setText(qsTest.toUtf8());
    else
    {
        ui->selectSrcBtn->setEnabled(false);
        ui->selectDesBtn->setEnabled(false);
        ui->startBtn->setEnabled(false);
    }
}

void MainWindow::copyStation(int flag,QString qsText=NULL)
{
    switch(flag)
    {
        case COPY_START: ui->staLab->setText("开始复制..."); break;

        case COPY_STOP:
        ui->staLab->setText("复制结束...");
        ui->startBtn->setEnabled(true);
        ui->selectSrcBtn->setEnabled(true);
        ui->selectDesBtn->setEnabled(true);
        break;

        case COPY_FILE_NAME: ui->staLab->setText(qsText); break;

        case COPY_TOTAL_RATE:
        ui->totalProgressBar->setValue(qsText.toInt());
        break;

        case COPY_SINGLE_RATE:
        ui->sigProgressBar->setValue(qsText.toInt());
        break;

        case COPY_ERROR_MEM_FULL:
        ui->staLab->setText("存储空间已满！");
        break;
    }
}
