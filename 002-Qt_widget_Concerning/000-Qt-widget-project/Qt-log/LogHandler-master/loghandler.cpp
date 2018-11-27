#include "LogHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <QFile>
#include <QFileInfo>
#include <iostream>
#include <QSettings>

// 初始化 static 变量
QMutex LogHandlerPrivate::logMutex;
QFile* LogHandlerPrivate::logFile = NULL;
QTextStream* LogHandlerPrivate::logOut = NULL;


LogHandlerPrivate::LogHandlerPrivate() {

    logDir.setPath("Log"); // 日志文件夹的路径，为 exe 所在目录下的 log 文件夹，可从配置文件读取
    QString logPath = logDir.absoluteFilePath("log.txt"); // 日志的路径
    // 日志文件创建的时间
    // QFileInfo::created(): On most Unix systems, this function returns the time of the last status change.
    // 所以不能运行时使用这个函数检查创建时间，因为会在运行时变化，所以在程序启动时保存下日志文件创建的时间
    // 在 Windows 又遇到一 Bug，log.txt 的创建时间总是过去的某一天，就算新创建的也不会变，其实这里使用
    // lastModified() 代替 created() 也是可以的，因为这个时间在程序启动时记录的
    logFileCreatedDate = QFileInfo(logPath).created().date();

    // 打开日志文件，如果不是当天创建的，备份已有日志文件
    openAndBackupLogFile();

    // 十分钟检查一次日志文件创建时间
    renameLogFileTimer.setInterval(1000 * 60 * 10); // TODO: 可从配置文件读取
    renameLogFileTimer.start();
    QObject::connect(&renameLogFileTimer, &QTimer::timeout, [this] {
        QMutexLocker locker(&LogHandlerPrivate::logMutex);
        openAndBackupLogFile();
    });

    flushLogFileTimer.setInterval(1000);
    flushLogFileTimer.start();
    QObject::connect(&flushLogFileTimer, &QTimer::timeout, [this] {
        QMutexLocker locker(&LogHandlerPrivate::logMutex);
        if (NULL != logOut) {
            logOut->flush();
        }
    });
}

LogHandlerPrivate::~LogHandlerPrivate() {
    if (NULL != logFile) {
        logFile->flush();
        logFile->close();
        delete logOut;
        delete logFile;
        // 因为他们是 static 变量
        logOut  = NULL;
        logFile = NULL;
    }
}

// 打开日志文件 log.txt，如果不是当天创建的，则使用创建日期把其重命名为 yyyy-MM-dd.log，并重新创建一个 log.txt
void LogHandlerPrivate::openAndBackupLogFile() {
    makeSureLogDirectory(); // 如果日志所在目录不存在，则创建
    QString logPath = logDir.absoluteFilePath("log.txt"); // 日志的路径

    checkFile(logDir, logPath);

    // [[1]] 程序启动时 logFile 为 NULL
    if (NULL == logFile) {
        logFile = new QFile(logPath);
        qDebug()<<"create file:"<<logFile->fileName();
        logOut  = (logFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) ?  new QTextStream(logFile) : NULL;
        if (NULL != logOut) {
            logOut->setCodec("UTF-8");
        }
        if (logFileCreatedDate.isNull()) {
            logFileCreatedDate = QDate::currentDate();
        }
        qDebug()<<"create date:"<<logFileCreatedDate;
    }

    // [[3]] 程序运行时如果创建日期不是当前日期，则使用创建日期重命名，并生成一个新的 log.txt
    if (logFileCreatedDate != QDate::currentDate()) {
        logFile->flush();
        logFile->close();
        delete logOut;
        delete logFile;
        QString newLogPath = logDir.absoluteFilePath(logFileCreatedDate.toString("yyyy-MM-dd.log"));;
        QFile::copy(logPath, newLogPath); // Bug: 按理说 rename 会更合适，但是 rename 时最后一个文件总是显示不出来，需要 killall Finder 后才出现
        QFile::remove(logPath); // 删除重新创建，改变创建时间
        logFile = new QFile(logPath);
        logOut  = (logFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) ?  new QTextStream(logFile) : NULL;
        logFileCreatedDate = QDate::currentDate();
        if (NULL != logOut) {
            logOut->setCodec("UTF-8");
        }
    }
}

// 如果日志所在目录不存在，则创建
void LogHandlerPrivate::makeSureLogDirectory() const {
    if (!logDir.exists()) {
        logDir.mkpath("."); // 可以递归的创建文件夹
    }
}


// 检查日志文件是否超过配置的大小，若超过则创建新文件，若最后一个的大小也超过，则删除后重新创建
void LogHandlerPrivate::checkFile(QDir logDir, QString &logPath)
{
    QSettings setting("setting.ini", QSettings::IniFormat);
    setting.beginGroup("option");
    int size = setting.value("size", 1*1024*1024).toInt();
    int num =setting.value("num",5).toInt();
    setting.endGroup();

    QFileInfo info;
    info.setFile(logPath);
    if(info.size() > size)
    {
        for(int n=1; n< num; n++)
        {
            QString name = QString("log_%1.txt").arg(n);
            QFileInfo fi(logDir.absoluteFilePath(name));
            if(fi.exists() && fi.size()> size)
            {
                if(n== num-1)
                {
                    logDir.remove(name);
                    logPath = logDir.absoluteFilePath(name);
                }
                else continue;
            }
            else
            {
                logPath = logDir.absoluteFilePath(name);
                break;
            }
        }
    }
}

// 消息处理函数
void LogHandlerPrivate::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QMutexLocker locker(&LogHandlerPrivate::logMutex);
    QString level;
    switch (type) {
    case QtDebugMsg:
        level = "调试";
        break;
    case QtInfoMsg:
        level = "信息";
        break;
    case QtWarningMsg:
        level = "警告";
        break;
    case QtCriticalMsg:
        level = "错误";
        break;
    default:;
    }
    // 输出到标准输出
    QByteArray localMsg = msg.toLocal8Bit();
    std::cout << std::string(localMsg) << std::endl;
    if (NULL == LogHandlerPrivate::logOut)      return;

    // 输出到日志文件, 格式: 时间 - [Level] (文件名:行数, 函数): 消息
    QString fileName = context.file;
    int index = fileName.lastIndexOf(QDir::separator());
    fileName = fileName.mid(index + 1);
    (*LogHandlerPrivate::logOut) << QString("%1 - [%2] (%3:%4, %5): %6\n")
                                    .arg(QDateTime::currentDateTime().toString("MM-dd hh:mm:ss")).arg(level)
                                    .arg(fileName).arg(context.line).arg(context.function).arg(msg);
}

/************************************************************************************************************
 *                                               LogHandler                                                 *
 ***********************************************************************************************************/
LogHandler::LogHandler() : d(NULL) {
}
LogHandler::~LogHandler() {
}


void LogHandler::installMsgHandler() {
    QMutexLocker locker(&LogHandlerPrivate::logMutex);
    if (NULL == d) {
        d = new LogHandlerPrivate();
        qInstallMessageHandler(LogHandlerPrivate::messageHandler); // 给 Qt 安装自定义消息处理函数
    }
}


void LogHandler::release() {
    QMutexLocker locker(&LogHandlerPrivate::logMutex);
    qInstallMessageHandler(0);      //restore the message handler
    delete d;
    d = NULL;
}
