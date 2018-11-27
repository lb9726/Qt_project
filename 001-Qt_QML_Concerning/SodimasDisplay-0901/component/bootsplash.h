#ifndef BOOTSPLASH_H
#define BOOTSPLASH_H

#include <QObject>
#include "define.h"
#include "globalfun.h"

class BootSplash : public QObject
{
    Q_OBJECT
public:
    explicit BootSplash(QObject *parent = 0);

signals:

public slots:
    Q_INVOKABLE QString getAppVersion();
    Q_INVOKABLE QString getBootLogoPath();
    Q_INVOKABLE QString getBackgroundPath();
public:
    QString          m_appVersion;
    QString          m_bootLogoPath;
    QString          m_backgroundPath;



};

#endif // BOOTSPLASH_H
