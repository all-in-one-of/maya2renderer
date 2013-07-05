#include "rmPhysicalsun.impl"

/*
	physical sun
*/
light rmPhysicalsun(
	float i_multiplier = 1.0;
	color i_rgb_unit_conversion = color(0, 0, 0);
	float i_haze = 0.0;
	float i_redblueshift = 0.0;
	float i_saturation = 0.7;
	float i_horizon_height = 0.0;
	float i_shadow_softness = 0.0;

	string i_shadowmap = "raytrace";
	float i_shadow_samples = 3;
	bool i_y_is_up = false; )
{
	rmPhysicalsun_impl(
		i_multiplier,
		i_rgb_unit_conversion,
		i_haze,
		i_redblueshift,
		i_saturation,
		i_horizon_height,
		i_shadow_softness,

		i_shadowmap,
		i_shadow_samples,
		i_y_is_up
	);
}
