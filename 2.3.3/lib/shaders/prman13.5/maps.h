/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/maps.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
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
/*
 * This file contains functions to perform decal mapping, which is especially
 * useful for putting texture maps onto surfaces consisting of many small
 * polygons.
 *
 * Major function is decalmap, which calls the mapping functions:
 * cylinmap, planemap, sphermap, automap (and bilerp).
 *
 */

/*----------------------------------------------------------------------
 *
 * bilerp does a bilinear interpolation of one number between the
 * four given corners in texture space to the given point in parameter
 * space.
 *
 * uu,vv - point in parameter space
 * x1,x2,x3,x4 - corners in texture space
 *
 *--------------------------------------------------------------------*/

float
bilerp(float uu,vv,x1,x2,x3,x4)
{
    varying float left, right;

    left = x1 + uu * (x2 - x1);
    right = x3+ uu * (x4 - x3);
    return(left + vv * (right - left));
}

/*----------------------------------------------------------------------
 *
 * cylinmap performs a cylindrical projection to create texture coordinates
 *	uu and vv
 *
 * p - point on surface to be projected
 * uu,vv - new projected coordinates
 * maporigin - the bottom point of the center axis of the cylindrical proj.
 * xaxis, yaxis, zaxis - 3 points defining the coordinate system of
 *        the cylinder.  The central axis is along zaxis-maporigin.
 *	  The point on the equator where (uu,vv)==(0,0) is mapped is at xaxis.
 *--------------------------------------------------------------------*/

void cylinmap(point p;
	      output float uu,vv;
	      point maporigin, xaxis, yaxis, zaxis)
{
    varying vector	V,Vn,
			XX, YY, ZZ;
    varying float	xx, yy;


    V = p - maporigin;
    Vn = normalize(V);

    XX = normalize(xaxis - maporigin);
    YY = normalize(yaxis - maporigin);
    ZZ = normalize(zaxis - maporigin);
    xx = Vn.XX;
    yy = Vn.YY;

    vv = V.ZZ;

    uu = atan(yy, xx) / (2*PI);  /* -.5 -> .5 */
    uu = uu + step(0,-uu); /* remaps to 0 -> 1 */
}



/*---------------------------------------------------------------------
 * planemap performs a planar projection to create texture coordinates
 *	uu and vv
 *
 * p - point on surface to be projected
 * uu,vv - new projected coordinates
 * maporigin, xaxis, yaxis - The projection is performed as if the texture
 *	  map were placed with (uu,vv)==(0,0) at 'maporigin', uu==0 along
 *        yaxis-maporigin and vv==0 along xaxis-maporigin.  The map is then
 *        projected parallel to itself onto the surface.
 *--------------------------------------------------------------------*/

void
planemap (point p;
	  output float uu,vv;
	  point maporigin, xaxis, yaxis)
{
    varying vector	V,
			XX, YY;
    varying float	xx, yy;


    V = p - maporigin;

    XX = normalize(xaxis - maporigin);
    YY = normalize(yaxis - maporigin);
    uu = V.XX;
    vv = V.YY;
}




/*--------------------------------------------------------------------
 *
 * sphermap performs a spherical projection to create texture coordinates
 *	uu and vv
 *
 * p - point on surface to be projected
 * uu,vv - new projected coordinates
 * maporigin - the center of the spherical projection.
 * xaxis, yaxis, zaxis - 3 points defining the coordinate system of
 *        the sphere.  The north pole is at zaxis.
 *	  The point on the equator where uu==0 is mapped is at xaxis.
 *--------------------------------------------------------------------*/

void
sphermap(point p;
	 output float uu,vv;
	 point maporigin, xaxis, yaxis, zaxis)
{
    varying vector	V,
			XX, YY, ZZ;
    varying float	xx, yy, zz;


    V = normalize(p - maporigin);

    XX = normalize(xaxis - maporigin);
    YY = normalize(yaxis - maporigin);
    ZZ = normalize(zaxis - maporigin);
    xx = V.XX;
    yy = V.YY;
    zz = V.ZZ;

    uu = atan(yy, xx) / (2*PI);  /* -.5 -> .5 */
    uu = uu + step(0,-uu); /* remaps to 0 -> 1 */
    vv = acos(-zz) / PI;
}


/*---------------------------------------------------------------------
 * automap performs a planar projection using the normal to the surface
 *	as the normal to the plane to create texture coordinates
 *	uu and vv
 * NOTE: this will only work very well on faceted (polygonal or bilinear)
 *	surfaces.
 *
 * p - point on surface to be projected
 * uu,vv - new projected coordinates
 * maporigin, yaxis - The projection is performed as if the texture
 *	  map were placed with (uu,vv)==(0,0) at 'maporigin', uu==0 along
 *        yaxis-maporigin and vv==0 along the vector perpendicular to
 *	  both yaxis-maporigin and the normal to the surface.  The map is
 *	  then projected parallel to itself onto the surface.  This has
 *	  the effect of giving each facet of a surface a separate planar
 *	  projection.  This will work better for periodic textures than
 *	  for non-periodic textures.  For surfaces that are perpendicular
 *	  to the yaxis-maporigin vector, a reasonable guess at a good
 *	  "up" vector is attempted.
 * n - surface normal
 *--------------------------------------------------------------------*/

void
automap(point p;
	output float uu,vv;
	point maporigin, yaxis;
	normal n)
{
    varying vector	V,
			XX, YY, ZZ;
    varying float	xx, yy,
			dot;
    uniform vector	yvector, zvector;


    /*
     * The yvector and zvector are the axes of shader space, but expressed
     * in shader space coordinates, not in current coordinates, so no
     * space conversion is required.  Everything else has already been
     * put into shader space coordinates by the callers of this routine.
     */

    yvector = vector (0,1,0);
    zvector = vector (0,0,1);

    V = p - maporigin;

    ZZ = normalize(n);
    YY = normalize(yaxis - maporigin);
    dot = ZZ . YY;
    /* if "up" (YY) vector parallel to "at" (ZZ) vector, find new "up" */
    if ((dot > (1-0.01))||(dot < (-1+0.01))) {
	dot = ZZ . yvector;
	if ((dot > (1-0.01))||(dot < (-1+0.01))) YY = zvector;
	else YY = yvector;
    }
    /* get orthogonal coordinate system */
    XX = YY ^ ZZ;
    YY = ZZ ^ XX;

    uu = V.XX;
    vv = V.YY;
}




/*--------------------------------------------------------------------
 *
 * decalmap - perform decal mapping. Calls one of the above mapping functions.
 *
 * p - point on surface
 * s, t - unmapped texture coordinates
 * ss, tt - returned mapped texture coordinates
 * maptype - the type of mapping (0 for planar, 1 for cylinder, 2 for sphere,
 *	3 for "standard" mapping using textcoords, 4 for "auto-planar"
 *	mapping - planar perpendicular to surface normal)
 * maporigin - the center of the projection.
 * xaxis, yaxis, zaxis - a local coordinate system that defines the
 *        orientation of the projection.
 * s1,t1,... - corner points for parameter mapping as in textcoords
 * n - normal to the surface
 *--------------------------------------------------------------------*/

float decalmap(point p;
	float s,t;
	output float ss, tt;
	float maptype;
	point maporigin, xaxis, yaxis, zaxis;
	float s1,t1,s2,t2,s3,t3,s4,t4;
	varying normal n)
{
	varying float uu, vv;

	if (maptype == 0) {
		planemap(p, uu, vv, maporigin, xaxis, yaxis);
	} else if (maptype == 1) {
		cylinmap(p, uu, vv, maporigin, xaxis, yaxis, zaxis);
	} else if (maptype == 2) {
		sphermap(p, uu, vv, maporigin, xaxis, yaxis, zaxis);
	} else if (maptype == 4) {
		automap(p, uu, vv, maporigin, yaxis, n);
	}
	if (maptype != 3) {
	   /* bilinear interpolation to get texture parameters */
	    ss=bilerp(uu,vv,s1,s2,s3,s4);
	    tt=bilerp(uu,vv,t1,t2,t3,t4);
	} else {
		ss = s;
		tt = t;
	}
    return 0;
}
