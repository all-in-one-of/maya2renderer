/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/threads.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
/*
** Copyright (c) 1999 PIXAR.  All rights reserved.  This program or
** documentation contains proprietary confidential information and trade
** secrets of PIXAR.  Reverse engineering of object code is prohibited.
** Use of copyright notice is precautionary and does not imply
** publication.
**
**                      RESTRICTED RIGHTS NOTICE
**
** Use, duplication, or disclosure by the Government is subject to the
** following restrictions:  For civilian agencies, subparagraphs (a) through
** (d) of the Commercial Computer Software--Restricted Rights clause at
** 52.227-19 of the FAR; and, for units of the Department of Defense, DoD
** Supplement to the FAR, clause 52.227-7013 (c)(1)(ii), Rights in
** Technical Data and Computer Software.
**
** Pixar
** 1001 West Cutting Blvd.
** Richmond, CA  94804
*/
#include <maps.h>

/*----------------------------------------------------------------------
 *
 * threads puts a thread pattern on the surface
 * of a "cylindrical workpiece" with a cylindrical projection.
 *
 * freq - "frequency" of the threads - number per unit length of cylinder
 * depth - depth the threads are cut into the surface of the cylinder
 * phase - rotates the threads about the central axis as this goes from
 *	0 to 1, to connect threads from different cylinders
 * zmin,zmax - extent of threads. No threads are cut outside here.
 * dampzone - distance inward from zmin,zmax to do a smoothstep diminishing
 * 	of depth
 * maporigin - the origin of the cylindrical projection
 * xaxis, yaxis, zaxis - 3 points defining the coordinate system of
 *        the cylinder.  The central axis is along zaxis-maporigin.
 *	  The point on the equator where (uu,vv)==(0,0) is mapped is at xaxis.
 *--------------------------------------------------------------------*/
displacement
threads ( float freq = 5.0, depth = 0.1, phase = 0.0,
	zmin = -1e20, zmax = 1e20, dampzone = 0;
	point maporigin=point "shader" (0,0,0),
	xaxis=point "shader" (1,0,0), yaxis=point "shader" (0,1,0),
	zaxis=point "shader" (0,0,1))
{
    varying point	PP,O,X,Y,Z;
    varying normal      sN;
    varying float	uu, vv, T,
			scale;


    /* do projection in shader space */
	PP=transform("shader",P);
	O=transform("shader",maporigin);
	X=transform("shader",xaxis);
	Y=transform("shader",yaxis);
	Z=transform("shader",zaxis);
	cylinmap(PP,uu,vv,O,X,Y,Z);

    if ((vv >= zmin) && (vv <= zmax)) {
    /* calculate new surface point and normal */
	if ((vv >= zmin+dampzone) && (vv <= zmax-dampzone)) {
		scale = depth;
	} else {
		scale = depth * min(smoothstep(zmin,zmin+dampzone,vv),
			1-smoothstep(zmax-dampzone,zmax,vv));
	}
	T = freq*vv + uu + phase;
	T = T - floor(T);
	scale = (2 * abs(T-0.5) - 1) * scale;
    /* do scaling in shader space */
	sN = normalize(ntransform("shader",N));
	P = transform("shader","current",PP + sN*scale);
	N = calculatenormal(P);
    }
}
