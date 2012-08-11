#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#include "as_shaderOutput.h"
//#include <liqlog.h>
#include "../common/mayacheck.h"
//#include "../shadergraph/convertShadingNetwork.h"
//#include "../shadergraph/shadermgr.h"
#include <trace/trace.hpp>

namespace appleseed
{

// @node	maya shader node name
void Visitor::visitAnisotropic(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitAnisotropic("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
void Visitor::visitBlinn(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitBlinn("<<node<<")");

	OutputHelper o(file);

	o.begin(node);

	o.addTo("ei_shader_param_string(\"desc\", \"plastic\");");
	o.addVariable("vector", "inColor",		"color",		node);
	o.addVariable("vector", "transparency", "transparency", node);
	o.addVariable("vector", "ambColor",		"ambientColor", node);
	o.addVariable("vector", "incandescence","incandescence",node);
	o.addVariable("float",  "diffusion",	"diffuse",		node);
	o.addVariable("float",  "eccentricity", "eccentricity", node);
	o.addVariable("float",  "specRollOff",	"specularRollOff",node);
	o.addVariable("vector", "specColor",	"specularColor",node);
	o.addVariable("vector", "outColor",		"outColor",		node);

//	MStringArray con;
//	IfMErrorWarn(MGlobal::executeCommand( ("listConnections(\""+MString(node)+"\" + \".reflectedColor\")"), con));
//	if( con.length() != 0 )
//	{
//		o.addVariable( "float", "reflectivity", "reflectivity", node);
//		o.addVariable( "vector", "refColor", "reflectedColor", node);
//		o.addTo( "Cspecular += ( reflectivity * refColor );");
//	}

	o.end();
}
void Visitor::visitHairTubeShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitHairTubeShader("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
void Visitor::visitLambert(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLambert("<<node<<")");

	OutputHelper o(file);

	o.begin(node);

	o.addTo("ei_shader_param_string(\"desc\", \"maya_lambert_transparent\");");
	o.addVariable("vector", "Cs",		"color",		node);
	o.addVariable("vector", "transparency", "transparency", node);
	o.addVariable("vector", "ambientColor",	"ambientColor", node);
	o.addVariable("vector", "incandescence","incandescence",node);
	o.addVariable("float",  "diffusion",	"diffuse",		node);
	o.addVariable("vector", "outColor",		"outColor",		node);
	o.addVariable("vector", "outTransparency","outTransparency",node);

	o.end();
}
// @node	maya shader node name
void Visitor::visitLayeredShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLayeredShader("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visitOceanShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitOceanShader("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visitPhong(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPhong("<<node<<")");

	OutputHelper o(file);
	o.begin(node);

	o.addTo("ei_shader_param_string(\"desc\", \"maya_phong\");");
	//Common Material Attributes
	o.addVariable("vector", "color_",		"color",		node);
	o.addVariable("vector", "transparency",	"transparency", node);
	o.addVariable("vector", "ambientColor",	"ambientColor", node);
	o.addVariable("vector", "incandescence",	"incandescence",node);
	o.addVariable("normal", "normalCamera",	"normalCamera",	node);
	o.addVariable("float",  "diffuse",		"diffuse",		node);
	o.addVariable("float",  "translucence",			"translucence",		node);
	o.addVariable("float",  "translucenceDepth",		"translucenceDepth",node);
	o.addVariable("float",  "translucenceFocus",		"translucenceFocus",node);
	//Specular Shading
	o.addVariable("float",	"cosinePower",			"cosinePower",		node);
	o.addVariable("vector",	"specularColor",		"specularColor",	node);
	o.addVariable("float",	"reflectivity",			"reflectivity",		node);
	o.addVariable("vector",	"reflectedColor",		"reflectedColor",	node);
	//Matte Opacity
	o.addVariable("index",	"matteOpacityMode",		"matteOpacityMode",	node);
	o.addVariable("float",	"matteOpacity",			"matteOpacity",		node);
	//Raytrace Options
	o.addVariable("index",	"reflectionLimit",		"reflectionLimit",	node);
	//output
	o.addVariable("vector", "outColor",		"outColor",		node);
	o.addVariable("vector", "outTransparency","outTransparency",node);

	o.end();
}
// @node	maya shader node name
void Visitor::visitPhongE(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPhongE("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visitRampShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitRampShader("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visitShadingMap(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitShadingMap("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visitSurfaceShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSurfaceShader("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visitUseBackground(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitUseBackground("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
/// Volumetric ///
// @node	maya shader node name
void Visitor::visitEnvFog(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitEnvFog("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visitFluidShape(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitFluidShape("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visitLightFog(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLightFog("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visitParticleCloud(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitParticleCloud("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visitVolumeFog(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVolumeFog("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visitVolumeShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVolumeShader("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
/// DISPLACEMENT ///
// @node	maya shader node name
void Visitor::visitCMuscleShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitCMuscleShader("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visitDisplacementShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitDisplacementShader("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}

/// Lights ///
// @node	maya shader node name
void Visitor::visitAmbientLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitAmbientLight("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visitAreaLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitAreaLight("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visitDirectionalLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitDirectionalLight("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visitPointLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPointLight("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visitSpotLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSpotLight("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visitVolumeLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVolumeLight("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
// @node	maya shader node name
void Visitor::visit_liquidSurface(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_liquidSurface("<<node<<")");

	OutputHelper o(file);
	o.begin(node);
	o.addTo("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.end();
}
}//namespace appleseed
#endif//_USE_APPLESEED_