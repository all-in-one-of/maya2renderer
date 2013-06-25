#ifndef _ER_CLASSIFICATION_H_
#define _ER_CLASSIFICATION_H_

#include <common/prerequest_maya.h>

#include "er_.h"
namespace elvishray
{
	//const MString CR_NAME = RENDER_NAME;
	const MString CR_NAME = "liquid";

	const MString surface_classification = 
		"rendernode/"+CR_NAME+"/shader/surface:shader/surface:swatch/FlatColorSwatch";
	const MString env_classification = 
		"rendernode/"+CR_NAME+"/texture/environment:texture/environment:swatch/PhysicalSkySwatch";
	const MString light_classification = 
		"rendernode/"+CR_NAME+"/light:light:swatch/SkyLightSwatch";

	
}//namespace elvishray

#endif//_ER_CLASSIFICATION_H_