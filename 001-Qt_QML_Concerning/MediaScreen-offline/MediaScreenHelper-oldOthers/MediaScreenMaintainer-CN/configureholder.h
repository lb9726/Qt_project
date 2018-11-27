#ifndef CONGFIGUREHOLDER_H
#define CONGFIGUREHOLDER_H

#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMap>
#include <windows.h>

class ConfigureHolder : public QObject
{
    Q_OBJECT
public:
    explicit ConfigureHolder(QObject *parent = 0);
    bool copyReourceFile(QString src, QString dst);
signals:
    void copyFileProgress(int pro);

public slots:
    bool createXml(QString dir);
    void orderFile(QString dir);
    void setApkPath(QString apaPath);
public:
    QString apkPath;
    QString usbPath;
    QString GetFilePath(QString pFilePath);
};

#endif // CONGFIGUREHOLDER_H
