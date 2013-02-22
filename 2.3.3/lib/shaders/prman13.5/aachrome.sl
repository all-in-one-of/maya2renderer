/* $Id: //depot/main/rmanprod/rman/shaders/aachrome.sl#1 $  (Pixar - RenderMan Division)  $Date: 2001/03/06 $ */
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
** Pixar Animation Studios
** 1200 Park Ave
** Emeryville, CA 94608
*/

/*
 * aachrome: anti-aliased reflective chrome shader.
 * This shader shoots 'samples' rays to reduce aliasing in directly visible
 * surfaces.  It only shoots 1 ray at secondary reflections to avoid an
 * exponential explosion in the number of rays.
 *
 * Warning: only use this shader on objects that have normals facing out!
 */

surface
aachrome (float Kr = 1, samples = 1; string envmap = "raytrace")
{
    color Crefl;
    float depth, s;

    rayinfo("depth", depth);
    s = (depth == 0) ? samples : 1; /* only 1 ray for secondary reflections */

    if (N.I < 0) {
        normal Nn = normalize(N);
        vector In = normalize(I);
        vector reflDir = reflect(In,Nn);
        Crefl = environment(envmap, reflDir, "samples", s);
    } else { /* don't reflect inside object */
        Crefl = 0;
    }

    Ci = Kr * Crefl;
    Oi = 1;
}
