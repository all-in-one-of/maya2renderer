#ifndef liqRiInterface_H
#define liqRiInterface_H

// use aqsis for rib exportion
#include <liqConfig.h>
#include <liqtypes.h>
//-------------------------------------------------------
#define RIBLIB_AQSIS

#ifdef RIBLIB_AQSIS

//typedef int RtLightHandle;
#define INVALID_LIGHT_INDEX 0

#	include <aqsis/ri/ri.h>

//#elif defined()
#else 
#	include "../riblib/ri.h"
#endif
//-------------------------------------------------------
RtVoid RiBegin_liq(RtToken name);
//-------------------------------------------------------
//#ifdef LIQUID_SEPARATE
//extern LIQUID_EXPORT liqToken RI_COMMENT;
//extern LIQUID_EXPORT liqToken RI_VERBATIM;
//#endif
// #define RI_COMMENT  "comment"
// #define RI_VERBATIM "verbatim"
// #define RI_RGBA     "rgba"
// #define RI_FOV		"fov"
#endif//liqRiInterface_H