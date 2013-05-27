#include "shaderOutputER.h"
#include <liqlog.h>
#include "../common/mayacheck.h"
//#include "../shadergraph/convertShadingNetwork.h"
//#include "../shadergraph/shadermgr.h"
#include "er_output_mgr.h"
#include "er_helper.h"

namespace ER
{
/// General utilities ///
// @node	maya shader node name
void Visitor::visitArrayMapper(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitArrayMapper("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_arrayMapper", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitBump2d(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitBump2d("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_bump2d", node);
	//input
	o.addRSLVariable("float",	"i_bumpValue",		"bumpValue",	node);
	o.addRSLVariable("float",	"i_bumpDepth",		"bumpDepth",	node);
	o.addRSLVariable("index",	"i_bumpInterp",		"bumpInterp",	node);
	o.addRSLVariable("float",	"i_bumpFilter",		"bumpFilter",	node);
	o.addRSLVariable("float",	"i_bumpFilterOffset",	"bumpFilterOffset",	node);
	o.addRSLVariable("normal",	"i_normalCamera",		"normalCamera",	node);
	if(doesPlugExist(node, "bumpNormal")){
		o.addRSLVariable("normal",  "i_bumpNormal",	"bumpNormal",	node);
	}
	//output
	o.addRSLVariable("normal",  "o_outNormal",		"outNormal",	node);

	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitBump3d(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitBump3d("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_bump3d", node);
	//input
	o.addRSLVariable("scalar",  "i_bumpValue",		"bumpValue",	node);
	o.addRSLVariable("scalar",  "i_bumpDepth",		"bumpDepth",	node);
	o.addRSLVariable("scalar",  "i_bumpFilter",		"bumpFilter",	node);
	o.addRSLVariable("scalar",  "i_bumpFilterOffset",	"bumpFilterOffset",	node);
	o.addRSLVariable("normal",  "i_normalCamera",		"normalCamera",	node);
	//output
	o.addRSLVariable("normal",  "o_outNormal",		"outNormal",	node);
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitCondition(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitCondition("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_condition", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitDistanceBetween(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitDistanceBetween("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_distanceBetween", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitHeightField(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitHeightField("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_heightField", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitLightInfo(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLightInfo("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_lightInfo", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitMultiplyDivide(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitMultiplyDivide("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_multiplyDivide", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
void Visitor::visitPlace2dTexture(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPlace2dTexture("<<node<<")");

	//check whether this node uses internal uv
	int liq_UserDefinedU = 0;//0: use elvishray internal u.   1: use user defined u.
	int liq_UserDefinedV = 0;//0: use elvishray internal v.   1: use user defined v.

	if( elvishray::isPlugConnectedIn(node, "uvCoord") == 1 )
	{
		liq_UserDefinedU = 1;
		liq_UserDefinedV = 1;
	}
	else if( elvishray::isPlugConnectedIn(node, "uCoord") == 1 )
	{
		liq_UserDefinedU = 1;
	}
	else if( elvishray::isPlugConnectedIn(node, "vCoord") == 1 )
	{
		liq_UserDefinedV = 1;
	}

	//
	OutputHelper o;
	o.beginRSL("maya_place2dTexture", node);

	//liquid defined parameters
	out.ei_shader_param_int( "liq_UserDefinedU", liq_UserDefinedU );
	out.ei_shader_param_int( "liq_UserDefinedV", liq_UserDefinedV );
	//Inputs
	o.addRSLVariable("vector",  "i_uvCoord",	"uvCoord",		node);
	o.addRSLVariable("float",	"i_coverageU",	"coverageU",	node);
	o.addRSLVariable("float",	"i_coverageV",	"coverageV",	node);
	o.addRSLVariable("bool",	"i_mirrorU",	"mirrorU",		node);
	o.addRSLVariable("bool",	"i_mirrorV",	"mirrorV",		node);
	o.addRSLVariable("float",	"i_noiseU",		"noiseU",		node);
	o.addRSLVariable("float",	"i_noiseV",		"noiseV",		node);
	o.addRSLVariable("float",	"i_offsetU",	"offsetU",		node);
	o.addRSLVariable("float",	"i_offsetV",	"offsetV",		node);
	o.addRSLVariable("float",	"i_repeatU",	"repeatU",		node);
	o.addRSLVariable("float",	"i_repeatV",	"repeatV",		node);
	o.addRSLVariable("float",	"i_rotateFrame","rotateFrame",	node);
	o.addRSLVariable("float",	"i_rotateUV",	"rotateUV",		node);
	o.addRSLVariable("bool",	"i_stagger",	"stagger",		node);
	o.addRSLVariable("float",	"i_translateFrameU",	"translateFrameU",	node);
	o.addRSLVariable("float",	"i_translateFrameV",	"translateFrameV",	node);
	o.addRSLVariable("bool",	"i_wrapU",		"wrapU",		node);
	o.addRSLVariable("bool",	"i_wrapV",		"wrapV",		node);
	//Outputs
	o.addRSLVariable("vector",  "o_outUV",		"outUV",		node);

	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitPlace3dTexture(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPlace3dTexture("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_place3dTexture", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitPlusMinusAverage(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPlusMinusAverage("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_plusMinusAverage", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader nodeplusMinusAverage name
void Visitor::visitProjection(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitProjection("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_projection", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitReverse(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitReverse("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_reverse", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitSamplerInfo(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSamplerInfo("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_samplerInfo", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitSetRange(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSetRange("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_setRange", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitStencil(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitStencil("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_stencil", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitStudioClearCoat(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitStudioClearCoat("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_studioClearCoat", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitUVChooser(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitUVChooser("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_uvChooser", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitVectorProduct(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVectorProduct("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_vectorProduct", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
/// color utilities ///
// @node	maya shader node name
void Visitor::visitBlendColors(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitBlendColors("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_blendColors", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitClamp(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitClamp("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_clamp", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitContrast(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitContrast("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_contrast", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitGammaCorrect(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitGammaCorrect("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_gammaCorrect", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitHsvToRgb(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitHsvToRgb("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_hsvToRgb", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitLuminance(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLuminance("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_luminance", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitRemapColor(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitRemapColor("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_remapColor", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitrRemapHsv(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitrRemapHsv("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_remapHsv", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitRemapValue(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitRemapValue("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_remapValue", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitRgbToHsv(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitRgbToHsv("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_rgbToHsv", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitSmear(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSmear("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_smear", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitSurfaceLuminance(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSurfaceLuminance("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_surfaceLuminance", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
/// switch utilities ///
// @node	maya shader node name
void Visitor::visitDoubleShadingSwitch(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitDoubleShadingSwitch("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_doubleShadingSwitch", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitQuadShadingSwitch(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitQuadShadingSwitch("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_quadShadingSwitch", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitSingleShadingSwitch(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSingleShadingSwitch("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_singleShadingSwitch", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitTripleShadingSwitch(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitTripleShadingSwitch("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_tripleShadingSwitch", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
/// particle utilities ///	
// @node	maya shader node name
void Visitor::visitParticleSamplerInfo(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitParticleSamplerInfo("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_particleSamplerInfo", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
/// image planes ///
// @node	maya shader node name
void Visitor::visitImagePlane(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitImagePlane("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_imagePlane", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
/// glow ///
// @node	maya shader node name
void Visitor::visitOpticalFX(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitOpticalFX("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_opticalFX", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
}//namespace ER