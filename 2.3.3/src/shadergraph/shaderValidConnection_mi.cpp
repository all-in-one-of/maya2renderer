#include "shaderValidConnection.h"
#include <liqlog.h>

namespace liquidmaya
{

	void ShaderValidConnection::setValidConnection_mi()
	{
		//mib_amb_occlusion
		validConnectionMap.begin("mib_amb_occlusion");
		validConnectionMap.append("samples");
		validConnectionMap.append("brightR");
		validConnectionMap.append("brightG");
		validConnectionMap.append("brightB");
		validConnectionMap.append("bright");
		validConnectionMap.append("darkR");
		validConnectionMap.append("darkG");
		validConnectionMap.append("darkB");
		validConnectionMap.append("dark");
		validConnectionMap.append("spread");
		validConnectionMap.append("max_distance");
		validConnectionMap.append("reflective");
// 		validConnectionMap.append("output_mode");
// 		validConnectionMap.append("occlusion_in_alpha");
// 		validConnectionMap.append("falloff");
// 		validConnectionMap.append("id_inclexcl");
// 		validConnectionMap.append("id_nonself");
		validConnectionMap.append("outValueR");
		validConnectionMap.append("outValueG");
		validConnectionMap.append("outValueB");
		validConnectionMap.append("outValue");
		validConnectionMap.end();	

		//mia_physicalsky
		validConnectionMap.begin("mia_physicalsky");
		validConnectionMap.append("on");
		validConnectionMap.append("multiplier");
		validConnectionMap.append("rgb_unit_conversionR");
		validConnectionMap.append("rgb_unit_conversionG");
		validConnectionMap.append("rgb_unit_conversionB");
		validConnectionMap.append("rgb_unit_conversion");
		validConnectionMap.append("haze");
		validConnectionMap.append("redblueshift");
		validConnectionMap.append("saturation");
		validConnectionMap.append("horizon_height");
		validConnectionMap.append("horizon_blur");
		validConnectionMap.append("ground_colorR");
		validConnectionMap.append("ground_colorG");
		validConnectionMap.append("ground_colorB");
		validConnectionMap.append("ground_color");
		validConnectionMap.append("night_colorR");
		validConnectionMap.append("night_colorG");
		validConnectionMap.append("night_colorB");
		validConnectionMap.append("night_color");
		validConnectionMap.append("sun_directionX");
		validConnectionMap.append("sun_directionY");
		validConnectionMap.append("sun_directionZ");
		validConnectionMap.append("sun_direction");
		validConnectionMap.append("sun");
		validConnectionMap.append("sun_disk_intensity");
		validConnectionMap.append("sun_disk_scale");
		validConnectionMap.append("sun_glow_intensity");
		validConnectionMap.append("use_background");
		validConnectionMap.append("background");
		validConnectionMap.append("visibility_distance");
		validConnectionMap.append("y_is_up");
		validConnectionMap.append("flags");
		validConnectionMap.append("sky_luminance_mode");
		validConnectionMap.append("zenith_luminance");
		validConnectionMap.append("diffuse_horizontal_illuminance");
		validConnectionMap.append("a");
		validConnectionMap.append("b");
		validConnectionMap.append("c");
		validConnectionMap.append("d");
		validConnectionMap.append("e");
		validConnectionMap.append("physically_scaled_sun");
		validConnectionMap.end();	

		//mia_physicalsun
		validConnectionMap.begin("mia_physicalsun");
		validConnectionMap.append("on");
		validConnectionMap.append("multiplier");
		validConnectionMap.append("rgb_unit_conversionR");
		validConnectionMap.append("rgb_unit_conversionG");
		validConnectionMap.append("rgb_unit_conversionB");
		validConnectionMap.append("rgb_unit_conversion");
		validConnectionMap.append("haze");
		validConnectionMap.append("redblueshift");
		validConnectionMap.append("saturation");
		validConnectionMap.append("horizon_height");
		validConnectionMap.append("shadow_softness");
		validConnectionMap.append("samples");
		validConnectionMap.append("photon_bbox_minX");
		validConnectionMap.append("photon_bbox_minY");
		validConnectionMap.append("photon_bbox_minZ");
		validConnectionMap.append("photon_bbox_min");
		validConnectionMap.append("photon_bbox_maxX");
		validConnectionMap.append("photon_bbox_maxY");
		validConnectionMap.append("photon_bbox_maxZ");
		validConnectionMap.append("photon_bbox_max");
		validConnectionMap.append("automatic_photon_energy");
		validConnectionMap.append("y_is_up");
		validConnectionMap.append("illuminance_mode");
		validConnectionMap.append("direct_normal_illuminance");
		validConnectionMap.end();
	}
}//namespace liquidmaya