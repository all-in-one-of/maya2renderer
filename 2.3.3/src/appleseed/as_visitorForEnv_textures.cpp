#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#include "as_visitorForEnv.h"
#include <liqlog.h>
#include "../common/mayacheck.h"
//#include "../shadergraph/convertShadingNetwork.h"
//#include "../shadergraph/shadermgr.h"

namespace appleseed{
namespace env{
/// 2D Texture ///
// @node	maya shader node name
void Visitor::visitBulge(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitBulge("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
void Visitor::visitChecker(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitChecker("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
void Visitor::visitCloth(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitCloth("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
void Visitor::visitFile(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitFile("<<node<<")");

	//OutputHelper o(file);

	////generate texture and construct texture node
	//MString fileImageName(getFileNodeImageName(node));
	//{
	//	//test "fileImageName" exist or not.
	//	if( access(fileImageName.asChar(), 0) != 0){
	//		liquidMessage2(messageError,"%s not exist!", fileImageName.asChar());
	//		assert(0&&"image not exist.");
	//	}

	//	bool isERTex;//whether fileImageName is ER texture
	//	{
	//		std::string fileImageName_(fileImageName.asChar());
	//		std::size_t i_last_dot = fileImageName_.find_last_of('.');
	//		if( i_last_dot == std::string::npos ){
	//			liquidMessage2(messageWarning,"%s has no extention!", fileImageName_.c_str());
	//			assert(0&&"warrning: texture name has not extention.");
	//		}
	//		std::string imgext(fileImageName_.substr(i_last_dot+1));//imgext=tex
	//		std::transform(imgext.begin(),imgext.end(),imgext.begin(),tolower);
	//	
	//		isERTex = (imgext == "tex");
	//	}

	//	MString fileTextureName = (isERTex)? fileImageName : (fileImageName+".tex");

	//	//generate texture
	//	if ( access(fileTextureName.asChar(), 0) != 0 )//not exist
	//	{
	//		o.addTo( "ei_make_texture(\""+fileImageName+"\",\""+fileTextureName+"\","+
	//			"EI_TEX_WRAP_CLAMP, EI_TEX_WRAP_CLAMP, EI_FILTER_BOX, 1.0f, 1.0f)" );
	//	}
	//	//construct texture node
	//	//if (ei_file_exists(fileTextureName))
	//	{
	//		o.addTo( "ei_texture(\""+fileImageName+"\");" );
	//		o.addTo( "ei_file_texture(\""+fileTextureName+"\", eiFALSE);" );
	//		o.addTo( "ei_end_texture();" );
	//	}
	//}


	//o.begin(node);

	//o.addTo("ei_shader_param_string(\"desc\", \"maya_file\");");
	////input
	//o.addVariable("float",  "alphaGain",	"alphaGain",	node);
	//o.addVariable("bool",  "alphaIsLuminance",	"alphaIsLuminance",	node);
	//o.addVariable("float",  "alphaOffset",	"alphaOffset",	node);
	//o.addVariable("color",  "colorGain",	"colorGain",	node);
	//o.addVariable("color",  "colorOffset",	"colorOffset",	node);
	//o.addVariable("color",  "defaultColor",	"defaultColor",	node);
	//o.addVariable("vector",  "uvCoord",	"uvCoord",	node);
	//o.addTo("ei_shader_param_texture(\"fileTextureName\", \""+fileImageName+"\")");
	//o.addVariable("index", "filterType",	"filterType",	node);
	//o.addVariable("float",  "filter",	"filter",	node);
	//o.addVariable("float",  "filterOffset",	"filterOffset",	node);
	//o.addVariable("bool",  "invert",	"invert",	node);
	//o.addVariable("bool",  "fileHasAlpha",	"fileHasAlpha",	node);
	////o.addVariable("index", "num_channels",	"num_channels",	node);
	////output
	//o.addVariable("float", "outAlpha",	"outAlpha",	node);
	//o.addVariable("color", "outColor",	"outColor",	node);
	//o.addVariable("color", "outTransparency",	"outTransparency",	node);
	//o.end();
}
//
// @node	maya shader node name
void Visitor::visitFluidTexture2D(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitFluidTexture2D("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitFractal(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitFractal("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitGrid(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitGrid("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitMountain(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitMountain("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitMovie(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitMovie("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitNoise(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitNoise("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitOcean(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitOcean("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitPSDFileTex(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPSDFileTex("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitRamp(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitRamp("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitWater(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitWater("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
/// 3D Textures ///
// @node	maya shader node name
void Visitor::visitBrownian(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitBrownian("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitCloud(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitCloud("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitCrater(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitCrater("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitFluidTexture3D(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitFluidTexture3D("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitGranite(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitGranite("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitLeather(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLeather("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitMarbler(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitMarbler("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitRock(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitRock("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitSnow(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSnow("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitSolidFractal(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSolidFractal("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitStucco(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitStucco("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitVolumeNoise(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVolumeNoise("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitWood(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitWood("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
/// Env Textures ///
// @node	maya shader node name
void Visitor::visitEnvBall(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitEnvBall("<<node<<")");

// 	OutputHelper o;
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitEnvChrome(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitEnvChrome("<<node<<")");

// 	OutputHelper o;
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitEnvCube(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitEnvCube("<<node<<")");

// 	OutputHelper o;
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitEnvSky(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitEnvSky("<<node<<")");

// 	OutputHelper o;
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
// @node	maya shader node name
void Visitor::visitEnvSphere(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitEnvSphere("<<node<<")");

// 	OutputHelper o;
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
/// other textures ///
// @node	maya shader node name
void Visitor::visitLayeredTexture(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLayeredTexture("<<node<<")");

// 	OutputHelper o(file);
// 	o.begin(node);
// 	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
// 	o.end();
}
}//namespace env
}//namespace appleseed
#endif//_USE_APPLESEED_