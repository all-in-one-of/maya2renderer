/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/spherelight.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
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
 * Sphere light, an example of a pseudo area light.
 * The sample positions are randomly distributed on the surface of a sphere;
 * they are chosen in a uniform, stratified manner -- see Graphics Gems III,
 * page 81.  Occlusion is computed with ray tracing.
 * The illuminate point is chosen such that L.N is 1 in the diffuse
 * calculation in the surface shader.
 */
light
spherelight(
    float intensity = 1;
    color lightcolor = 1;
    float falloff = 2;   /* 0 = none, 1 = linear, 2 = squared, ... */
    point from = point "shader" (0,0,0); /* center of sphere */
    float radius = 1;   /* radius of sphere */
    float samples = 16;   /* number of sample points on the light source */
    output float __nonspecular = 1;)
{
    uniform float anglesamples, zsamples, i, j;
    color c;
    normal Ns = shadingnormal(N);
    vector l, ln;
    point p;
    float angle, z, r, dist, dot;

    /* Choose number of strata for stratification */
    anglesamples = floor(sqrt(samples));
    zsamples = floor(samples/anglesamples);

    /* Compute illumination */
    illuminate (Ps + Ns) {  /* force execution independent of light location */
        for (j = 0; j < zsamples; j += 1)  {
            for (i = 0; i < anglesamples; i += 1)  {
                /* Compute stratified random angle in [0,2pi] */
	        angle = 2 * PI * (i + random()) / anglesamples;
	        /* Compute stratified random z in [-1,1] */
                z = (j + random()) / zsamples;
                z = 2 * z - 1;
                /* Compute point p on sphere */
                r = sqrt(1 - z*z);
                p = from + radius * vector(r * cos(angle), r * sin(angle), z);
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
