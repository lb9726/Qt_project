#ifndef LOGFUN_H
#define LOGFUN_H

#include "globalfun.h"
#ifdef SURRPORT_LOG
#include "logger.h"
#include "propertyconfigurator.h"
#include "fileappender.h"

extern Log4Qt::Logger* g_Logger;
#endif

typedef struct{
    MSG_LEVEL level;
    QString content;
}LOGTIP_INFO;

class logTipManager : public xmlParse
{
public:
    logTipManager(QString fileName);

    ~logTipManager(){}

    static const QString LogTipFileName;
    static const QString LogTipFileType;

    bool getNewLogTipCategory();
};

class logManager : public xmlParse
{
public:
    logManager(QString fileName);
    ~logManager(){}

    bool NewLog();
    bool OpenLog();
    bool CloseLog();
    bool SaveLog();
};

#endif // LOGFUN_H

