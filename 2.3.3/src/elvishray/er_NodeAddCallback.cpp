#include "er_NodeAddCallback.h"

#include <liqlog.h>
#include "er_rnode_visitor_mgr.h"

namespace elvishray
{
	MCallbackId		NodeAddCallback::sId;
	//
	NodeAddCallback::NodeAddCallback(){;}
	//
	NodeAddCallback::~NodeAddCallback(){;}
	//
	void NodeAddCallback::registCallback()
	{
		//CM_TRACE_FUNC("NodeAddCallback::registCallback()");
		NodeAddCallback::sId = 
			MDGMessage::addNodeAddedCallback( NodeAddCallback::NodeAddCallbackFunc );
	}
	//
	void NodeAddCallback::removeCallback()
	{
		//CM_TRACE_FUNC("NodeAddCallback::removeCallback()");
		MDGMessage::removeCallback( NodeAddCallback::sId );
	}
	//
	void NodeAddCallback::NodeAddCallbackFunc( MObject& node, void* clientData )
	{
		//CM_TRACE_FUNC("NodeAddCallback::NodeAddCallbackFunc("<<MFnDependencyNode(node).name().asChar()<<")");

		MFnDependencyNode dn( node );
		MString nodeName(dn.name());
		MString typeName(dn.typeName());

		if( typeName =="er_physicalsky" )
		{
			liquidMessage2(messageInfo, "NodeAddCallback::NodeAddCallbackFunc(), nodeName=%s", nodeName.asChar());

			MString $shaderFilePath =
				"E:/dev/Autodesk/maya/myplugin/project/liquid_/2.3.3/test/test_er_env/maya_physicalsky.dll";

			MString cmd = 
			"setAttr -type \"string\" (\""+nodeName+".rmanShaderLong\") \""+$shaderFilePath+"\";"
			//+"liquidShaderNodes_reloadAndRefresh(\""+nodeName+"\");"
			+"liquidShaderNodes_initParams_pl(\""+nodeName+"\", \""+$shaderFilePath+"\");"
			;
			MGlobal::executeCommandOnIdle(cmd, true);
		}
		else if( typeName =="er_skylight" ){
			MString cmd = 
				"setAttr -type \"string\" (\""+nodeName+".rmanShader\") \"liq_skylight\";"
				+"setAttr -type \"string\" (\""+nodeName+".rmanShaderType\") \"light\";"
				;
			MGlobal::executeCommandOnIdle(cmd, true);
		}
		else {
			//on other node types
			RNodeVisitorMgr::getInstancePtr()->onCreateInstance(nodeName.asChar());
		}
		

	}
}//namespace elvishray