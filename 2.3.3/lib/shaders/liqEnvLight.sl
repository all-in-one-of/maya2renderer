
#include "liqEnvLight.impl"
light
me_envlight (
    uniform string envname = "";
    uniform string envspace = "world";
    uniform float  Intensity = 1.0;
    uniform float  reflBlur = 0;
    uniform float	 samples = 4;
		uniform float	 Coloration = 0;
		uniform color	 cFilt = 1;
		uniform string Filter = "gaussian";
		uniform float  filterWidth = 1;
		uniform float  useSampledBlur = 0;
		uniform float  DiffHemisphere = .95;
    uniform float  lerp = 1;
    output uniform string __category = "environment";
    output varying float  __nondiffuse = 1;
    output varying float  __nonspecular = 1;
  
)
{
    //extern point Ps;
    //point Q = Ps;

    //extern vector I;
    //extern normal N;	
    //varying float blur;
	
    me_envlight(
    envname,
    envspace,
    Intensity,
    reflBlur,
    samples,
    Coloration,
    cFilt,
    Filter,
    filterWidth,
    useSampledBlur,
    DiffHemisphere,
    lerp,
    __category,
    __nondiffuse,
    __nonspecular
    );

}

