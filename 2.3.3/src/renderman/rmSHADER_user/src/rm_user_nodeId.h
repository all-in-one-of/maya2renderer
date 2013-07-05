#ifndef _RM_USER_NODE_ID_
#define _RM_USER_NODE_ID_

#include <common/prerequest_maya.h>
#include "../../rm_nodeId.h"

namespace renderman
{
	// 0x00000000 - 0x0007ffff is used for local testing of nodes
	enum UserTypeID
	{
		UTI_START			= 0x00041000,

		UTI_Flat_NODE		= 0x00041000,
		UTI_Physicalsun_NODE= 0x00041001,

		UTI_NUM,
	};
	//

	//
}//namespace renderman
#endif//_RM_USER_NODE_ID_