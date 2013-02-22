/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/rectanglelight.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
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
 * Rectangle light, an example of a "pseudo area light".
 * The rectangle is in the x-y plane (centered around the origin) and
 * has area xsize * ysize.
 * The sample positions are randomly distributed on a rectangle;
 * they are chosen in a stratified manner.
 * Occlusion is computed with ray tracing.
 * The illuminate point is chosen such that L.N is 1 in the diffuse
 * calculation in the surface shader.
 */
light
rectanglelight(
    float intensity = 1;
    color lightcolor = 1;
    float falloff = 2;   /* 0 = none, 1 = linear, 2 = squared, ... */
    float xsize = 1;   /* size of one side of the rectangle */ 
    float ysize = 1;   /* size of other side of the rectangle */
    float samples = 16;   /* number of sample points on the light source */
    output float __nonspecular = 1;
)
{
    uniform float xsamples, ysamples, i, j;
    color c;
    normal Ns = shadingnormal(N);
    vector l, ln;
    float x, y, dist, dot;
    point p;

    /* Choose best stratification: x, y, or both.  (If the light source is
       undergoing a non-uniform scaling, this choice might not be optimal.) */
    if (xsize > 4 * ysize) {
        xsamples = samples;
        ysamples = 1;
    } else if (ysize > 4 * xsize) {
        xsamples = 1;
        ysamples = samples;
    } else {
        xsamples = floor(sqrt(samples));
        ysamples = floor(samples/xsamples);
    }

    /* Compute illumination */
    illuminate (Ps + Ns) {  /* force execution independent of light location */
        for (j = 0; j < ysamples; j += 1)  {
            for (i = 0; i < xsamples; i += 1)  {
                /* Compute point (x,y) on unit square */
                x = (i + random()) / xsamples; 
                y = (j + random()) / ysamples;
                /* Compute point p on rectangle */
                p = point "shader" ((x - 0.5) * xsize, (y - 0.5) * ysize, 0);
                /* Compute distance from point p to surface point Ps */
                l = p - Ps;
                dist = length(l);
                ln = l / dist;
                /* Compute light from point p to surface point Ps */
                dot = ln.Ns;
	        if (dot > 0) {
                    c = intensity * lightcolor;
                    c *= pow(dist, -falloff);   /* distance falloff */
                    c *= transmission(Ps, p);   /* ray traced occlusion */
                    c *= dot;   /* Lambert's cosine law at the surface */
                    Cl += c;
                }
            }
        }
        Cl /= xsamples * ysamples; /* can be different from 'samples' */
    }
}
