#ifndef _LIQ_MATH_H_
#define _LIQ_MATH_H_

#include <cmath>

#ifndef MM_TO_INCH
#  define MM_TO_INCH 0.03937
#endif

#define LIQ_SCALAR_EPSILON (0.0001f)

inline bool equiv( float val1, float val2 )
{
	return ( fabsf( val1 - val2 ) < LIQ_SCALAR_EPSILON );
}

#if defined(_WIN32) && !defined(M_PI)
#  define M_PI 3.1415926535897932384626433832795
#endif

#endif//_LIQ_MATH_H_