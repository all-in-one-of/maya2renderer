/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/glassbal.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
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
/*------------------------------------------------------------------------
 * glassbal - solid glass sphere shader (turns everything upside-down
 *	and backwards). Also works fairly well for other "sort of spherical"
 *	objects (like teapots).
 *
 * Ka, Ks, Kd, roughness, specularcolor - the usual
 * eta - index of refraction of air/glass
 * envname - environment map file name
 *----------------------------------------------------------------------*/

surface
glassbal( float Ka=0, Ks=.6, roughness=.2, Kd=0, eta=.6;
	color specularcolor=1; string envname="" )
{
    float Kr, Kt, Ktr, Ktt;
    vector Rray, Tray, R, T;
    normal Nf;
    vector V;
    normal N2;

    /* N and I are not automatically normalized in this implementation. */
    Nf = faceforward( normalize(N), I);
    V = -normalize(I) ;

    /* find direction of refracted ray */
    fresnel(-V, Nf, eta, Kr, Kt, Rray, T);
    T = normalize(T);
    N2 = reflect(Nf, -T);
    fresnel(T, N2, 1/eta, Ktr, Ktt, R, Tray);
    Rray = vtransform("world", Rray);
    Tray = vtransform("world", Tray);

    /* calculate color */
    Oi = Os;
    Ci = Cs*(Ka*ambient() + Kd*diffuse(Nf)) +
   		specularcolor*Ks*specular(Nf,V,roughness);
    if (envname != "") {
	    Ci = Ci + Kr * color environment(envname,Rray)
		+ min(Kt*Ktt, 1) * Cs * color environment(envname,Tray);
    }
    Ci = Oi * Ci;
}
