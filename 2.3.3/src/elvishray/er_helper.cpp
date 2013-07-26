#include <common/mayacheck.h>
#include "er_helper.h"
#include "er_renderer.h"

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
	// outColor --> o_Color
	// color    --> i_color
	MString convertMayaPlugNameToERShaderParameterName(const MString& attr)
	{
		MString shaderParameterName(attr);
		if( shaderParameterName.substring(0, 2)=="out"  )//attr is an output plug
		{
			//e.g. outColor --> o_outColor
			shaderParameterName = "o_" + shaderParameterName;
		}
		else{//src attribute is an input plug
			if(shaderParameterName.substring(0, 1) == "i_")
			{	
				//e.g. i_color --> i_color
				// do nothing
			}
			//else if(shaderParameterName.substring(0, 1) == "in")
			//{	// e.g. inColor --> i_Color
			//	shaderParameterName = "i_" + shaderParameterName.substring(2, shaderParameterName.length()-1);
			//}
			else{// e.g. color --> i_color
				shaderParameterName = "i_" + shaderParameterName;
			}
		}
		return shaderParameterName;
	}

	//----------------------------------------------------
	OutputMgr& getOutputMgr()
	{
		return Renderer::getOutputMgr();
	}


}//namespace elvishray