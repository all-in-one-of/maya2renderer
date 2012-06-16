#include <liqConfig.h>
#ifdef _USE_NEWRENDERER_

#include "nr_shaderOutputCall.h"
#include <liqlog.h>
#include "../common/mayacheck.h"
#include "../shadergraph/convertShadingNetwork.h"
#include "../shadergraph/shadermgr.h"

namespace newrenderer{
namespace call{
// @node	mr shader node name
void Visitor::visit_mib_amb_occlusion(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_mib_amb_occlusion("<<node<<")");
	
	OutputHelper o;
	o.begin(node);

	o.addVariable("index",	"samples",		"samples",		node);
	o.addVariable("color",	"bright",		"bright",		node);
	o.addVariable("color",	"dark",			"dark",			node);
	o.addVariable("scalar",	"spread",		"spread",		node);
	o.addVariable("scalar",	"max_distance",	"max_distance",	node);
	o.addVariable("bool",	"reflective",	"reflective",	node);
	//o.addVariable("color",	"outValue",		"outValue",		node);

	o.end();
}
}//namespace call
}//namespace newrenderer
#endif//_USE_NEWRENDERER_