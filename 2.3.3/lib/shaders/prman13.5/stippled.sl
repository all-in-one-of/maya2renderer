/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/stippled.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
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
 *  'stippled' shades objects as though they were made out of plastic with
 *  lots of little bumps.
 *  This shader makes a fairly good attempt at automatic anti-aliasing.
 *
 *  Ks, Ka, Kd, roughness, specularcolor - the usual meaning
 *  grainsize - the size in shader space of the granules
 *  stippling - magnitude of speckling intensity variation
 * -------------------------------------------------------------------------- */
surface
stippled( float Ks=.3, Kd=.8, Ka=.1, roughness=.3,
	grainsize=.01, stippling=.2; color specularcolor=1 )
{
    normal Nf, Nfake, Ndiddle;
    vector V;
    point PP;
    float pixelsize;
    color Cstippled,Cflat;

    /* N and I are not automatically normalized in this implementation. */
    Nf = faceforward( normalize(N), I);
    V = -normalize(I) ;

    PP = transform("shader",P);
    /* get a vector pointing in some random direction and add it to normal */
    Ndiddle =  stippling * (normal noise(PP / grainsize) - .5);
    Nfake =  normalize(Nf + Ndiddle);
    Cstippled = Os * (Cs * (Ka*ambient() + Kd*diffuse(Nfake)) +
	    specularcolor * Ks * specular(Nfake,V,roughness) );

    /* anti-alias by mixing between flat color and stippled color as pixels */
    /*  get small */
    pixelsize = sqrt(area(PP));
    Oi = Os;
    if (grainsize >= pixelsize) {
	Ci = Cstippled;
    } else {
        /* calculate flat color to shade toward for anti-aliasing */
	    Cflat = Os * (Cs * (Ka*ambient() + Kd*diffuse(Nf)) +
		    specularcolor * Ks * specular(Nf,V,roughness) );
	    Ci = mix(Cflat,Cstippled,smoothstep(0,1,grainsize/pixelsize));
    }
}
