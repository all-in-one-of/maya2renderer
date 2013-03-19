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

#include "texmap.impl"

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
  texmap(
	texname,
	maptype,
	maporigin,
	xaxis,
	yaxis,
	zaxis,
	s1,
	t1,
	s2,
	t2,
	s3,
	t3,
	s4,
	t4,
	Ka,
	Kd,
	Ks,
	roughness,
	specularcolor
  );
}
