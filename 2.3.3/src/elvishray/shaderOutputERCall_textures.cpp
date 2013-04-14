#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#include "shaderOutputERCall.h"
#include <liqlog.h>
#include "../common/mayacheck.h"
#include "../shadergraph/convertShadingNetwork.h"
#include "../shadergraph/shadermgr.h"
#include "er_output_mgr.h"
//#include <eiAPI/ei.h>

namespace ERCall
{
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

	o.beginRSL("maya_checker", node);

	// Inputs:
	o.addRSLVariable("float",	"alphaGain",		"alphaGain",		node);
	o.addRSLVariable("bool",	"alphaIsLuminance",	"alphaIsLuminance",	node);
	o.addRSLVariable("float",	"alphaOffset",		"alphaOffset",		node);
	o.addRSLVariable("color",	"color1",			"color1",			node);
	o.addRSLVariable("color",	"color2",			"color2",			node);
	o.addRSLVariable("color",	"colorGain",		"colorGain",		node);
	o.addRSLVariable("color",	"colorOffset",		"colorOffset",		node);
	o.addRSLVariable("float",	"contrast",			"contrast",			node);
	o.addRSLVariable("color",	"defaultColor",		"defaultColor",		node);
	o.addRSLVariable("float",	"filter",			"filter",			node);
	o.addRSLVariable("float",	"filterOffset",		"filterOffset",		node);
	o.addRSLVariable("bool",	"invert",			"invert",			node);
	o.addRSLVariable("vector",	"uvCoord",			"uvCoord",			node);
	// Outputs:
	o.addRSLVariable("float",	"outAlpha",			"outAlpha",			node);
	o.addRSLVariable("color",	"outColor",			"outColor",			node);

	o.endRSL();
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
	{
		//test "fileImageName" exist or not.
		if( _access(fileImageName.asChar(), 0) != 0){
			liquidMessage2(messageError,"%s not exist!", fileImageName.asChar());
			assert(0&&"image not exist.");
		}

		bool isERTex;//whether fileImageName is ER texture
		{
			std::string fileImageName_(fileImageName.asChar());
			std::size_t i_last_dot = fileImageName_.find_last_of('.');
			if( i_last_dot == std::string::npos ){
				liquidMessage2(messageWarning,"%s has no extention!", fileImageName_.c_str());
				assert(0&&"warrning: texture name has not extention.");
			}
			std::string imgext(fileImageName_.substr(i_last_dot+1));//imgext=tex
			std::transform(imgext.begin(),imgext.end(),imgext.begin(),tolower);
		
			isERTex = (imgext == "tex");
		}

		MString fileTextureName = (isERTex)? fileImageName : (fileImageName+".tex");

		//generate texture
		if ( _access(fileTextureName.asChar(), 0) != 0 )//not exist
		{
			out.ei_make_texture(fileImageName.asChar(), fileTextureName.asChar(),
				EI_TEX_WRAP_CLAMP, EI_TEX_WRAP_CLAMP, EI_FILTER_BOX, 1.0f, 1.0f);
		}
		//construct texture node
		//if (ei_file_exists(fileTextureName))
		{
			out.ei_texture(fileImageName.asChar());
				out.ei_file_texture(fileTextureName.asChar(), eiFALSE);
			out.ei_end_texture();
		}
	}


	o.beginRSL("maya_file", node);

	//input
	o.addRSLVariable("float",  "alphaGain",	"alphaGain",	node);
	o.addRSLVariable("bool",  "alphaIsLuminance",	"alphaIsLuminance",	node);
	o.addRSLVariable("float",  "alphaOffset",	"alphaOffset",	node);
	o.addRSLVariable("color",  "colorGain",	"colorGain",	node);
	o.addRSLVariable("color",  "colorOffset",	"colorOffset",	node);
	o.addRSLVariable("color",  "defaultColor",	"defaultColor",	node);
	o.addRSLVariable("vector",  "uvCoord",	"uvCoord",	node);
	out.ei_shader_param_texture("fileTextureName", fileImageName.asChar());
	o.addRSLVariable("index", "filterType",	"filterType",	node);
	o.addRSLVariable("float",  "filter",	"filter",	node);
	o.addRSLVariable("float",  "filterOffset",	"filterOffset",	node);
	o.addRSLVariable("bool",  "invert",	"invert",	node);
	o.addRSLVariable("bool",  "fileHasAlpha",	"fileHasAlpha",	node);
	//o.addRSLVariable("index", "num_channels",	"num_channels",	node);
	//output
	o.addRSLVariable("float", "outAlpha",	"outAlpha",	node);
	o.addRSLVariable("color", "outColor",	"outColor",	node);
	o.addRSLVariable("color", "outTransparency",	"outTransparency",	node);
	o.endRSL();
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
}//namespace ERCall
#endif//_USE_ELVISHRAY_