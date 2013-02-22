/* $Id: //depot/main/rmanprod/rman/shaders/aaglass.sl#1 $  (Pixar - RenderMan Division)  $Date: 2003/06/19 $ */
/*
** Copyright (c) 2003 PIXAR.  All rights reserved.  This program or
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
 * aaglass.sl
 * An antialiasing glass shader where the resulting color is the sum of 
 * reflection, refraction, and Phong specular.  This version only has 
 * mirror reflections at raydepth < maxReflDepth.  With the default 
 * value of maxRayDepth (1), this means that reflections are only
 * computed at directly visible surfaces (i.e. raydepth == 0).
 *
 * This shader shoots 'samples' rays to reduce aliasing on directly visible
 * surfaces.  It only shoots 1 ray at secondary reflections/refractions to
 * avoid an exponential explosion in the number of rays.
 *
 * This shader relies on the surface normals to face the right way (i.e.
 * point out of objects) when determining the relative index of refraction.
 */

surface
aaglass(float Kr = 1.0;   /* ideal (mirror) reflection */
        float Kt = 1.0;   /* ideal refraction */
        float ior = 1.5;   /* index of refraction */
        float Ks = 1.0;   /* specular reflection coeff */
        float shinyness = 50.0;   /* Phong exponent */
        float samples = 4;   /* number of rays (for antialiasing) */
        float maxReflDepth = 1)
{
  normal Nn = normalize(N);
  vector In = normalize(I);
  normal Nf = faceforward(Nn, In);
  vector V = -In;   /* view direction */
  vector reflDir, refrDir;
  float eta = (In.Nn < 0) ? 1/ior : ior;   /* relative index of refraction */
  float kr, kt;
  float raydepth, s;

  rayinfo("depth", raydepth);
  s = (raydepth == 0) ? samples : 1; /* only 1 ray for secondary refr/refl! */

  /* Compute kr, kt, reflDir, and refrDir.  If there is total internal
     reflection kt is set to 0 and refrDir is set to (0,0,0). */
  fresnel(In, Nf, eta, kr, kt, reflDir, refrDir);
  kt = 1 - kr;

  Ci = 0;

  /* Mirror reflection */
  if (Kr * kr > 0 && raydepth < maxReflDepth)
    Ci += Kr * kr * environment("raytrace", reflDir, "samples", s);

  /* Ideal refraction */
  if (Kt * kt > 0)
    Ci += Kt * kt * environment("raytrace", refrDir, "samples", s);

  /* Specular highlights */
  if (Ks > 0) {
    illuminance (P, Nf, PI/2) {
      vector Ln = normalize(L);   /* normalized direction to light source */
      vector H = normalize(Ln + V);   /* half-way vector */
      Ci += Ks * pow(H . Nf, shinyness) * Cl;
    }
  }
}
