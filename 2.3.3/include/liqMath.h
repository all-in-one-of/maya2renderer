#ifndef _LIQMATH_H_
#define _LIQMATH_H_

#include <common/prerequest_std.h>
#ifndef MM_TO_INCH
#  define MM_TO_INCH 0.03937
#endif


#define FLOAT_EPSILON 0.0001

inline bool equiv( float val1, float val2 )
{
	return ( fabsf( val1 - val2 ) < FLOAT_EPSILON );
}

#if defined(_WIN32) && !defined(M_PI)
#  define M_PI 3.1415926535897932384626433832795
#endif

#endif//_LIQMATH_H_