#include "bootsplash.h"

BootSplash::BootSplash(QObject *parent) :
    QObject(parent)
{
    m_appVersion = "";
    m_bootLogoPath = "";
    m_backgroundPath = "";
}

QString BootSplash::getAppVersion(){
    IDE_TRACE_STR(m_appVersion);
    return m_appVersion;
}

QString BootSplash::getBootLogoPath(){
    IDE_TRACE_STR(m_bootLogoPath);
    return m_bootLogoPath;
}

QString BootSplash::getBackgroundPath(){
    return m_backgroundPath;
}


