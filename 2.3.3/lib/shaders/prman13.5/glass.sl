/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/glass.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
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
/*----------------------------------------------------------------------------
 * glass - a transparent glass shader - no refraction
 *	works for colored glass (set surface color) and clear glass
 *	(Cs = [1 1 1]).
 *
 * Ka, Ks, Kd, roughness, specularcolor - the usual meaning
 * Kr - strength of environment map reflection
 * envname - environment map name
 *---------------------------------------------------------------------------*/
surface
glass( float Ka=0, Kd=0, Ks=.6, Kr=0.5, roughness=.025;
	color specularcolor=1; string envname="" )
{
    float Kl;
    vector Rray;
    normal Nf;
    vector V;
    color highlight;

    /* N and I are not automatically normalized in this implementation. */
    Nf = faceforward( normalize(N), I);
    V = -normalize(I) ;

    highlight = specularcolor*Ks*specular(Nf,V,roughness);
    Ci = Cs * (Ka * ambient() + Kd * diffuse(Nf)) + highlight;
    if (envname != "") {
        Rray = reflect(-V,Nf);
        Rray = vtransform ("world", Rray);
	Ci = Ci + Kr * color environment(envname,Rray);
    }
    Kl = (comp(Ci,0) + comp(Ci,1) + comp(Ci,2))/3;
    Oi = Kl + (1-Cs);
}
