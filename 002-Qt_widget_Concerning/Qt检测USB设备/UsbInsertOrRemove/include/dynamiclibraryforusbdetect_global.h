#ifndef DYNAMICLIBRARYFORUSBDETECT_GLOBAL_H
#define DYNAMICLIBRARYFORUSBDETECT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DYNAMICLIBRARYFORUSBDETECT_LIBRARY)
#  define DYNAMICLIBRARYFORUSBDETECTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DYNAMICLIBRARYFORUSBDETECTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DYNAMICLIBRARYFORUSBDETECT_GLOBAL_H
