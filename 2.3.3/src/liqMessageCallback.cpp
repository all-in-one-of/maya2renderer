#include "liqMessageCallback.h"

#include <liqlog.h>
#include <common/mayacheck.h>

namespace liquid
{
	MCallbackId		liqMessageCallback::sNodeAddCallbackId;
	//
	liqMessageCallback::liqMessageCallback(){;}
	//
	liqMessageCallback::~liqMessageCallback(){;}
	//
	void liqMessageCallback::registCallback()
	{
		//CM_TRACE_FUNC("liqMessageCallback::registCallback()");
		IfMErrorWarn(MGlobal::executeCommand("source \"liqMessageCallback.mel\";"));
		liqMessageCallback::sNodeAddCallbackId = 
			MDGMessage::addNodeAddedCallback( liqMessageCallback::NodeAddCallbackFunc );
	}
	//
	void liqMessageCallback::removeCallback()
	{
		//CM_TRACE_FUNC("liqMessageCallback::removeCallback()");
		MDGMessage::removeCallback( liqMessageCallback::sNodeAddCallbackId );
	}
	//
	void liqMessageCallback::NodeAddCallbackFunc( MObject& node, void* clientData )
	{
		//CM_TRACE_FUNC("liqMessageCallback::NodeAddCallbackFunc("<<MFnDependencyNode(node).name().asChar()<<")");

		MFnDependencyNode dn( node );
		MString nodeName(dn.name());

		IfMErrorWarn(MGlobal::executeCommand("liqMDGMessage_addNodeAddedCallback(\""+nodeName+"\")"));
	}
}//namespace liquid