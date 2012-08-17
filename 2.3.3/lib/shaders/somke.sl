/**************************************************************************
 * smoke.sl
 *
 * Description:
 *    This is a volume shader for smoke.  Trapezoidal integration is
 *    used to integrate to find scattering and extinction.
 *
 * Parameters:
 *   density - overall smoke density control
 *   integstart, integend - bounds along the viewing ray direction of the
 *          integration of atmospheric effects.
 *   stepsize - step size for integration.  Note that the overall speed
 *          of this shader is inversely proportional to the stepsize.
 *   use_noise - makes the smoke noisy (nonuniform) when nonzero
 *   freq, octaves, smokevary - control the fBm of the noisy smoke
 *   lightscale - multiplier for light scattered toward viewer in volume
 *   debug - if nonzero, copious output will be sent to stderr.
 *
 **************************************************************************/


#define snoise(p) (2*noise(p)-1)


volume
smoke (float density = 60;
       float integstart = 0, integend = 1000;
       float stepsize = 0.1;
       float debug = 0;
       float use_noise = 1;
       color scatter = 1;   /* for sky, try (1, 2.25, 21) */
       float octaves = 3, freq = 1, smokevary = 1;
       float lightscale = 15;
    )
{
  point Worigin = P - I;
  vector incident = vtransform ("shader", I);
  point origin = transform ("shader", Worigin);
  vector IN, WIN;
  float tau;
  color Cv = 0, Ov = 0;           /* net color & opacity of volume */
  color dC, dO;                   /* differential color & opacity */
  float ss, dtau, last_dtau;
  color li, last_li, lighttau;
  color scat;
  point PP, PW, Psmoke;
  float smoke, f, i;

  float end = min (length (incident), integend) - 0.0001;

  /* Integrate forwards from the start point */
  float d = integstart + random()*stepsize;
  if (d < end) {
      IN = normalize (incident);
      WIN = vtransform ("shader", "current", IN);
      dtau = 0;
      li = 0;
      ss = min (stepsize, end-d);
      d += ss;

      while (d <= end) {
	  PP = origin + d*IN;
	  PW = Worigin + d*WIN;
	  last_dtau = dtau;
	  last_li = li;

	  li = 0;
	  illuminance (PW, vector(0,0,1), PI) { li += Cl; }
          if (use_noise != 0) {
              Psmoke = PP*freq;
              smoke = snoise (Psmoke);
              /* Optimize: one octave only if not lit */
	      if (comp(li,0)+comp(li,1)+comp(li,2) > 0.01) {
                  f = 1;
                  for (i=1;  i < octaves;  i+=1) {
                       f *= 0.5;  Psmoke *= 2;
                       smoke += f*snoise(Psmoke);
                  }
              }
              dtau = density * smoothstep(-1,1,smokevary*smoke);
          } else dtau = density;

	  /* Our goal now is to find dC and dO, the color and opacity
	   * of the portion of the volume covered by this step.
	   */
	  tau = .5 * ss * (dtau + last_dtau);
	  lighttau = .5 * ss * (li*dtau + last_li*last_dtau);

	  scat = -tau * scatter;
	  dO = 1 - color (exp(comp(scat,0)), exp(comp(scat,1)), exp(comp(scat,2)));
	  dC = lighttau * dO;

	  /* Now we adjust Cv/Ov to account for dC and dO */
	  Cv += (1-Ov)*dC;
	  Ov += (1-Ov)*dO;

	  ss = max (min (ss, end-d), 0.005);
	  d += ss;
        }
    }

  Ci = lightscale*Cv + (1-Ov)*Ci;
  Oi = Ov + (1-Ov)*Oi;
}