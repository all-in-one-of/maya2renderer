/* $Id: //depot/main/rmanprod/rman/shaders/glassrefr.sl#1 $  (Pixar - RenderMan Division)  $Date: 2001/03/06 $ */
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
 * A simple glass shader where the result color is the sum of 
 * reflection, refraction, and Phong specular.
 * Note that this shader can result in a combinatorial explosion in
 * the number of rays since each ray spawns both a reflection and a
 * refraction ray.  In the worst case, each ray from a REYES shading
 * point will result in 2^maxspeculardepth rays.
 * This shader relies on the surface normals to face the right way (i.e.
 * point out of objects) when determining the relative index of refraction.
 */

surface 
glassrefr (float Kr = 1; /* ideal (mirror) reflection */
           float Kt = 1; /* ideal refraction */
           float ior = 1.5; /* index of refraction */
           float Ks = 1; /* specular reflection coeff */
           float shinyness = 50) /* Phong exponent */
{
  normal Nn = normalize(N);
  vector In = normalize(I);
  normal Nf = faceforward(Nn, In, Nn);
  vector V = -In;   /* view direction */
  vector reflDir, refrDir;
  float eta = (In.Nn < 0) ? 1/ior : ior; /* relative index of refraction */
  float kr, kt;

  Ci = 0;

  /* Compute kr, kt, reflDir, and refrDir.  If there is total internal
     reflection kt is set to 0 and refrDir is set to (0,0,0). */
  fresnel(In, Nf, eta, kr, kt, reflDir, refrDir);
  kt = 1 - kr;
  
  /* Mirror reflection */
  if (Kr * kr > 0)
    Ci += Kr * kr * trace(P, reflDir);

  /* Ideal refraction */
  if (Kt * kt > 0)
    Ci += Kt * kt * trace(P, refrDir);	

  /* Specular highlights */
  if (Ks > 0) {
    illuminance (P, Nf, PI/2) {
      vector Ln = normalize(L);   /* normalized direction to light source */
      vector H = normalize(Ln + V);   /* half-way vector */
      Ci += Ks * pow(H . Nf, shinyness) * Cl;
    }
  }
}
