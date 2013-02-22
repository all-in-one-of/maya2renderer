/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/carpet.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
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
/*-------------------------------------------------------------------------
 *	carpet - creates a carpeted surface with variable nap and scuffing.
 *	Makes a reasonable attempt at anti-aliasing.
 *
 *	Kd, Ka - the usual meaning
 *	scuff - the "amount of scuffing": footprints, vacuum cleaner marks,
 *	   etc. (the relative frequency of intensity variation)
 *	nap - the "shagginess" of the carpet (size of the speckles)
 *-------------------------------------------------------------------------*/
surface
carpet( float Kd=.6, Ka=.1, scuff = 1, nap = 1)
{
        varying normal  Nf;
	varying point	PP;
	varying float	pixelsize, twice,
			pixscale, weight, turbulence,
			napfreq, speckle, mottling;

	Nf = faceforward( normalize(N), I);


    /* large-scale mottling */
	if (scuff==0) mottling = 1;
	else {
	    /* get pixel size */
		PP = transform("shader",P) * scuff;
		PP = PP/2;	/* frequency adjustment (S-shaped curve) */
		pixelsize = sqrt(area(PP)); twice = 2 * pixelsize;
	    /* create some turbulence */
		turbulence = 0;
		for (pixscale = 1; pixscale > twice; pixscale /= 2) {
			turbulence += pixscale * abs(noise(PP/pixscale)-0.5);
		}
	    /* gradual fade out of highest frequency component near limit */
		if (pixscale > pixelsize) {
			weight = (pixscale / pixelsize) - 1;
			weight = clamp(weight, 0, 1);
			turbulence += weight * pixscale * abs(noise(PP/pixscale)-0.5);
		}
	    /* scuffing will be between 0.5 and 1 */
		mottling = min(turbulence+0.5, 1);
	}

    /* small-scale speckling */
	if (nap==0) speckle = 1;
	else {
	    /* get high-frequency speckles */
		napfreq = nap/100;	/* scale up speckle frequency */
		speckle = noise(transform("shader",P)/napfreq);
		speckle = speckle * speckle;
	    /* two-level speckles */
		speckle = (speckle <= 0.25) ? 0.2 : 1;
	    /* smooth out speckles as they get smaller */
		pixelsize = sqrt(area(transform("shader",P)));
		if (speckle<1) speckle = (1-smoothstep(0,1,napfreq/pixelsize))
					*(1-speckle) + speckle;
	}

    /* get color */
	Oi = Os;
	Ci = Os * Cs * mottling * speckle * (Ka*ambient() + Kd*diffuse(Nf));
}
