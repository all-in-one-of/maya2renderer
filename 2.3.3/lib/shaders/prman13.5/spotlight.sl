/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/spotlight.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
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
light
spotlight(
    float  intensity = 1;
    color  lightcolor = 1;
    point  from = point "shader" (0,0,0);	/* light position */
    point  to = point "shader" (0,0,1);
    float  coneangle = radians(30);
    float  conedeltaangle = radians(5);
    float  beamdistribution = 2;)
{
    float  atten, cosangle;
    uniform vector A = (to - from) / length(to - from);
    uniform float cosoutside= cos(coneangle),
		  cosinside = cos(coneangle-conedeltaangle);

    illuminate( from, A, coneangle ) {
	cosangle = L.A / length(L);
	atten = pow(cosangle, beamdistribution) / (L.L);
	atten *= smoothstep( cosoutside, cosinside, cosangle );
	Cl = atten * intensity * lightcolor;
    }
}
