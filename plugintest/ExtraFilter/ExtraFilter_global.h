#ifndef EXTRAFILTER_GLOBAL_H
#define EXTRAFILTER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EXTRAFILTER_LIBRARY)
#  define EXTRAFILTER_EXPORT Q_DECL_EXPORT
#else
#  define EXTRAFILTER_EXPORT Q_DECL_IMPORT
#endif

#endif // EXTRAFILTER_GLOBAL_H
