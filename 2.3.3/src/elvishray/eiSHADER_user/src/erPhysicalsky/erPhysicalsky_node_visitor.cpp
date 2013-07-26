
#include "erPhysicalsky_node_visitor.h"
#include <common/mayacheck.h>
#include <liqNodeSwatch.h>

#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>

#include "../er_log.h"
#include "../er_output_mgr.h"
#include "../shaderOutputER.h"
#include "erPhysicalsky_node.h"
#include "../er_rnode_visitor_mgr.h"
#include "../er_classification.h"
#include "../er_helper.h"

namespace elvishray
{
	erPhysicalskyNodeVisitor* erPhysicalskyNodeVisitor::m_instance = NULL;
	//
	erPhysicalskyNodeVisitor* erPhysicalskyNodeVisitor::getInstancePtr()
	{
		if( m_instance == NULL )
		{
			m_instance = new erPhysicalskyNodeVisitor();
		}
		return m_instance;
	}
	//
	erPhysicalskyNodeVisitor::erPhysicalskyNodeVisitor()
	{

	}
	//
	erPhysicalskyNodeVisitor::~erPhysicalskyNodeVisitor()
	{

	}
	void erPhysicalskyNodeVisitor::setValidConnection(liquidmaya::ShaderConnectionMap& validConnectionMap)
	{
		CM_TRACE_FUNC("erPhysicalskyNodeVisitor::setValidConnection(...)");

		//erSkylight
		validConnectionMap.begin(erPhysicalskyNode::getTypeName());
		validConnectionMap.append("sun_dirX");
		validConnectionMap.append("sun_dirY");
		validConnectionMap.append("sun_dirZ");
		validConnectionMap.append("sun_dir");
		validConnectionMap.append("sun_disk_size");
		validConnectionMap.append("sun_disk_intensity");
		validConnectionMap.append("sun_glow_size");
		validConnectionMap.append("sun_glow_intensity");
		validConnectionMap.append("sun_glow_falloff");
		validConnectionMap.append("ground_colorR");
		validConnectionMap.append("ground_colorG");
		validConnectionMap.append("ground_colorB");
		validConnectionMap.append("ground_color");
		validConnectionMap.append("ground_blur");
		validConnectionMap.append("type");
		validConnectionMap.append("haze");
		validConnectionMap.append("zenith_colorR");
		validConnectionMap.append("zenith_colorG");
		validConnectionMap.append("zenith_colorB");
		validConnectionMap.append("zenith_color");
		validConnectionMap.append("a");
		validConnectionMap.append("b");
		validConnectionMap.append("c");
		validConnectionMap.append("d");
		validConnectionMap.append("e");
		validConnectionMap.append("intensity");
		validConnectionMap.end();
	}
	bool erPhysicalskyNodeVisitor::visit(const char *node)
	{
		CM_TRACE_FUNC("erPhysicalskyNodeVisitor::visit("<<node<<")");

		ER::OutputHelper o;
		o.beginRSL(erPhysicalskyNode::getTypeName(), node);

		o.addRSLVariable("vector",	ER_SPN("sun_dir"),				"sun_dir",		node);
		o.addRSLVariable("float",	ER_SPN("sun_disk_size"),		"sun_disk_size",node);
		o.addRSLVariable("float",	ER_SPN("sun_disk_intensity"),	"sun_disk_intensity",	node);
		o.addRSLVariable("float",	ER_SPN("sun_glow_size"),		"sun_glow_size",node);
		o.addRSLVariable("float",	ER_SPN("sun_glow_intensity"),	"sun_glow_intensity",	node);
		o.addRSLVariable("float",	ER_SPN("sun_glow_falloff"),		"sun_glow_falloff",		node);
		o.addRSLVariable("color",	ER_SPN("ground_color"),			"ground_color",	node);
		o.addRSLVariable("float",	ER_SPN("ground_blur"),			"ground_blur",	node);
		o.addRSLVariable("int",		ER_SPN("type"),					"type",			node);
		o.addRSLVariable("float",	ER_SPN("haze"),					"haze",			node);
		o.addRSLVariable("color",	ER_SPN("zenith_color"),			"zenith_color",	node);
		o.addRSLVariable("float",	ER_SPN("a"),					"a",			node);
		o.addRSLVariable("float",	ER_SPN("b"),					"b",			node);
		o.addRSLVariable("float",	ER_SPN("c"),					"c",			node);
		o.addRSLVariable("float",	ER_SPN("d"),					"d",			node);
		o.addRSLVariable("float",	ER_SPN("e"),					"e",			node);
		o.addRSLVariable("float",	ER_SPN("intensity"),			"intensity",	node);
		
		o.endRSL();

		return true;
	}
	//
	bool erPhysicalskyNodeVisitor::onCreateInstance(const char *node)
	{
		//in batch mode, this function is called after the rendering, 
		//so it complains the cm::trace file is not open, so I omit this CM_TRACE_FUNC()
		//CM_TRACE_FUNC("erPhysicalskyNodeVisitor::onCreateInstance("<<node<<")");
		
		return true;
	}
	//
	bool  erPhysicalskyNodeVisitor::regist(MFnPlugin &plugin)
	{
		//CM_TRACE_FUNC("erPhysicalskyNodeVisitor::regist()");

		MStatus status;

		status = plugin.registerNode( 
			elvishray::erPhysicalskyNode::getTypeName(), 
			elvishray::erPhysicalskyNode::getTypeId(), 
			elvishray::erPhysicalskyNode::creator, elvishray::erPhysicalskyNode::initialize, MPxNode::kDependNode,
			&elvishray::env_classification);
		IfMErrorMsgReturnIt( status, "Can't register "+elvishray::erPhysicalskyNode::getTypeName()+" node" );
		status.clear();
		status = MSwatchRenderRegister::registerSwatchRender( elvishray::erPhysicalskyNode::getTypeName()+"Swatch", liqNodeSwatch::creator );
		IfMErrorMsgReturnIt( status, "Can't register "+elvishray::erPhysicalskyNode::getTypeName()+"Swatch" );
		
		RNodeVisitorMgr::getInstancePtr()->regist(
			erPhysicalskyNode::getTypeName().asChar(),
			erPhysicalskyNodeVisitor::getInstancePtr()
			);
		return true;
	}
	bool  erPhysicalskyNodeVisitor::unregist(MFnPlugin &plugin)
	{
		//CM_TRACE_FUNC("erPhysicalskyNodeVisitor::unregist()");

		MStatus status;

		RNodeVisitorMgr::getInstancePtr()->unregist(
			erPhysicalskyNode::getTypeName().asChar()
			);

		status = MSwatchRenderRegister::unregisterSwatchRender(elvishray::erPhysicalskyNode::getTypeName()+"Swatch");
		IfMErrorMsgReturnIt( status, "Can't deregister "+elvishray::erPhysicalskyNode::getTypeName()+"Swatch generator" );
		status = plugin.deregisterNode( elvishray::erPhysicalskyNode::getTypeId() );
		IfMErrorMsgReturnIt( status, "Can't deregister "+elvishray::erPhysicalskyNode::getTypeName()+" node" );

		return true;
	}

}//namespace