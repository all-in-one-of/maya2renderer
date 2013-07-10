#ifndef _RM_CLASSIFICATION_H_
#define _RM_CLASSIFICATION_H_

#include <common/prerequest_maya.h>

#include "rm_.h"
namespace renderman
{
	const MString CR_NAME = RENDER_NAME;
	//const MString CR_NAME = "liquid";

	const MString surface_classification = 
		"rendernode/"+CR_NAME+"/shader/surface:shader/surface:swatch/liquidSwatch";
	const MString env_classification = 
		"rendernode/"+CR_NAME+"/texture/environment:texture/environment:swatch/liquidSwatch";
	//It is wierd that liquidLight is not displayed in Hypershade tpo tab 'light', so I put liquidLight into surface tab temporarily.
	const MString light_classification = 
		"rendernode/"+CR_NAME+"/shader/surface:shader/surface:swatch/liquidSwatch";
	const MString texture2d_classification = 
		"rendernode/"+CR_NAME+"/texture/2d:texture/2d:swatch/liquidSwatch";
	
}//namespace renderman

#endif//_RM_CLASSIFICATION_H_