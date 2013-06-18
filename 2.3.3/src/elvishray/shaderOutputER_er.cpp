#include "shaderOutputER.h"
//#include <liqlog.h>
#include <common/mayacheck.h>
//#include "../shadergraph/convertShadingNetwork.h"
//#include "../shadergraph/shadermgr.h"
#include "er_log.h"
#include "er_output_mgr.h"

namespace ER
{

// @node	mr shader node name
bool Visitor::visit_render_node_in_subrenderer(const char* shaderNodeName, const char* nodetype)
{
	CM_TRACE_FUNC("Visitor::visit_render_node_in_subrenderer("<<shaderNodeName<<","<<nodetype<<")");
	
	bool ret = true;

	if( strcmp("er_physicalsky", nodetype) == 0 ){
		visit_er_physicalsky(shaderNodeName);
	}
	else if( strcmp("er_skylight", nodetype) == 0 ){
		visit_er_skylight(shaderNodeName);
	}
	else{
		liquidMessage2(messageError, ("Visitor::visit_render_node_in_subrenderer(), shader type ["+std::string(nodetype)+"] is not supported.").c_str() );
		assert( 0 && "shader type is not support.");
		ret = false;
	}
	return ret;
}
// @node	mr shader node name
void Visitor::visit_er_physicalsky(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_er_physicalsky("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_physicalsky", node);
#if 0
	o.addRSLVariable("vector",	"sun_dir",			"sun_dir",				node);
	o.addRSLVariable("float",	"sun_disk_size",	"sun_disk_size",		node);
	o.addRSLVariable("float",	"sun_disk_intensity","sun_disk_intensity",	node);
	o.addRSLVariable("float",	"sun_glow_size",	"sun_glow_size",		node);
	o.addRSLVariable("float",	"sun_glow_intensity","sun_glow_intensity",	node);
	o.addRSLVariable("float",	"sun_glow_falloff",	"sun_glow_falloff",		node);
	o.addRSLVariable("color",	"ground_color",		"ground_color",			node);
	o.addRSLVariable("float",	"ground_blur",		"ground_blur",			node);
	o.addRSLVariable("int",		"type",				"type",					node);
	o.addRSLVariable("float",	"haze",				"haze",					node);
	o.addRSLVariable("color",	"zenith_color",		"zenith_color",			node);
	o.addRSLVariable("float",	"a",				"a",					node);
	o.addRSLVariable("float",	"b",				"b",					node);
	o.addRSLVariable("float",	"c",				"c",					node);
	o.addRSLVariable("float",	"d",				"d",					node);
	o.addRSLVariable("float",	"e",				"e",					node);
	o.addRSLVariable("float",	"intensity","intensity",					node);
#else
	MStringArray rmanParams, rmanTypes;
	IfMErrorWarn(MGlobal::executeCommand( ("getAttr \""+MString(node)+".rmanParams\""), rmanParams ));
	IfMErrorWarn(MGlobal::executeCommand( ("getAttr \""+MString(node)+".rmanTypes\""),	rmanTypes ));
	for(std::size_t i = 0; i < rmanParams.length(); ++i)
	{
		o.addRSLVariable(rmanTypes[i],	rmanParams[i],  rmanParams[i],	node);
	}
#endif
	o.endRSL();
}
void Visitor::visit_er_skylight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_er_skylight("<<node<<")");

	OutputHelper o;
	o.beginRSL("liq_skylight", node);

	o.addRSLVariable("shader",	"env_shader",	"env_shader",	node);
	o.addRSLVariable("int",		"resolution",	"resolution",	node);
	o.addRSLVariable("float",	"max_dist",		"max_dist",		node);

	o.endRSL();
}
}//namespace ER