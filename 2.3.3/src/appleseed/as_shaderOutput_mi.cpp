#include "as_shaderOutput.h"
//#include <liqlog.h>
#include "../common/mayacheck.h"
//#include "../shadergraph/convertShadingNetwork.h"
//#include "../shadergraph/shadermgr.h"
#include <trace/trace.hpp>

namespace appleseed
{

// @node	mr shader node name
void Visitor::visit_mib_amb_occlusion(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_mib_amb_occlusion("<<node<<")");

	OutputHelper o(file);
	o.begin(node);

	o.addTo("ei_shader_param_string(\"desc\", \"maya_mib_amb_occlusion\");");
	o.addVariable("index",	"samples",		"samples",		node);
	o.addVariable("color",	"bright",		"bright",		node);
	o.addVariable("color",	"dark",			"dark",			node);
	o.addVariable("float",	"spread",		"spread",		node);
	o.addVariable("float",	"max_distance",	"max_distance",	node);
	o.addVariable("bool",	"reflective",	"reflective",	node);
	//o.addVariable("color",	"outValue",		"outValue",		node);

	o.end();
}

}//namespace appleseed