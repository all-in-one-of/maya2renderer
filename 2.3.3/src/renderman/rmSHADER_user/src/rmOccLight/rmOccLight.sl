/* renderman shader */
#include "rmOccLight.impl"
/*
	A light source that computes geometric occlusion 
	
	PARAMETERS
	Kocc      : occlusion factor (0 = no occlusion) 
	samples   : number of samples to use when calling occlusion().
	maxdist   : maximum intersection distance to consider
	distribution
	          : environment map to use for ray distributions. The default
	            "cosine" value will sample rays from a cosine distribution 
	---
	Aghiles Kheffache.
	Wed Feb 15 17:39:08 EST 2006	
*/
light rmOccLight(
	float Kocc = 1.0;
	float samples = 64;
	float maxdist = 1e36;
	string distribution = "cosine";
	float __nonspecular = 1; 
)
{
	rmOccLight_impl(
		Kocc, 
		samples,

		maxdist,
		distribution,
		__nonspecular
	);
}
