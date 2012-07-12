#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#include "as_shaderOutputCall.h"
#include <liqlog.h>
#include "../common/mayacheck.h"
#include "../shadergraph/convertShadingNetwork.h"
#include "../shadergraph/shadermgr.h"
#include "as_renderer.h"

namespace appleseed{
namespace call{
// @node	maya shader node name
void Visitor::visitAnisotropic(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitAnisotropic("<<node<<")");
}
void Visitor::visitBlinn(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitBlinn("<<node<<")");

	OutputHelper o;

	o.begin(node);

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
}
void Visitor::visitLambert(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLambert("<<node<<")");

// 	OutputHelper o;
// 
// 	o.begin(node);
// 
// 	o.addVariable("vector", "Cs",		"color",		node);
// 	o.addVariable("vector", "transparency", "transparency", node);
// 	o.addVariable("vector", "ambientColor",	"ambientColor", node);
// 	o.addVariable("vector", "incandescence","incandescence",node);
// 	o.addVariable("float",  "diffusion",	"diffuse",		node);
// 	o.addVariable("vector", "outColor",		"outColor",		node);
// 	o.addVariable("vector", "outTransparency","outTransparency",node);

//	o.end();

	asf::auto_release_ptr<asr::Assembly> &assembly = m_renderer->getAssembly();
	//asf::auto_release_ptr<asr::Assembly> &assembly = m_renderer->current_assembly;



	//material parameters
	asr::ParamArray material_params;

	// bsdf
	{
		asr::ParamArray bsdf_params;

		{
			//reflectance color
			MDoubleArray val; 
			val.setLength(3);
			IfMErrorWarn(MGlobal::executeCommand("getAttr \""+MString(node)+".color\"", val));

			float color[] = { val[0], val[1], val[2] };
			assembly->colors().insert(
				asr::ColorEntityFactory::create(
				(MString(node)+"_color").asChar(),
				asr::ParamArray().insert("color_space", "srgb"), asr::ColorValueArray(3, color)
				)
			);
			//
			bsdf_params.insert("reflectance", (MString(node)+"_color").asChar());
		}

		//
		assembly->bsdfs().insert(
			asr::LambertianBRDFFactory().create(
				(MString(node)+"_lambert_brdf").asChar(),
				bsdf_params
			)
		);
		//
		material_params.insert("bsdf", (MString(node)+"_lambert_brdf").asChar());
	}//bsdf



	// surface shader
	{
		assembly->surface_shaders().insert(
			asr::PhysicalSurfaceShaderFactory().create(
				(MString(node)+"_physical_surface_shader").asChar(),
				asr::ParamArray()
			)
		);
		//
		material_params.insert("surface_shader", (MString(node)+"_physical_surface_shader").asChar());
	}//surface shader

	// edf
	{

	}

	// material
	assembly->materials().insert(
		asr::MaterialFactory::create( node, material_params )
	);

}
// @node	maya shader node name
void Visitor::visitLayeredShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLayeredShader("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitOceanShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitOceanShader("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitPhong(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPhong("<<node<<")");

	OutputHelper o;
	o.begin(node);

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
}
// @node	maya shader node name
void Visitor::visitRampShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitRampShader("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitShadingMap(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitShadingMap("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitSurfaceShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSurfaceShader("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitUseBackground(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitUseBackground("<<node<<")");
}
/// Volumetric ///
// @node	maya shader node name
void Visitor::visitEnvFog(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitEnvFog("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitFluidShape(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitFluidShape("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitLightFog(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLightFog("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitParticleCloud(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitParticleCloud("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitVolumeFog(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVolumeFog("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitVolumeShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVolumeShader("<<node<<")");
}
/// DISPLACEMENT ///
// @node	maya shader node name
void Visitor::visitCMuscleShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitCMuscleShader("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitDisplacementShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitDisplacementShader("<<node<<")");
}

/// Lights ///
// @node	maya shader node name
void Visitor::visitAmbientLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitAmbientLight("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitAreaLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitAreaLight("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitDirectionalLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitDirectionalLight("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitPointLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPointLight("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitSpotLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSpotLight("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitVolumeLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVolumeLight("<<node<<")");
}
}//namespace call
}//namespace appleseed
#endif//_USE_APPLESEED_