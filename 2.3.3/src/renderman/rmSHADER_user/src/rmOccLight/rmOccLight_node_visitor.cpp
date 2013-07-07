
#include "rmOcclight_node_visitor.h"
#include <common/mayacheck.h>

#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>

#include "../rm_log.h"
//#include "../rm_output_mgr.h"
#include "../shaderOutputRSL.h"
#include "rmOccLight_node.h"
#include "../rm_rnode_visitor_mgr.h"
#include "../rm_classification.h"
#include "../rm_export.h"

namespace renderman
{
	OccLightNodeVisitor* OccLightNodeVisitor::m_instance = NULL;
	//
	OccLightNodeVisitor* OccLightNodeVisitor::getInstancePtr()
	{
		if( m_instance == NULL )
		{
			m_instance = new OccLightNodeVisitor();
		}
		return m_instance;
	}
	//
	OccLightNodeVisitor::OccLightNodeVisitor()
	{

	}
	//
	OccLightNodeVisitor::~OccLightNodeVisitor()
	{

	}
	void OccLightNodeVisitor::setValidConnection(liquidmaya::ShaderConnectionMap& validConnectionMap)
	{
		CM_TRACE_FUNC("OccLightNodeVisitor::setValidConnection(...)");

		validConnectionMap.begin(OccLightNode::getTypeName());
		validConnectionMap.append("Kocc");
		validConnectionMap.append("samples");
		validConnectionMap.append("maxdist");
		validConnectionMap.append("distribution");
		validConnectionMap.append("__nonspecular");
		validConnectionMap.end();
	}
	bool OccLightNodeVisitor::visit(const char *node)
	{
		CM_TRACE_FUNC("OccLightNodeVisitor::visit("<<node<<")");

		RSL::OutputHelper o(getOutfstreamRef(), getNodePlugInfoRef());
		
		o.addIncludeUserShader(OccLightNode::getTypeName());

		o.beginRSL(node);

		o.addRSLVariable("",	"float",	"Kocc",			"Kocc",			node);
		o.addRSLVariable("",	"float",	"samples",		"samples",		node);
		o.addRSLVariable("",	"float",	"maxdist",		"maxdist",		node);
		o.addRSLVariable("",	"string",	"distribution",	"distribution",	node);
		o.addRSLVariable("",	"float",	"__nonspecular","__nonspecular",node);

		o.addToRSL(" color outColor;");
		o.addToRSL(" color outTransparency;");
		o.addToRSL(" rmOccLight_impl(		\n\t"
						//Inputs
						"Kocc,				\n\t"
						"samples,			\n\t"
						//Outputs
						"maxdist,			\n\t"
						"distribution,		\n\t"
						"__nonspecular		\n"
					"  );");
		o.endRSL();

		return true;
	}
	//
	bool OccLightNodeVisitor::onCreateInstance(const char *node)
	{
		//in batch mode, this function is called after the rendering, 
		//so it complains the cm::trace file is not open, so I omit this CM_TRACE_FUNC()
		//CM_TRACE_FUNC("OccLightNodeVisitor::onCreateInstance("<<node<<")");
		
		return true;
	}
	//
	bool  OccLightNodeVisitor::regist(MFnPlugin &plugin)
	{
		//CM_TRACE_FUNC("OccLightNodeVisitor::regist()");

		MStatus status;

		status = plugin.registerNode( 
			renderman::OccLightNode::getTypeName(), 
			renderman::OccLightNode::getTypeId(), 
			renderman::OccLightNode::creator, renderman::OccLightNode::initialize, MPxNode::kDependNode,
			&renderman::light_classification);
		LIQCHECKSTATUS( status, "Can't register "+renderman::OccLightNode::getTypeName()+" node" );
		status.clear();

		RNodeVisitorMgr::getInstancePtr()->regist(
			OccLightNode::getTypeName().asChar(),
			OccLightNodeVisitor::getInstancePtr()
			);
		return true;
	}
	bool  OccLightNodeVisitor::unregist(MFnPlugin &plugin)
	{
		//CM_TRACE_FUNC("OccLightNodeVisitor::unregist()");

		MStatus status;

		RNodeVisitorMgr::getInstancePtr()->unregist(
			OccLightNode::getTypeName().asChar()
			);

		status = plugin.deregisterNode( renderman::OccLightNode::getTypeId() );
		LIQCHECKSTATUS( status, "Can't deregister "+renderman::OccLightNode::getTypeName()+" node" );

		return true;
	}

}//namespace renderman