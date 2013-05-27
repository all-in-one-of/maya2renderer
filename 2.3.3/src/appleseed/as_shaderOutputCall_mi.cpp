#include "as_shaderOutputCall.h"
#include <liqlog.h>
#include "../common/mayacheck.h"
#include "../shadergraph/convertShadingNetwork.h"
#include "../shadergraph/shadermgr.h"
#include "prerequest_as.h"
#include "as_renderer.h"
#include "../renderermgr.h"
#include "as_helper5.h"

namespace appleseed{
namespace call{
// @node	mr shader node name
void Visitor::visit_mib_amb_occlusion(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_mib_amb_occlusion("<<node<<")");
	if( m_assembly == NULL )
	{
		Renderer* m_renderer = dynamic_cast<appleseed::Renderer*>( liquid::RendererMgr::getInstancePtr()->getRenderer() );
		assert(m_renderer != NULL );

		m_assembly = m_renderer->getAssembly().get();
		assert(m_assembly != nullptr);
	}

	MStatus status;
	MObject mnode;
	getDependNodeByName(mnode, node);

	int samples;
	IfMErrorWarn(liquidGetPlugValue(mnode, "samples", samples, status));
	
	float max_distance;
	IfMErrorWarn(liquidGetPlugValue(mnode, "max_distance", max_distance, status));
	//If max_distance is zero, the entire scene is sampled
	if(max_distance<=0.0f){
		max_distance = FLT_MAX;
	}

	std::string surfaceshader_name(getSurfaceShaderName(node));
	if(m_assembly->surface_shaders().get_by_name(surfaceshader_name.c_str()) == nullptr)
	{
		m_assembly->surface_shaders().insert(
			asr::AOSurfaceShaderFactory().create(
			surfaceshader_name.c_str(),
			asr::ParamArray()
				.insert("sampling_method",	"uniform")//uniform, cosine
				.insert("samples",			samples)
				.insert("max_distance",		max_distance)
			)
		);
	}
}
}//namespace call
}//namespace appleseed