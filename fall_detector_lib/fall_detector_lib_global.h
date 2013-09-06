#ifndef FALL_DETECTOR_LIB_GLOBAL_H
#define FALL_DETECTOR_LIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FALL_DETECTOR_LIB_LIBRARY)
#  define FALL_DETECTOR_LIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FALL_DETECTOR_LIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FALL_DETECTOR_LIB_GLOBAL_H