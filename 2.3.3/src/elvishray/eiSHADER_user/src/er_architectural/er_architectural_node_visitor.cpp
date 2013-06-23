
#include "er_architectural_node_visitor.h"
#include <common/mayacheck.h>

#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>

#include "../er_log.h"
#include "../er_output_mgr.h"
#include "../shaderOutputER.h"
#include "er_architectural_node.h"
#include "../er_rnode_visitor_mgr.h"
#include "../er_nodeId.h"

namespace elvishray
{
	ArchitecturalNodeVisitor* ArchitecturalNodeVisitor::m_instance = NULL;
	//
	ArchitecturalNodeVisitor* ArchitecturalNodeVisitor::getInstancePtr()
	{
		if( m_instance == NULL )
		{
			m_instance = new ArchitecturalNodeVisitor();
		}
		return m_instance;
	}
	//
	ArchitecturalNodeVisitor::ArchitecturalNodeVisitor()
	{

	}
	//
	ArchitecturalNodeVisitor::~ArchitecturalNodeVisitor()
	{

	}
	void ArchitecturalNodeVisitor::setValidConnection(liquidmaya::ShaderConnectionMap& validConnectionMap)
	{
		CM_TRACE_FUNC("ArchitecturalNodeVisitor::setValidConnection(...)");

		validConnectionMap.begin(ArchitecturalNode::getTypeName());
		validConnectionMap.append("surface_colorR");
		validConnectionMap.append("surface_colorG");
		validConnectionMap.append("surface_colorB");
		validConnectionMap.append("surface_color");
		validConnectionMap.append("diffuse_colorR");
		validConnectionMap.append("diffuse_colorG");
		validConnectionMap.append("diffuse_colorB");
		validConnectionMap.append("diffuse_color");
		validConnectionMap.append("diffuse_weight");
		validConnectionMap.append("specular_colorR");
		validConnectionMap.append("specular_colorG");
		validConnectionMap.append("specular_colorB");
		validConnectionMap.append("specular_color");
		validConnectionMap.append("specular_weight");
		validConnectionMap.append("roughness");
		validConnectionMap.append("specular_mode");
		validConnectionMap.append("glossiness");
		validConnectionMap.append("reflection_colorR");
		validConnectionMap.append("reflection_colorG");
		validConnectionMap.append("reflection_colorB");
		validConnectionMap.append("reflection_color");
		validConnectionMap.append("reflection_weight");
		validConnectionMap.append("refraction_colorR");
		validConnectionMap.append("refraction_colorG");
		validConnectionMap.append("refraction_colorB");
		validConnectionMap.append("refraction_color");
		validConnectionMap.append("refraction_weight");
		validConnectionMap.append("refraction_glossiness");
		validConnectionMap.append("refraction_thickness");
		validConnectionMap.append("translucency_colorR");
		validConnectionMap.append("translucency_colorG");
		validConnectionMap.append("translucency_colorB");
		validConnectionMap.append("translucency_color");
		validConnectionMap.append("translucency_weight");
		validConnectionMap.append("translucency_mode");
		validConnectionMap.append("sss_colorR");
		validConnectionMap.append("sss_colorG");
		validConnectionMap.append("sss_colorB");
		validConnectionMap.append("sss_color");
		validConnectionMap.append("sss_scale_colorR");
		validConnectionMap.append("sss_scale_colorG");
		validConnectionMap.append("sss_scale_colorB");
		validConnectionMap.append("sss_scale_color");
		validConnectionMap.append("sss_scale_weight");
		validConnectionMap.append("sss_phase");
		validConnectionMap.append("anisotropy");
		validConnectionMap.append("rotation");
		validConnectionMap.append("ior");
		validConnectionMap.append("fresnel_by_ior");
		validConnectionMap.append("fresnel_0_degree_refl");
		validConnectionMap.append("fresnel_90_degree_refl");
		validConnectionMap.append("fresnel_curve");
		validConnectionMap.append("is_metal");
		validConnectionMap.append("diffuse_samples");
		validConnectionMap.append("reflection_samples");
		validConnectionMap.append("refraction_samples");
		validConnectionMap.append("sss_samples");
		validConnectionMap.append("cutoff_threshold");
		validConnectionMap.append("bump_shader");
		validConnectionMap.append("bump_factor");
		validConnectionMap.append("displace_shader");
		validConnectionMap.append("displace_factor");
		validConnectionMap.append("liq_bump_shader_token");
		validConnectionMap.append("liq_displace_shader_token");
		validConnectionMap.end();
	}
	bool ArchitecturalNodeVisitor::visit(const char *node)
	{
		CM_TRACE_FUNC("ArchitecturalNodeVisitor::visit("<<node<<")");

		ER::OutputHelper o;
		o.beginRSL(ArchitecturalNode::getTypeName(), node);
	
		o.addRSLVariable("color",	"surface_color",	"surface_color",		node);
		o.addRSLVariable("color",	"diffuse_color",	"diffuse_color",		node);
		o.addRSLVariable("float",	"diffuse_weight",	"diffuse_weight",		node);
		o.addRSLVariable("color",	"specular_color",	"specular_color",		node);
		o.addRSLVariable("float",	"specular_weight",	"specular_weight",		node);
		o.addRSLVariable("float",	"roughness",	"roughness",		node);
		o.addRSLVariable("int",	"specular_mode",	"specular_mode",		node);
		o.addRSLVariable("float",	"glossiness",	"glossiness",		node);
		o.addRSLVariable("color",	"reflection_color",	"reflection_color",		node);
		o.addRSLVariable("float",	"reflection_weight",	"reflection_weight",		node);
		o.addRSLVariable("color",	"refraction_color",	"refraction_color",		node);
		o.addRSLVariable("float",	"refraction_weight",	"refraction_weight",		node);
		o.addRSLVariable("float",	"refraction_glossiness",	"refraction_glossiness",		node);
		o.addRSLVariable("float",	"refraction_thickness",	"refraction_thickness",		node);
		o.addRSLVariable("color",	"translucency_color",	"translucency_color",		node);
		o.addRSLVariable("float",	"translucency_weight",	"translucency_weight",		node);
		o.addRSLVariable("int",	"translucency_mode",	"translucency_mode",		node);
		o.addRSLVariable("color",	"sss_color",	"sss_color",		node);
		o.addRSLVariable("color",	"sss_scale_color",	"sss_scale_color",		node);
		o.addRSLVariable("float",	"sss_scale_weight",	"sss_scale_weight",		node);
		o.addRSLVariable("float",	"sss_phase",	"sss_phase",		node);
		o.addRSLVariable("float",	"anisotropy",	"anisotropy",		node);
		o.addRSLVariable("float",	"rotation",	"rotation",		node);
		o.addRSLVariable("float",	"ior",	"ior",		node);
		o.addRSLVariable("bool",	"fresnel_by_ior",	"fresnel_by_ior",		node);
		o.addRSLVariable("float",	"fresnel_0_degree_refl",	"fresnel_0_degree_refl",		node);
		o.addRSLVariable("float",	"fresnel_90_degree_refl",	"fresnel_90_degree_refl",		node);
		o.addRSLVariable("float",	"fresnel_curve",	"fresnel_curve",		node);
		o.addRSLVariable("bool",	"is_metal",	"is_metal",		node);
		o.addRSLVariable("int",	"diffuse_samples",	"diffuse_samples",		node);
		o.addRSLVariable("int",	"reflection_samples",	"reflection_samples",		node);
		o.addRSLVariable("int",	"refraction_samples",	"refraction_samples",		node);
		o.addRSLVariable("int",	"sss_samples",	"sss_samples",		node);
		o.addRSLVariable("float",	"cutoff_threshold",	"cutoff_threshold",		node);
		//o.addRSLVariable("shader",	"bump_shader",	"bump_shader",		node);
		o.addRSLVariable("float",	"bump_factor",	"bump_factor",		node);
		//o.addRSLVariable("shader",	"displace_shader",	"displace_shader",		node);
		o.addRSLVariable("float",	"displace_factor",	"displace_factor",		node);
		
		o.addRSLVariable("string",	"liq_bump_shader_token",	"liq_bump_shader_token",		node);
		o.addRSLVariable("string",	"liq_displace_shader_token","liq_displace_shader_token",	node);

		o.endRSL();

		return true;
	}
	//
	bool ArchitecturalNodeVisitor::onCreateInstance(const char *node)
	{
		//in batch mode, this function is called after the rendering, 
		//so it complains the cm::trace file is not open, so I omit this CM_TRACE_FUNC()
		//CM_TRACE_FUNC("ArchitecturalNodeVisitor::onCreateInstance("<<node<<")");
		
		return true;
	}
	//
	bool  ArchitecturalNodeVisitor::regist(MFnPlugin &plugin)
	{
		CM_TRACE_FUNC("ArchitecturalNodeVisitor::regist()");

		MStatus status;

		status = plugin.registerNode( 
			elvishray::ArchitecturalNode::getTypeName(), 
			elvishray::ArchitecturalNode::getTypeId(), 
			elvishray::ArchitecturalNode::creator, elvishray::ArchitecturalNode::initialize, MPxNode::kDependNode,
			&elvishray::surface_classification);
		LIQCHECKSTATUS( status, "Can't register "+elvishray::ArchitecturalNode::getTypeName()+" node" );
		status.clear();

		RNodeVisitorMgr::getInstancePtr()->regist(
			ArchitecturalNode::getTypeName().asChar(),
			ArchitecturalNodeVisitor::getInstancePtr()
			);
		return true;
	}
	bool  ArchitecturalNodeVisitor::unregist(MFnPlugin &plugin)
	{
		CM_TRACE_FUNC("ArchitecturalNodeVisitor::unregist()");

		MStatus status;

		RNodeVisitorMgr::getInstancePtr()->unregist(
			ArchitecturalNode::getTypeName().asChar()
			);

		status = plugin.deregisterNode( elvishray::ArchitecturalNode::getTypeId() );
		LIQCHECKSTATUS( status, "Can't deregister "+elvishray::ArchitecturalNode::getTypeName()+" node" );

		return true;
	}

}//namespace