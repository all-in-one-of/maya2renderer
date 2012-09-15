#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#include "as_shaderOutputCall.h"
#include <liqlog.h>
#include "../common/mayacheck.h"
#include "../shadergraph/convertShadingNetwork.h"
#include "../shadergraph/shadermgr.h"
#include "as_renderer.h"
#include "as_helper.h"
#include "../renderermgr.h"

namespace appleseed{
namespace call{
/// 2D Texture ///
// @node	maya shader node name
void Visitor::visitBulge(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitBulge("<<node<<")");
}
void Visitor::visitChecker(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitChecker("<<node<<")");

	OutputHelper o;

	o.begin(node);

	// Inputs:
	o.addVariable("float",	"alphaGain",		"alphaGain",		node);
	o.addVariable("bool",	"alphaIsLuminance",	"alphaIsLuminance",	node);
	o.addVariable("float",	"alphaOffset",		"alphaOffset",		node);
	o.addVariable("color",	"color1",			"color1",			node);
	o.addVariable("color",	"color2",			"color2",			node);
	o.addVariable("color",	"colorGain",		"colorGain",		node);
	o.addVariable("color",	"colorOffset",		"colorOffset",		node);
	o.addVariable("float",	"contrast",			"contrast",			node);
	o.addVariable("color",	"defaultColor",		"defaultColor",		node);
	o.addVariable("float",	"filter",			"filter",			node);
	o.addVariable("float",	"filterOffset",		"filterOffset",		node);
	o.addVariable("bool",	"invert",			"invert",			node);
	o.addVariable("vector",	"uvCoord",			"uvCoord",			node);
	// Outputs:
	o.addVariable("float",	"outAlpha",			"outAlpha",			node);
	o.addVariable("color",	"outColor",			"outColor",			node);

	o.end();
}
void Visitor::visitCloth(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitCloth("<<node<<")");
}
void Visitor::visitFile(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitFile("<<node<<")");

	OutputHelper o;

	//generate texture and construct texture node
	MString fileImageName(getFileNodeImageName(node));
	MString fileTextureName;
	{
		//test "fileImageName" exist or not.
		if( access(fileImageName.asChar(), 0) != 0){
			liquidMessage2(messageError,"%s not exist!", fileImageName.asChar());
			assert(0&&"image not exist.");
		}

		bool needToConvert;//whether fileImageName is exr texture
		{
			std::string fileImageName_(fileImageName.asChar());
			std::size_t i_last_dot = fileImageName_.find_last_of('.');
			if( i_last_dot == std::string::npos ){
				liquidMessage2(messageWarning,"%s has no extention!", fileImageName_.c_str());
				assert(0&&"warrning: texture name has not extention.");
			}
			std::string imgext(fileImageName_.substr(i_last_dot+1));//imgext=exr
			std::transform(imgext.begin(),imgext.end(),imgext.begin(),tolower);

			needToConvert = (imgext != "exr");
		}

		fileTextureName = (needToConvert)? (fileImageName+".exr") : fileImageName;

		//generate texture
		if ( access(fileTextureName.asChar(), 0) != 0 )//not exist
		{
			makeTexture( fileImageName.asChar(), fileTextureName.asChar() );
		}
		//construct texture node
		//if (ei_file_exists(fileTextureName))
		{
			//ei_texture(fileImageName.asChar());
			//	ei_file_texture(fileTextureName.asChar(), eiFALSE);
			//ei_end_texture();
		}
	}

	// AS stuff
	Renderer* m_renderer = dynamic_cast<appleseed::Renderer*>( liquid::RendererMgr::getInstancePtr()->getRenderer() );
	assert(m_renderer != NULL );
	asr::Assembly* m_assembly = m_renderer->getAssembly().get();
	assert(m_assembly != nullptr);

	//texture
	if( m_assembly->textures().get_by_name(node) == nullptr)
	{
		asr::ParamArray texture_params;
		texture_params.insert("filename", fileTextureName.asChar());
		texture_params.insert("color_space", "srgb");

		asf::SearchPaths search_paths;
		{
			MString dirname;
			MGlobal::executeCommand("dirname \""+fileTextureName+"\"", dirname, false, true);
			search_paths.push_back(dirname.asChar());
		}
		asf::auto_release_ptr<asr::Texture> texture = 
			asr::DiskTexture2dFactory().create(node, texture_params, search_paths);

		std::size_t texture_index = m_assembly->textures().insert(texture);
	}


	//instance
	if( m_assembly->texture_instances().get_by_name(getTextureInstanceName(node).c_str()) == nullptr)
	{
		const std::string texture_instance_name = getTextureInstanceName(node);

		asr::ParamArray texture_instance_params;
		texture_instance_params.insert("addressing_mode", "clamp");
		texture_instance_params.insert("filtering_mode", "bilinear");

		bool isOutTransparncyConnected = false;
		{
			MStringArray srcNodePlug;
			IfMErrorWarn(MGlobal::executeCommand("listConnections -destination true -plugs true \""+MString(node)+".outTransparency\"", srcNodePlug));
			isOutTransparncyConnected = srcNodePlug.length()>0;
		}
		texture_instance_params.insert("alpha_mode",	
			isOutTransparncyConnected ? "alpha_channel" : "luminance");

		asf::auto_release_ptr<asr::TextureInstance> texture_instance =
			asr::TextureInstanceFactory::create(
				texture_instance_name.c_str(),
				texture_instance_params,
				node
			);

		m_assembly->texture_instances().insert(texture_instance);
	}

}
//
//
// @node	maya shader node name
void Visitor::visitFluidTexture2D(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitFluidTexture2D("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitFractal(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitFractal("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitGrid(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitGrid("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitMountain(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitMountain("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitMovie(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitMovie("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitNoise(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitNoise("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitOcean(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitOcean("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitPSDFileTex(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPSDFileTex("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitRamp(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitRamp("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitWater(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitWater("<<node<<")");
}
/// 3D Textures ///
// @node	maya shader node name
void Visitor::visitBrownian(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitBrownian("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitCloud(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitCloud("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitCrater(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitCrater("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitFluidTexture3D(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitFluidTexture3D("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitGranite(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitGranite("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitLeather(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLeather("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitMarbler(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitMarbler("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitRock(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitRock("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitSnow(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSnow("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitSolidFractal(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSolidFractal("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitStucco(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitStucco("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitVolumeNoise(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVolumeNoise("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitWood(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitWood("<<node<<")");
}
/// Env Textures ///
// @node	maya shader node name
void Visitor::visitEnvBall(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitEnvBall("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitEnvChrome(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitEnvChrome("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitEnvCube(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitEnvCube("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitEnvSky(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitEnvSky("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitEnvSphere(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitEnvSphere("<<node<<")");
}
/// other textures ///
// @node	maya shader node name
void Visitor::visitLayeredTexture(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLayeredTexture("<<node<<")");
}
}//namespace call
}//namespace appleseed
#endif//_USE_APPLESEED_