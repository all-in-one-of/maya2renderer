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


#include "smoke.impl"


volume 
smoke (float density = 60;
       float integstart = 0;
       float integend = 1000;
       float stepsize = 0.1;
       float debug = 0;
       float use_noise = 1;
       color scatter = 1;   /* for sky, try (1, 2.25, 21) */
       float octaves = 3; 
       float freq    = 1; 
       float smokevary = 1;
       float lightscale = 15
    )
{
   smoke (density,
       integstart , 
       integend,
       stepsize,
       debug,
       use_noise,
       scatter,
       octaves, 
       freq, 
       smokevary,
       lightscale
    );
}