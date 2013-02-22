/* $Id: //depot/main/rmanprod/rman/shaders/cosinelight_rts.sl#1 $  (Pixar - RenderMan Division)  $Date: 2001/03/06 $ */
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
** Pixar Animation Studios
** 1200 Park Ave
** Emeryville, CA 94608
*/

/*
 * cosinelight_rts: cosine light with ray traced shadows.
 * A light source that illuminates the lower hemisphere (y < 0) with
 * cosine distribution: full power in the direction 'dir' tapering
 * off to zero power in the directions perpendicular to 'dir'.
 * Shadows are computed with ray tracing.
 */

light
cosinelight_rts(
    float intensity = 1;
    color lightcolor = 1;
    float falloff = 2;   /* default: inverse square fall-off */
    point from = point "shader" (0,0,0);   /* light position */
    vector dir = (0, -1, 0);
)
{
    illuminate(from, dir, PI/2) {
        float dist = length(L);
	Cl = intensity * lightcolor * pow(dist, -falloff);   /* fall-off */
        Cl *= (L.dir) / (length(L) * length(dir));   /* cosine term */
	Cl *= transmission(Ps, from);   /* ray traced shadow */
    }
}
