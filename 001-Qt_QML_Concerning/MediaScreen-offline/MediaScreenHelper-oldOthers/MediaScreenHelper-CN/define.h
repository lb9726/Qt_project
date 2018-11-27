#ifndef DEFINE_H
#define DEFINE_H

#include<QDebug>
#include<QObject>

#ifdef DEBUG
#define IDE_DEBUG(x)         qDebug() << (QString("[%1,%2(),Line %3]-%4").arg(__FILE__).arg(__FUNCTION__).arg(__LINE__).arg(QString(x)))
#define IDE_TRACE()			 qDebug() << (QString("[%1,%2(),Line %3]-Trace").arg(__FILE__).arg(__FUNCTION__).arg(__LINE__))
#define IDE_TRACE_INT(x)	 qDebug() << (QString("[%1,%2(),Line %3]-%4=%5").arg(__FILE__).arg(__FUNCTION__).arg(__LINE__).arg(#x).arg((int)x))
#define IDE_TRACE_STR(x)	 qDebug() << (QString("[%1,%2(),Line %3]-%4=%5").arg(__FILE__).arg(__FUNCTION__).arg(__LINE__).arg(#x).arg(x))
#define IDE_PRINTF(fmt, ...) qDebug() << (QString("[%s,%s(),Line %s]-"#fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__))
#endif


//默认搜索最大的盘符数量
#define MAX_DIR_NUMBER 20
//软件版本号-见Main.qml
//#define RELEASE_VERSION ("1.0.0")
#define        D_FILE_10M    10485760  //10Mb
#define        D_FILE_50M    52428800  //50Mb
#define        D_FILE_512M   536870912 //512GMb
#define        D_FILE_PER_BUFFER   10240 //10Kb
#endif // DEFINE_H
