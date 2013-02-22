/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/cylinderlight.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
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
 * Cylinder light, an example of a "pseudo area light".
 * The cylinder is oriented along the z axis.
 * The sample positions are randomly distributed on the surface of a 
 * cylinder; they are chosen in a stratified manner.
 * Occlusion is computed with ray tracing.
 * The illuminate point is chosen such that L.N is 1 in the diffuse
 * calculation in the surface shader.
 */
light
cylinderlight(
    float intensity = 1;
    color lightcolor = 1;
    float falloff = 2;   /* 0 = none, 1 = linear, 2 = squared, ... */
    float zmin = 0;   /* bottom edge of cylinder */
    float zmax = 1;   /* top edge of cylinder */
    float radius = 1;   /* radius of cylinder */
    float samples = 16;   /* number of sample points on the light source */
    output float __nonspecular = 1;)
{
    uniform float anglesamples, zsamples;
    uniform float i, j;
    color c, cl = 0;
    normal Ns = shadingnormal(N);
    vector l, ln;
    point p;
    float angle, z, dist, dot;

    /* Choose stratification: angle, z, or both.  (If the light source is
       undergoing a non-uniform scaling, this choice might not be optimal.) */
    if (zmax - zmin < 0.2 * radius) { /* cylinder is rather flat */
        anglesamples = samples;
        zsamples = 1;
    } else if (zmax - zmin > 5 * radius) { /* cylinder is rather skinny */
        anglesamples = 1;
        zsamples = samples;
    } else { /* cylinder neither flat nor skinny */
        anglesamples = floor(sqrt(samples));
        zsamples = floor(samples/anglesamples);
    }

    /* Compute illumination */
    illuminate(Ps + Ns) {  /* force execution independent of light location */
        for (j = 0; j < zsamples; j += 1)  {
            for (i = 0; i < anglesamples; i += 1)  {
                /* Compute stratified random angle in [0,2pi] */
                angle = 2 * PI * (i + random()) / anglesamples;
                /* Compute stratified random z in [zmin,zmax] */
                z = (j + random()) / zsamples;
                z = zmin + z * (zmax - zmin);
                /* Compute point p on cylinder */
                p = point "shader" (radius * cos(angle), radius * sin(angle), z);
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
        Cl /= zsamples * anglesamples; /* can be different from 'samples' */
    }
}
