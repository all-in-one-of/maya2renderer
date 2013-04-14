#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#include "shaderOutputERCall.h"
#include <liqlog.h>
#include "../common/mayacheck.h"
#include "../shadergraph/convertShadingNetwork.h"
#include "../shadergraph/shadermgr.h"

namespace ERCall
{
// @node	mr shader node name
void Visitor::visit_mib_amb_occlusion(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_mib_amb_occlusion("<<node<<")");
	
	OutputHelper o;
	o.beginRSL("maya_mib_amb_occlusion", node);

	o.addRSLVariable("index",	"samples",		"samples",		node);
	o.addRSLVariable("color",	"bright",		"bright",		node);
	o.addRSLVariable("color",	"dark",			"dark",			node);
	o.addRSLVariable("scalar",	"spread",		"spread",		node);
	o.addRSLVariable("scalar",	"max_distance",	"max_distance",	node);
	o.addRSLVariable("bool",	"reflective",	"reflective",	node);
	//o.addRSLVariable("color",	"outValue",		"outValue",		node);

	o.endRSL();
}

}//namespace ERCall
#endif//_USE_ELVISHRAY_