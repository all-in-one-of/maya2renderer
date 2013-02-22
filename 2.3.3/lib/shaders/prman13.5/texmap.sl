/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/texmap.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
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

/*----------------------------------------------------------------------------
 * texmap - texture-map shader
 *
 * Puts a texture map onto a surface.  Can have 3 different projection
 * types or standard mapping.
 *
 * texname - the name of the texture file
 * maptype - the type of mapping (0 for planar, 1 for cylinder, 2 for sphere,
 *      3 for "standard" mapping using textcoords)
 * maporigin - the center of the projection.
 * xaxis, yaxis, zaxis - 3 points defining the orientation of the projection.
 *	Subtracting maporigin from each of these gives the appropriate axis.
 * s1,t1,... - corner points for parameter mapping as in textcoords
 * Ka, Kd, Ks, roughness, specularcolor - the usual meaning
 *---------------------------------------------------------------------------*/

surface
texmap(string texname = "";
	float maptype=3;
	point maporigin=point "shader" (0,0,0),
	xaxis=point "shader" (1,0,0), yaxis=point "shader" (0,1,0),
	zaxis=point "shader" (0,0,1);
	float s1=0,t1=0,s2=1,t2=0,s3=0,t3=1,s4=1,t4=1;
	float Ka=1, Kd=1, Ks=0, roughness=.25;
	color specularcolor = 1)

{
	uniform float ssize, tsize;
	varying normal Nf, NN;
	varying vector NI;
	varying point PP,O,X,Y,Z;
	varying color ctx;
	varying float ss,tt,ds,dsu,dsv,dt,dtu,dtv;

	/* calculate size of st space for boundary check and correction */
	if(s1 < s2) ssize = s2 - s1;
	else        ssize = s1 - s2;
	if(t1 < t2) tsize = t2 - t1;
	else        tsize = t1 - t2;

	/* do projection in shader space */
	PP=transform("shader",P);
	O=transform("shader",maporigin);
	X=transform("shader",xaxis);
	Y=transform("shader",yaxis);
	Z=transform("shader",zaxis);
	NN=normalize(ntransform("shader",N));
	decalmap(PP,s,t,ss,tt,maptype,O,X,Y,Z,s1,t1,s2,t2,s3,t3,s4,t4,NN);

	Nf = faceforward( normalize(N), I);

	/*
	 * If we allow the texture coordinates to wrap around we will get
	 * incorrect behavior at the boundaries between max and min value.
	 * To avoid this we correct for the boundary crossing condition.  We
	 * compute the texture coordinate and check if we cross the boundary.
	 * The current check is a kludge which relies on the "fact" that
	 * the size of a micropolygon is less than half the total texture
	 * space size, but if the coordinates warp around the delta
         * is greater than half the total texture space size.  If we have
	 * crossed the boundary we adjust the filter width accordingly.
	 */
	dsu = abs(Du(ss)*du);
	if((2*dsu) > ssize) dsu = ssize - dsu;
	dsv = abs(Dv(ss)*dv);
	if((2*dsv) > ssize) dsv = ssize - dsv;
	ds = dsu + dsv;
	dtu = abs(Du(tt)*du);
	if((2*dtu) > tsize) dtu = tsize - dtu;
	dtv = abs(Dv(tt)*dv);
	if((2*dtv) > tsize) dtv = tsize - dtv;
	dt = dtu + dtv;

	if (texname == "")
		ctx = 1;
	else
		ctx = texture(texname,ss,tt, ss+ds,tt, ss,tt+dt, ss+ds,tt+dt);

	Ci = (Ka*ambient() + Kd*diffuse(Nf)) * ctx;
	if (Ks != 0.0)
	{
		NI = normalize(-I);
		Ci += Ks*specularcolor*specular(Nf,NI,roughness);
	}
	Oi = Os;
	Ci = Ci * Oi;
}
