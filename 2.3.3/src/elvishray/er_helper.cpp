#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_
#include "../common/mayacheck.h"
#include "er_helper.h"


namespace elvishray
{
// 	MString getGroupName(const MString& objname)
// 	{
// 		return objname+"_grp";
// 	}
	int isBumpMapConnected(const char* nodename)
	{
		MStringArray srcNode;
		//we assume this node has normalCamera plug
		IfMErrorWarn(MGlobal::executeCommand("listConnections -source on -destination off -plugs off \""+MString(nodename)+".normalCamera\"", srcNode));
		return (srcNode.length()>0)? 1 : 0;
	}




}//namespace elvishray
#endif//_USE_ELVISHRAY_