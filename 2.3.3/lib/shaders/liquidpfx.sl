/**
 *  Liquid Maya Paint Effects default surface shader
 */


#include "liquidpfx.impl"

surface liquidpfx(
	varying float roughness        = 0.1;

    // Rest is filled-in by primvars
	varying float width            = 0;
	varying float pfxflatness      = 0;
	varying color pfxincandescence = 0;
) 
{

    liquidpfx(
	roughness,

        // Rest is filled-in by primvars
	width,
	pfxflatness,
	pfxincandescence
    );

}
