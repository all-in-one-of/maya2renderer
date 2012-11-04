#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#include "er_renderer.h"
#include <liqPreviewShader.h>
#include "../common/prerequest_maya.h"
#include "../common/mayacheck.h"
#include "ercall.h"
#include "log_helper.h"
//#include <liqRibNode.h>
#include <liqRibTranslator.h>

namespace elvishray
{

	int Renderer::preview( const std::string& fileName, const liqPreviewShaderOptions& options )
	{
		CM_TRACE_FUNC("Renderer::preview("<<fileName<<","<<options.shaderNodeName<<")");

		return 1;
	}
	//

}//namespace elvishray
#endif//_USE_ELVISHRAY_
