/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/rmarble.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
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
 * rmarble - a surface shader producing white marble with red veins
 *		similar to blue_marble (16.19 in the RenderMan Companion)
 *		except for the color spline, of course, and lines pointed
 *		out with comments ****.
 *
 * Ks, Ks, Ka, roughness, specularcolor - the usual meaning
 * veining - frequency of veins
 *-----------------------------------------------------------------------*/
surface
rmarble(
	float Ks=.4,Kd=.6, Ka=.1, roughness=.1;
	float veining = 1;
	color specularcolor=1)
{
	varying point PP;
	varying float cmi;
	varying normal Nf;
	varying vector V;
	color diffusecolor ;
	float pixelsize, twice, pixscale, weight, turbulence;

	Nf = faceforward( normalize(N), I);
	V = -normalize(I);

	PP = transform("shader",P) * veining;
	PP = PP/2;	/* frequency adjustment (S-shaped curve) */
	pixelsize = sqrt(area(PP)); twice = 2 * pixelsize;

	/* compute turbulence */
	turbulence = 0;
	for (pixscale = 1; pixscale > twice; pixscale /= 2) {
		/**** This function is different - abs() and -0.5 ****/
		turbulence += pixscale * abs(noise(PP/pixscale)-0.5);
	}
	/* gradual fade out of highest freq component near visibility limit */
	if (pixscale > pixelsize) {
		weight = (pixscale / pixelsize) - 1;
		weight = clamp(weight, 0, 1);
		/**** This function is different - abs() and -0.5 ****/
		turbulence += weight * pixscale * abs(noise(PP/pixscale)-0.5);
	}
	/*
	 * turbulence now has a range of 0:2, but its values actually
	 * tend strongly to lie around 0.75 to 1.
	 */

	/**** This is different - no multiply by 4 and subtract 3 ****/
	cmi = turbulence;
	cmi = clamp(cmi, 0, 1);
	diffusecolor =
		color spline(cmi,
		color (0.8, 0.2, 0.05),
		color (0.8, 0.2, 0.05),
		color (0.8,0.5,0.3),
		color (0.6,0.594,0.58),
		color (0.3,0.3,0.4),
		color (0.05, 0.05, 0.1),
		color (0.8,0.79,0.77),
		color (0.8,0.8,0.79)
		);
	/* use color spline to compute basic color */
	Oi = Os;
	Ci = diffusecolor * (Ka*ambient() + Kd*diffuse(Nf));
	/* add in specular component */
	Ci = Os * (Ci + specularcolor * Ks * specular(Nf,V,roughness));
}
