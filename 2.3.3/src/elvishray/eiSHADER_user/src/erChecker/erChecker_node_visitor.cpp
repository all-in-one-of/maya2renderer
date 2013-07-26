
#include "erChecker_node_visitor.h"
#include <common/mayacheck.h>
#include <liqNodeSwatch.h>

#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>

#include "../er_log.h"
#include "../er_output_mgr.h"
#include "../shaderOutputER.h"
#include "erChecker_node.h"
#include "../er_rnode_visitor_mgr.h"
#include "../er_classification.h"
#include "../er_helper.h"

namespace elvishray
{
	CheckerNodeVisitor* CheckerNodeVisitor::m_instance = NULL;
	//
	CheckerNodeVisitor* CheckerNodeVisitor::getInstancePtr()
	{
		if( m_instance == NULL )
		{
			m_instance = new CheckerNodeVisitor();
		}
		return m_instance;
	}
	//
	CheckerNodeVisitor::CheckerNodeVisitor()
	{

	}
	//
	CheckerNodeVisitor::~CheckerNodeVisitor()
	{

	}
	void CheckerNodeVisitor::setValidConnection(liquidmaya::ShaderConnectionMap& validConnectionMap)
	{
		CM_TRACE_FUNC("CheckerNodeVisitor::setValidConnection(...)");

		validConnectionMap.begin(CheckerNode::getTypeName());
		validConnectionMap.append("color1R");
		validConnectionMap.append("color1G");
		validConnectionMap.append("color1B");
		validConnectionMap.append("color1");
		validConnectionMap.append("color2R");
		validConnectionMap.append("color2G");
		validConnectionMap.append("color2B");
		validConnectionMap.append("color2");
		validConnectionMap.append("xscale");
		validConnectionMap.append("yscale");
		//validConnectionMap.append("outColorR");
		//validConnectionMap.append("outColorG");
		//validConnectionMap.append("outColorB");
		//validConnectionMap.append("outColor");
		//validConnectionMap.append("outTransparencyR");
		//validConnectionMap.append("outTransparencyG");
		//validConnectionMap.append("outTransparencyB");
		//validConnectionMap.append("outTransparency");
		validConnectionMap.end();
	}
	bool CheckerNodeVisitor::visit(const char *node)
	{
		CM_TRACE_FUNC("CheckerNodeVisitor::visit("<<node<<")");

		ER::OutputHelper o;
		o.beginRSL(CheckerNode::getTypeName(), node);

		o.addRSLVariable("color",	ER_SPN("color1"),	"color1",		node);
		o.addRSLVariable("color",	ER_SPN("color2"),	"color2",		node);
		o.addRSLVariable("float",	ER_SPN("xscale"),	"xscale",		node);
		o.addRSLVariable("float",	ER_SPN("yscale"),	"yscale",		node);
		//o.addRSLVariable("color",	ER_SPN("result"),	"result",		node);
		
		o.endRSL();

		return true;
	}
	//
	bool CheckerNodeVisitor::onCreateInstance(const char *node)
	{
		//in batch mode, this function is called after the rendering, 
		//so it complains the cm::trace file is not open, so I omit this CM_TRACE_FUNC()
		//CM_TRACE_FUNC("CheckerNodeVisitor::onCreateInstance("<<node<<")");
		
		return true;
	}
	//
	bool  CheckerNodeVisitor::regist(MFnPlugin &plugin)
	{
		//CM_TRACE_FUNC("CheckerNodeVisitor::regist()");

		MStatus status;

		status = plugin.registerNode( 
			elvishray::CheckerNode::getTypeName(), 
			elvishray::CheckerNode::getTypeId(), 
			elvishray::CheckerNode::creator, elvishray::CheckerNode::initialize, MPxNode::kDependNode,
			&elvishray::texture2d_classification);
		IfMErrorMsgReturnIt( status, "Can't register "+elvishray::CheckerNode::getTypeName()+" node" );
		status.clear();
		status = MSwatchRenderRegister::registerSwatchRender( elvishray::CheckerNode::getTypeName()+"Swatch", liqNodeSwatch::creator );
		IfMErrorMsgReturnIt( status, "Can't register "+elvishray::CheckerNode::getTypeName()+"Swatch" );
		
		RNodeVisitorMgr::getInstancePtr()->regist(
			CheckerNode::getTypeName().asChar(),
			CheckerNodeVisitor::getInstancePtr()
			);
		return true;
	}
	bool  CheckerNodeVisitor::unregist(MFnPlugin &plugin)
	{
		//CM_TRACE_FUNC("CheckerNodeVisitor::unregist()");

		MStatus status;

		RNodeVisitorMgr::getInstancePtr()->unregist(
			CheckerNode::getTypeName().asChar()
			);

		status = MSwatchRenderRegister::unregisterSwatchRender(elvishray::CheckerNode::getTypeName()+"Swatch");
		IfMErrorMsgReturnIt( status, "Can't deregister "+elvishray::CheckerNode::getTypeName()+"Swatch generator" );
		status = plugin.deregisterNode( elvishray::CheckerNode::getTypeId() );
		IfMErrorMsgReturnIt( status, "Can't deregister "+elvishray::CheckerNode::getTypeName()+" node" );

		return true;
	}

}//namespace