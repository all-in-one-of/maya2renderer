#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#include "as_visitorForEnv.h"
//#include <liqlog.h>
#include "../common/mayacheck.h"
//#include "../shadergraph/convertShadingNetwork.h"
//#include "../shadergraph/shadermgr.h"
#include <trace/trace.hpp>

namespace appleseed{
namespace env{
// @node	mr shader node name
void Visitor::visit_mib_amb_occlusion(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_mib_amb_occlusion("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
}//namespace env
}//namespace appleseed
#endif//_USE_APPLESEED_