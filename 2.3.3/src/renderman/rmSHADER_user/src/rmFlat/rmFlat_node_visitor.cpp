
#include "rmFlat_node_visitor.h"
#include <common/mayacheck.h>

#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>
#include <liqNodeSwatch.h>

#include "../rm_log.h"
//#include "../rm_output_mgr.h"
#include "../shaderOutputRSL.h"
#include "rmFlat_node.h"
#include "../rm_rnode_visitor_mgr.h"
#include "../rm_classification.h"
#include "../rm_export.h"

namespace renderman
{
	FlatNodeVisitor* FlatNodeVisitor::m_instance = NULL;
	//
	FlatNodeVisitor* FlatNodeVisitor::getInstancePtr()
	{
		if( m_instance == NULL )
		{
			m_instance = new FlatNodeVisitor();
		}
		return m_instance;
	}
	//
	FlatNodeVisitor::FlatNodeVisitor()
	{

	}
	//
	FlatNodeVisitor::~FlatNodeVisitor()
	{

	}
	void FlatNodeVisitor::setValidConnection(liquidmaya::ShaderConnectionMap& validConnectionMap)
	{
		CM_TRACE_FUNC("FlatNodeVisitor::setValidConnection(...)");

		validConnectionMap.begin(FlatNode::getTypeName());
		validConnectionMap.append("icolorR");
		validConnectionMap.append("icolorG");
		validConnectionMap.append("icolorB");
		validConnectionMap.append("icolor");
		validConnectionMap.append("iopacityR");
		validConnectionMap.append("iopacityG");
		validConnectionMap.append("iopacityB");
		validConnectionMap.append("iopacity");
		validConnectionMap.end();
	}
	bool FlatNodeVisitor::visit(const char *node)
	{
		CM_TRACE_FUNC("FlatNodeVisitor::visit("<<node<<")");

		RSL::OutputHelper o(getOutfstreamRef(), getNodePlugInfoRef());
		
		o.addIncludeUserShader(FlatNode::getTypeName());

		o.beginRSL(node);

		o.addRSLVariable("",	"color",	"icolor",	"icolor",		node);
		o.addRSLVariable("",	"color",	"iopacity",	"iopacity",		node);
		
		o.addToRSL(" color outColor;");
		o.addToRSL(" color outTransparency;");
		o.addToRSL(" rmFlat(					\n\t"
						//Inputs
						"icolor,				\n\t"
						"iopacity,				\n\t"
						//Outputs
						"outColor,				\n\t"
						"outTransparency		\n"
					"  );");

		o.addToRSL(" Oi = outTransparency;");
		o.addToRSL(" Ci = outColor * outTransparency;");
		o.endRSL();

		return true;
	}
	//
	bool FlatNodeVisitor::onCreateInstance(const char *node)
	{
		//in batch mode, this function is called after the rendering, 
		//so it complains the cm::trace file is not open, so I omit this CM_TRACE_FUNC()
		//CM_TRACE_FUNC("FlatNodeVisitor::onCreateInstance("<<node<<")");
		
		return true;
	}
	//
	bool  FlatNodeVisitor::regist(MFnPlugin &plugin)
	{
		//CM_TRACE_FUNC("FlatNodeVisitor::regist()");

		MStatus status;

		status = plugin.registerNode( 
			renderman::FlatNode::getTypeName(), 
			renderman::FlatNode::getTypeId(), 
			renderman::FlatNode::creator, renderman::FlatNode::initialize, MPxNode::kDependNode,
			&renderman::surface_classification);
		IfMErrorMsgReturnIt( status, "Can't register "+renderman::FlatNode::getTypeName()+" node" );
		status.clear();
		status = MSwatchRenderRegister::registerSwatchRender( renderman::FlatNode::getTypeName()+"Swatch", liqNodeSwatch::creator );
		IfMErrorMsgReturnIt( status, "Can't register "+renderman::FlatNode::getTypeName()+"Swatch" );

		RNodeVisitorMgr::getInstancePtr()->regist(
			FlatNode::getTypeName().asChar(),
			FlatNodeVisitor::getInstancePtr()
			);
		return true;
	}
	bool  FlatNodeVisitor::unregist(MFnPlugin &plugin)
	{
		//CM_TRACE_FUNC("FlatNodeVisitor::unregist()");

		MStatus status;

		RNodeVisitorMgr::getInstancePtr()->unregist(
			FlatNode::getTypeName().asChar()
			);

		status = MSwatchRenderRegister::unregisterSwatchRender(renderman::FlatNode::getTypeName()+"Swatch");
		IfMErrorMsgReturnIt( status, "Can't deregister "+renderman::FlatNode::getTypeName()+"Swatch generator" );
		status = plugin.deregisterNode( renderman::FlatNode::getTypeId() );
		IfMErrorMsgReturnIt( status, "Can't deregister "+renderman::FlatNode::getTypeName()+" node" );

		return true;
	}

}//namespace renderman