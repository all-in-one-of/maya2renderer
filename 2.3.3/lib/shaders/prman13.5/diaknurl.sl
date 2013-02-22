/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/diaknurl.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
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
 * diaknurl puts an impressed diamond pattern on the surface
 * of a "cylindrical workpiece" with a cylindrical projection.
 *
 * freq - "frequency" of the knurl
 * depth - magnitude of knurl
 * width - width of slots
 * radius - radius of cylinder (so diamonds will be diamond-shaped)
 * zmin,zmax - extent of knurl
 * dampzone - distance from ends to do a smoothstep diminishing of depth
 * maporigin - the bottom point of the center axis of the cylindrical proj.
 * xaxis, yaxis, zaxis - 3 points defining the coordinate system of
 *        the cylinder.  The central axis is along zaxis-cylinder.
 *	  The point on the equator where (uu,vv)==(0,0) is mapped is at xaxis.
 *--------------------------------------------------------------------*/
displacement
diaknurl(float freq = 10, depth = 0.025, width = 0.05, radius = 1/(2*PI),
	zmin = -1e20, zmax = 1e20, dampzone = 0;
	point maporigin=point "shader" (0,0,0),
	xaxis=point "shader" (1,0,0), yaxis=point "shader" (0,1,0),
	zaxis=point "shader" (0,0,1))
{
    varying point	PP,O,X,Y,Z;
    varying normal      sN;
    varying float	uu, vv,
			dist1,dist2,rim,pos1,pos2,S,T,displace,
			magnitude;

    /* do projection in shader space */
	PP=transform("shader",P);
	O=transform("shader",maporigin);
	X=transform("shader",xaxis);
	Y=transform("shader",yaxis);
	Z=transform("shader",zaxis);
	cylinmap(PP,uu,vv,O,X,Y,Z);

    /* if inside diamond pattern area... */
    if ((vv >= zmin) && (vv <= zmax)) {
    /* calculate new surface point and normal */
	if ((vv >= zmin+dampzone) && (vv <= zmax-dampzone)) {
		magnitude = depth;
	} else {
		magnitude = depth * min(smoothstep(zmin,zmin+dampzone,vv),
			1-smoothstep(zmax-dampzone,zmax,vv));
	}

    /* calculate diamond pattern */
	S = uu * freq * 2 * PI * radius;
	T = vv * freq;
	pos1 = mod(S-T,1);
	S = (1-uu) * freq * 2 * PI * radius;
	pos2 = mod(S-T,1);

    /* have a smoothstep rim */
	rim = 2*width; /* rim = blur boundary */
	dist1 = abs(pos1 - (width+rim));
	dist2 = abs(pos2 - (width+rim));

	if ( dist2 < dist1 )  /* reverse loops */
		dist1 = dist2;

    /* displace surface */
	displace = 0.0;
	if (dist1 < width)
		displace = -1.0;
	else if (dist1 < width+rim)
		displace = -(1.0-smoothstep(0.0,1.0,(dist1-width)/rim));

	sN = normalize(ntransform("shader",N));
	P = transform("shader","current",PP + sN*displace*magnitude);
	N = calculatenormal(P);
    }
}
