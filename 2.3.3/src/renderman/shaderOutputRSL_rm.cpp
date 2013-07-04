#include "shaderOutputRSL.h"
#include "../common/mayacheck.h"
#include "ri_interface.h"
#include "rm_log.h"
#include "rm_rnode_visitor_mgr.h"

namespace RSL
{
	// @node	mr shader node name
	bool Visitor::visit_render_node_in_subrenderer(const char* shaderNodeName, const char* nodetype)
	{
		CM_TRACE_FUNC("Visitor::visit_render_node_in_subrenderer("<<shaderNodeName<<","<<nodetype<<")");
	
		bool ret = true;

		if( renderman::RNodeVisitorMgr::getInstancePtr()->visit(shaderNodeName) ){
			liquidMessage2(messageInfo, "RNodeVisitorMgr::visit(%s)", shaderNodeName );
		}
		else{
			liquidMessage2(messageError, ("Visitor::visit_render_node_in_subrenderer(), shader type ["+std::string(nodetype)+"] is not supported.").c_str() );
			assert( 0 && "shader type is not support.");
			ret = false;
		}

		return ret;
	}

}//namespace RSL