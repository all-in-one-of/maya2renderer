/* Aqsis
 / Copyright (C) 1997 - 2001, Paul C. Gregory
 /
 / Contact: pgregory@aqsis.org
 /
 / This library is free software; you can redistribute it and/or
 / modify it under the terms of the GNU General Public
 / License as published by the Free Software Foundation; either
 / version 2 of the License, or (at your option) any later version.
 /
 / This library is distributed in the hope that it will be useful,
 / but WITHOUT ANY WARRANTY; without even the implied warranty of
 / MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 / General Public License for more details.
 /
 / You should have received a copy of the GNU General Public
 / License along with this library; if not, write to the Free Software
 / Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/** \file
 * \brief Definitions of types used by the RenderMan Interface
 *
 * ===================================================================
 * C-compatible header. C++ constructs must be preprocessor-protected.
 * ===================================================================
 */

#ifndef LIQUID_TYPES_H_INCLUDED
#define LIQUID_TYPES_H_INCLUDED

#define INVALID_LIGHT_INDEX 0

// DLL export symbols must be specified under Win32
#ifdef _WIN32
#	ifdef LIQUID_EXPORT_DLL
#		define LIQUID_EXPORT _declspec(dllexport)
#	else
#		define LIQUID_EXPORT _declspec(dllimport)
#	endif
#else
#	define LIQUID_EXPORT
#endif

#ifdef __cplusplus
extern	"C"
{
#endif

typedef	short	liqBoolean;
typedef	int		liqInt;
typedef	float	liqFloat;

typedef	char*	liqToken;

typedef	liqFloat	liqColor[ 3 ];
typedef	liqFloat	liqPoint[ 3 ];
typedef liqFloat liqVector[ 3 ];
typedef liqFloat liqNormal[ 3 ];
typedef liqFloat liqHpoint[ 4 ];
typedef	liqFloat	liqMatrix[ 4 ][ 4 ];
typedef	liqFloat	liqBasis[ 4 ][ 4 ];
typedef	liqFloat	liqBound[ 6 ];
typedef	char*	liqString;

typedef	void*	liqPointer;
typedef	void	liqVoid;

/* between Maya 3.0 /4.0/Linux/Other Platforms some functions changed their input type from long to int so
a stand-in type called liquidlong was created to get around the problem */
#ifdef LINUX
typedef int liquidlong;
#else
#if MAYA_API_VERSION > 300
typedef int liquidlong;
#else
typedef long liquidlong;
#endif
#endif

typedef unsigned long		ulong;
typedef unsigned int		uint;



typedef	liqFloat	( *liqFilterFunc ) ( liqFloat, liqFloat, liqFloat, liqFloat );
typedef	liqFloat	( *liqFloatFunc ) ();
typedef	liqVoid	( *liqFunc ) ();
typedef	liqVoid	( *liqErrorFunc ) ( liqInt code, liqInt severity, liqString message );
typedef	liqErrorFunc	liqErrorHandler;

typedef	liqVoid	( *liqProcSubdivFunc ) ( liqPointer, liqFloat );
typedef	liqVoid	( *liqProcFreeFunc ) ( liqPointer );
typedef	liqVoid	( *liqArchiveCallback ) ( liqToken, char *, ... );

typedef	liqPointer	liqObjectHandle;
typedef	liqPointer	liqLightHandle;
typedef	liqPointer	liqContextHandle;


/* Aqsis-specific typedefs */
typedef	liqVoid	( *liqProgressFunc ) ( liqFloat PercentComplete, liqInt FrameNo );


#ifdef	__cplusplus
}
#endif

LIQUID_EXPORT void setColor(liqColor& color, const liqFloat &c );
LIQUID_EXPORT void setColor(liqColor& to, const liqColor& from );
//LIQUID_EXPORT void setColor(liqColor& to, const liqFloat from[3] );
LIQUID_EXPORT liqFloat toRadians(const liqFloat &angle);


#endif//LIQUID_TYPES_H_INCLUDED
