#ifndef BIG_GLOBAL_H
#define BIG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BIG_LIBRARY)
#  define BIG_EXPORT Q_DECL_EXPORT
#else
#  define BIG_EXPORT Q_DECL_IMPORT
#endif

#endif // BIG_GLOBAL_H
