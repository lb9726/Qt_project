#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDir>
#include "define.h"

QString GetDirectoryName(QString pDirPath);
QString getFileDirectory(QString pFilePath);
bool CreatPath(QString pPath);
QStringList ErgodicDirectory(QString d);
QString getFileName(QString pFilePath);
QString getParentDirectory(QString pDirPath);
QString  BiteorMega(int peso);
#endif // GLOBAL_H
