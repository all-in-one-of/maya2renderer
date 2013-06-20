
#include "er_checker_node_visitor.h"
#include <common/mayacheck.h>
#include "../er_log.h"
#include "../er_output_mgr.h"
#include "../shaderOutputER.h"
#include "er_checker_node.h"
#include "../er_rnode_visitor_mgr.h"

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
// 		validConnectionMap.append("resultR");
// 		validConnectionMap.append("resultG");
// 		validConnectionMap.append("resultB");
// 		validConnectionMap.append("result");
		validConnectionMap.end();
	}
	bool CheckerNodeVisitor::visit(const char *node)
	{
		CM_TRACE_FUNC("CheckerNodeVisitor::visit("<<node<<")");

		ER::OutputHelper o;
		o.beginRSL(CheckerNode::getTypeName(), node);

		o.addRSLVariable("color",	"color1",	"color1",		node);
		o.addRSLVariable("color",	"color2",	"color2",		node);
		o.addRSLVariable("float",	"xscale",	"xscale",		node);
		o.addRSLVariable("float",	"yscale",	"yscale",		node);
		//o.addRSLVariable("color",	"result",	"result",		node);
		
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
	bool  CheckerNodeVisitor::regist()
	{
		CM_TRACE_FUNC("CheckerNodeVisitor::regist()");

		RNodeVisitorMgr::getInstancePtr()->regist(
			CheckerNode::getTypeName().asChar(),
			CheckerNodeVisitor::getInstancePtr()
			);
		return true;
	}
	bool  CheckerNodeVisitor::unregist()
	{
		CM_TRACE_FUNC("CheckerNodeVisitor::unregist()");

		RNodeVisitorMgr::getInstancePtr()->unregist(
			CheckerNode::getTypeName().asChar()
			);
		return true;
	}

}//namespace