/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/stone.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
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
/* --------------------------------------------------------------------------
 *  'stone' shades objects as though they were made out granular stone
 *  Creates speckles with intensity varying between the surface color
 *  and graincolor.
 *
 *  Ks, Ka, Kd, roughness, specularcolor - the usual meaning
 *  scale - the size in shader space of the stone granules
 *  nshades - the number of intensity levels in the stone
 *  exponent - used to make the distribution come out more toward graincolor
 *  graincolor - the color of the stone grain
 * -------------------------------------------------------------------------- */
surface
stone( float Ks=.9, Kd=.8, Ka=.2, roughness=.3,
	scale=.02, nshades=4, exponent=2;
	    color specularcolor=1, graincolor=0)
{
    normal Nf;
    vector V;
    float tone;
    color grain;

 /* N and I are not automatically normalized in this implementation. */
    Nf = faceforward( normalize(N), I);
    V = -normalize(I) ;

 /* high-frequency noise */
    tone = noise( transform("shader",P) / scale);
 /* exponent sharpens peaks and pushes average toward darker end */
 /* round chops into nshades different intensity levels */
    tone = round(nshades * pow(tone, exponent)) / nshades;

    Oi = Os;
 /* interpolate between different colors using tone to get speckles */
    grain = mix(graincolor,Cs,tone);
    Ci = Os * (grain * (Ka*ambient() + Kd*diffuse(Nf)) +
    		specularcolor * Ks * specular(Nf,V,roughness) );
}
