#ifndef _LIQ_CLASSIFICATION_H_
#define _LIQ_CLASSIFICATION_H_

#include <common/prerequest_maya.h>

namespace liquid
{
	const MString CR_NAME = "liquid";//update with liq_getRendererName() in liq_RendererName.mel

	const MString texture2d_classification = 
		"rendernode/"+CR_NAME+"/texture/2d:texture/2d:swatch/liquidSwatch";
	const MString texture3d_classification = 
		"rendernode/"+CR_NAME+"/texture/3d:texture/3d:swatch/liquidSwatch";
	const MString textureEnv_classification = 
		"rendernode/"+CR_NAME+"/texture/environment:texture/environment:swatch/liquidSwatch";
	
	const MString surface_classification = 
		"rendernode/"+CR_NAME+"/shader/surface:shader/surface:swatch/liquidSwatch";
	
	const MString volume_classification = 
		"rendernode/"+CR_NAME+"/shader/volume:shader/volume:swatch/liquidSwatch";
	
	const MString displacement_classification = 
		"rendernode/"+CR_NAME+"/shader/displacement:shader/displacement:swatch/liquidSwatch";
	//It is weird that liquidLight is not displayed in Hypershade top tab 'light', so I put liquidLight into surface tab temporarily.
	const MString light_classification = 
		"rendernode/"+CR_NAME+"/shader/surface:shader/surface:swatch/liquidSwatch";

	const MString utility_general_classification = 
		"rendernode/"+CR_NAME+"/utility/general:utility/general:swatch/liquidSwatch";
	const MString utility_color_classification = 
		"rendernode/"+CR_NAME+"/utility/color:utility/color:swatch/liquidSwatch";
	const MString utility_particle_classification = 
		"rendernode/"+CR_NAME+"/utility/particle:utility/particle:swatch/liquidSwatch";

	const MString img_classification = 
		"rendernode/"+CR_NAME+"/imageplane:imageplane:swatch/liquidSwatch";

	const MString postprocess_classification = 
		"rendernode/"+CR_NAME+"/postprocess:postprocess:swatch/liquidSwatch";
}//namespace liquid

#endif//_LIQ_CLASSIFICATION_H_