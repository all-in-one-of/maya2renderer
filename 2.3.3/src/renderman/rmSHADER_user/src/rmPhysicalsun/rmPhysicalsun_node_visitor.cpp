
#include "rmPhysicalsun_node_visitor.h"
#include <common/mayacheck.h>
#include <liqNodeSwatch.h>

#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>

#include <liqNodeSwatch.h>
#include "../rm_log.h"
//#include "../rm_output_mgr.h"
#include "../shaderOutputRSL.h"
#include "rmPhysicalsun_node.h"
#include "../rm_rnode_visitor_mgr.h"
#include "../rm_classification.h"
#include "../rm_export.h"

namespace renderman
{
	PhysicalsunNodeVisitor* PhysicalsunNodeVisitor::m_instance = NULL;
	//
	PhysicalsunNodeVisitor* PhysicalsunNodeVisitor::getInstancePtr()
	{
		if( m_instance == NULL )
		{
			m_instance = new PhysicalsunNodeVisitor();
		}
		return m_instance;
	}
	//
	PhysicalsunNodeVisitor::PhysicalsunNodeVisitor()
	{

	}
	//
	PhysicalsunNodeVisitor::~PhysicalsunNodeVisitor()
	{

	}
	void PhysicalsunNodeVisitor::setValidConnection(liquidmaya::ShaderConnectionMap& validConnectionMap)
	{
		CM_TRACE_FUNC("PhysicalsunNodeVisitor::setValidConnection(...)");

		validConnectionMap.begin(PhysicalsunNode::getTypeName());
		validConnectionMap.append("multiplier");
		validConnectionMap.append("rgb_unit_conversionR");
		validConnectionMap.append("rgb_unit_conversionG");
		validConnectionMap.append("rgb_unit_conversionB");
		validConnectionMap.append("rgb_unit_conversion");
		validConnectionMap.append("haze");
		validConnectionMap.append("redblueshift");
		validConnectionMap.append("saturation");
		validConnectionMap.append("horizon_height");
		validConnectionMap.append("shadow_softness");
		validConnectionMap.append("shadowmap");
		validConnectionMap.append("shadow_samples");
		validConnectionMap.append("y_is_up");
		validConnectionMap.end();
	}
	bool PhysicalsunNodeVisitor::visit(const char *node)
	{
		CM_TRACE_FUNC("PhysicalsunNodeVisitor::visit("<<node<<")");

		RSL::OutputHelper o(getOutfstreamRef(), getNodePlugInfoRef());
		
		o.addIncludeUserShader(PhysicalsunNode::getTypeName());

		o.beginRSL(node);

		o.addRSLVariable("",	"float",	"i_multiplier",		"multiplier",		node);
		o.addRSLVariable("",	"color",	"i_rgb_unit_conversion",	"rgb_unit_conversion",		node);
		o.addRSLVariable("",	"float",	"i_haze",			"haze",				node);
		o.addRSLVariable("",	"float",	"i_redblueshift",	"redblueshift",		node);
		o.addRSLVariable("",	"float",	"i_saturation",		"saturation",		node);
		o.addRSLVariable("",	"float",	"i_horizon_height",	"horizon_height",	node);
		o.addRSLVariable("",	"float",	"i_shadow_softness","shadow_softness",	node);
		o.addRSLVariable("",	"string",	"i_shadowmap",		"shadowmap",		node);
		o.addRSLVariable("",	"float",	"i_shadow_samples",	"shadow_samples",	node);
		o.addRSLVariable("",	"bool",		"i_y_is_up",		"y_is_up",			node);

		o.addToRSL(" rmPhysicalsun_impl(		\n\t"
						//Inputs
						"i_multiplier,			\n\t"
						"i_rgb_unit_conversion,	\n\t"
						"i_haze,				\n\t"
						"i_redblueshift,		\n\t"
						"i_saturation,			\n\t"
						"i_horizon_height,		\n\t"
						"i_shadow_softness,		\n\t"
						"i_shadowmap,			\n\t"
						"i_shadow_samples,		\n\t"
						"i_y_is_up,				\n"
					"  );");
		o.endRSL();

		return true;
	}
	//
	bool PhysicalsunNodeVisitor::onCreateInstance(const char *node)
	{
		//in batch mode, this function is called after the rendering, 
		//so it complains the cm::trace file is not open, so I omit this CM_TRACE_FUNC()
		//CM_TRACE_FUNC("PhysicalsunNodeVisitor::onCreateInstance("<<node<<")");
		
		return true;
	}
	//
	bool  PhysicalsunNodeVisitor::regist(MFnPlugin &plugin)
	{
		//CM_TRACE_FUNC("PhysicalsunNodeVisitor::regist()");

		MStatus status;

		status = plugin.registerNode( 
			renderman::PhysicalsunNode::getTypeName(), 
			renderman::PhysicalsunNode::getTypeId(), 
			renderman::PhysicalsunNode::creator, renderman::PhysicalsunNode::initialize, MPxNode::kDependNode,
			&renderman::light_classification);
		IfMErrorMsgReturnIt( status, "Can't register "+renderman::PhysicalsunNode::getTypeName()+" node" );
		status.clear();
		status = MSwatchRenderRegister::registerSwatchRender( renderman::PhysicalsunNode::getTypeName()+"Swatch", liqNodeSwatch::creator );
		IfMErrorMsgReturnIt( status, "Can't register "+renderman::PhysicalsunNode::getTypeName()+"Swatch" );

		RNodeVisitorMgr::getInstancePtr()->regist(
			PhysicalsunNode::getTypeName().asChar(),
			PhysicalsunNodeVisitor::getInstancePtr()
			);
		return true;
	}
	bool  PhysicalsunNodeVisitor::unregist(MFnPlugin &plugin)
	{
		//CM_TRACE_FUNC("PhysicalsunNodeVisitor::unregist()");

		MStatus status;

		RNodeVisitorMgr::getInstancePtr()->unregist(
			PhysicalsunNode::getTypeName().asChar()
			);

		status = MSwatchRenderRegister::unregisterSwatchRender(renderman::PhysicalsunNode::getTypeName()+"Swatch");
		IfMErrorMsgReturnIt( status, "Can't deregister "+renderman::PhysicalsunNode::getTypeName()+"Swatch generator" );
		status = plugin.deregisterNode( renderman::PhysicalsunNode::getTypeId() );
		IfMErrorMsgReturnIt( status, "Can't deregister "+renderman::PhysicalsunNode::getTypeName()+" node" );

		return true;
	}

}//namespace renderman