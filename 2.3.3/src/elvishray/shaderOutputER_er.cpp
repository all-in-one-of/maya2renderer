#include "shaderOutputER.h"
//#include <liqlog.h>
#include <common/mayacheck.h>
//#include "../shadergraph/convertShadingNetwork.h"
//#include "../shadergraph/shadermgr.h"
#include "er_log.h"
#include "er_output_mgr.h"
#include "er_rnode_visitor_mgr.h"
#include "er_helper.h"

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
	else if( strcmp("er_flatcolor", nodetype) == 0 ){
		visit_er_flatcolor(shaderNodeName);
	}
	else if( elvishray::RNodeVisitorMgr::getInstancePtr()->visit(shaderNodeName) ){
		liquidMessage2(messageInfo, "RNodeVisitorMgr::visit(%s)", shaderNodeName );
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
	o.addRSLVariable("vector",	ER_SPN("sun_dir"),			"sun_dir",				node);
	o.addRSLVariable("float",	ER_SPN("sun_disk_size"),	"sun_disk_size",		node);
	o.addRSLVariable("float",	ER_SPN("sun_disk_intensity"),"sun_disk_intensity",	node);
	o.addRSLVariable("float",	ER_SPN("sun_glow_size"),	"sun_glow_size",		node);
	o.addRSLVariable("float",	ER_SPN("sun_glow_intensity"),"sun_glow_intensity",	node);
	o.addRSLVariable("float",	ER_SPN("sun_glow_falloff"),	"sun_glow_falloff",		node);
	o.addRSLVariable("color",	ER_SPN("ground_color"),		"ground_color",			node);
	o.addRSLVariable("float",	ER_SPN("ground_blur"),		"ground_blur",			node);
	o.addRSLVariable("int",		ER_SPN("type"),				"type",					node);
	o.addRSLVariable("float",	ER_SPN("haze"),				"haze",					node);
	o.addRSLVariable("color",	ER_SPN("zenith_color"),		"zenith_color",			node);
	o.addRSLVariable("float",	ER_SPN("a"),				"a",					node);
	o.addRSLVariable("float",	ER_SPN("b"),				"b",					node);
	o.addRSLVariable("float",	ER_SPN("c"),				"c",					node);
	o.addRSLVariable("float",	ER_SPN("d"),				"d",					node);
	o.addRSLVariable("float",	ER_SPN("e"),				"e",					node);
	o.addRSLVariable("float",	ER_SPN("intensity","intensity",					node);
#else
	MStringArray rmanParams, rmanTypes;
	IfMErrorWarn(MGlobal::executeCommand( ("getAttr \""+MString(node)+".rmanParams\""), rmanParams ));
	IfMErrorWarn(MGlobal::executeCommand( ("getAttr \""+MString(node)+".rmanTypes\""),	rmanTypes ));
	for(std::size_t i = 0; i < rmanParams.length(); ++i)
	{
		o.addRSLVariable(rmanTypes[i],	ER_SPN(rmanParams[i]),  rmanParams[i],	node);
	}
#endif
	o.endRSL();
}
void Visitor::visit_er_skylight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_er_skylight("<<node<<")");

	OutputHelper o;
	o.beginRSL("liq_skylight", node);

	o.addRSLVariable("shader",	ER_SPN("env_shader"),	"env_shader",	node);
	o.addRSLVariable("int",		ER_SPN("resolution"),	"resolution",	node);
	o.addRSLVariable("float",	ER_SPN("max_dist"),		"max_dist",		node);

	o.endRSL();
}
void Visitor::visit_er_flatcolor(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_er_flatcolor("<<node<<")");

	OutputHelper o;
	o.beginRSL("liq_flatcolor", node);

	o.addRSLVariable("color",	ER_SPN("color"),			"color",			node);
	o.addRSLVariable("color",	ER_SPN("transparency"),	"transparency",	node);

	o.endRSL();
}
}//namespace ER