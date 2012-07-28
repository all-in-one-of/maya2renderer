#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#include "as_renderer.h"
// Maya headers
#include "../common/prerequest_maya.h"
// Liquid headers
#include <liqShader.h>
#include <liqShaderFactory.h>
#include <liqGlobalHelpers.h>
#include "../common/mayacheck.h"
#include "log_helper.h"
#include "as_material3.h"
#include "as_GlobalNodeHelper.h"

namespace appleseed
{
	//////////////////////////////////////////////////////////////////////////
	void Renderer::createEnvironment()
	{
		CM_TRACE_FUNC("Renderer::createEnvironment()");


		MaterialFactory3 mf;
		mf.begin(m_globalNodeName.asChar());
		mf.createEnvironmentEDF( (EnvironmentEDFModel)m_gnode->getInt("env_edf_model") );
		mf.createEnvironmentShader("edf_environment_shader");
		mf.createEnvironment("generic_environment");
		mf.end();

	}
}//namespace appleseed
#endif//_USE_APPLESEED_