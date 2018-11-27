#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>

#include <QtCore>
#include <QtGui>
#if (QT_VERSION > QT_VERSION_CHECK(4, 9, 9))
#include <QtWidgets>
#endif

#define D_CODE_LOCATE        (QString("[%1,%2(),Line %3]").arg(__FILE__).arg(__func__).arg(__LINE__))

#define IDE_DEBUG(x)         qDebug() << (QString("%1-%2").arg(D_CODE_LOCATE).arg(QString(x)).toStdString().c_str())
#define IDE_TRACE()			 qDebug() << (QString("%1-Trace").arg(D_CODE_LOCATE).toStdString().c_str())
#define IDE_TRACE_INT(x)	 qDebug() << (QString("%1-%2=%3").arg(D_CODE_LOCATE).arg(#x).arg((int)x).toStdString().c_str())
#define IDE_TRACE_STR(x)	 qDebug() << (QString("%1-%2=%3").arg(D_CODE_LOCATE).arg(#x).arg(x).toStdString().c_str())

#define GBKToUnicode(x)                         QTextCodec::codecForName("GBK")->toUnicode(x)
#define GBKToUTF8(x)                            QTextCodec::codecForName("UTF-8")->fromUnicode(GBKToUnicode(x)).data()

#define D_FSRCPATH           QCoreApplication::applicationDirPath() + QString("/Resource/")

#endif // COMMON_H

