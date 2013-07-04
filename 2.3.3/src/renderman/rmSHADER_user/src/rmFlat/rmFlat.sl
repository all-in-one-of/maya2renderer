/* renderman shader */
#include "rmFlat.impl"

surface rmFlat(
	color icolor   = color(0.8, 0.8, 0.8);
	color iopacity = color(0.0, 0.0, 0.0);
	
	output varying color o_outColor			= color(0.0, 0.0, 0.0);
	output varying color o_outTransparency	= color(0.0, 0.0, 0.0);
)
{
	extern color Cs;
	extern color Os;

	rmFlat(
		icolor, 
		iopacity,

		o_outColor,
		o_outTransparency
	);

	Oi = o_outTransparency;
	Ci = o_outColor * o_outTransparency;
}
