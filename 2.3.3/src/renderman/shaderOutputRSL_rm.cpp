#include "shaderOutputRSL.h"
#include "../common/mayacheck.h"
#include "ri_interface.h"
#include "rm_log.h"

namespace RSL
{
	// @node	mr shader node name
	bool Visitor::visit_render_node_in_subrenderer(const char* shaderNodeName, const char* nodetype)
	{
		CM_TRACE_FUNC("Visitor::visit_render_node_in_subrenderer("<<shaderNodeName<<","<<nodetype<<")");

		return false;
	}

}//namespace RSL