#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#include "as_shaderOutputCall.h"
#include <liqlog.h>
#include "../common/mayacheck.h"
#include "../shadergraph/convertShadingNetwork.h"
#include "../shadergraph/shadermgr.h"
#include "prerequest_as.h"
#include "as_renderer.h"
#include "../renderermgr.h"


namespace appleseed{
namespace call{
// @node	mr shader node name
void Visitor::visit_mib_amb_occlusion(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_mib_amb_occlusion("<<node<<")");
	
	asr::ParamArray param;
	param.insert("samples", 16);
	param.insert("max_distance", 1);
// 	OutputHelper o;
// 	o.begin(param);
// 	o.addVariable("index",	"samples",		"samples",		node);//add to param
// 	o.addVariable("float",	"max_distance",	"max_distance",	node);//add to param
// 	o.end();
	Renderer* m_renderer;
	asr::Assembly* m_assembly;
	m_renderer = dynamic_cast<appleseed::Renderer*>( liquid::RendererMgr::getInstancePtr()->getRenderer() );
	assert(m_renderer != NULL );
	m_assembly = m_renderer->getAssembly().get();
	assert(m_assembly != nullptr);

	if( m_assembly->surface_shaders().get_by_name(node) ){
		return;//already exists.
	}

	m_assembly->surface_shaders().insert(
		asr::AOSurfaceShaderFactory().create(
		node,
		param
		)
	);
}
}//namespace call
}//namespace appleseed
#endif//_USE_APPLESEED_