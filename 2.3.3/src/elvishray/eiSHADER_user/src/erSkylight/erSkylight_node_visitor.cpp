
#include "erSkylight_node_visitor.h"
#include <common/mayacheck.h>
#include <liqNodeSwatch.h>

#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>

#include "../er_log.h"
#include "../er_output_mgr.h"
#include "../shaderOutputER.h"
#include "erSkylight_node.h"
#include "../er_rnode_visitor_mgr.h"
#include "../er_classification.h"
#include "../er_helper.h"

namespace elvishray
{
	erSkylightNodeVisitor* erSkylightNodeVisitor::m_instance = NULL;
	//
	erSkylightNodeVisitor* erSkylightNodeVisitor::getInstancePtr()
	{
		if( m_instance == NULL )
		{
			m_instance = new erSkylightNodeVisitor();
		}
		return m_instance;
	}
	//
	erSkylightNodeVisitor::erSkylightNodeVisitor()
	{

	}
	//
	erSkylightNodeVisitor::~erSkylightNodeVisitor()
	{

	}
	void erSkylightNodeVisitor::setValidConnection(liquidmaya::ShaderConnectionMap& validConnectionMap)
	{
		CM_TRACE_FUNC("erSkylightNodeVisitor::setValidConnection(...)");

		//erSkylight
		validConnectionMap.begin(erSkyLightNode::getTypeName());
		validConnectionMap.append("env_shader");
		validConnectionMap.append("resolution");
		validConnectionMap.append("max_dist");
		validConnectionMap.end();
	}
	bool erSkylightNodeVisitor::visit(const char *node)
	{
		CM_TRACE_FUNC("erSkylightNodeVisitor::visit("<<node<<")");

		ER::OutputHelper o;
		o.beginRSL(erSkyLightNode::getTypeName(), node);

		o.addRSLVariable("shader",	ER_SPN("env_shader"),	"env_shader",	node);
		o.addRSLVariable("int",		ER_SPN("resolution"),	"resolution",	node);
		o.addRSLVariable("float",	ER_SPN("max_dist"),		"max_dist",		node);
		
		o.endRSL();

		return true;
	}
	//
	bool erSkylightNodeVisitor::onCreateInstance(const char *node)
	{
		//in batch mode, this function is called after the rendering, 
		//so it complains the cm::trace file is not open, so I omit this CM_TRACE_FUNC()
		//CM_TRACE_FUNC("erSkylightNodeVisitor::onCreateInstance("<<node<<")");
		
		return true;
	}
	//
	bool  erSkylightNodeVisitor::regist(MFnPlugin &plugin)
	{
		//CM_TRACE_FUNC("erSkylightNodeVisitor::regist()");

		MStatus status;

		status = plugin.registerNode( 
			elvishray::erSkyLightNode::getTypeName(), 
			elvishray::erSkyLightNode::getTypeId(), 
			elvishray::erSkyLightNode::creator, elvishray::erSkyLightNode::initialize, MPxNode::kDependNode,
			&elvishray::light_classification);
		IfMErrorMsgReturnIt( status, "Can't register "+elvishray::erSkyLightNode::getTypeName()+" node" );
		status.clear();
		status = MSwatchRenderRegister::registerSwatchRender( elvishray::erSkyLightNode::getTypeName()+"Swatch", liqNodeSwatch::creator );
		IfMErrorMsgReturnIt( status, "Can't register "+elvishray::erSkyLightNode::getTypeName()+"Swatch" );
		
		RNodeVisitorMgr::getInstancePtr()->regist(
			erSkyLightNode::getTypeName().asChar(),
			erSkylightNodeVisitor::getInstancePtr()
			);
		return true;
	}
	bool  erSkylightNodeVisitor::unregist(MFnPlugin &plugin)
	{
		//CM_TRACE_FUNC("erSkylightNodeVisitor::unregist()");

		MStatus status;

		RNodeVisitorMgr::getInstancePtr()->unregist(
			erSkyLightNode::getTypeName().asChar()
			);

		status = MSwatchRenderRegister::unregisterSwatchRender(elvishray::erSkyLightNode::getTypeName()+"Swatch");
		IfMErrorMsgReturnIt( status, "Can't deregister "+elvishray::erSkyLightNode::getTypeName()+"Swatch generator" );
		status = plugin.deregisterNode( elvishray::erSkyLightNode::getTypeId() );
		IfMErrorMsgReturnIt( status, "Can't deregister "+elvishray::erSkyLightNode::getTypeName()+" node" );

		return true;
	}

}//namespace