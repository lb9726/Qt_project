#include "globalfun.h"

QIcon GetIcon(QPixmap pPixmap, QRect pRect)
{
    if(pPixmap.isNull())
        return QIcon();
    QPixmap icoPixmap = pPixmap.copy(pRect);
    return QIcon(icoPixmap);
}

QPixmap GetPixmap(QPixmap pPixmap, QRect pRect)
{
    if(pPixmap.isNull())
        return QPixmap();
    return pPixmap.copy(pRect);
}

void QtSleep(unsigned int msec, QEventLoop::ProcessEventsFlags flags)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents(flags, 1);
#ifdef WINDOWS
        Sleep(1);
#elif UBUNTU
        usleep(1000);//sleep和usleep都已经obsolete，建议使用nanosleep代替
#elif LINUX
        usleep(1000);
#elif IOS
        ocdelay(1);
#else
    #if 1
        QMutex mutex;
        QWaitCondition sleep;
        mutex.lock();
        sleep.wait(&mutex, 10);
        mutex.unlock();
    #else
        QEventLoop eventloop;
        QTimer::singleShot(time, &eventloop, SLOT(quit()));
        eventloop.exec();
    #endif
#endif
    }
}


bool QtWait(bool &pFlag, bool pEqlFlag, unsigned int msec)
{
    if(msec == 0)
    {
        while(pFlag!=pEqlFlag)
            QtSleep(10);
    }
    else
    {
        int tmpNum = msec/10;
        int tmpCount = 0;
        while((tmpCount++<tmpNum) && (pFlag!=pEqlFlag))
            QtSleep(10);
    }
    return (bool)(pFlag==pEqlFlag);
}

bool QtWait(int &pFlag, int pEqlFlag, unsigned int msec)
{
    if(msec == 0)
    {
        while(pFlag!=pEqlFlag)
            QtSleep(10);
    }
    else
    {
        int tmpNum = msec/10;
        int tmpCount = 0;
        while((tmpCount++<tmpNum) && (pFlag!=pEqlFlag))
            QtSleep(10);
    }
    return (bool)(pFlag==pEqlFlag);
}


iniParse::iniParse(QString pfileName) :
    QSettings(pfileName, QSettings::IniFormat)
{
}

bool iniParse::getProperty(QString pkey, QString &pValue)
{
    QVariant tmpVariant = value(pkey);
    if(tmpVariant.isNull())
        return false;
    pValue = tmpVariant.toString();
    return true;
}

bool  iniParse::getProperty(QString pkey, int &pValue)
{
    QVariant tmpVariant = value(pkey);
    if(tmpVariant.isNull())
        return false;
    bool ok = false;
    int tmpValue = tmpVariant.toInt(&ok);
    if(ok == false)
        return false;
    pValue = tmpValue;
    return true;
}

bool isDigitStr(QString src)
{
    QByteArray ba = src.toLatin1();//QString 转换为 char*
    const char *s = ba.data();
    while(*s && *s>='0' && *s<='9') s++;
    if (*s) //不是纯数字
        return false;
    return true;     //纯数字
}
