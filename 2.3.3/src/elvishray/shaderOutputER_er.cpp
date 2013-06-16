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
	o.endRSL();
}

}//namespace ER