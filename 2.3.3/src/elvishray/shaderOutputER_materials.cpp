#include "shaderOutputER.h"
#include <common/mayacheck.h>
//#include "../shadergraph/convertShadingNetwork.h"
//#include "../shadergraph/shadermgr.h"
#include <liqShader.h>
#include <liqShaderFactory.h>
#include <liqTokenPointer.h>
#include "er_output_mgr.h"
#include "er_log.h"

namespace ER
{

// @node	maya shader node name
void Visitor::visitAnisotropic(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitAnisotropic("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_anisotropic", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
void Visitor::visitBlinn(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitBlinn("<<node<<")");

	OutputHelper o;

	o.beginRSL("maya_blinn", node);
	// Inputs:
	o.addRSLVariable("color",	"i_ambientColor",	"ambientColor",	node);
	o.addRSLVariable("color",	"i_color",			"color",		node);
	o.addRSLVariable("float",	"i_diffuse",		"diffuse",		node);
	o.addRSLVariable("float",	"i_eccentricity",	"eccentricity", node);
	o.addRSLVariable("color",	"i_incandescence",	"incandescence",node);
	o.addRSLVariable("index",	"i_matteOpacityMode","matteOpacityMode",node);//uniform
	o.addRSLVariable("float",	"i_matteOpacity",	"matteOpacity",node);
	o.addRSLVariable("color",	"i_specularColor",	"specularColor",node);
	o.addRSLVariable("float",	"i_specularRollOff","specularRollOff",node);
	o.addRSLVariable("float",	"i_reflectivity",	"reflectivity",node);
	o.addRSLVariable("color",	"i_reflectedColor",	"reflectedColor",node);
	/* Refraction. */
	o.addRSLVariable("bool",	"i_refractions",	"refractions",node);//uniform
	o.addRSLVariable("float",	"i_refractiveIndex","refractiveIndex",node);
	o.addRSLVariable("index",	"i_refractionLimit","refractionLimit",node);//uniform
	o.addRSLVariable("float",	"i_lightAbsorbance","lightAbsorbance",node);
	o.addRSLVariable("float",	"i_shadowAttenuation","shadowAttenuation",node);
	/* Reflection. */
	o.addRSLVariable("index",	"i_reflectionLimit",	"reflectionLimit",node);//uniform

	o.addRSLVariable("float",	"i_translucence",	"translucence",node);
	o.addRSLVariable("float",	"i_translucenceDepth",	"translucenceDepth",node);
	o.addRSLVariable("float",	"i_translucenceFocus",	"translucenceFocus",node);
	o.addRSLVariable("color",	"i_transparency",	"transparency",node);
	o.add_liq_UserDefinedNormal(node);
	o.addRSLVariable("normal",	"i_normalCamera",	"normalCamera",node);
	//3delight defined parameter
	out.ei_shader_param_scalar( "i_reflectionMaxDistance",		0.0f );
	out.ei_shader_param_index(	"i_reflectionSamples",			1 );
	out.ei_shader_param_scalar( "i_reflectionBlur",				0.0f );
	out.ei_shader_param_scalar( "i_reflectionNoiseAmplitude",	0.0f );
	out.ei_shader_param_scalar( "i_reflectionNoiseFrequency",	0.0f );
	// Outputs:
	o.addRSLVariable("color", "o_outColor",			"outColor",			node);
	o.addRSLVariable("color", "o_outTransparency",	"outTransparency",	node);

	o.endRSL();
}
void Visitor::visitHairTubeShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitHairTubeShader("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_hairTubeShader", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
void Visitor::visitLambert(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLambert("<<node<<")");

	OutputHelper o;

	o.beginRSL("maya_lambert", node);
	// Inputs:
	o.addRSLVariable("color",	"i_ambientColor",	"ambientColor",	node);
	o.addRSLVariable("color",	"i_color",			"color",		node);
	o.addRSLVariable("float",	"i_diffuse",		"diffuse",		node);
	o.addRSLVariable("color",	"i_incandescence",	"incandescence",node);
	o.addRSLVariable("index",	"i_matteOpacityMode","matteOpacityMode",node);//uniform
	o.addRSLVariable("float",	"i_matteOpacity",	"matteOpacity",node);
	/* Refraction. */
	o.addRSLVariable("bool",	"i_refractions",	"refractions",node);//uniform
	o.addRSLVariable("float",	"i_refractiveIndex","refractiveIndex",node);
	o.addRSLVariable("index",	"i_refractionLimit","refractionLimit",node);//uniform
	o.addRSLVariable("float",	"i_lightAbsorbance","lightAbsorbance",node);
	o.addRSLVariable("float",	"i_shadowAttenuation","shadowAttenuation",node);
	o.add_liq_UserDefinedNormal(node);
	o.addRSLVariable("normal",	"i_normalCamera",	"normalCamera",node);
	o.addRSLVariable("color",	"i_transparency",	"transparency",node);
	o.addRSLVariable("float",	"i_translucence",	"translucence",node);
	o.addRSLVariable("float",	"i_translucenceDepth",	"translucenceDepth",node);
	o.addRSLVariable("float",	"i_translucenceFocus",	"translucenceFocus",node);
	// Outputs:
	o.addRSLVariable("color", "o_outColor",			"outColor",			node);
	o.addRSLVariable("color", "o_outTransparency",	"outTransparency",	node);

	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitLayeredShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLayeredShader("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_layeredShader", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitOceanShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitOceanShader("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_oceanShader", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitPhong(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPhong("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_phong", node);
	//o.beginRSL("maya_phong_architectural", node);

	//Common Material Attributes
	o.addRSLVariable("vector",	"i_ambientColor",	"ambientColor",			node);
	o.addRSLVariable("vector",	"i_color",			"color",				node);
	o.addRSLVariable("float",	"i_cosinePower",		"cosinePower",		node);
	o.addRSLVariable("float",	"i_diffuse",			"diffuse",			node);	
	o.addRSLVariable("vector",	"i_incandescence",		"incandescence",	node);
	o.addRSLVariable("index",	"i_matteOpacityMode",	"matteOpacityMode",	node);
	o.addRSLVariable("float",	"i_matteOpacity",		"matteOpacity",		node);
	o.add_liq_UserDefinedNormal(node);
	o.addRSLVariable("normal",	"i_normalCamera",	"normalCamera",		node);
	o.addRSLVariable("color",	"i_specularColor",	"specularColor",	node);
	o.addRSLVariable("float",	"i_reflectivity",	"reflectivity",		node);
	o.addRSLVariable("color",	"i_reflectedColor",	"reflectedColor",	node);
	//uniform float i_refractions;
	//float i_refractiveIndex;
	//uniform float i_refractionLimit;
	//float i_lightAbsorbance;
	//float i_shadowAttenuation;
	o.addRSLVariable("index",	"i_reflectionLimit",	"reflectionLimit",	node);
	o.addRSLVariable("float",	"i_translucence",		"translucence",		node);
	o.addRSLVariable("float",	"i_translucenceDepth",	"translucenceDepth",node);
	o.addRSLVariable("float",	"i_translucenceFocus",	"translucenceFocus",node);
	o.addRSLVariable("vector",	"i_transparency",		"transparency",		node);
	//uniform float i_reflectionMaxDistance;
	//uniform float i_reflectionSamples;
	//uniform float i_reflectionBlur;
	//uniform float i_reflectionNoiseAmplitude;
	//uniform float i_reflectionNoiseFrequency;
	//output
	o.addRSLVariable("color", "o_outColor",			"outColor",			node);
	o.addRSLVariable("color", "o_outTransparency",	"outTransparency",	node);

	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitPhongE(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPhongE("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_phongE", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitRampShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitRampShader("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_rampShader", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitShadingMap(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitShadingMap("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_shadingMap", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitSurfaceShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSurfaceShader("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_surfaceShader", node);
	o.addRSLVariable("color", "o_outColor",			"outColor",			node);
	o.addRSLVariable("color", "o_outTransparency",	"outTransparency",	node);
	o.addRSLVariable("color", "o_outMatteOpacity",	"outMatteOpacity",	node);
	o.addRSLVariable("color", "o_outGlowColor",		"outGlowColor",		node);
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitUseBackground(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitUseBackground("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_useBackground", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
/// Volumetric ///
// @node	maya shader node name
void Visitor::visitEnvFog(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitEnvFog("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_envFog", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitFluidShape(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitFluidShape("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_fluidShape", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitLightFog(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLightFog("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_lightFog", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitParticleCloud(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitParticleCloud("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_particleCloud", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitVolumeFog(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVolumeFog("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_volumeFog", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitVolumeShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVolumeShader("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_volumeShader", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
/// DISPLACEMENT ///
// @node	maya shader node name
void Visitor::visitCMuscleShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitCMuscleShader("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_cMuscleShader", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitDisplacementShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitDisplacementShader("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_displacementShader", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}

/// Lights ///
// @node	maya shader node name
void Visitor::visitAmbientLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitAmbientLight("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_ambientLight", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitAreaLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitAreaLight("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_areaLight", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitDirectionalLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitDirectionalLight("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_directionalLight", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitPointLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPointLight("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_pointLight", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitSpotLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSpotLight("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_spotLight", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visitVolumeLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVolumeLight("<<node<<")");

	OutputHelper o;
	o.beginRSL("maya_volumeLight", node);
	out.a("the type of node '"+std::string(node)+"' is not implemented yet. And don't forget to add the valid connections of this type to ShaderValidConnection::setValidConnection()");
	o.endRSL();
}
// @node	maya shader node name
void Visitor::visit_liquidShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_liquidShader("<<node<<")");

	liqShader &shader = liqShaderFactory::instance().getShader( node );
	std::string const& liquidShaderName=shader.getName();//e.g."lambert1", or "liquidSurface1", NOTE: it is liquidShader, not maya shader.
	std::string const& rmSloFilePath=shader.getShaderFileName();
	std::string const& mayaShaderName=rmSloFilePath.substr(rmSloFilePath.find_last_of('/')+1);//Renderman slo file name, e.g."your_shader_dir/liquidchecker"

	OutputHelper o;

	o.beginRSL(mayaShaderName.c_str(), node);

	//tokenPointerArray only store parameters of user-defined shader
	size_t parameterNum =  shader.tokenPointerArray.size() - 1;
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
			{
				paramType = "float";
			}
			break;
		case rInt:
			{
				paramType = "int";
			}
			break;
		case rBool:
			{
				paramType = "bool";
			}
			break;
		case rPoint: 
			{
				paramType = "point";
			}
			break;
		case rVector: 
			{
				paramType = "vector";
			}
			break;
		case rNormal: 
			{
				paramType = "normal";
			}
			break;
		case rColor:
			{
				paramType = "color";
			}
			break;
		case rString:
			{
				paramType = "string";
			}
			break; 		
		case rShader:
			{
				paramType = "shader";
			}
			break; 
		case rHpoint:
			{
				paramType = "vector4";//not supported
			}
			break;
		case rMatrix:
			{
				paramType = "matrix";
			}
			;break;
		default :
			assert(0);
		}//switch
		o.addRSLVariable(paramType.c_str(),  vp->getTokenName().c_str(), vp->getTokenName().c_str(), node);
	}//for

	o.endRSL();
}
}//namespace ER