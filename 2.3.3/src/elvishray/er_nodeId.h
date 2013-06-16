#ifndef _ER_NODE_ID_
#define _ER_NODE_ID_
 
#include <common/prerequest_maya.h>
#include "er_.h"
namespace elvishray
{
	enum TypeID
	{
		TI_START				= 0xFFFFF360,

		TI_GLOBAL_NODE			= 0xFFFFF360,
		TI_Physicalsky_NODE		= 0xFFFFF361,

		TI_NUM,
	};
	//
	const MString env_classification = 
		"rendernode/"+RENDER_NAME+"/texture/environment:texture/environment:swatch/PhysicalskySwatch";

	//
}//namespace elvishray
#endif//_ER_NODE_ID_