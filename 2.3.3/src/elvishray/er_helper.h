#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#ifndef _ER_HELPER_H_
#define _ER_HELPER_H_
#include "../common/prerequest_maya.h"

namespace elvishray
{	
	//MString getGroupName(const MString& objname);
	int isBumpMapConnected(const char* nodename);
	//
	MString getTestShaderName();
	MString getTestMaterialName();
	//
}

#endif//_ER_HELPER_H_
#endif//_USE_ELVISHRAY_