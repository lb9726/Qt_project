#ifndef FILE_SYSTEM_WATCHER_H
#define FILE_SYSTEM_WATCHER_H

#include <QObject>
#include <QMap>
#include <QFileSystemWatcher>
#include <QDebug>

class FileSystemWatcher : public QObject
{
    Q_OBJECT

public:
    static void addWatchPath(QString path);
    ~FileSystemWatcher();

public slots:
    void directoryUpdated(const QString &path);  // 目录更新时调用，path是监控的路径
    void fileUpdated(const QString &path);   // 文件被修改时调用，path是监控的路径

private:
    explicit FileSystemWatcher(QObject *parent = 0);
    class GarbageCollection
    {
    public:
        ~GarbageCollection()
        {
            if (m_pInstance)
            {
                delete m_pInstance;
                m_pInstance = NULL;
                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"delete";
            }
        }
    };

private:
    static FileSystemWatcher *m_pInstance; // 单例
    QFileSystemWatcher *m_pSystemWatcher;  // QFileSystemWatcher变量
    QMap<QString, QStringList> m_currentContentsMap; // 当前每个监控的内容目录列表
    static GarbageCollection mGc;
};

#endif // FILE_SYSTEM_WATCHER_H
/*
---------------------
作者：一去丶二三里
来源：CSDN
原文：https://blog.csdn.net/liang19890820/article/details/51849252
版权声明：本文为博主原创文章，转载请附上博文链接！
*/
