/* $Id: //depot/main/rmanprod/rman/shaders/pointlight_rts.sl#1 $  (Pixar - RenderMan Division)  $Date: 2004/07/21 $ */
/*
** Copyright (c) 2004 PIXAR.  All rights reserved.  This program or
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
** Pixar Animation Studios
** 1200 Park Ave
** Emeryville, CA 94608
*/

/*
 * pointlight_rts: point light with ray traced shadows.
 * Square fall-off (like pointlight.sl) by default.
 * One shadow ray (sample) by default, and always use one sample ray
 * at raydepths higher than 0.
 */

light
pointlight_rts(
    float intensity = 1;
    color lightcolor = 1;
    float falloff = 2;
    float samples = 1;
    point from = point "shader" (0,0,0);   /* light position */
)
{
    illuminate(from) {
        if (N.L < 0) {
	    /* Compute illumination (ignoring shadow) */
            float dist = length(L);
            Cl = intensity * lightcolor * pow(dist, -falloff);

            /* Compute shadow */
            uniform float raydepth, s;
            rayinfo("depth", raydepth);
            s = (raydepth == 0) ? samples : 1;
            Cl *= transmission(Ps, from, "samples", s);
        }
    }
}
