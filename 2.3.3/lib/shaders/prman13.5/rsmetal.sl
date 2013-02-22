/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/rsmetal.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
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
/*-----------------------------------------------------------------------
 * rsmetal - a surface shader producing a highly specular finish
 *	     by randomly perturbing the specular reflections and
 *	     whose color is determined from Cs.
 *
 * Ka, Ks, Kr, roughness - the usual meaning
 *-----------------------------------------------------------------------*/

surface
rsmetal (float Ka=1, Ks=1, Kr = 0.6, roughness=.1;)
{
    normal Nf;
    vector V, D;
    color Cr;
    float w;

    Nf = faceforward(normalize(N), I) ;
    V = normalize(-I);
    D = reflect(V, Nf) ;
    D = vtransform("world", D);

#pragma nolint 4 /* Yes, we really DO want this noise in world space */
    w = 0.6 * noise(D * 2.5) +
        0.2 * noise(D * 5) +
        0.1 * noise(D * 10) +
        0.05 * noise(D * 20);
    w = w * w * w;
    Cr = Kr * w;

    Oi = Os;
    Ci = Os * Cs * ( Ka*ambient() + Ks*specular(Nf,V,roughness) + Cr);
}

