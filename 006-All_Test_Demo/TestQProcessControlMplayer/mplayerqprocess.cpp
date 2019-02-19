#include "mplayerqprocess.h"
#include <QDebug>
#include <QCoreApplication>

MPlayerQProcess::MPlayerQProcess(QObject *parent) : QObject(parent)
{    
    lastVolume = 10;
    connectSignalAndSlot();     /* 连接信号和槽函数 */
    initParameter();            /* 初始化所有参数 */
    connect(&mTimer1, &QTimer::timeout, this, &MPlayerQProcess::onTimeOut);
    mTimer1.setInterval(10000);
    mTimer1.setSingleShot(true);
    mTimer1.start();
    mRunPath = QCoreApplication::applicationDirPath();
    if (!mRunPath.endsWith("/"))
    {
        mRunPath += "/";
    }
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<mRunPath;
}

void MPlayerQProcess::stop()
{

}

void MPlayerQProcess::play()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    QString cmd = QString("loadfile ") + mBackGroundMusicPath + QString("\n");
    qDebug()<<"cmd = "<<cmd;
    int flag = mBackGroundProcess.write(cmd.toLocal8Bit().constData());
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"flag = "<<flag;
    mBackGroundProcess.waitForBytesWritten();
}

void MPlayerQProcess::pause()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<< mBackGroundMuiscStatus ;
    if (mBackGroundMuiscStatus.contains("play"))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
        mBackGroundMuiscStatus = QString("pause\n");
        mBackGroundProcess.write(QString("pause\n").toLocal8Bit().constData());
    }
    else if (mBackGroundMuiscStatus.contains("pause"))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
        mBackGroundMuiscStatus = QString("play\n");
        mBackGroundProcess.write(QString("pause\n").toLocal8Bit().constData());
    }
}

void MPlayerQProcess::setMusicVolume(int pVolume)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"Volume set "<<pVolume;
    QString volumeCmd = QString("volume %1 1\n").arg(pVolume);
    mBackGroundProcess.write(volumeCmd.toLocal8Bit().constData());
}

void MPlayerQProcess::connectSignalAndSlot()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"called";
    connect(&mBackGroundProcess, &QProcess::started, this, &MPlayerQProcess::onStartStatus);
    connect(&mBackGroundProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onFinished(int,QProcess::ExitStatus)));
    connect(&mBackGroundProcess, &QProcess::errorOccurred, this, &MPlayerQProcess::onOccurError);
    connect(&mBackGroundProcess, &QProcess::readyReadStandardOutput, this, &MPlayerQProcess::onReadyReadStandardOutput);
    connect(&mLoopMusicProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onFinished(int,QProcess::ExitStatus)));
    connect(&mLoopMusicProcess, &QProcess::errorOccurred, this, &MPlayerQProcess::onOccurError);
}

void MPlayerQProcess::initParameter()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    mBackGroundLoopFlag = true;
    mLoopMusicFlag  = true;
    mBackGroundMuiscStatus = QString("play\n");
#if defined(UBUTUN14_04)
    mCmd = mRunPath + QString("mplayer");
    mBackGroundMusicPath = mRunPath + "M0.wav";
    mLoopMusicPath = mRunPath + "M43.wav";
#else
    mCmd = QString("mplayer");
    mBackGroundMusicPath = QString("/usr/bst/usrfs/Theme/sodimas/music/M0.wav");
    mLoopMusicPath = QString("/usr/bst/usrfs/Theme/sodimas/media/M43.wav");
#endif
    QStringList arguments;
    arguments << QString("-slave");
    arguments << QString("-quiet");
    arguments << QString("-ao");
    arguments << QString("alsa");
    arguments << QString("-volume");
    arguments << QString::number(lastVolume);
//    arguments << QString("null");
    arguments << mBackGroundMusicPath;
    mArgumentsList = arguments;

    mBackGroundProcess.start(mCmd, mArgumentsList);
    mBackGroundProcess.waitForStarted();

    lastVolume = 40;
//    play();

//    mLoopMusicProcess.start(mCmd, mArgumentsList);
    //    mLoopMusicProcess.waitForStarted();
}

void MPlayerQProcess::onStartStatus()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"isStart";
}

void MPlayerQProcess::onFinished(int exitCode, QProcess::ExitStatus pErrorNo)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"exitCode = "<<exitCode;
    switch (pErrorNo) {
    case QProcess::NormalExit:
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<" NormalExit";
        break;
    case QProcess::CrashExit:
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<" CrashExit";
        break;
    default:
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<" other ExitStatus";
        break;
    }
//    if (mBackGroundMuiscStatus.contains("play"))
//    {

//    }
//    mBackGroundProcess.start(mCmd, mArgumentsList);
//    mBackGroundProcess.waitForStarted();
    mBackGroundProcess.start(mCmd, mArgumentsList);
    mBackGroundProcess.waitForStarted();
//    setMusicVolume(10);
}

void MPlayerQProcess::onTimeOut()
{
//    static int flag = -10;
    static int flag = -1;
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<" flag = "<<flag;
    if (-1 == flag) //  第一次播放音乐
    {
//        play();
        setMusicVolume(10);
        ++flag;
    }
    if (0 == flag) // 设置音量20
    {
        ++flag;
        setMusicVolume(20);
        mTimer1.start();
    }
    else if (1 == flag) // 设置暂停
    {
        setMusicVolume(30);
        ++flag;
        mTimer1.start();
    }
    else if (2 == flag) // 设置音量40
    {
        setMusicVolume(15);
        mTimer1.start();
        ++flag;
    }
    else if (3 == flag)
    {
        setMusicVolume(40);
        mTimer1.start();
        flag = 0;
//        ++flag;
    }
//    else if (4 == flag)
//    {
//        pause();
//        mTimer1.start();
//        ++flag;
//    }
//    else
//    {
//        pause();
//        flag = 0;
//        mTimer1.start();
    //    }
}

void MPlayerQProcess::onReadyReadStandardOutput()
{
    while (mBackGroundProcess.canReadLine())
    {
        QString output(mBackGroundProcess.readLine());

        if (output.contains("End of file"))
        {
            qDebug() << "output" << output;
        }
//        else
//        {
//            if (output != QString("\n"))
//            {
//                qDebug() << "output" << output;
//            }
//        }
    }
}

void MPlayerQProcess::onOccurError(QProcess::ProcessError pErrorNo)
{
    switch (pErrorNo) {
    case QProcess::FailedToStart:
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"error FailedToStart";
        break;
    case QProcess::Crashed:
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"error Crashed";
        break;
    case QProcess::Timedout:
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"error Timedout";
        break;
    case QProcess::WriteError:
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"error WriteError";
        break;
    case QProcess::ReadError:
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"error ReadError";
        break;
    case QProcess::UnknownError:
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"error UnknownError";
        break;
    default:
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"error others";
        break;
    }
}
