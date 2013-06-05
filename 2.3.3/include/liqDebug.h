#ifndef _LIQ_DEBUG_H_
#define _LIQ_DEBUG_H_

#include <common/prerequest_std.h>
#include "liqtypes.h"

extern LIQUID_EXPORT int  debugMode;
#if !defined(LINUX) && !defined(OSX)
#define STDERR stderr
#  ifndef LIQDEBUGPRINTF
#    define LIQDEBUGPRINTF(msg,...)  if( debugMode ) fprintf(STDERR,(msg),__VA_ARGS__); fflush(STDERR); 
#  endif
#else
#define STDERR stdout
// gcc compatible variable args macro version of LIQDEBUGPRINTF
#  ifndef LIQDEBUGPRINTF
#    define LIQDEBUGPRINTF(msg,...) if( debugMode ) { fprintf(STDERR,(msg),## __VA_ARGS__) ; fflush(STDERR); }
#  endif
#endif


#endif//_LIQ_DEBUG_H_