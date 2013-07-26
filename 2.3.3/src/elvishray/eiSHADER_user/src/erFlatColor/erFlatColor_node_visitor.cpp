
#include "erFlatColor_node_visitor.h"
#include <common/mayacheck.h>
#include <liqNodeSwatch.h>

#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>

#include "../er_log.h"
#include "../er_output_mgr.h"
#include "../shaderOutputER.h"
#include "erFlatColor_node.h"
#include "../er_rnode_visitor_mgr.h"
#include "../er_classification.h"
#include "../er_helper.h"

namespace elvishray
{
	FlatColorNodeVisitor* FlatColorNodeVisitor::m_instance = NULL;
	//
	FlatColorNodeVisitor* FlatColorNodeVisitor::getInstancePtr()
	{
		if( m_instance == NULL )
		{
			m_instance = new FlatColorNodeVisitor();
		}
		return m_instance;
	}
	//
	FlatColorNodeVisitor::FlatColorNodeVisitor()
	{

	}
	//
	FlatColorNodeVisitor::~FlatColorNodeVisitor()
	{

	}
	void FlatColorNodeVisitor::setValidConnection(liquidmaya::ShaderConnectionMap& validConnectionMap)
	{
		CM_TRACE_FUNC("FlatColorNodeVisitor::setValidConnection(...)");

		validConnectionMap.begin(FlatColorNode::getTypeName());
		validConnectionMap.append("colorR");
		validConnectionMap.append("colorG");
		validConnectionMap.append("colorB");
		validConnectionMap.append("color");
		validConnectionMap.append("transparencyR");
		validConnectionMap.append("transparencyG");
		validConnectionMap.append("transparencyB");	
		validConnectionMap.append("transparency");
		validConnectionMap.end();
	}
	bool FlatColorNodeVisitor::visit(const char *node)
	{
		CM_TRACE_FUNC("FlatColorNodeVisitor::visit("<<node<<")");

		ER::OutputHelper o;
		o.beginRSL(FlatColorNode::getTypeName(), node);

		o.addRSLVariable("color",	ER_SPN("color"),	"color",		node);
		o.addRSLVariable("color",	ER_SPN("transparency"),	"transparency",		node);
		
		o.endRSL();

		return true;
	}
	//
	bool FlatColorNodeVisitor::onCreateInstance(const char *node)
	{
		//in batch mode, this function is called after the rendering, 
		//so it complains the cm::trace file is not open, so I omit this CM_TRACE_FUNC()
		//CM_TRACE_FUNC("FlatColorNodeVisitor::onCreateInstance("<<node<<")");
		
		return true;
	}
	//
	bool  FlatColorNodeVisitor::regist(MFnPlugin &plugin)
	{
		//CM_TRACE_FUNC("FlatColorNodeVisitor::regist()");

		MStatus status;

		status = plugin.registerNode( 
			elvishray::FlatColorNode::getTypeName(), 
			elvishray::FlatColorNode::getTypeId(), 
			elvishray::FlatColorNode::creator, elvishray::FlatColorNode::initialize, MPxNode::kDependNode,
			&elvishray::surface_classification);
		IfMErrorMsgReturnIt( status, "Can't register "+elvishray::FlatColorNode::getTypeName()+" node" );
		status.clear();
		status = MSwatchRenderRegister::registerSwatchRender( elvishray::FlatColorNode::getTypeName()+"Swatch", liqNodeSwatch::creator );
		IfMErrorMsgReturnIt( status, "Can't register "+elvishray::FlatColorNode::getTypeName()+"Swatch" );
		
		RNodeVisitorMgr::getInstancePtr()->regist(
			FlatColorNode::getTypeName().asChar(),
			FlatColorNodeVisitor::getInstancePtr()
			);
		return true;
	}
	bool  FlatColorNodeVisitor::unregist(MFnPlugin &plugin)
	{
		//CM_TRACE_FUNC("FlatColorNodeVisitor::unregist()");

		MStatus status;

		RNodeVisitorMgr::getInstancePtr()->unregist(
			FlatColorNode::getTypeName().asChar()
			);

		status = MSwatchRenderRegister::unregisterSwatchRender(elvishray::FlatColorNode::getTypeName()+"Swatch");
		IfMErrorMsgReturnIt( status, "Can't deregister "+elvishray::FlatColorNode::getTypeName()+"Swatch generator" );
		status = plugin.deregisterNode( elvishray::FlatColorNode::getTypeId() );
		IfMErrorMsgReturnIt( status, "Can't deregister "+elvishray::FlatColorNode::getTypeName()+" node" );

		return true;
	}

}//namespace