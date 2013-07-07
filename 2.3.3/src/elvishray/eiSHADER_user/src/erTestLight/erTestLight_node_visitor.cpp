
#include "erTestLight_node_visitor.h"
#include <common/mayacheck.h>

#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>

#include "../er_log.h"
#include "../er_output_mgr.h"
#include "../shaderOutputER.h"
#include "erTestLight_node.h"
#include "../er_rnode_visitor_mgr.h"
#include "../er_classification.h"
#include "../er_helper.h"

namespace elvishray
{
	TestLightNodeVisitor* TestLightNodeVisitor::m_instance = NULL;
	//
	TestLightNodeVisitor* TestLightNodeVisitor::getInstancePtr()
	{
		if( m_instance == NULL )
		{
			m_instance = new TestLightNodeVisitor();
		}
		return m_instance;
	}
	//
	TestLightNodeVisitor::TestLightNodeVisitor()
	{

	}
	//
	TestLightNodeVisitor::~TestLightNodeVisitor()
	{

	}
	void TestLightNodeVisitor::setValidConnection(liquidmaya::ShaderConnectionMap& validConnectionMap)
	{
		CM_TRACE_FUNC("TestLightNodeVisitor::setValidConnection(...)");

		validConnectionMap.begin(TestLightNode::getTypeName());
		validConnectionMap.append("lightcolorR");
		validConnectionMap.append("lightcolorG");
		validConnectionMap.append("lightcolorB");
		validConnectionMap.append("lightcolor");
		validConnectionMap.append("intensity");
		validConnectionMap.end();
	}
	bool TestLightNodeVisitor::visit(const char *node)
	{
		CM_TRACE_FUNC("TestLightNodeVisitor::visit("<<node<<")");

		OutputMgr &out = getOutputMgr();

		ER::OutputHelper o;
		o.beginRSL(TestLightNode::getTypeName(), node);
	
		o.addRSLVariable("color",	"lightcolor",	"lightcolor",		node);
		o.addRSLVariable("float",	"intensity",	"intensity",		node);

		o.endRSL();

		return true;
	}
	//
	bool TestLightNodeVisitor::onCreateInstance(const char *node)
	{
		//in batch mode, this function is called after the rendering, 
		//so it complains the cm::trace file is not open, so I omit this CM_TRACE_FUNC()
		//CM_TRACE_FUNC("TestLightNodeVisitor::onCreateInstance("<<node<<")");
		
		return true;
	}
	//
	bool  TestLightNodeVisitor::regist(MFnPlugin &plugin)
	{
		CM_TRACE_FUNC("TestLightNodeVisitor::regist()");

		MStatus status;

		status = plugin.registerNode( 
			elvishray::TestLightNode::getTypeName(), 
			elvishray::TestLightNode::getTypeId(), 
			elvishray::TestLightNode::creator, elvishray::TestLightNode::initialize, MPxNode::kDependNode,
			&elvishray::light_classification);
		LIQCHECKSTATUS( status, "Can't register "+elvishray::TestLightNode::getTypeName()+" node" );
		status.clear();

		RNodeVisitorMgr::getInstancePtr()->regist(
			TestLightNode::getTypeName().asChar(),
			TestLightNodeVisitor::getInstancePtr()
			);
		return true;
	}
	bool  TestLightNodeVisitor::unregist(MFnPlugin &plugin)
	{
		CM_TRACE_FUNC("TestLightNodeVisitor::unregist()");

		MStatus status;

		RNodeVisitorMgr::getInstancePtr()->unregist(
			TestLightNode::getTypeName().asChar()
			);

		status = plugin.deregisterNode( elvishray::TestLightNode::getTypeId() );
		LIQCHECKSTATUS( status, "Can't deregister "+elvishray::TestLightNode::getTypeName()+" node" );

		return true;
	}

}//namespace