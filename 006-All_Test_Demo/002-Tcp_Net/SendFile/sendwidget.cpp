#include "sendwidget.h"
#include "ui_sendwidget.h"
#include <QFileDialog>
#include <QHostAddress>

SendWidget::SendWidget(QWidget *      parent) :
    QMainWindow(parent),
    ui(new Ui::SendWidget)
{
    ui->setupUi(this);

    loadSize = 4*1024;
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
    tcpClient = new QTcpSocket(this);
    connect(tcpClient, SIGNAL(connected()), this, SLOT(startTransfer()));
    //当连接服务器成功时，发出connected()信号，我们开始传送文件
    connect(tcpClient, SIGNAL(bytesWritten(qint64)), this, SLOT(updateClientProgress(qint64)));
    //当有数据发送成功时，我们更新进度条
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    ui->sendButton->setEnabled(false);
    ui->hostLineEdit->setText("127.0.0.1");
    ui->portLineEdit->setText("6111");
}

SendWidget::~SendWidget()
{
    delete ui;
}

void SendWidget::openFile()   //打开文件
{
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        ui->sendButton->setEnabled(true);
        ui->clientStatusLabel->setText(tr("打开文件 %1 成功！").arg(fileName));
    }
}


void SendWidget::send()   //连接到服务器，执行发送
{
    ui->sendButton->setEnabled(false);
    bytesWritten = 0;
    //初始化已发送字节为0
    ui->clientStatusLabel->setText(tr("连接中…"));
    tcpClient->connectToHost(QHostAddress(ui->hostLineEdit->text()), ui->portLineEdit->text().toInt());//连接
}


void SendWidget::startTransfer()  //实现文件大小等信息的发送
{
    localFile.setFileName(fileName);
    qDebug()<<__PRETTY_FUNCTION__<<"new QFile";
    if(!localFile.open(QFile::ReadOnly))
    {
        qDebug() << "open file error!";
        return;
    }
    totalBytes = localFile.size();
    //文件总大小
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_7);
    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    sendOut << qint64(0) << qint64(0) << currentFileName;
    //依次写入总大小信息空间，文件名大小信息空间，文件名---仅是文件名不含路径，没必要含路径
    totalBytes += outBlock.size();
    //这里的总大小是文件名大小等信息和实际文件大小的总和
    sendOut.device()->seek(0);
    sendOut<<totalBytes<<qint64((outBlock.size() - sizeof(qint64)*2));
    //totalBytes是文件总大小，即两个quint64的大小+文件名+文件实际内容的大小
    //qint64((outBlock.size() - sizeof(qint64)*2))得到的是文件名大小
    bytesToWrite = totalBytes - tcpClient->write(outBlock);
    //发送完头数据后剩余数据的大小，即文件实际内容的大小
    ui->clientStatusLabel->setText(tr("已连接"));
    outBlock.resize(0);
    qDebug()<<"#####"<<totalBytes;
}

void SendWidget::updateClientProgress(qint64 numBytes) //更新进度条，实现文件的传送
{
    qDebug()<<"#######已发送："<<bytesWritten<<"剩余："<<bytesToWrite;
    bytesWritten += (int)numBytes;
    //已经发送数据的大小
    if(bytesToWrite > 0) //如果已经发送了数据
    {
        outBlock = localFile.read(qMin(bytesToWrite, loadSize));
      //每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，
      //就发送剩余数据的大小
        bytesToWrite -= (int)tcpClient->write(outBlock);
       //发送完一次数据后还剩余数据的大小
        outBlock.resize(0);
       //清空发送缓冲区
    }
    else
    {
        localFile.close(); //如果没有发送任何数据，则关闭文件
    }
    ui->clientProgressBar->setMaximum(totalBytes);
    ui->clientProgressBar->setValue(bytesWritten);
    //更新进度条
    if(bytesWritten == totalBytes) //发送完毕
    {
        ui->clientStatusLabel->setText(tr("传送文件 %1 成功").arg(fileName));
        localFile.close();
        tcpClient->close();
        tcpClient->deleteLater();
    }
}

void SendWidget::displayError(QAbstractSocket::SocketError) //显示错误
{
    qDebug() << tcpClient->errorString();
    tcpClient->close();
    ui->clientProgressBar->reset();
    ui->clientStatusLabel->setText(tr("客户端就绪"));
    ui->sendButton->setEnabled(true);
}

void SendWidget::on_sendButton_clicked()
{
    send();
}

void SendWidget::on_openButton_clicked()
{
    openFile();
}
