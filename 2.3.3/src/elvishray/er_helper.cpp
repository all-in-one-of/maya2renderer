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
	//
	int isPlugConnectedIn(const MString &nodename, const MString &plugname)
	{
		MStringArray srcNode;
		//we assume this node has normalCamera plug
		IfMErrorWarn(MGlobal::executeCommand("listConnections -source on -destination off -plugs off \""+nodename+"."+plugname+"\"", srcNode));
		return (srcNode.length()>0)? 1 : 0;
	}
	//
	int isBumpMapConnected(const char* nodename)
	{
		MStringArray srcNode;
		//we assume this node has normalCamera plug
		IfMErrorWarn(MGlobal::executeCommand("listConnections -source on -destination off -plugs off \""+MString(nodename)+".normalCamera\"", srcNode));
		return (srcNode.length()>0)? 1 : 0;
	}
	//
	MString getTestShaderName()
	{
		return "liquid_shader_for_test";
	}
	MString getTestMaterialName()
	{
		return "liquid_material_for_test";
	}
	//
	MString convertMayaPlugNameToERShaderParameterName(const MString& attr)
	{
		MString shaderParameterName(attr);
		if( shaderParameterName.substring(0, 2)=="out"  )//attr is an output plug
		{
			shaderParameterName = "o_" + shaderParameterName;
		}else{//src attribute is an input plug
			if(shaderParameterName.substring(0, 1) == "i_")
			{
				// do nothing
			}else{
				shaderParameterName = "i_" + shaderParameterName;
			}
		}
		return shaderParameterName;
	}



}//namespace elvishray
#endif//_USE_ELVISHRAY_