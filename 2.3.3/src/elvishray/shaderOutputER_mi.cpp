#include "shaderOutputER.h"
//#include <liqlog.h>
#include <common/mayacheck.h>
//#include "../shadergraph/convertShadingNetwork.h"
//#include "../shadergraph/shadermgr.h"
#include "er_log.h"
#include "er_output_mgr.h"

namespace ER
{

// @node	mr shader node name
void Visitor::visit_mib_amb_occlusion(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_mib_amb_occlusion("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_mib_amb_occlusion", node);

	o.addRSLVariable("index",	"i_samples",		"samples",		node);
	o.addRSLVariable("color",	"i_bright",			"bright",		node);
	o.addRSLVariable("color",	"i_dark",			"dark",			node);
	o.addRSLVariable("float",	"i_spread",			"spread",		node);
	o.addRSLVariable("float",	"i_max_distance",	"max_distance",	node);
	o.addRSLVariable("bool",	"i_reflective",		"reflective",	node);
	//liquidMessage2(messageWarning, "visit_mib_amb_occlusion(): outValue should be added?");
	//o.addRSLVariable("color",	"o_outValue",		"outValue",		node);

	o.endRSL();
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
	out.ei_shader_param_token( "env_shader", "mia_physicalsky1" );

	o.endRSL();
}

}//namespace ER