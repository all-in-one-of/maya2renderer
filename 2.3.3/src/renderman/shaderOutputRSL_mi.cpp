#include "shaderOutputRSL.h"
#include "../common/mayacheck.h"
#include "ri_interface.h"
#include "rm_log.h"

namespace RSL
{
//

//
// @node	mr shader node name
void Visitor::visit_mib_amb_occlusion(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_mib_amb_occlusion("<<node<<")");
#ifdef USE_LIQUID_RM_SHADE
	OutputHelper o(RSLfile, m_NodePlugInfo);
	o.beginRSL(node);

	o.addRSLVariable(       "", "float",	"i_samples",	"samples",		node);
	o.addRSLVariable(       "", "color",	"i_bright",		"bright",		node);
	o.addRSLVariable(       "", "color",	"i_dark",		"dark",			node);
	o.addRSLVariable(       "", "float",	"i_spread",		"spread",		node);
	o.addRSLVariable(       "", "float",	"i_max_distance","max_distance",node);
	o.addRSLVariable(       "", "float",	"i_reflective",	"reflective",	node);
	o.addRSLVariable(       "", "color",	"o_outValue",	"outValue",		node);

	o.addToRSL( 
		"extern point P;\n"
		"extern normal N;\n"
		"extern vector I;\n"
		"if(i_max_distance < 0.0001){//If it is zero, the entire scene is sampled\n"
		"      i_max_distance = 1.0e37;\n"
		"}\n"
		"\n"
		"normal Nn = faceforward(normalize(N), I);\n"
		"\n"
		"if(i_reflective != 0)				\n"
		"	Nn = normal reflect( I, Nn );	\n"
		"\n"
		"float occ= 1 - occlusion(P, Nn, i_samples,				\n"
		"                        \"adaptive\", 1,					\n"
		"                        \"maxdist\", i_max_distance,		\n"
		"                        \"coneangle\", i_spread * PI / 2);	\n"
		"\n"
		"o_outValue = mix( i_dark, i_bright, occ );				\n"
	);

	o.endRSL();
#else
	OutputHelper o(RSLfile, m_NodePlugInfo);

	o.addInclude("mib_amb_occlusion.h");
	o.beginRSL(node);

	o.addRSLVariable(       "", "float",	"i_samples",	"samples",		node);
	o.addRSLVariable(       "", "color",	"i_bright",		"bright",		node);
	o.addRSLVariable(       "", "color",	"i_dark",		"dark",			node);
	o.addRSLVariable(       "", "float",	"i_spread",		"spread",		node);
	o.addRSLVariable(       "", "float",	"i_max_distance","max_distance",node);
	o.addRSLVariable(       "", "float",	"i_reflective",	"reflective",	node);
	o.addRSLVariable(       "", "color",	"o_outValue",	"outValue",		node);

	o.addToRSL("{");

	o.addToRSL(
		"if(i_max_distance < 0.0001){//If it is zero, the entire scene is sampled\n"
		"      i_max_distance = 1.0e37;\n"
		"}\n"
		);
	o.addToRSL("  maya_mib_amb_occlusion("
					//Inputs
					"i_samples,		\n\t"
					"i_bright,		\n\t"
					"i_dark,		\n\t"
					"i_spread,		\n\t"
					"i_max_distance,\n\t"
					"i_reflective,	\n\t"
					//Outputs
					"o_outValue		\n"
			"   );");
	o.addToRSL("}");
	o.endRSL();

#endif
}
// @node	mr shader node name
void Visitor::visit_mia_physicalsky(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_mia_physicalsky("<<node<<")");

	OutputHelper o;
	o.beginRSL("mia_physicalsky", node);

	o.addRSLVariable("bool",	"i_on",				"on",					node);
	o.addRSLVariable("float",	"i_multiplier",		"multiplier",			node);
	o.addRSLVariable("color",	"i_rgb_unit_conversion","rgb_unit_conversion",	node);
	o.addRSLVariable("float",	"i_haze",			"haze",					node);
	o.addRSLVariable("float",	"i_redblueshift",	"redblueshift",			node);
	o.addRSLVariable("float",	"i_saturation",		"saturation",			node);
	o.addRSLVariable("float",	"i_horizon_height",	"horizon_height",		node);
	o.addRSLVariable("float",	"i_horizon_blur",	"horizon_blur",			node);
	o.addRSLVariable("color",	"i_ground_color",	"ground_color",			node);
	o.addRSLVariable("color",	"i_night_color",	"night_color",			node);
	o.addRSLVariable("vector",	"i_sun_direction",	"sun_direction",		node);
	o.addRSLVariable("shader",	"i_sun",			"sun",					node);
	o.addRSLVariable("float",	"i_sun_disk_intensity",	"sun_disk_intensity",	node);
	o.addRSLVariable("float",	"i_sun_disk_scale",		"sun_disk_scale",		node);
	o.addRSLVariable("float",	"i_sun_glow_intensity",	"sun_glow_intensity",	node);
	o.addRSLVariable("bool",	"i_use_background",		"use_background",		node);
	o.addRSLVariable("shader",	"i_background",			"background",			node);
	o.addRSLVariable("float",	"i_visibility_distance","visibility_distance",	node);
	o.addRSLVariable("bool",	"i_y_is_up",			"y_is_up",				node);
	o.addRSLVariable("int",		"i_flags",				"flags",				node);
	o.addRSLVariable("int",		"i_sky_luminance_mode",	"sky_luminance_mode",	node);
	o.addRSLVariable("float",	"i_zenith_luminance",	"zenith_luminance",		node);
	o.addRSLVariable("float",	"i_diffuse_horizontal_illuminance",	"diffuse_horizontal_illuminance",	node);
	o.addRSLVariable("float",	"i_a",	"a",	node);
	o.addRSLVariable("float",	"i_b",	"b",	node);
	o.addRSLVariable("float",	"i_c",	"c",	node);
	o.addRSLVariable("float",	"i_d",	"d",	node);
	o.addRSLVariable("float",	"i_e",	"e",	node);
	o.addRSLVariable("bool",	"i_physically_scaled_sun","physically_scaled_sun",	node);

	o.endRSL();
}
// @node	mr shader node name
void Visitor::visit_mia_physicalsun(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_mia_physicalsun("<<node<<")");

	OutputHelper o;
	o.beginRSL("mia_physicalsun", node);

	o.addRSLVariable("bool",	"i_on",				"on",					node);
	o.addRSLVariable("float",	"i_multiplier",		"multiplier",			node);
	o.addRSLVariable("color",	"i_rgb_unit_conversion",	"rgb_unit_conversion",		node);
	o.addRSLVariable("float",	"i_haze",			"haze",					node);
	o.addRSLVariable("float",	"i_redblueshift",	"redblueshift",			node);
	o.addRSLVariable("float",	"i_saturation",		"saturation",			node);
	o.addRSLVariable("float",	"i_horizon_height",	"horizon_height",		node);
	o.addRSLVariable("float",	"i_shadow_softness","shadow_softness",		node);
	o.addRSLVariable("int",		"i_samples",		"samples",				node);
	o.addRSLVariable("vector",	"i_photon_bbox_min",		"photon_bbox_min",	node);
	o.addRSLVariable("vector",	"i_photon_bbox_max",		"photon_bbox_max",	node);
	o.addRSLVariable("bool",	"i_automatic_photon_energy","automatic_photon_energy",	node);
	o.addRSLVariable("bool",	"i_y_is_up",				"y_is_up",							node);
	o.addRSLVariable("int",		"i_illuminance_mode",		"illuminance_mode",			node);
	o.addRSLVariable("float",	"i_direct_normal_illuminance",	"direct_normal_illuminance",	node);

	o.endRSL();
}

}//namespace RSL