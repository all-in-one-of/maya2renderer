/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/linearlight.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
/*
** Copyright (c) 2002 PIXAR.  All rights reserved.  This program or
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
** 1200 Park Avenue
** Emeryville, CA 94608
*/

#include "normals.h"

/*
 * Linear light, an example of a pseudo area light.
 * The sample positions are evenly spread out along a line segment between
 * point1 and point2.  Occlusion is computed with ray tracing.
 * The illuminate point is chosen such that L.N is 1 in the diffuse
 * calculation in the surface shader.
 */
light
linearlight(
    float intensity = 1;
    color lightcolor = 1;
    float falloff = 2;   /* 0 = none, 1 = linear, 2 = squared, ... */
    point point1 = point "shader" (0,0,0);   /* one end of line */
    point point2 = point "shader" (1,0,0);   /* other end of line */
    float samples = 16;   /* number of sample points on the light source */
    output float __nonspecular = 1;)
{
    uniform vector dir = point2 - point1;
    uniform float i;
    normal Ns = shadingnormal(N);
    point p;
    float offset = random();

    /* Compute illumination */
    illuminate(Ps + Ns) {   /* force execution independent of light location */
        for (i = 0; i < samples; i += 1)  {
            /* Compute point p on line segment between point1 and point2 */
            p = point1 + (i + offset) / samples * dir;
            /* Compute distance from point p to surface point Ps */
            vector l = p - Ps;
            float dist = length(l);
            vector ln = l / dist;
            /* Compute light from point p to surface point Ps */
            float dot = ln.Ns;
            if (dot > 0) {
	        color c = intensity * lightcolor;
                c *= pow(dist, -falloff);   /* distance falloff */
	        c *= transmission(Ps, p);   /* ray traced occlusion (shadow) */
                c *= dot;   /* Lambert's cosine law at the surface */
                Cl += c; 
            }
        }
        Cl /= samples;
    }
}
