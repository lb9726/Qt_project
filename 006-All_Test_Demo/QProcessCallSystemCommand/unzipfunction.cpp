#include "unzipfunction.h"
#include <QString>
#include <QDebug>
#include <QTime>

UnZipFunction::UnZipFunction(QObject *parent) : QObject(parent)
{
    connect(&m_ZipProcess, SIGNAL(finished(int)), this, SLOT(sltUnZipFinished(int)));
    connect(&m_ZipProcess, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(sltOcurrErrors(QProcess::ProcessError)));
    connect(&m_ZipProcess, SIGNAL(started()), this, SLOT(sltStarted()));
    generateAscendRandomNumber();
    connect(&m_timer, &QTimer::timeout, this, &UnZipFunction::sltTimeOut);
}

void UnZipFunction::generateAscendRandomNumber()
{
    int i;
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    for(i = 0; i < 80; ++i) //生成100个大小在[0, 100]之间的随机数
    {
        numbersList.append(qrand()%80);
    }
    numbersList.append(100);
    //递增排序
    std::stable_sort(numbersList.begin(), numbersList.end());
    for(i = 0; i < 80; ++i) //生成80个大小在[0, 80]之间的随机数
    {
        qDebug()<<__PRETTY_FUNCTION__<<"num = "<<numbersList.at(i);
    }
}

void UnZipFunction::startUnZip(QString str)
{
    // unzip -q /home/libo/Desktop/Sodimas.zip -d /home/libo/Desktop/
    QString cmd = QString("unzip -q /home/ftp/pub/%1 -o -d /usr/bst/").arg(str);
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"str = "<<str;
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"cmd = "<<cmd;
    m_ZipProcess.start(cmd);
}

void UnZipFunction::sltStarted()
{
    qDebug()<<__PRETTY_FUNCTION__<<"cmd is executable started";
}

void UnZipFunction::sltUnZipFinished(int code)
{
    qDebug()<<__PRETTY_FUNCTION__<<"cmd is executable finished"<<" code = "<<code;
    m_ZipProcess.close();
}

void UnZipFunction::sltOcurrErrors(QProcess::ProcessError error)
{
    qDebug()<<__PRETTY_FUNCTION__<<"error = "<<error;
    //    m_ZipProcess.close();
}

void UnZipFunction::sltTimeOut()
{

}
