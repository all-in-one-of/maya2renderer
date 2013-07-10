#ifndef _ER_CLASSIFICATION_H_
#define _ER_CLASSIFICATION_H_

#include <common/prerequest_maya.h>

#include "er_.h"
namespace elvishray
{
	const MString CR_NAME = RENDER_NAME;
	//const MString CR_NAME = "liquid";

	const MString surface_classification = 
		"rendernode/"+CR_NAME+"/shader/surface:shader/surface:swatch/liquidSwatch";
	const MString env_classification = 
		"rendernode/"+CR_NAME+"/texture/environment:texture/environment:swatch/liquidSwatch";
	//It is weird that liquidLight is not displayed in Hypershade top tab 'light', so I put liquidLight into surface tab temporarily.
	const MString light_classification = 
		"rendernode/"+CR_NAME+"/shader/surface:shader/surface:swatch/liquidSwatch";
	const MString texture2d_classification = 
		"rendernode/"+CR_NAME+"/texture/2d:texture/2d:swatch/liquidSwatch";
	
}//namespace elvishray

#endif//_ER_CLASSIFICATION_H_