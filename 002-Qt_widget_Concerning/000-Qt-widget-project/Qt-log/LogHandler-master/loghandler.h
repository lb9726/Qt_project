#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include <QDebug>
#include "Singleton.h"
#include <QTimer>
#include <QDir>
#include <QTextStream>
#include <QMutexLocker>
#include <QtGlobal>
#include <QDateTime>

struct LogHandlerPrivate {
    LogHandlerPrivate();
    ~LogHandlerPrivate();

    void openAndBackupLogFile();
    // 消息处理函数
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    // 如果日志所在目录不存在，则创建
    void makeSureLogDirectory() const;
    void checkFile(QDir logDir, QString &logPath);

    QDir   logDir;              // 日志文件夹
    QTimer renameLogFileTimer;  // 重命名日志文件使用的定时器
    QTimer flushLogFileTimer;   // 刷新输出到日志文件的定时器
    QDate  logFileCreatedDate;  // 日志文件创建的时间
    static QFile *logFile;      // 日志文件
    static QTextStream *logOut; // 输出日志的 QTextStream，使用静态对象就是为了减少函数调用的开销
    static QMutex logMutex;     // 同步使用的 mutex
};


class LogHandler {
    SINGLETON(LogHandler)       // 使用单例模式

public:
    void release();             // 释放资源
    void installMsgHandler();   // 给 Qt 安装消息处理函数

private:
    LogHandlerPrivate *d;
};

#endif // LOGHANDLER_H
