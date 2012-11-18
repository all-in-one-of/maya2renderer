#include <liqConfig.h>
#ifdef _USE_RENDERMAN_

#include "shaderOutputRSL.h"
#include "../common/mayacheck.h"
#include "ri_interface.h"
#include <trace/trace.hpp>
#include <liqShader.h>
#include <liqShaderFactory.h>
#include <liqTokenPointer.h>
namespace RSL
{
//

//
// @node	maya shader node name
void Visitor::visitAnisotropic(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitAnisotropic("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitBlinn(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitBlinn("<<node<<")");
#ifdef USE_LIQUID_RM_SHADE
	OutputHelper o(RSLfile);

	o.beginRSL(node);

	o.addRSLVariable(       "", "color", "inColor",			"color",		node);
	o.addRSLVariable(       "", "color", "transparency",	"transparency", node);
	o.addRSLVariable(       "", "color", "ambColor",		"ambientColor", node);
	o.addRSLVariable(       "", "color", "incandescence",	"incandescence",node);
	o.addRSLVariable(       "", "float", "diffusion",		"diffuse",		node);
	o.addRSLVariable(       "", "float", "eccentricity",	"eccentricity", node);
	o.addRSLVariable(       "", "float", "specRollOff",		"specularRollOff",node);
	o.addRSLVariable(       "", "color", "specColor",		"specularColor",node);
	o.addRSLVariable(       "", "color", "outColor",		"outColor",		node);

	o.addToRSL( "extern normal N;");
	o.addToRSL( "normal Nn = normalize( N );");
	o.addToRSL( "Oi = Os * color ( 1 - transparency );");
	o.addToRSL( "color Cdiffuse;");
	o.addToRSL( "Cdiffuse = incandescence +");
	o.addToRSL( "           ( inColor * ( diffusion * ");
	o.addToRSL( "                         diffuse( Nn ) +");
	o.addToRSL( "                         ambColor ) );");
	o.addToRSL( "color Cspecular = 0;");
	o.addToRSL( "float eccSq = pow( eccentricity, 2 );");
	o.addToRSL( "vector V = normalize( -I );");
	o.addToRSL( "float NV = Nn . V;");
	o.addToRSL( "illuminance( P, Nn, PI / 2 )");
	o.addToRSL( "{");
	o.addToRSL( " vector Ln = normalize( L );");
	o.addToRSL( " vector H = normalize( V + Ln );");
	o.addToRSL( " float NH = Nn . H;");
	o.addToRSL( " float NL = Nn . Ln;");
	o.addToRSL( " float VH = V . H;");
	o.addToRSL( " float D = pow( eccSq / ");
	o.addToRSL( "                ( pow( NH, 2 ) * ");
	o.addToRSL( "                  ( eccSq - 1 ) + 1 ), 2 );");
	o.addToRSL( " float G = min( min( 1, 2 * NH * NV / VH ), ");
	o.addToRSL( "                2 * NH * NL / VH );");
	o.addToRSL( " Cspecular += Cl * ( D * G / NV ) * mix( 1, specRollOff, pow(1-VH, 3) );");
	o.addToRSL( "}");

	MStringArray con;
	IfMErrorWarn(MGlobal::executeCommand( ("listConnections(\""+MString(node)+"\" + \".reflectedColor\")"), con));
	if( con.length() != 0 )
	{
		o.addRSLVariable(       "", "float", "reflectivity", "reflectivity", node);
		o.addRSLVariable(       "", "color", "refColor", "reflectedColor", node);
		o.addToRSL( "Cspecular += ( reflectivity * refColor );");
	}
	o.addToRSL( "Cspecular *= specColor;");
	o.addToRSL( "outColor = Cdiffuse + Cspecular;");
	o.addToRSL( "Ci = Cs * Oi * color outColor;");

	o.endRSL();
#else
	OutputHelper o(RSLfile);

	o.addInclude("blinn.h");

	o.beginRSL(node);
	// Inputs:
	o.addRSLVariable(       "", "vector", "ambientColor",		"ambientColor",		node);
	o.addRSLVariable(       "", "vector", "i_color",			"color",			node);
	o.addRSLVariable(       "", "float",  "diffuse",			"diffuse",			node);
	o.addRSLVariable(       "", "float",  "eccentricity",		"eccentricity",		node);
	o.addRSLVariable(       "", "vector", "incandescence",		"incandescence",	node);
	o.addRSLVariable("uniform", "float",  "matteOpacityMode",	"matteOpacityMode",	node);
	o.addRSLVariable(       "", "float",  "matteOpacity",		"matteOpacity",		node);
	o.addRSLVariable(       "", "vector", "specularColor",		"specularColor",	node);
	o.addRSLVariable(       "", "float",  "specularRollOff",	"specularRollOff",	node);
	o.addRSLVariable(       "", "float",  "reflectivity",		"reflectivity",		node);
	o.addRSLVariable(       "", "vector", "reflectedColor",		"reflectedColor",	node);
	/* Refraction. */
	o.addRSLVariable("uniform", "float", "refractions",			"refractions",		node);
	o.addRSLVariable(       "", "float", "refractiveIndex",		"refractiveIndex",	node);
	o.addRSLVariable("uniform", "float", "refractionLimit",		"refractionLimit",	node);
	o.addRSLVariable(       "", "float", "lightAbsorbance",		"lightAbsorbance",	node);
	o.addRSLVariable(       "", "float", "shadowAttenuation",	"shadowAttenuation",node);
	/* Reflection. */
	o.addRSLVariable("uniform", "float", "reflectionLimit",		"reflectionLimit",	node);

	o.addRSLVariable(       "", "float", "translucence",		"translucence",		node);
	o.addRSLVariable(       "", "float", "translucenceDepth",	"translucenceDepth",node);
	o.addRSLVariable(       "", "float", "translucenceFocus",	"translucenceFocus",node);
	o.addRSLVariable(       "", "vector","transparency",		"transparency",		node);
	o.addToRSL("normal normalCamera = N;");

	o.addToRSL("uniform float i_reflectionMaxDistance   =4;");
	o.addToRSL("uniform float i_reflectionSamples       =4;");
	o.addToRSL("uniform float i_reflectionBlur          =4;");
	o.addToRSL("uniform float i_reflectionNoiseAmplitude=4;");
	o.addToRSL("uniform float i_reflectionNoiseFrequency=4;");
	// Outputs
	o.addRSLVariable(       "", "vector", "outColor",			"outColor",			node);
	o.addRSLVariable(       "", "vector", "outTransparency",	"outTransparency",  node);

	o.addToRSL("{");
	o.addToRSL("  color _outColor;");
	o.addToRSL("  color _outTransparency;");
	o.addToRSL("  maya_blinn("
						//Inputs
						"color ambientColor,		\n\t"
						"color i_color,				\n\t"
						"diffuse,					\n\t"
						"eccentricity,				\n\t"
						"color incandescence,		\n\t"
						"matteOpacityMode,			\n\t"
						"matteOpacity,				\n\t"
						"color specularColor,		\n\t"
						"specularRollOff,			\n\t"
						"reflectivity,				\n\t"
						"color reflectedColor,		\n\t"
						/* Refraction. */
						"refractions,				\n\t"
						"refractiveIndex,			\n\t"
						"refractionLimit,			\n\t"
						"lightAbsorbance,			\n\t"
						"shadowAttenuation,			\n\t"
						/* Reflection. */
						"reflectionLimit,			\n\t"

						"translucence,				\n\t"
						"translucenceDepth,			\n\t"
						"translucenceFocus,			\n\t"
						"color transparency,		\n\t"
						"normal normalCamera,		\n\t"

						"i_reflectionMaxDistance,	\n\t"
						"i_reflectionSamples,		\n\t"
						"i_reflectionBlur,			\n\t"
						"i_reflectionNoiseAmplitude,\n\t"
						"i_reflectionNoiseFrequency,\n\t"
						//Outputs
						"_outColor,					\n\t"
						"_outTransparency			\n"
			"   );");
	o.addToRSL("   Ci             = _outColor;");
	o.addToRSL("   Oi             = _outTransparency;");
	o.addToRSL("  outColor        = vector Ci;");
	o.addToRSL("  outTransparency = vector Oi;");
	o.addToRSL("}");

	o.endRSL();
#endif
}
// @node	maya shader node name
void Visitor::visitHairTubeShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitHairTubeShader("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitLambert(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLambert("<<node<<")");

#ifdef USE_LIQUID_RM_SHADE
	OutputHelper o(RSLfile);

	o.beginRSL(node);

	o.addRSLVariable(       "", "color", "inColor",			"color",		node);
	o.addRSLVariable(       "", "color", "transparency",	"transparency", node);
	o.addRSLVariable(       "", "color", "ambientColor",	"ambientColor", node);
	o.addRSLVariable(       "", "color", "incandescence",	"incandescence",node);
	o.addRSLVariable(       "", "float", "diffusion",		"diffuse",		node);
	o.addRSLVariable(       "", "color", "outColor",		"outColor",		node);
	o.addRSLVariable(       "", "color", "outTransparency",	"outTransparency",node);

	o.addToRSL( "extern normal N;");
	o.addToRSL( "normal Nn = normalize( N );");
	o.addToRSL( "outTransparency = transparency;");
	o.addToRSL( "Oi = Os * color ( 1 - outTransparency );");
	o.addToRSL( "outColor = incandescence +");
	o.addToRSL( "           ( inColor * ( diffusion * ");
	o.addToRSL( "                         diffuse( Nn ) +");
	o.addToRSL( "                         ambientColor ) );");
	o.addToRSL( "Ci = Cs * Oi * color outColor;");

	o.endRSL();
#else
	OutputHelper o(RSLfile);

	o.addInclude("lambert.h");

	o.beginRSL(node);
	// Inputs:
	o.addRSLVariable(       "", "vector", "ambientColor",		"ambientColor",		node);
	o.addRSLVariable(       "", "vector", "i_color",			"color",			node);
	o.addRSLVariable(       "", "float",  "diffuse",			"diffuse",			node);
	o.addRSLVariable(       "", "vector", "incandescence",		"incandescence",	node);
	o.addRSLVariable(       "", "float",  "matteOpacityMode",	"matteOpacityMode",	node);
	o.addRSLVariable(       "", "float",  "matteOpacity",		"matteOpacity",		node);
	/* Refraction. */
	o.addRSLVariable("uniform", "float", "refractions",			"refractions",		node);
	o.addRSLVariable(       "", "float", "refractiveIndex",		"refractiveIndex",	node);
	o.addRSLVariable("uniform", "float", "refractionLimit",		"refractionLimit",	node);
	o.addRSLVariable(       "", "float", "lightAbsorbance",		"lightAbsorbance",	node);
	o.addRSLVariable(       "", "float", "shadowAttenuation",	"shadowAttenuation",node);

	o.addToRSL("normal normalCamera = N;");
	o.addRSLVariable(       "", "vector","transparency",		"transparency",		node);
	o.addRSLVariable(       "", "float", "translucence",		"translucence",		node);
	o.addRSLVariable(       "", "float", "translucenceDepth",	"translucenceDepth",node);
	o.addRSLVariable(       "", "float", "translucenceFocus",	"translucenceFocus",node);
	// Outputs:
	o.addRSLVariable(       "", "vector", "outColor",		"outColor",				node);
	o.addRSLVariable(       "", "vector", "outTransparency","outTransparency",		node);

	o.addToRSL("{");
	o.addToRSL("  color _outColor;");
	o.addToRSL("  color _outTransparency;");
	o.addToRSL("  maya_lambert("
						//Inputs
						"color ambientColor,		\n\t"
						"color i_color,				\n\t"
						"diffuse,					\n\t"
						"color incandescence,		\n\t"
						"matteOpacityMode,			\n\t"
						"matteOpacity,				\n\t"
						/* Refraction. */
						"refractions,				\n\t"
						"refractiveIndex,			\n\t"
						"refractionLimit,			\n\t"
						"lightAbsorbance,			\n\t"
						"shadowAttenuation,			\n\t"
						
						"normal normalCamera,		\n\t"
						"color transparency,		\n\t"
						"translucence,				\n\t"
						"translucenceDepth,			\n\t"
						"translucenceFocus,			\n\t"
						//Outputs
						"_outColor,					\n\t"
						"_outTransparency			\n"
			"   );");
	o.addToRSL("   Ci             = _outColor;");
	o.addToRSL("   Oi             = _outTransparency;");
	o.addToRSL("  outColor        = vector Ci;");
	o.addToRSL("  outTransparency = vector Oi;");
	o.addToRSL("}");

	o.endRSL();
#endif
}
// @node	maya shader node name
void Visitor::visitLayeredShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLayeredShader("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitOceanShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitOceanShader("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitPhong(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPhong("<<node<<")");
#ifdef USE_LIQUID_RM_SHADE
	OutputHelper o(RSLfile);
	o.beginRSL(node);

	o.addRSLVariable(       "", "color", "inColor",			"color",		node);
	o.addRSLVariable(       "", "color", "transparency",	"transparency", node);
	o.addRSLVariable(       "", "color", "ambColor",		"ambientColor", node);
	o.addRSLVariable(       "", "color", "incandescence",	"incandescence",node);
	o.addRSLVariable(       "", "float", "diffusion",		"diffuse",		node);
	o.addRSLVariable(       "", "float", "cosinePower",		"cosinePower",	node);
	o.addRSLVariable(       "", "color", "specColor",		"specularColor",node);
	o.addRSLVariable(       "", "color", "outColor",		"outColor",		node);
	o.addRSLVariable(       "", "color", "outTransparency",	"outTransparency", node);

	o.addToRSL("extern normal N;");
	o.addToRSL("normal Nn = normalize( N );");
	o.addToRSL("outTransparency = transparency;");
	o.addToRSL("Oi = Os * color ( 1 - outTransparency );");
	o.addToRSL("color Cdiffuse;");
	o.addToRSL("Cdiffuse = incandescence +");
	o.addToRSL("           ( inColor * ( diffusion * ");
	o.addToRSL("                         diffuse( Nn ) +");
	o.addToRSL("                         ambColor ) );");
	o.addToRSL("color Cspecular;");
	o.addToRSL("Cspecular = phong( Nn,");
	o.addToRSL("                          normalize( -I ),");
	o.addToRSL("                          cosinePower );");

	MStringArray con;
	IfMErrorWarn(MGlobal::executeCommand( ("listConnections(\""+MString(node)+"\" + \".reflectedColor\")"), con));
	if( con.length() != 0 )
	{
		o.addRSLVariable(       "", "float", "reflectivity",	"reflectivity",		node);
		o.addRSLVariable(       "", "color", "refColor",		"reflectedColor",	node);
		o.addToRSL("Cspecular += ( reflectivity * refColor );");
	}
	o.addToRSL("Cspecular *= specColor;");
	o.addToRSL("outColor = Cdiffuse + Cspecular;");
	o.addToRSL("Ci = Cs * Oi * color outColor;");

	o.endRSL();
#else
	OutputHelper o(RSLfile);

	o.addInclude("phong.h");

	o.beginRSL(node);
	// Inputs:
	o.addRSLVariable(       "", "vector", "ambientColor",		"ambientColor",		node);
	o.addRSLVariable(       "", "vector", "i_color",			"color",			node);
	o.addRSLVariable(       "", "float",  "cosinePower",		"cosinePower",		node);
	o.addRSLVariable(       "", "float",  "diffuse",			"diffuse",			node);
	o.addRSLVariable(       "", "vector", "incandescence",		"incandescence",	node);
	o.addRSLVariable(       "", "float",  "matteOpacity",		"matteOpacity",		node);
	o.addRSLVariable(       "", "float",  "matteOpacityMode",	"matteOpacityMode",	node);
	o.addToRSL("normal normalCamera = N;");
	o.addRSLVariable(       "", "vector", "specularColor",		"specularColor",	node);
	o.addRSLVariable(       "", "float",  "reflectivity",		"reflectivity",		node);
	o.addRSLVariable(       "", "vector", "reflectedColor",		"reflectedColor",	node);
	/* Refraction. */
	o.addRSLVariable("uniform", "float", "refractions",			"refractions",		node);
	o.addRSLVariable(       "", "float", "refractiveIndex",		"refractiveIndex",	node);
	o.addRSLVariable("uniform", "float", "refractionLimit",		"refractionLimit",	node);
	o.addRSLVariable(       "", "float", "lightAbsorbance",		"lightAbsorbance",	node);
	o.addRSLVariable(       "", "float", "shadowAttenuation",	"shadowAttenuation",node);
	/* Reflection. */
	o.addRSLVariable("uniform", "float", "reflectionLimit",		"reflectionLimit",	node);

	o.addRSLVariable(       "", "float", "translucence",		"translucence",		node);
	o.addRSLVariable(       "", "float", "translucenceDepth",	"translucenceDepth",node);
	o.addRSLVariable(       "", "float", "translucenceFocus",	"translucenceFocus",node);
	o.addRSLVariable(       "", "vector","transparency",		"transparency",		node);

	o.addToRSL("uniform float i_reflectionMaxDistance   =4;");
	o.addToRSL("uniform float i_reflectionSamples       =4;");
	o.addToRSL("uniform float i_reflectionBlur          =4;");
	o.addToRSL("uniform float i_reflectionNoiseAmplitude=4;");
	o.addToRSL("uniform float i_reflectionNoiseFrequency=4;");

	// Outputs
	o.addRSLVariable(       "", "vector", "outColor",			"outColor",			node);
	o.addRSLVariable(       "", "vector", "outTransparency",	"outTransparency",  node);

	o.addToRSL("{");
	o.addToRSL("  color _outColor;");
	o.addToRSL("  color _outTransparency;");
	o.addToRSL("  maya_phong("
						//Inputs
						"color ambientColor,		\n\t"
						"color i_color,				\n\t"
						"cosinePower,				\n\t"
						"diffuse,					\n\t"
						"color incandescence,		\n\t"
						"matteOpacity,				\n\t"
						"matteOpacityMode,			\n\t"
						"normal normalCamera,		\n\t"
						"color specularColor,		\n\t"
						"reflectivity,				\n\t"
						"color reflectedColor,		\n\t"
						/* Refraction. */
						"refractions,				\n\t"
						"refractiveIndex,			\n\t"
						"refractionLimit,			\n\t"
						"lightAbsorbance,			\n\t"
						"shadowAttenuation,			\n\t"
						/* Reflection. */
						"reflectionLimit,			\n\t"

						"translucence,				\n\t"
						"translucenceDepth,			\n\t"
						"translucenceFocus,			\n\t"
						"color transparency,		\n\t"

						"i_reflectionMaxDistance,	\n\t"
						"i_reflectionSamples,		\n\t"
						"i_reflectionBlur,			\n\t"
						"i_reflectionNoiseAmplitude,\n\t"
						"i_reflectionNoiseFrequency,\n\t"

						//Outputs
						"_outColor,					\n\t"
						"_outTransparency			\n"
				"   );");
	o.addToRSL("   Ci             = _outColor;");
	o.addToRSL("   Oi             = _outTransparency;");
	o.addToRSL("  outColor        = vector Ci;");
	o.addToRSL("  outTransparency = vector Oi;");
	o.addToRSL("}");

	o.endRSL();
#endif
}
// @node	maya shader node name
void Visitor::visitPhongE(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPhongE("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitRampShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitRampShader("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitShadingMap(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitShadingMap("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitSurfaceShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSurfaceShader("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitUseBackground(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitUseBackground("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
/// Volumetric ///
// @node	maya shader node name
void Visitor::visitEnvFog(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitEnvFog("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitFluidShape(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitFluidShape("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitLightFog(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLightFog("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitParticleCloud(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitParticleCloud("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitVolumeFog(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVolumeFog("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitVolumeShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVolumeShader("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
/// DISPLACEMENT ///
// @node	maya shader node name
void Visitor::visitCMuscleShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitCMuscleShader("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitDisplacementShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitDisplacementShader("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}

/// Lights ///
// @node	maya shader node name
void Visitor::visitAmbientLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitAmbientLight("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitAreaLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitAreaLight("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitDirectionalLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitDirectionalLight("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitPointLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPointLight("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitSpotLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSpotLight("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitVolumeLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVolumeLight("<<node<<")");

	OutputHelper o(RSLfile);
	o.beginRSL(node);
	o.addToRSL("//the type of node '"+MString(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visit_liquidShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_liquidShader("<<node<<")");


	liqShader &shader = liqShaderFactory::instance().getShader( node );
	std::string const& liquidShaderName = shader.getName();//e.g."lambert1", or "liquidSurface1", NOTE: it is liquidShader, not maya shader.
	std::string const& rmSloFilePath=shader.getShaderFileName();
	std::string const& mayaShaderName=rmSloFilePath.substr(rmSloFilePath.find_last_of('/')+1);//e.g."your_shader_dir/liquidchecker"

	OutputHelper o(RSLfile);
	o.addInclude((rmSloFilePath+".impl").c_str());
	o.beginRSL(node);

	std::size_t parameterNum =  shader.tokenPointerArray.size() - 1;
	for(size_t i=0; i<parameterNum; ++i)
	{
		//_s("//- "
		//	<<const_cast<liqTokenPointer*>(&tokenPointerArray[i])->getDetailedTokenName()<<","//uniform float intensity
		//	<<tokenPointerArray[i].getDetailType()<<","
		//	<<"//tokenPointerArray[i].getTokenFloatArray()"<<","
		//	<<"//[error]tokenPointerArray[i].getTokenString()"<<","
		//	<<tokenPointerArray[i].getTokenName()<<","//intensity,
		//	<<tokenPointerArray[i].getParameterType()<<","//rFloat,
		//	<<tokenPointerArray[i].getRiDeclare()<<","//uniform float,
		//	);
		// 			_s("// "<<tokenPointerArray[i].getTokenName());
		std::string paramType;

		liqTokenPointer* vp = const_cast< liqTokenPointer* >( &shader.tokenPointerArray[i] );
		switch( shader.tokenPointerArray[i].getParameterType() )
		{
		case rFloat:	
			paramType = "float";
			break;
		case rPoint: 
			paramType = "point";
			break;
		case rVector: 
			paramType = "vector";
			break;
		case rNormal: 
			paramType = "normal";
			break;
		case rColor:	
			paramType = "color";
			break;
		case rString: 
		case rShader:	
			paramType = "string";
			break; 
		case rHpoint:	
			paramType = "vector4";//not supported	
			break;
		case rMatrix:
			paramType = "matrix";	
			break;
		default :
			assert(0);
		}//switch
		o.addRSLVariable("", paramType.c_str(),  vp->getTokenName().c_str(), vp->getTokenName().c_str(), node);
	}//for

	//call shader function
	//function name
	o.addToRSL((mayaShaderName+"(").c_str());
	//parameters
	for(size_t i=0; i<parameterNum; ++i)
	{
		liqTokenPointer* vp = const_cast< liqTokenPointer* >( &shader.tokenPointerArray[i] );
		if(i == 0)
			o.addToRSL(vp->getTokenName().c_str());
		else
			o.addToRSL((","+vp->getTokenName()).c_str());
	}
	o.addToRSL(");");
	//

	o.endRSL();
}
}//namespace RSL

#endif//_USE_RENDERMAN_