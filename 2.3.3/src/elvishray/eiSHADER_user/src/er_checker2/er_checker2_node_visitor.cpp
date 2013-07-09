
#include "er_checker2_node_visitor.h"
#include <common/mayacheck.h>
#include <liqNodeSwatch.h>

#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>

#include "../er_log.h"
#include "../er_output_mgr.h"
#include "../shaderOutputER.h"
#include "er_checker2_node.h"
#include "../er_rnode_visitor_mgr.h"
#include "../er_classification.h"

namespace elvishray
{
	Checker2NodeVisitor* Checker2NodeVisitor::m_instance = NULL;
	//
	Checker2NodeVisitor* Checker2NodeVisitor::getInstancePtr()
	{
		if( m_instance == NULL )
		{
			m_instance = new Checker2NodeVisitor();
		}
		return m_instance;
	}
	//
	Checker2NodeVisitor::Checker2NodeVisitor()
	{

	}
	//
	Checker2NodeVisitor::~Checker2NodeVisitor()
	{

	}
	void Checker2NodeVisitor::setValidConnection(liquidmaya::ShaderConnectionMap& validConnectionMap)
	{
		CM_TRACE_FUNC("Checker2NodeVisitor::setValidConnection(...)");

		validConnectionMap.begin(Checker2Node::getTypeName());
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
// 		validConnectionMap.append("resultR");
// 		validConnectionMap.append("resultG");
// 		validConnectionMap.append("resultB");
// 		validConnectionMap.append("result");
		validConnectionMap.end();
	}
	bool Checker2NodeVisitor::visit(const char *node)
	{
		CM_TRACE_FUNC("Checker2NodeVisitor::visit("<<node<<")");

		ER::OutputHelper o;
		o.beginRSL(Checker2Node::getTypeName(), node);

		o.addRSLVariable("color",	"color1",	"color1",		node);
		o.addRSLVariable("color",	"color2",	"color2",		node);
		o.addRSLVariable("float",	"xscale",	"xscale",		node);
		o.addRSLVariable("float",	"yscale",	"yscale",		node);
		//o.addRSLVariable("color",	"result",	"result",		node);
		
		o.endRSL();

		return true;
	}
	//
	bool Checker2NodeVisitor::onCreateInstance(const char *node)
	{
		//in batch mode, this function is called after the rendering, 
		//so it complains the cm::trace file is not open, so I omit this CM_TRACE_FUNC()
		//CM_TRACE_FUNC("Checker2NodeVisitor::onCreateInstance("<<node<<")");
		
		return true;
	}
	//
	bool  Checker2NodeVisitor::regist(MFnPlugin &plugin)
	{
		CM_TRACE_FUNC("Checker2NodeVisitor::regist()");

		MStatus status;

		status = plugin.registerNode( 
			elvishray::Checker2Node::getTypeName(), 
			elvishray::Checker2Node::getTypeId(), 
			elvishray::Checker2Node::creator, elvishray::Checker2Node::initialize, MPxNode::kDependNode,
			&elvishray::texture2d_classification);
		IfMErrorMsgReturnIt( status, "Can't register "+elvishray::Checker2Node::getTypeName()+" node" );
		status.clear();
		status = MSwatchRenderRegister::registerSwatchRender( elvishray::Checker2Node::getTypeName()+"Swatch", liqNodeSwatch::creator );
		IfMErrorMsgReturnIt( status, "Can't register "+elvishray::Checker2Node::getTypeName()+"Swatch" );
		
		RNodeVisitorMgr::getInstancePtr()->regist(
			Checker2Node::getTypeName().asChar(),
			Checker2NodeVisitor::getInstancePtr()
			);
		return true;
	}
	bool  Checker2NodeVisitor::unregist(MFnPlugin &plugin)
	{
		CM_TRACE_FUNC("Checker2NodeVisitor::unregist()");

		MStatus status;

		RNodeVisitorMgr::getInstancePtr()->unregist(
			Checker2Node::getTypeName().asChar()
			);

		status = MSwatchRenderRegister::unregisterSwatchRender(elvishray::Checker2Node::getTypeName()+"Swatch");
		IfMErrorMsgReturnIt( status, "Can't deregister "+elvishray::Checker2Node::getTypeName()+"Swatch generator" );
		status = plugin.deregisterNode( elvishray::Checker2Node::getTypeId() );
		IfMErrorMsgReturnIt( status, "Can't deregister "+elvishray::Checker2Node::getTypeName()+" node" );

		return true;
	}

}//namespace