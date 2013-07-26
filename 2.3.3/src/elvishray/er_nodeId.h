#ifndef _ER_NODE_ID_
#define _ER_NODE_ID_
 
#include <common/prerequest_maya.h>

namespace elvishray
{
	enum TypeID
	{
		TI_START				= 0xFFFFF360,

		TI_GLOBAL_NODE			= 0xFFFFF360,
		TI_Physicalsky_NODE		= 0xFFFFF361,
		TI_SkyLight_NODE		= 0xFFFFF362,


		TI_NUM,
	};
	//
}//namespace elvishray
#endif//_ER_NODE_ID_