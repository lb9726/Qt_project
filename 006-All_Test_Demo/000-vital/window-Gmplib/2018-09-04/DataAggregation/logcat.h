#ifndef LOGCAT_H
#define LOGCAT_H

//#define QT_NO_DEBUG_OUTPUT 0
//#define CONFIG_DEBUG 0
#define CONFIG_DEBUG 1
#if CONFIG_DEBUG
#include <QDebug>
#define MYDebug(fmt, ...) qDebug("%s lines = %d: "#fmt, __PRETTY_FUNCTION__,__LINE__, ##__VA_ARGS__)
#else
#define MYDebug(fmt, ...)
#endif //CONFIG_DEBUG

#endif // LOGCAT_H
