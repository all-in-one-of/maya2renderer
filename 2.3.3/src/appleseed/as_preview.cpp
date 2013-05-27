#include "as_renderer.h"
#include <liqPreviewShader.h>
#include "../common/prerequest_maya.h"
#include "../common/mayacheck.h"
#include "as_log_helper.h"
//#include <liqRibNode.h>
#include <liqRibTranslator.h>

namespace appleseed
{
	int Renderer::preview( const std::string& fileName, const liqPreviewShaderOptions& options )
	{
		CM_TRACE_FUNC("Renderer::preview("<<fileName<<","<<options.shaderNodeName<<")");

		return 1;
	}
	//
}//namespace appleseed